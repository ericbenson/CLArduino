var Hapi = require('hapi');

var serialport = require('./server/serialPort')

//create a server with a host and port
var server = new Hapi.Server();

server.connection({
  host: process.env.host,
  port: process.env.port
});

server.route({
  method: "GET",
  path: '/',
  handler: {
    file: 'index.html'
  }
})

server.register(require('./server/routes'),
  function(err) {
    if (err) {
      console.log('Failed loading plugin');
    }
  }
);

server.start();
