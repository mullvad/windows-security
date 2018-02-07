#include <atlstr.h>
#include <atlsecurity.h>
#include <node.h>

#define CHECK_ARG_COUNT(ISOLATE, ARGS, EXPECTED) \
  if((ARGS).Length() < (EXPECTED)) { \
    (ISOLATE)->ThrowException(v8::Exception::TypeError( \
      v8::String::NewFromUtf8((ISOLATE), "Wrong number of arguments.")) \
    ); \
    return; \
  }

#define CHECK_ARG(ISOLATE, COND) \
  if(!(COND)) { \
    (ISOLATE)->ThrowException(v8::Exception::TypeError( \
      v8::String::NewFromUtf8(ISOLATE, "Wrong arguments")) \
    ); \
    return; \
  }

#define NODE_DEFINE_CONSTANT2(target, key, constant)                          \
  do {                                                                        \
    v8::Isolate* isolate = target->GetIsolate();                              \
    v8::Local<v8::Context> context = isolate->GetCurrentContext();            \
    v8::Local<v8::String> constant_name =                                     \
        v8::String::NewFromUtf8(isolate, key);                                \
    v8::Local<v8::Number> constant_value =                                    \
        v8::Number::New(isolate, static_cast<double>(constant));              \
    v8::PropertyAttribute constant_attributes =                               \
        static_cast<v8::PropertyAttribute>(v8::ReadOnly | v8::DontDelete);    \
    (target)->DefineOwnProperty(context,                                      \
                                constant_name,                                \
                                constant_value,                               \
                                constant_attributes).FromJust();              \
  }                                                                           \
  while (0)

CAtlString GetWin32ErrorMessage(DWORD dwError);
CAtlString GetErrorMessage(HRESULT hr);

BOOL GetSidFromSidString(LPCTSTR pszSid, LPCTSTR pszSystem /* = NULL */, CSid *pSidOutput);

v8::Local<v8::String> GetV8String(v8::Isolate *isolate, LPCTSTR str);
CAtlString GetStringFromV8Value(v8::Isolate *isolate, v8::Local<v8::Value> value);