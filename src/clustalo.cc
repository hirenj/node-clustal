// clustalo.cc
#include <napi.h>

// Wrapping the C library here
// is critical for getting the
// linker working
extern "C" {
#include <clustal-omega.h>
}

namespace clustal {

Napi::Value Clustalo(const Napi::CallbackInfo& args) {

    Napi::Env env = args.Env();

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
    int seqtype = SEQTYPE_PROTEIN;

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

    // NODE - converted

    if (args.Length() < 2) {
        Napi::TypeError::New(env, "Wrong number of arguments")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    Napi::Object options = args[1].As<Napi::Object>();

    Napi::String options_key = Napi::String::New(env,"numIterations");
    if (options.Has(options_key) && options.Get(options_key).IsNumber() ) {
        int value = options.Get(options_key).As<Napi::Number>().Int32Value();
        numCombinedIterations = value;
    }

    options_key = Napi::String::New(env,"maxGuidetreeIterations");
    if (options.Has(options_key) && options.Get(options_key).IsNumber() ) {
        int value = options.Get(options_key).As<Napi::Number>().Int32Value();
        maxGuidetreeIterations = value;
    }

    options_key = Napi::String::New(env,"maxHMMIterations");
    if (options.Has(options_key) && options.Get(options_key).IsNumber() ) {
        int value = options.Get(options_key).As<Napi::Number>().Int32Value();
        maxHMMIterations = value;
    }

    options_key = Napi::String::New(env,"useMbed");
    if (options.Has(options_key) && options.Get(options_key).IsBoolean() ) {
        bool value = options.Get(options_key).As<Napi::Boolean>().Value();
        useMbed = value;
    }

    options_key = Napi::String::New(env,"useMbedForIteration");
    if (options.Has(options_key) && options.Get(options_key).IsBoolean() ) {
        bool value = options.Get(options_key).As<Napi::Boolean>().Value();
        useMbedForIteration = value;
    }

    options_key = Napi::String::New(env,"threads");
    if (options.Has(options_key) && options.Get(options_key).IsNumber() ) {
        int value = options.Get(options_key).As<Napi::Number>().Int32Value();
        numThreads = value;
    }

    options_key = Napi::String::New(env,"sequenceType");
    if (options.Has(options_key) && options.Get(options_key).IsNumber() ) {
        int value = options.Get(options_key).As<Napi::Number>().Int32Value();
        switch(value)
        {
            case 0:
                seqtype = SEQTYPE_UNKNOWN;
                break;
            case 1:
                seqtype = SEQTYPE_PROTEIN;
                break;
            case 2:
                seqtype = SEQTYPE_RNA;
                break;
            case 3:
                seqtype = SEQTYPE_DNA;
                break;
        }
    }

    InitClustalOmega(numThreads);

    prMSeq->seqtype = seqtype;


    rAlnOpts.bUseMbed = useMbed;
    rAlnOpts.bUseMbedForIteration = useMbedForIteration;
    rAlnOpts.iNumIterations = numCombinedIterations;
    rAlnOpts.iMaxGuidetreeIterations = maxGuidetreeIterations;
    rAlnOpts.iMaxHMMIterations = maxHMMIterations;

    // NODE - converted
    Napi::Object sequences = args[0].As<Napi::Object>();
    Napi::Array seq_names = sequences.GetPropertyNames();

    // NODE - converted
    for (unsigned int i = 0; i < seq_names.Length(); ++i) {
        Napi::Value key = seq_names.Get(i);
        Napi::Value sequence = sequences.Get(key);
        if (key.IsString() && sequence.IsString()) {

            char * utf8_key = strdup(key.As<Napi::String>().Utf8Value().c_str());
            char * utf8_sequence = strdup(sequence.As<Napi::String>().Utf8Value().c_str());
            AddSeq(&prMSeq, utf8_key,utf8_sequence);
        } else {
            Napi::TypeError::New(env, "Invalid sequence")
                .ThrowAsJavaScriptException();
            return env.Null();
        }
    }

    int rv;
    rv = Align(prMSeq, NULL, &rAlnOpts);

    // NODE - converted
    if (rv) {
        Napi::TypeError::New(env, "Error running clustal omega")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    // NODE - converted Return the aligned results in a dict.
    Napi::Object aligned = Napi::Object::New(env);

    int idx;
    for (idx = 0; idx < prMSeq->nseqs; idx++) {
        const char *key = prMSeq->sqinfo[idx].name;
        const char *seq = prMSeq->seq[idx];
        aligned.Set(Napi::String::New(env,key),Napi::String::New(env,seq));
    }
    return aligned;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "clustalo"),Napi::Function::New(env, Clustalo));
  return exports;
}

NODE_API_MODULE(clustalo, Init)

}  // namespace clustal