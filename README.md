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
RANLIB=emranlib AR=emar CC=emcc CFLAGS="-s WASM=1 -DWASM -Os" ././autogen.sh --build=i386-apple-darwin10  --host=i686-linux --enable-minimal=jit --enable-interpreter --disable-mcs-build --disable-nls --disable-boehm  --disable-btls --with-lazy-gc-thread-creation=yes --with-libgc=none --disable-executables --disable-visibility-hidden
```

5) Edit eglib-config.h and remove the asm block from G_BREAKPOINT

I have it like this locally:
```
#define G_BREAKPOINT()           G_STMT_START { printf ("HALP\n"); abort (); } G_STMT_END
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