# windows-security
[![dependencies Status](https://david-dm.org/pronebird/windows-security/status.svg)](https://david-dm.org/pronebird/windows-security)
[![devDependencies Status](https://david-dm.org/pronebird/windows-security/dev-status.svg)](https://david-dm.org/pronebird/windows-security?type=dev)
[![AppVeyor](https://img.shields.io/appveyor/ci/pronebird/windows-security.svg)](https://ci.appveyor.com/project/pronebird/windows-security)

A Node.js addon that wraps Windows Security APIs and provides a convenient interface for working with them from JavaScript.

Currently only a limited subset of the API is available, such as SID resolution, file owner resolution and check against the well known SIDs.

## Building

To compile the extension for the first time, run

```
$ npm i
$ npm run configure
$ npm run build
```

All subsequent builds only need `npm run build`

You can confirm everything built correctly by [running the test suite](#to-run-tests).

### To run tests:

```
$ npm test
```
