var serialport = require('../serialPort');

function register(server, options, next) {

  server.route({
    method: 'POST',
    path: '/light',
    handler: write
  });

  return next();
}

function write(request, reply) {
  var payload = request.payload;
  serialport.write(new Buffer(payload.num, 'ascii'));
  reply('Success');
}

register.attributes = {
  name: 'registerLight',
  version: '1.0.0'
}

module.exports = register;
