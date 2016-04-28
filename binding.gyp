{
  "targets": [
    {
      "target_name": "clustalo",
      "sources": [ "src/clustalo.cc" ],
      'include_dirs' : [ '/usr/local/include/clustalo' ],
      "libraries": [ "-lclustalo" ],
      "cflags" : [ "-fopenmp"],
      "link_settings" : {
      	"ldflags" : [ "-fopenmp" ]
      }
    }
  ]
}