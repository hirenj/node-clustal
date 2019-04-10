{
  "targets": [
    {
      "target_name": "clustalo",
      "sources": [ "src/clustalo.cc" ],
      'conditions' : [
      ['OS=="mac"', {
        'include_dirs' : [ './clustal-omega-1.2.4/src/' ],
        "libraries": [ "../clustal-omega-1.2.4/src/.libs/libclustalo.a" ],
      }],
      ['OS=="linux"', {
        'include_dirs' : [ '/usr/local/include/clustalo' ],
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