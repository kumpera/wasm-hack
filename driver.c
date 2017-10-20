#include <emscripten.h>
#include <stdio.h>

typedef enum {
	/* Disables AOT mode */
	MONO_AOT_MODE_NONE,
	/* Enables normal AOT mode, equivalent to mono_jit_set_aot_only (false) */
	MONO_AOT_MODE_NORMAL,
	/* Enables hybrid AOT mode, JIT can still be used for wrappers */
	MONO_AOT_MODE_HYBRID,
	/* Enables full AOT mode, JIT is disabled and not allowed,
	 * equivalent to mono_jit_set_aot_only (true) */
	MONO_AOT_MODE_FULL,
	/* Same as full, but use only llvm compiled code */
	MONO_AOT_MODE_LLVMONLY,
	/* Uses Interpreter, JIT is disabled and not allowed,
	 * equivalent to "--full-aot --interpreter" */
	MONO_AOT_MODE_INTERP,
	/* Same as INTERP, but use only llvm compiled code */
	MONO_AOT_MODE_INTERP_LLVMONLY,
} MonoAotMode;

typedef enum {
	MONO_IMAGE_OK,
	MONO_IMAGE_ERROR_ERRNO,
	MONO_IMAGE_MISSING_ASSEMBLYREF,
	MONO_IMAGE_IMAGE_INVALID
} MonoImageOpenStatus;



typedef struct MonoDomain_ MonoDomain;
typedef struct MonoAssembly_ MonoAssembly;
typedef struct MonoMethod_ MonoMethod;
typedef struct MonoException_ MonoException;
typedef struct MonoString_ MonoString;
typedef struct MonoClass_ MonoClass;
typedef struct MonoImage_ MonoImage;
typedef struct MonoObject_ MonoObject;
typedef struct MonoThread_ MonoThread;



void mono_jit_set_aot_mode (MonoAotMode mode);
MonoDomain *  mono_jit_init_version (const char *root_domain_name, const char *runtime_version);
MonoAssembly *mono_assembly_open (const char *filename, MonoImageOpenStatus *status);
int mono_jit_exec (MonoDomain *domain, MonoAssembly *assembly, int argc, char *argv[]);
void mono_set_assemblies_path (const char* path);
int monoeg_g_setenv(const char *variable, const char *value, int overwrite);
void mono_free (void*);
MonoString* mono_string_new (MonoDomain *domain, const char *text);
MonoDomain* mono_domain_get (void);
MonoClass* mono_class_from_name (MonoImage *image, const char* name_space, const char *name);
MonoMethod* mono_class_get_method_from_name (MonoClass *klass, const char *name, int param_count);

MonoString* mono_object_to_string (MonoObject *obj, MonoObject **exc);
char* mono_string_to_utf8 (MonoString *string_obj);
MonoObject* mono_runtime_invoke (MonoMethod *method, void *obj, void **params, MonoObject **exc);
MonoImage* mono_assembly_get_image (MonoAssembly *assembly);


static MonoAssembly *main_assembly;

EMSCRIPTEN_KEEPALIVE int
load_runtime (void)
{
	char *argv[1] = { "hello.exe" };

	monoeg_g_setenv ("MONO_LOG_LEVEL", "debug", 1);
	monoeg_g_setenv ("MONO_LOG_MASK", "gc", 1);
	mono_jit_set_aot_mode (MONO_AOT_MODE_INTERP_LLVMONLY);
	mono_set_assemblies_path ("managed");
	MonoDomain *domain = mono_jit_init_version ("hello", "v4.0.30319");
    main_assembly = mono_assembly_open ("managed/hello.exe", NULL);

	return 0;
}

static MonoMethod *send_method;
static char *last_str;

EMSCRIPTEN_KEEPALIVE char*
send_message (const char *key, const char *val)
{
	if (last_str)
		mono_free (last_str);
	last_str = NULL;

	void * params[] = {
		mono_string_new (mono_domain_get (), key),
		mono_string_new (mono_domain_get (), val),
	};

	if (!send_method) {
		MonoClass *driver_class = mono_class_from_name (mono_assembly_get_image (main_assembly), "", "Driver");
		send_method = mono_class_get_method_from_name (driver_class, "Send", -1);
	}

	MonoException *exc = NULL;
	MonoString *res = (MonoString *)mono_runtime_invoke (send_method, NULL, params, (MonoObject**)&exc);
	if (exc) {
		MonoException *second_exc = NULL;
		res = mono_object_to_string ((MonoObject*)exc, (MonoObject**)&second_exc);
		if (second_exc)
			res = mono_string_new (mono_domain_get (), "DOUBLE FAULTED EXCEPTION");
	}

	if (res)
		last_str = mono_string_to_utf8 (res);

	return last_str;
}
