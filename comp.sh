emcc -c  -s WASM=1 driver.c -o=driver.o
cp ../mono/mono/mini/.libs/libmonosgen-2.0.a .
rm -rf x
mkdir x
(cd x; ar -x ../libmonosgen-2.0.a)
# emcc -Os -g -s WASM=1 -s ASSERTIONS=2 -s ALLOW_MEMORY_GROWTH=1  -s TOTAL_MEMORY=134217728 driver.o x/*o --preload-file managed -o driver.html

emcc -Os -s WASM=1 -s ALLOW_MEMORY_GROWTH=1  -s TOTAL_MEMORY=134217728 driver.o x/*o --preload-file managed -o driver.html
