-s ALIASING_FUNCTION_POINTERS=0

This repo is a collection of hacks I'm using to work on wasm, take with a huge grain of salt.


# Setup

## Checkout layout:

I have:
wasm/
	emsdk
	mono
	wasm-hack

You want to have a normal mono around to source you a bcl to stuff into wasm-hack/managed

1) Build emscripten as webassembly.org suggests. http://webassembly.org/getting-started/developers-guide/

2) Use this mono fork/branch: https://github.com/kumpera/mono/tree/wasm-port

3) source emscripten
```
source emsdk/emsdk_env.sh 
```

4) Build mono with this configure enchantment:

```
CFLAGS="-Os -g" PATH=/usr/local/Cellar/libtool/2.4.6_1/bin/:$PATH RANLIB=emranlib AR=emar CXX=emcc CC=emcc ./autogen.sh --enable-wasm --enable-interpreter --disable-mcs-build --disable-nls --disable-boehm  --disable-btls --with-lazy-gc-thread-creation=yes --with-libgc=none --disable-executables --disable-visibility-hidden --enable-wasm --enable-minimal=ssa,com,jit,reflection_emit_save,reflection_emit,portability,assembly_remapping,attach,verifier,full_messages,appdomains,security,sgen_remset,sgen_marksweep_par,sgen_marksweep_fixed,sgen_marksweep_fixed_par,sgen_copying,logging,remoting,shared_perfcounters --host=i386-apple-darwin10
```


6) Build mono
```
make -j8
```

7) Compile driver.c using the comp.sh script

WTF with that script? emcc doesn't handle .a files :(
Plus, shell files are great for the dirty hack vibe.

8) Fire the webserver and check it out on a recent chrome

```
emrun --no_browser --port 8080 --no_emrun_detect .
```
http://localhost:8080/driver.html
```

9) Good luck and god speed.

Odds are that you'll end up with some random garbage in your setup.


# TODO
	fix autoconf detection of wasm target
	add atomic.h intrinsics
	DWARF_DATA_ALIGN is zero
	Fix eglib-config.h