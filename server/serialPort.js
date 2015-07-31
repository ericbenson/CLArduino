var SerialPort = require('serialport').SerialPort;
var serialport = new SerialPort(process.env.serialport);

serialport.on('open', function() {
  console.log('Serial Port Opened');
  serialport.on('data', function(data) {
    console.log(data[0]);
  });
});

module.exports = serialport;
