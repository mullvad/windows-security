#include <node.h>
#include <node_object_wrap.h>
#include <atlsecurity.h>

class CSidWrap: public node::ObjectWrap {
public:
  static void Init(v8::Local<v8::Object> exports);
  static v8::Local<v8::Object> CreateObject(v8::Isolate *isolate, CSid &sid);
  static bool IsInstanceOf(v8::Isolate *isolate, v8::Local<v8::Value> &other);

  CSid& GetCSid();

private:
  explicit CSidWrap(CSid &sid) : m_sid(sid) {};
  ~CSidWrap() {};

  static void PrivateNew(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void AccountName(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Domain(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void SidNameUse(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void Sid(const v8::FunctionCallbackInfo<v8::Value>& args);

  static v8::Persistent<v8::Function> m_publicConstructor;
  static v8::Persistent<v8::Function> m_privateConstructor;
  static v8::Persistent<v8::FunctionTemplate> m_prototype;

  CSid m_sid;
};