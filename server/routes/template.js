var serialport = require('../serialPort');

module.exports = function(action, index) {

  return function(server, options, next) {

    server.route({
      method: 'GET',
      path: '/'+action,
      handler: write
    });

    return next();
  }

  function write(request, reply) {
    serialport.write(new Buffer(index, 'ascii'));
    reply.file('index.html');
  }

};
