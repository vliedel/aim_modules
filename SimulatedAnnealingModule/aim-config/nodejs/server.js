var nodeModule = require('./build/Release/SimulatedAnnealingModule');
//var nodeModule = require('./build/Debug/SimulatedAnnealingModule');

var obj = new nodeModule.SimulatedAnnealingModule("0");

var candidateCallBack = function(msg){
	console.log("candidate:");
	console.log(JSON.parse(msg));
};
obj.RegReadCandidate(candidateCallBack);

var searchSpace = {
	"statevars" : [
		{
			"name" : "foo",
			"type" : "int",
			"min" : 0,
			"max" : 255,
		},
		{
			"name" : "bar",
			"type" : "float",
			"min" : -1.0,
			"max" : 1.0,
		},
		{
			"name" : "lala",
			"type" : "set",
			"set" : ["a", "b", "c"],
		},
	],
};

obj.WriteSearchspace(JSON.stringify(searchSpace));
//console.log(searchSpace);

// Give some time to process stuff
//setTimeout(function(){obj.Destroy()},3000);
