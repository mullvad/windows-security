// @flow

var security = require('../');
var chai = require('chai');
var assert = chai.assert;

describe('windows-security', function() {

  it('should get file owner sid without throwing', function() {
    assert.doesNotThrow(function () {
      security.getFileOwnerSid(__filename);
    });
  });

  it('should throw on non existing file', function() {
    assert.throws(function () {
      security.getFileOwnerSid(__dirname + '/test-exception');
    });
  });

  it('should create sid from string', function() {
    assert.doesNotThrow(function () {
      new security.Sid("S-1-1-0", null);
    });
  });

  it('should throw on invalid sid', function() {
    assert.throws(function () {
      new security.Sid("S-FOUL-ID", null);
    });
  });

  it('should resolve well known sid', function() {
    var sid = new security.Sid("S-1-1-0", null);
    assert.isTrue(security.isWellKnownSid(sid, security.WellKnownSid.WorldSid));
  });

});
