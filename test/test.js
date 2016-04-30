var assert = require('chai').assert;
describe('clustalo', function() {
  describe('load', function () {
    it('should do an alignment', function () {
      var clustal = require('..');
      var result = clustal.clustalo({'foo' : 'MMMNMMM', 'bar' : 'MMM'});
      assert.equal('MMMNMMM', result.foo);
      assert.equal('----MMM', result.bar);
    });
  });
});