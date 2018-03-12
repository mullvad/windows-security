{
  'conditions': [
    [
      'OS=="win"', {
        "targets": [{
          "target_name": "windows_security",
          "defines": ["UNICODE", "WIN32_LEAN_AND_MEAN", "WINVER=0x0601"],
          "sources": [
            "src/addon.cpp",
            "src/csidwrap.cpp",
            "src/helpers.cpp"
          ],
          "libraries": ["advapi32.lib"],
          "msbuild_settings": {
            "ClCompile": {
              "ExceptionHandling": "Async",
              "RuntimeLibrary": "MultiThreaded"
            }
          }
        }, {
          "target_name": "action_after_build",
          "type": "none",
          "dependencies": [ "<(module_name)" ],
          "copies": [
            {
              "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
              "destination": "<(module_path)"
            }
          ]
        }]
      }
    ]
  ]
}
