//
// Windows-security addon for Node.JS
// Author: Andrej Mihajlov <and@codeispoetry.ru>
// https://github.com/pronebird
//

#include "helpers.h"

CAtlString GetWin32ErrorMessage(DWORD dwError) {
  LPTSTR lpMessageBuffer = NULL;
  DWORD dwCchLength = FormatMessage(
    FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,
    dwError,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
    (LPTSTR)&lpMessageBuffer,
    0,
    NULL
  );

  CAtlString cs(lpMessageBuffer, dwCchLength);

  ::LocalFree(lpMessageBuffer);

  return cs;
}

CAtlString GetErrorMessage(HRESULT hr) {
  switch(HRESULT_FACILITY(hr)) {
    case FACILITY_WIN32:
      return GetWin32ErrorMessage(HRESULT_CODE(hr));

    default: {
      CAtlString cs;
      cs.Format(_T("Unknown error: 0x%08x"), hr);
      return cs;
    }
  }
}

BOOL GetSidFromSidString(LPCTSTR pszSid, LPCTSTR pszSystem /* = NULL */, CSid *pSidOutput) {
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
  return CAtlString((LPCTSTR)*s, s.length());
#else
  const v8::String::Utf8Value s(value);
  return CAtlString(*s, s.length());
#endif

}