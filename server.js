//
// # SimpleServer
//
// A simple chat server using Socket.IO, Express, and Async.
//
var http = require('http');
var path = require('path');

var async = require('async');
var socketio = require('socket.io');
var express = require('express');

var router = express();
var server = http.createServer(router);
var io = socketio.listen(server);

router.use(express.static(path.resolve(__dirname, 'client')));
router.set('views', __dirname);
router.set('view engine', 'ejs');

var Data = require('./models/getData.js');

router.get('/', function(req,res){
  
  var data = new Data();
  
    // Api call
  data.getData(function(result){
    
    console.log(result[0]['len']);
    console.log(result);
    
      res.render('./views/layout',{
        dataValue : result,
        len : result.length
        });

  });

});
router.get('/getData/:len', function(req,res){
  	var stuff = req.params.len;

  		
  		console.log(stuff);
  	



// Your accountSid and authToken from twilio.com/user/account
var accountSid = 'AC848f3efdf96c202952eac7b2c50bae52';
var authToken = "b9fcd32d99c3a3cbc20d1d7a09728c59";
var client = require('twilio')(accountSid, authToken);

console.log(client);
 
  client.messages.create({
      body: "Warning Detected movment in your house",
      to: "+18634305535",
      from: "+13237962441"
  }, function(err, message) {
    console.log(message);
    console.log(err);
      //process.stdout.write(message.sid);
  });


      res.render('./views/layout');


});
server.listen(process.env.PORT || 3000, process.env.IP || "0.0.0.0", function(){
  var addr = server.address();
  console.log("Chat server listening at", addr.address + ":" + addr.port);
});
