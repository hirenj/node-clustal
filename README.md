# Nodejs wrapper for Clustal Omega

Wrapper around `libclustalo` to allow for alignment of sequences in a nodejs environment. This wraps things up
 so that you can drop this function into a lambda environment (e.g. on AWS) and have an ad-hoc sequence aligner
 that you don't have to share with people (which you would have to do when calling a web service).

Binaries are provided for Mac OS and Linux x64 architectures so you don't need to compile anything else.

## Installation

```
npm install hirenj/node-clustal
```

## About Clustal omega

All the hard work for doing sequence alignment comes from [http://www.clustal.org/omega/](Clustal Omega).


## Building binaries for your own platform
In order to build this module, we need a copy of the Clustal Omega library. Download and build Clustal Omega:
```
cd clustal-omega-1.2.1
./configure --with-pic --with-openmp
make
```

### Only for Linux
On linux you can install clustal omega globally
```
make install
```

On OS X, make sure the clustal-omega-1.2.1 folder is in the root of this folder

### Building nodejs module
```
npm install --build-from-source
npm test
```