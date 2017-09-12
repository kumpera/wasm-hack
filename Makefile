D8=/Users/kumpera/src/wasm/v8/out.gn/x64.release/d8
EMCC=/Users/kumpera/src/wasm/emsdk/emscripten/incoming/emcc
MINI_PATH=/Users/kumpera/src/wasm/mono/mono/mini
BCL_DIR=/Users/kumpera/src/wasm/wasm-hack/managed
MONO_SOURCE_PATH=/Users/kumpera/src/mono

MINI_TEST_FILES= \
    TestDriver.cs   \
    aot-tests.cs \
    arrays.cs       \
    basic-calls.cs  \
    basic-float.cs  \
    basic-long.cs   \
    basic-math.cs   \
    basic.cs        \
    exceptions.cs   \
    generics.cs     \
    gshared.cs      \
    objects.cs

BCL_ASSEMBLIES= \
        mscorlib.dll    \
        System.dll      \
        System.Core.dll

DEPS_ASSEMBLIES= \
        nunitlite.dll

BCL_FILES=$(patsubst %,$(BCL_DIR)/%,$(BCL_ASSEMBLIES))
DEPS_FILES=$(patsubst %,$(BCL_DIR)/%,$(DEPS_ASSEMBLIES))

MINI_TEST_SOURCES= $(patsubst %,$(MINI_PATH)/%,$(MINI_TEST_FILES))
MINI_TEST_DEPS= $(patsubst %,-r:%,$(BCL_FILES) $(DEPS_FILES))

libmonosgen-2.0.a: ../mono/mono/mini/.libs/libmonosgen-2.0.a
	cp $< $@
	rm -rf x
	mkdir x
	(cd x; ar -x ../libmonosgen-2.0.a)

	
driver.o: driver.c
	$(EMCC) -g -Os -s WASM=1 -s ALLOW_MEMORY_GROWTH=1  -s TOTAL_MEMORY=134217728 -s ALIASING_FUNCTION_POINTERS=0 driver.c -c -o driver.o

managed/nunitlite.dll: /Users/kumpera/src/mono/mcs/class/lib/net_4_x-darwin/nunitlite.dll
	cp /Users/kumpera/src/mono/mcs/class/lib/net_4_x-darwin/nunitlite.dll managed/nunitlite.dll

managed/hello.exe: hello.cs
	mcs hello.cs -r:managed/nunitlite.dll -out:managed/hello.exe

managed/mini_tests.dll: $(MINI_TEST_SOURCES) mini-test-runner.cs $(BCL_FILES) $(DEPS_FILES)
	mcs /nostdlib /unsafe -target:library -out:managed/mini_tests.dll -define:__MOBILE__ $(MINI_TEST_DEPS) $(MINI_TEST_SOURCES) mini-test-runner.cs 

mono.js: driver.o libmonosgen-2.0.a
	$(EMCC) -g -Os -s WASM=1 -s ALLOW_MEMORY_GROWTH=1  -s TOTAL_MEMORY=134217728 -s ALIASING_FUNCTION_POINTERS=0 driver.o x/*o -o mono.js

run: mono.js managed/hello.exe managed/mini_tests.dll managed/nunitlite.dll
	@$(D8) --expose_wasm test.js

