/**
 * Created by nbokdam on 20-5-17.
 */
const config = require('../config.json').hue;
var SerialPort = require('serialport');
var Promise = require('bluebird');

var port = new SerialPort(config.port, {
  baudRate: 9600,
  parser: SerialPort.parsers.readline('\n')
});

var colorFade = function(fader){};

var open = function(cb) {
  return new Promise(function(resolve, reject) {
    port.on('open', function(err) {
      var colorFade = function(fader) {
        console.log(fader);
        const fadestr = 'R' + fader.r + ' G' + fader.g + ' B' + fader.b + ' T' + fader.t + ' E';
        console.log("Fade: ", fadestr);
        port.write(fadestr);
      };
      console.log("opened");
      cb(colorFade);
      if(err) {
        reject();
      } else {
        resolve();
      }
    });
    port.on('data', function(d) {
      console.log(d);
    })

  })
};

module.exports = {
  open: open,
  fade: colorFade
}


