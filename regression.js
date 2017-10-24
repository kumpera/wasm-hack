var module_name = "foo.dll.wasm";
var env = { }

var module = new WebAssembly.Module(read(module_name, 'binary'))
var instance = new WebAssembly.Instance(module, env);

var exported_functions = Object.getOwnPropertyNames (instance.exports);
var total = 0;
var err = 0;
for (var i = 0; i < exported_functions.length; ++i) {
	var name = exported_functions [i];
	var re = /test_(\d+)_.*/
	var m = name.match (re)
	if (!m)
		continue;
	var expected = parseInt (m[1]);
	var res = instance.exports [name] ();

	++total;
	if (expected != res) {
		print ("failed: " + name + " expected " + expected + " but got " + res);
		++err;
	}
}
print ("Regression tests: " + total + " ran, " + err + " failed in " + module_name)
