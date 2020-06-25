{
  "targets": [
    {
      "target_name": "clustalo",
      "sources": [ "src/clustalo.cc" ],
      "defines": [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      'conditions' : [
      ['OS=="mac"', {
        'include_dirs' : [ "<!@(node -p \"require('node-addon-api').include\")", './clustal-omega-1.2.4/src/' ],
        "libraries": [ "../clustal-omega-1.2.4/src/.libs/libclustalo.a" ],
      }],
      ['OS=="linux"', {
        'include_dirs' : [ "<!@(node -p \"require('node-addon-api').include\")", '/usr/local/include/clustalo' ],
        "libraries": [ "-lclustalo" ],
      }],
      ],
      "cflags" : [ "-fopenmp"],
      "link_settings" : {
      	"ldflags" : [ "-fopenmp" ]
      }
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies": [
        {
          "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
          "destination": "<(module_path)"
        }
      ]
    }
  ]
}