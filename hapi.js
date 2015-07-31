var net = require('net');
var request = require('request');

var client = net.connect({
    host: '10.1.30.76',
    port: 7
  }, function() { //'connect' listener
    console.log('connected to server!');
    client.write('R2');
  });
client.on('data', function(data) {
  console.log(data.toString());
  client.end();
});
client.on('end', function() {
  console.log('disconnected from server');
});





//
// //===================================
// //create a server with a host and port
// var server = new Hapi.Server();
//
// server.connection({
//   host: process.env.host,
//   port: process.env.port
// });
//
// server.route({
//   method: "GET",
//   path: '/',
//   handler: {
//     file: 'index.html'
//   }
// })
//
// server.register(require('./server/routes'),
//   function(err) {
//     if (err) {
//       console.log('Failed loading plugin');
//     }
//   }
// );
//
// server.start();
