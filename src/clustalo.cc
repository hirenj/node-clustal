// clustalo.cc
#include <node.h>


// Wrapping the C library here
// is critical for getting the
// linker working
extern "C" {
#include <clustal-omega.h>
}

namespace clustal {

using v8::FunctionCallbackInfo;
using v8::Exception;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Array;

void Clustalo(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    // It doesn't look like this is very important
    // but this line here is really important, otherwise
    // you'll get a Segfault when doing the alignment
    LogDefaultSetup(&rLog);

    mseq_t *prMSeq = NULL;

    // Initialize sequence and alignment options.
    NewMSeq(&prMSeq);

    // Assuming input sequences are not aligned.
    prMSeq->aligned = false;

    opts_t rAlnOpts;
    SetDefaultAlnOpts(&rAlnOpts);

    // Optional
    // int seqtype = SEQTYPE_PROTEIN;

    // EXPORT
    // SEQTYPE_DNA
    // SEQTYPE_RNA
    // SEQTYPE_PROTEIN
    // EXPORT AMINO_ALPHABET
    // EXPORT DNA_ALPHABET
    // EXPORT RNA_ALPHABET


    int numCombinedIterations = rAlnOpts.iNumIterations;
    int maxGuidetreeIterations = rAlnOpts.iMaxGuidetreeIterations;
    int maxHMMIterations = rAlnOpts.iMaxHMMIterations;
    int numThreads = 1;
    bool useMbed = rAlnOpts.bUseMbed;
    bool useMbedForIteration = rAlnOpts.bUseMbedForIteration;

    if (args.Length() < 2) {
        isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
        return;
    }

    Local<Object> options = args[1]->ToObject();

    Local<String> options_key = String::NewFromUtf8(isolate,"numIterations");
    if (options->Has(options_key) && options->Get(options_key)->IsNumber() ) {
        int value = options->Get(options_key)->IntegerValue();
        numCombinedIterations = value;
    }

    options_key = String::NewFromUtf8(isolate,"maxGuidetreeIterations");
    if (options->Has(options_key) && options->Get(options_key)->IsNumber() ) {
        int value = options->Get(options_key)->IntegerValue();
        maxGuidetreeIterations = value;
    }

    options_key = String::NewFromUtf8(isolate,"maxHMMIterations");
    if (options->Has(options_key) && options->Get(options_key)->IsNumber() ) {
        int value = options->Get(options_key)->IntegerValue();
        maxHMMIterations = value;
    }

    options_key = String::NewFromUtf8(isolate,"useMbed");
    if (options->Has(options_key) && options->Get(options_key)->IsBoolean() ) {
        bool value = options->Get(options_key)->BooleanValue();
        useMbed = value;
    }

    options_key = String::NewFromUtf8(isolate,"useMbedForIteration");
    if (options->Has(options_key) && options->Get(options_key)->IsBoolean() ) {
        bool value = options->Get(options_key)->BooleanValue();
        useMbedForIteration = value;
    }

    options_key = String::NewFromUtf8(isolate,"threads");
    if (options->Has(options_key) && options->Get(options_key)->IsNumber() ) {
        int value = options->Get(options_key)->IntegerValue();
        numThreads = value;
    }

    InitClustalOmega(numThreads);



    prMSeq->seqtype = SEQTYPE_UNKNOWN;


    rAlnOpts.bUseMbed = useMbed;
    rAlnOpts.bUseMbedForIteration = useMbedForIteration;
    rAlnOpts.iNumIterations = numCombinedIterations;
    rAlnOpts.iMaxGuidetreeIterations = maxGuidetreeIterations;
    rAlnOpts.iMaxHMMIterations = maxHMMIterations;

    Local<Object> sequences = args[0]->ToObject();
    Local<Array> seq_names = sequences->GetOwnPropertyNames();

    for (unsigned int i = 0; i < seq_names->Length(); ++i) {
        Local<Value> key = seq_names->Get(i);
        Local<Value> sequence = sequences->Get(key);

        if (key->IsString() && sequence->IsString()) {
            String::Utf8Value utf8_key(key);
            String::Utf8Value utf8_sequence(sequence);
            AddSeq(&prMSeq, *utf8_key,*utf8_sequence);
        } else {
            isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Invalid sequence")));
            return;
        }
    }

    int rv;
    rv = Align(prMSeq, NULL, &rAlnOpts);

    if (rv) {
        isolate->ThrowException(Exception::Error(
        String::NewFromUtf8(isolate, "Error running clustal omega")));
        // Throw Exception
        return;
    }

    // Return the aligned results in a dict.
    Local<Object> aligned = Object::New(isolate);
    int idx;
    for (idx = 0; idx < prMSeq->nseqs; idx++) {
        const char *key = prMSeq->sqinfo[idx].name;
        const char *seq = prMSeq->seq[idx];
        aligned->Set(String::NewFromUtf8(isolate,key),String::NewFromUtf8(isolate,seq));
    }
    args.GetReturnValue().Set(aligned);
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "clustalo", Clustalo);
}

NODE_MODULE(clustalo, init)

}  // namespace clustal