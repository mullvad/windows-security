{
  'conditions': [
    [
      'OS=="win"', {
        "targets": [{
            "target_name": "windows_security",
            "defines": ["UNICODE", "WIN32_LEAN_AND_MEAN", "WINVER=0x0601"],
            "sources": [ "src/addon.cpp", "src/csidwrap.cpp", "src/helpers.cpp" ],
            "msbuild_settings": {
              "ClCompile": {
                "ExceptionHandling": "Async",
                "RuntimeLibrary": "MultiThreaded"
              }
            }
          }]
      }
    ]
  ]
}
