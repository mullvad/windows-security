//
// Windows-security addon for Node.JS
// Author: Andrej Mihajlov <and@codeispoetry.ru>
// https://github.com/pronebird
//

#include <windows.h>
#include <atlexcept.h>
#include <atlsecurity.h>
#include <atlstr.h>
#include "csidwrap.h"
#include "helpers.h"

v8::Persistent<v8::Function> CSidWrap::m_publicConstructor;
v8::Persistent<v8::Function> CSidWrap::m_privateConstructor;
v8::Persistent<v8::FunctionTemplate> CSidWrap::m_prototype;

void CSidWrap::Init(v8::Local<v8::Object> exports) {
  v8::Isolate* isolate = exports->GetIsolate();
  v8::HandleScope scope(isolate);

  // Prepare constructor template

  v8::Local<v8::FunctionTemplate> privateTemplate = v8::FunctionTemplate::New(isolate, PrivateNew);
  v8::Local<v8::FunctionTemplate> publicTemplate = v8::FunctionTemplate::New(isolate, New);

  publicTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "Sid"));
  publicTemplate->InstanceTemplate()->SetInternalFieldCount(1);

  privateTemplate->SetClassName(v8::String::NewFromUtf8(isolate, "Sid"));
  privateTemplate->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(privateTemplate, "getAccountName", AccountName);
  NODE_SET_PROTOTYPE_METHOD(privateTemplate, "getDomain", Domain);
  NODE_SET_PROTOTYPE_METHOD(privateTemplate, "getSidNameUse", SidNameUse);
  NODE_SET_PROTOTYPE_METHOD(privateTemplate, "toString", Sid);

  publicTemplate->Inherit(privateTemplate);

  m_publicConstructor.Reset(isolate, publicTemplate->GetFunction());
  m_privateConstructor.Reset(isolate, privateTemplate->GetFunction());
  m_prototype.Reset(isolate, privateTemplate);

  exports->Set(v8::String::NewFromUtf8(isolate, "Sid"), publicTemplate->GetFunction());
}

v8::Local<v8::Object> CSidWrap::CreateObject(v8::Isolate *isolate, CSid &sid) {
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::EscapableHandleScope scope(isolate);

  CSidWrap* object = new CSidWrap(sid);
  v8::Local<v8::Function> cons = v8::Local<v8::Function>::New(isolate, m_privateConstructor);
  v8::Local<v8::Object> instance = cons->NewInstance(context).ToLocalChecked();
  object->Wrap(instance);

  return scope.Escape(instance);
}

bool CSidWrap::IsInstanceOf(v8::Isolate *isolate, v8::Local<v8::Value> &other) {
  v8::HandleScope scope(isolate);
  v8::Local<v8::FunctionTemplate> proto = m_prototype.Get(isolate);

  return proto->HasInstance(other);
}

CSid& CSidWrap::GetCSid() {
  return m_sid;
}

void CSidWrap::PrivateNew(const v8::FunctionCallbackInfo<v8::Value>& args) {}

void CSidWrap::New(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);

  CHECK_ARG_COUNT(isolate, args, 2);
  CHECK_ARG(isolate, args[0]->IsString());
  CHECK_ARG(isolate, args[1]->IsString() || args[1]->IsNullOrUndefined());

  v8::String::Value sidValue(args[0]);
  v8::String::Value systemValue(args[1]);

  CAtlString sidString = GetStringFromV8Value(isolate, args[0]);
  CAtlString systemStr = GetStringFromV8Value(isolate, args[1]);
  CSid sid;

  if(GetSidFromSidString(sidString, args[1]->IsNullOrUndefined() ? (LPCTSTR)NULL : systemStr, &sid)) {
    CSidWrap* object = new CSidWrap(sid);
    object->Wrap(args.Holder());
    args.GetReturnValue().Set(args.Holder());
  } else {
    CAtlString message = GetWin32ErrorMessage(::GetLastError());
    isolate->ThrowException(
      v8::Exception::Error(GetV8String(isolate, message))
    );
  }
}

void CSidWrap::AccountName(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);

  try {
    CSidWrap *object = node::ObjectWrap::Unwrap<CSidWrap>(args.Holder());
    LPCTSTR accountName = object->m_sid.AccountName();
    args.GetReturnValue().Set(GetV8String(isolate, accountName));
  } catch(CAtlException &e) {
    CAtlString message = GetErrorMessage(e.m_hr);
    isolate->ThrowException(
      v8::Exception::Error(GetV8String(isolate, message))
    );
  }
}

void CSidWrap::Domain(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);

  try {
    CSidWrap *object = node::ObjectWrap::Unwrap<CSidWrap>(args.Holder());
    LPCTSTR domainName = object->m_sid.Domain();
    args.GetReturnValue().Set(GetV8String(isolate, domainName));
  } catch(CAtlException &e) {
    CAtlString message = GetErrorMessage(e.m_hr);
    isolate->ThrowException(
      v8::Exception::Error(GetV8String(isolate, message))
    );
  }
}

void CSidWrap::SidNameUse(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);

  try {
    CSidWrap *object = node::ObjectWrap::Unwrap<CSidWrap>(args.Holder());
    args.GetReturnValue().Set(object->m_sid.SidNameUse());
  } catch(CAtlException &e) {
    CAtlString message = GetErrorMessage(e.m_hr);
    isolate->ThrowException(
      v8::Exception::Error(GetV8String(isolate, message))
    );
  }
}

void CSidWrap::Sid(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);

  try {
    CSidWrap *object = node::ObjectWrap::Unwrap<CSidWrap>(args.Holder());
    args.GetReturnValue().Set(GetV8String(isolate, object->m_sid.Sid()));
  } catch(CAtlException &e) {
    CAtlString message = GetErrorMessage(e.m_hr);
    isolate->ThrowException(
      v8::Exception::Error(GetV8String(isolate, message))
    );
  }
}