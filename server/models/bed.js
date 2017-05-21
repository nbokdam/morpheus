'use strict';

module.exports = function(Bed) {
  var Fader = require('../hue');
  Fader.open(function(ff) {
    Bed.fade = function(f, cb) {
      ff(f);
      console.log("got here", f);
      cb();
    };
  });
  Bed.remoteMethod('fade', {
    accepts: {arg: 'f', type: 'object', http:
      {
        source: 'body'
      }}
  });
};
