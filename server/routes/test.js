var register = require('./template')('test', '5');

register.attributes = {
  name: 'registerTest',
  version: '1.0.0'
}

module.exports = register;
