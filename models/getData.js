var unirest = require("unirest");
var home = require('../server.js');


function Data() {
  // always initialize all instance properties

  this.data; // default value
}

Data.prototype.getData= function(callback) {
//https://data.sparkfun.com/streams/QGxKyK2wzGUdbLoYdmqp
//https://data.sparkfun.com/streams/q5y4E069JJTqQmlO8L0R
	 unirest.get('https://data.sparkfun.com/output/q5y4E069JJTqQmlO8L0R.json')
    .end(function (res) {
        
        var data = res.body;
       // console.log(typeof(temp));

        //console.log(data);




        callback(data);
        
    });

};

module.exports = Data;