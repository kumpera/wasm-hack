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
	MONO_AOT_MODE_INTERP
} MonoAotMode;

typedef enum {
	MONO_IMAGE_OK,
	MONO_IMAGE_ERROR_ERRNO,
	MONO_IMAGE_MISSING_ASSEMBLYREF,
	MONO_IMAGE_IMAGE_INVALID
} MonoImageOpenStatus;

typedef struct _MonoDomain MonoDomain;
typedef struct _MonoAssembly MonoAssembly;


void mono_jit_set_aot_mode (MonoAotMode mode);
MonoDomain *  mono_jit_init_version (const char *root_domain_name, const char *runtime_version);
MonoAssembly *mono_assembly_open (const char *filename, MonoImageOpenStatus *status);
int mono_jit_exec (MonoDomain *domain, MonoAssembly *assembly, int argc, char *argv[]);
void mono_set_assemblies_path (const char* path);


int main (int argc, char *argv[])
{
	printf ("hello world 2\n");
	mono_jit_set_aot_mode (MONO_AOT_MODE_INTERP);
	mono_set_assemblies_path ("managed");
	MonoDomain *domain = mono_jit_init_version ("hello", "v4.0.30319");

    MonoAssembly *assembly = mono_assembly_open ("hello.exe", NULL);
    mono_jit_exec (domain, assembly, argc, argv);

	return 0;
}
