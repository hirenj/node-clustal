# Nodejs wrapper for Clustal Omega

Wrapper around `libclustalo` to allow for alignment of sequences in a nodejs environment. This wraps things up
 so that you can drop this function into a lambda environment (e.g. on AWS) and have an ad-hoc sequence aligner
 that you don't have to share with people (which you would have to do when calling a web service).

## Compiling
Download and build Clustal Omega

```
cd clustal-omega-1.2.1
./configure --with-pic --with-openmp
make && make install
```

```
node-gyp clean configure build
```
