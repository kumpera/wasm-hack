# emcc -g -Os -s WASM=1  -s ALLOW_MEMORY_GROWTH=1  -s TOTAL_MEMORY=134217728  driver.c -o=driver.o
emcc -g -Os -s WASM=1 -s ALLOW_MEMORY_GROWTH=1  -s TOTAL_MEMORY=134217728 -s ALIASING_FUNCTION_POINTERS=0 driver.c -o=driver.o
cp ../mono/mono/mini/.libs/libmonosgen-2.0.a .
rm -rf x
mkdir x
(cd x; ar -x ../libmonosgen-2.0.a)

# emcc -g -Os -s WASM=1 -s ALLOW_MEMORY_GROWTH=1  -s TOTAL_MEMORY=134217728 driver.o x/*o --preload-file managed -o driver.html
emcc -g -Os -s WASM=1 -s ALLOW_MEMORY_GROWTH=1  -s TOTAL_MEMORY=134217728 -s ALIASING_FUNCTION_POINTERS=0 driver.o  --preload-file managed -o driver.html
