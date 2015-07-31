var requestMod = require('request');
var connection = new telnet();

var params = {
  host: '10.',
  port: 7,
}

connection.on('ready', function(prompt){
  connection.exec(cmd, function(response){
    console.log(response);
  });
});

connection.on('close', function(){
  console.log('connection closed');
});

connection.connect(params);

function register(server, options, next) {

  server.route({
    method: 'GET',
    path: '/api',
    handler: write
  });

  return next();
}

function write(request, reply) {
  // var payload = request.payload;
  // serialport.write(new Buffer(payload.num, 'ascii'));


  requestMod('https://pipe.creativelive.com/api/metrics/rolling/On-demand+video+view-channel:business', function(error, response, body) {
    if (!error && response.statusCode == 200) {
      reply(body);
    } else {
      reply('');

    }
  })
}

register.attributes = {
  name: 'registerApi',
  version: '1.0.0'
}

module.exports = register;
