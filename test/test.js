var assert = require('chai').assert;
describe('clustalo', function() {
  describe('load', function () {
    it('should do an alignment', function () {
      var clustal = require('./build/Release/clustalo');
      var result = clustal.clustalo({'foo' : 'MMM', 'bar' : 'MMM'});
      assert.equal('MMM', result.foo);
    });
  });
});