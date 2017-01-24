var assert = require('chai').assert;
describe('clustalo', function() {
  describe('load', function () {
    it('should be able to load library', function () {
      assert.doesNotThrow(function() { require('..'); },"Can load library");
    });
  });

  describe('parameter_type_checking', function () {
    it('should require two arguments', function () {
      var clustal = require('..');
      assert.throws(function() { clustal.clustalo({}) },TypeError,"Wrong number of arguments");
    });
    it('should check the type of sequences', function () {
      var clustal = require('..');
      assert.throws(function() { clustal.clustalo({'foo' : 'MMM', 'bar' : 12 },{}); },TypeError,"Invalid sequence");
    });
    it('should drop options of the wrong type', function () {
      var clustal = require('..');
      assert.doesNotThrow(function() { clustal.clustalo({'foo' : 'MMM', 'bar' : 'MMM' },{ 'numThreads' : 'ABCD' }); },"Drops options of wrong type");
    });
  });

  describe('align', function () {
    it('should do an alignment', function () {
      var clustal = require('..');
      var result = clustal.clustalo({'foo' : 'MMMNMMM', 'bar' : 'MMM'},{});
      assert.equal('MMMNMMM', result.foo);
      assert.equal('----MMM', result.bar);
    });
    it('should alignment more than two sequences', function () {
      var clustal = require('..');
      var result = clustal.clustalo({'foo' : 'MMMNMMM', 'bar' : 'MMM', 'baz' : 'MMMM'},{ sequenceType : clustal.SEQTYPE_PROTEIN });
      assert.equal('MMMNMMM', result.foo);
      assert.equal('----MMM', result.bar);
      assert.equal('MM---MM', result.baz);
    });

  });
});