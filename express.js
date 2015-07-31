// Module
var express = require('express');
var app = express();
var request = require('request');

// Define port
var port = 3000;

// Rest
var rest = require("arest")(app);

// rest.addDevice('http','10.1.30.76');
// rest.addDevice('serial','/dev/cu.usbmodem1411', 115200);

request('http://10.1.30.76/mode/6/o', function(error, response, body) {
  if (!error && response.statusCode === 200) {
    console.log(body);
  }

})
setInterval(function() {
  request('http://10.1.30.76/digitalWrite/6/1', function(error, response, body) {
    if (!error && response.statusCode === 200) {
      console.log(body);
    }
  })
}, 5000);

setTimeout(function() {

  setInterval(function() {
    request('http://10.1.30.76/digitalWrite/6/0', function(error, response, body) {
      if (!error && response.statusCode === 200) {
        console.log(body);
      }
    })
  }, 5000);

}, 2500);


// Start server
app.listen(port);
console.log("Listening on port " + port);
