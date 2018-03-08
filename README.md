# windows-security
[![dependencies Status](https://david-dm.org/pronebird/windows-security/status.svg)](https://david-dm.org/pronebird/windows-security)
[![devDependencies Status](https://david-dm.org/pronebird/windows-security/dev-status.svg)](https://david-dm.org/pronebird/windows-security?type=dev)
[![AppVeyor](https://img.shields.io/appveyor/ci/pronebird/windows-security.svg)](https://ci.appveyor.com/project/pronebird/windows-security)

A Node.js addon that wraps Windows Security APIs and provides a convenient interface for working with them from JavaScript.

Currently only a limited subset of the API is available, such as SID resolution, file owner resolution and check against the well known SIDs.

## Prerequisites

There are no prerequisites. This module comes with prebuilt binaries published on GitHub releases automatically via Appveyor.

However if you wish to build it from source make sure to install the Visual Studio 2015 Build Tools with MFC/ATL support. For the environment configuration please refer to [the Node.js guidelines by Microsoft](https://github.com/Microsoft/nodejs-guidelines/blob/master/windows-environment.md#environment-setup-and-configuration).

## Building

To compile the module simply run

```
./node_modules/.bin/node-pre-gyp build
```

You can confirm everything built correctly by [running the test suite](#to-run-tests).

### To run tests:

```
$ npm test
```
