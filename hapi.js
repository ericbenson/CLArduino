var net = require('net');
var request = require('request');
var Q = require('Q');

var maxRev = 0;
var maxViewer = 0;
var maxEnroll = 0;

var channels = {
  revenue: {
    url: 'https://pipe.creativelive.com/api/metrics/rolling/Completed+Order/revenue',
    avg: 745,
    ref: 'R'
  },
  onDemand: {
    url: 'https://pipe.creativelive.com/api/metrics/rolling/On-demand+video+view/',
    avg: 107,
    ref: 'V'
  },
  broadcast: {
    url: 'https://pipe.creativelive.com/api/metrics/rolling/Broadcast+Video+View',
    avg: 151,
    ref: 'V'
  },
  enrollment: {
    url: 'https://pipe.creativelive.com/api/metrics/rolling/Enrollment',
    avg: 7,
    ref: 'E'
  }
};

var client = net.connect({
  host: '10.1.31.4',
  port: 7
}, function() { //'connect' listener
  console.log('connected to server!');

  var counter = 0;

  setInterval(function() {
    switch (counter) {
      case 0:
        getData('onDemand').then(function(body) {
          var onDemand = body;
          getData('broadcast').then(function(body) {
            var broadcast = body;
            var current = onDemand.values[0].sum + broadcast.values[0].sum;
            // var current = channels.onDemand.avg + channels.broadcast.avg;
            if (current > maxViewer) {
              maxViewer = current;
            }
            var percentage = getPercentage(current, maxViewer);
            client.write('V' + percentage + '\n');
          });
        });

        break;
      case 1:
        getData('revenue').then(function(body) {
          var current = body.values[0].sum;
          if (current > maxRev) {
            maxRev = current;
          }
          var percentage = getPercentage(current, maxRev);
          client.write('R' + percentage + '\n');
        })
        break;

      case 2:
        getData('enrollment').then(function(body) {
          var current = body.values[0].sum;
          if(current > maxEnroll){
            maxEnroll = current;
          }
          var percentage = getPercentage(current, maxEnroll);
          client.write('E' + percentage + '\n');
        })
        break;
      default:
        client.write('S7\n');
    }
    if (counter < 3) {
      counter++;
    } else {
      counter = 0;
    }
  }, 4500);

});

client.on('data', function(data) {
  console.log(data.toString());
});
client.on('end', function() {
  console.log('disconnected from server');
});

function getData(type) {
  var deferred = Q.defer();
  request(channels[type].url, function(error, response, body) {
    deferred.resolve(JSON.parse(body));
  });
  return deferred.promise;
}

function getPercentage(val, max) {
  var percent = Math.round((val / max) * 100);
  if (percent > 99) {
    return 99;
  }

  return ('0' + percent).slice(-2);
}
