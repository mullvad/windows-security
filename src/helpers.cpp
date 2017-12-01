//
// Windows-security addon for Node.JS
// Author: Andrej Mihajlov <and@codeispoetry.ru>
// https://github.com/pronebird
//

#include <comdef.h>
#include "helpers.h"

CAtlString GetWin32ErrorMessage(DWORD dwError) {
  return GetComErrorMessage(HRESULT_FROM_WIN32(dwError));
}

CAtlString GetComErrorMessage(HRESULT hr) {
  _com_error error(hr);
  CAtlString cs;
  cs.Format(_T("%s (code: 0x%08x)"), error.ErrorMessage(), hr);
  return cs;
}

BOOL AtlSidFromSidString(LPCTSTR pszSid, LPCTSTR pszSystem /* = NULL */, CSid *pSidOutput) {
  ATLASSERT(pszSid && pSidOutput);

  PSID pRawSid = NULL;
  if(!ConvertStringSidToSid(pszSid, &pRawSid)) {
    return FALSE;
  }

  *pSidOutput = *((const SID *)pRawSid);
  ::LocalFree(pRawSid);
  return TRUE;
}

v8::Local<v8::String> GetV8String(v8::Isolate *isolate, LPCTSTR str) {
  v8::EscapableHandleScope scope(isolate);
#ifdef UNICODE
  v8::Local<v8::String> s = v8::String::NewFromTwoByte(isolate, (uint16_t *)str);
#else
  v8::Local<v8::String> s = v8::String::NewFromUtf8(isolate, str);
#endif
  return scope.Escape(s);
}

CAtlString GetStringFromV8Value(v8::Isolate *isolate, v8::Local<v8::Value> value) {
  v8::HandleScope scope(isolate);
#ifdef UNICODE
  const v8::String::Value s(value);
  return CAtlString((LPCTSTR)*s);
#else
  const v8::String::Utf8Value s(value);
  return CAtlString(*s);
#endif

}