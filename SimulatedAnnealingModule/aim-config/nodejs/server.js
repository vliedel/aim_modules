var nodeModule = require('./build/Release/SimulatedAnnealingModule');
//var nodeModule = require('./build/Debug/SimulatedAnnealingModule');

var obj = new nodeModule.SimulatedAnnealingModule("0"); // 0 is the ID of the aim module, should be a positive integer

var candidateCallBack = function(msg){
	console.log("candidate:"); // Here you get a msg which should be sent to the DER agent
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
			"type" : "real",
			"min" : -1.5,
			"max" : 1.5,
		},
		{
			"name" : "lala",
			"type" : "set",
			"set" : ["a", "b", "c"],
		},
	],
};

obj.WriteSearchspace(JSON.stringify(searchSpace)); // Initialize the search space
obj.WriteCost(..); // Response of the DER agent: the cost or error of how well the candidate matches the measure power
//console.log(searchSpace);

// Give some time to process stuff
//setTimeout(function(){obj.Destroy()},3000);
