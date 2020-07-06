const binding = require('node-gyp-build')(__dirname);
binding.SEQTYPE_UNKNOWN = 0;
binding.SEQTYPE_PROTEIN = 1;
binding.SEQTYPE_RNA = 2;
binding.SEQTYPE_DNA = 3;
module.exports = binding;