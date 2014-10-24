var nodeModule = require('./build/Release/TestModule');
//var nodeModule = require('./build/Debug/TestModule');

var obj = new nodeModule.TestModule("0");

var paramCallBack = function(msg){
	console.log("params:");
	console.log(msg);
};
obj.RegReadParam(paramCallBack);

var paramDesc = "bla bla bla";
var error = 0.3;

obj.WriteParamDesc(paramDesc);
obj.WriteError(error);

for (i=0; i<100; ++i) {
	id = i+1;
	obj = new nodeModule.TestModule(id.toString());
	obj.RegReadParam(paramCallBack);
	obj.WriteParamDesc(id.toString() + ".1 " + paramDesc);
	obj.WriteParamDesc(id.toString() + ".2 " + paramDesc);
	obj.WriteParamDesc(id.toString() + ".3 " + paramDesc);
}

// Give some time to process stuff
//setTimeout(function(){obj.Destroy()},3000);
