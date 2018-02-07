//
// Windows-security addon for Node.JS
// Author: Andrej Mihajlov <and@codeispoetry.ru>
// https://github.com/pronebird
//

#include <windows.h>
#include <atlexcept.h>
#include <atlsecurity.h>
#include <atlstr.h>
#include <node.h>
#include <node_object_wrap.h>
#include "helpers.h"
#include "csidwrap.h"

void AddonGetFileOwnerSid(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);
  v8::Local<v8::Context> context = isolate->GetCurrentContext();

  CHECK_ARG_COUNT(isolate, args, 1);
  CHECK_ARG(isolate, args[0]->IsString());

  CAtlString objectPath = GetStringFromV8Value(isolate, args[0]);
  CSid sid;

  if(AtlGetOwnerSid(objectPath, SE_FILE_OBJECT, &sid)) {
    v8::Local<v8::Object> object = CSidWrap::CreateObject(isolate, sid);
    args.GetReturnValue().Set(object);
  } else {
    CAtlString message = GetWin32ErrorMessage(::GetLastError());
    isolate->ThrowException(
      v8::Exception::Error(GetV8String(isolate, message))
    );
  }
}

void AddonIsWellKnownSid(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
  v8::HandleScope scope(isolate);

  CHECK_ARG_COUNT(isolate, args, 2);
  CHECK_ARG(isolate, CSidWrap::IsInstanceOf(isolate, args[0]));
  CHECK_ARG(isolate, args[1]->IsNumber());

  const WELL_KNOWN_SID_TYPE sidType = (WELL_KNOWN_SID_TYPE)args[1]->Uint32Value();
  CSidWrap *object = node::ObjectWrap::Unwrap<CSidWrap>(args[0]->ToObject());

  BOOL bResult = IsWellKnownSid((PSID)object->GetCSid().GetPSID(), sidType);

  args.GetReturnValue().Set(bResult == TRUE);
}


void InitWellKnownSidConstants(v8::Local<v8::Object> exports);
void InitSidNameUseConstants(v8::Local<v8::Object> exports);
void InitAll(v8::Local<v8::Object> exports, v8::Local<v8::Object> module) {
  v8::Isolate *isolate = exports->GetIsolate();
  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::HandleScope scope(isolate);

  // WELL_KNOWN_SID_TYPE constants
  v8::Local<v8::Object> wellKnownSidTypes = v8::Object::New(isolate);
  InitWellKnownSidConstants(wellKnownSidTypes);
  exports->Set(context, v8::String::NewFromUtf8(isolate, "WellKnownSid"), wellKnownSidTypes);

  // SID_NAME_USE constants
  v8::Local<v8::Object> sidNameUseTypes = v8::Object::New(isolate);
  InitSidNameUseConstants(sidNameUseTypes);
  exports->Set(context, v8::String::NewFromUtf8(isolate, "SidNameUse"), sidNameUseTypes);

  // Functions
  NODE_SET_METHOD(exports, "getFileOwnerSid", AddonGetFileOwnerSid);
  NODE_SET_METHOD(exports, "isWellKnownSid", AddonIsWellKnownSid);

  // Objects
  CSidWrap::Init(exports);
}

void InitWellKnownSidConstants(v8::Local<v8::Object> exports) {
  v8::Isolate *isolate = exports->GetIsolate();
  v8::HandleScope scope(isolate);

  NODE_DEFINE_CONSTANT2(exports, "NullSid", WinNullSid);
  NODE_DEFINE_CONSTANT2(exports, "WorldSid", WinWorldSid);
  NODE_DEFINE_CONSTANT2(exports, "LocalSid", WinLocalSid);
  NODE_DEFINE_CONSTANT2(exports, "CreatorOwnerSid", WinCreatorOwnerSid);
  NODE_DEFINE_CONSTANT2(exports, "CreatorGroupSid", WinCreatorGroupSid);
  NODE_DEFINE_CONSTANT2(exports, "CreatorOwnerServerSid", WinCreatorOwnerServerSid);
  NODE_DEFINE_CONSTANT2(exports, "CreatorGroupServerSid", WinCreatorGroupServerSid);
  NODE_DEFINE_CONSTANT2(exports, "NtAuthoritySid", WinNtAuthoritySid);
  NODE_DEFINE_CONSTANT2(exports, "DialupSid", WinDialupSid);
  NODE_DEFINE_CONSTANT2(exports, "NetworkSid", WinNetworkSid);
  NODE_DEFINE_CONSTANT2(exports, "BatchSid", WinBatchSid);
  NODE_DEFINE_CONSTANT2(exports, "InteractiveSid", WinInteractiveSid);
  NODE_DEFINE_CONSTANT2(exports, "ServiceSid", WinServiceSid);
  NODE_DEFINE_CONSTANT2(exports, "AnonymousSid", WinAnonymousSid);
  NODE_DEFINE_CONSTANT2(exports, "ProxySid", WinProxySid);
  NODE_DEFINE_CONSTANT2(exports, "EnterpriseControllersSid", WinEnterpriseControllersSid);
  NODE_DEFINE_CONSTANT2(exports, "SelfSid", WinSelfSid);
  NODE_DEFINE_CONSTANT2(exports, "AuthenticatedUserSid", WinAuthenticatedUserSid);
  NODE_DEFINE_CONSTANT2(exports, "RestrictedCodeSid", WinRestrictedCodeSid);
  NODE_DEFINE_CONSTANT2(exports, "TerminalServerSid", WinTerminalServerSid);
  NODE_DEFINE_CONSTANT2(exports, "RemoteLogonIdSid", WinRemoteLogonIdSid);
  NODE_DEFINE_CONSTANT2(exports, "LogonIdsSid", WinLogonIdsSid);
  NODE_DEFINE_CONSTANT2(exports, "LocalSystemSid", WinLocalSystemSid);
  NODE_DEFINE_CONSTANT2(exports, "LocalServiceSid", WinLocalServiceSid);
  NODE_DEFINE_CONSTANT2(exports, "NetworkServiceSid", WinNetworkServiceSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinDomainSid", WinBuiltinDomainSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinAdministratorsSid", WinBuiltinAdministratorsSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinUsersSid", WinBuiltinUsersSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinGuestsSid", WinBuiltinGuestsSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinPowerUsersSid", WinBuiltinPowerUsersSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinAccountOperatorsSid", WinBuiltinAccountOperatorsSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinSystemOperatorsSid", WinBuiltinSystemOperatorsSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinPrintOperatorsSid", WinBuiltinPrintOperatorsSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinBackupOperatorsSid", WinBuiltinBackupOperatorsSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinReplicatorSid", WinBuiltinReplicatorSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinPreWindows2000CompatibleAccessSid", WinBuiltinPreWindows2000CompatibleAccessSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinRemoteDesktopUsersSid", WinBuiltinRemoteDesktopUsersSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinNetworkConfigurationOperatorsSid", WinBuiltinNetworkConfigurationOperatorsSid);
  NODE_DEFINE_CONSTANT2(exports, "AccountAdministratorSid", WinAccountAdministratorSid);
  NODE_DEFINE_CONSTANT2(exports, "AccountGuestSid", WinAccountGuestSid);
  NODE_DEFINE_CONSTANT2(exports, "AccountKrbtgtSid", WinAccountKrbtgtSid);
  NODE_DEFINE_CONSTANT2(exports, "AccountDomainAdminsSid", WinAccountDomainAdminsSid);
  NODE_DEFINE_CONSTANT2(exports, "AccountDomainUsersSid", WinAccountDomainUsersSid);
  NODE_DEFINE_CONSTANT2(exports, "AccountDomainGuestsSid", WinAccountDomainGuestsSid);
  NODE_DEFINE_CONSTANT2(exports, "AccountComputersSid", WinAccountComputersSid);
  NODE_DEFINE_CONSTANT2(exports, "AccountControllersSid", WinAccountControllersSid);
  NODE_DEFINE_CONSTANT2(exports, "AccountCertAdminsSid", WinAccountCertAdminsSid);
  NODE_DEFINE_CONSTANT2(exports, "AccountSchemaAdminsSid", WinAccountSchemaAdminsSid);
  NODE_DEFINE_CONSTANT2(exports, "AccountEnterpriseAdminsSid", WinAccountEnterpriseAdminsSid);
  NODE_DEFINE_CONSTANT2(exports, "AccountPolicyAdminsSid", WinAccountPolicyAdminsSid);
  NODE_DEFINE_CONSTANT2(exports, "AccountRasAndIasServersSid", WinAccountRasAndIasServersSid);
  NODE_DEFINE_CONSTANT2(exports, "NTLMAuthenticationSid", WinNTLMAuthenticationSid);
  NODE_DEFINE_CONSTANT2(exports, "DigestAuthenticationSid", WinDigestAuthenticationSid);
  NODE_DEFINE_CONSTANT2(exports, "SChannelAuthenticationSid", WinSChannelAuthenticationSid);
  NODE_DEFINE_CONSTANT2(exports, "ThisOrganizationSid", WinThisOrganizationSid);
  NODE_DEFINE_CONSTANT2(exports, "OtherOrganizationSid", WinOtherOrganizationSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinIncomingForestTrustBuildersSid", WinBuiltinIncomingForestTrustBuildersSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinPerfMonitoringUsersSid", WinBuiltinPerfMonitoringUsersSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinPerfLoggingUsersSid", WinBuiltinPerfLoggingUsersSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinAuthorizationAccessSid", WinBuiltinAuthorizationAccessSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinTerminalServerLicenseServersSid", WinBuiltinTerminalServerLicenseServersSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinDCOMUsersSid", WinBuiltinDCOMUsersSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinIUsersSid", WinBuiltinIUsersSid);
  NODE_DEFINE_CONSTANT2(exports, "IUserSid", WinIUserSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinCryptoOperatorsSid", WinBuiltinCryptoOperatorsSid);
  NODE_DEFINE_CONSTANT2(exports, "UntrustedLabelSid", WinUntrustedLabelSid);
  NODE_DEFINE_CONSTANT2(exports, "LowLabelSid", WinLowLabelSid);
  NODE_DEFINE_CONSTANT2(exports, "MediumLabelSid", WinMediumLabelSid);
  NODE_DEFINE_CONSTANT2(exports, "HighLabelSid", WinHighLabelSid);
  NODE_DEFINE_CONSTANT2(exports, "SystemLabelSid", WinSystemLabelSid);
  NODE_DEFINE_CONSTANT2(exports, "WriteRestrictedCodeSid", WinWriteRestrictedCodeSid);
  NODE_DEFINE_CONSTANT2(exports, "CreatorOwnerRightsSid", WinCreatorOwnerRightsSid);
  NODE_DEFINE_CONSTANT2(exports, "CacheablePrincipalsGroupSid", WinCacheablePrincipalsGroupSid);
  NODE_DEFINE_CONSTANT2(exports, "NonCacheablePrincipalsGroupSid", WinNonCacheablePrincipalsGroupSid);
  NODE_DEFINE_CONSTANT2(exports, "EnterpriseReadonlyControllersSid", WinEnterpriseReadonlyControllersSid);
  NODE_DEFINE_CONSTANT2(exports, "AccountReadonlyControllersSid", WinAccountReadonlyControllersSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinEventLogReadersGroup", WinBuiltinEventLogReadersGroup);
  NODE_DEFINE_CONSTANT2(exports, "NewEnterpriseReadonlyControllersSid", WinNewEnterpriseReadonlyControllersSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinCertSvcDComAccessGroup", WinBuiltinCertSvcDComAccessGroup);
  NODE_DEFINE_CONSTANT2(exports, "MediumPlusLabelSid", WinMediumPlusLabelSid);
  NODE_DEFINE_CONSTANT2(exports, "LocalLogonSid", WinLocalLogonSid);
  NODE_DEFINE_CONSTANT2(exports, "ConsoleLogonSid", WinConsoleLogonSid);
  NODE_DEFINE_CONSTANT2(exports, "ThisOrganizationCertificateSid", WinThisOrganizationCertificateSid);
  NODE_DEFINE_CONSTANT2(exports, "ApplicationPackageAuthoritySid", WinApplicationPackageAuthoritySid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinAnyPackageSid", WinBuiltinAnyPackageSid);
  NODE_DEFINE_CONSTANT2(exports, "CapabilityInternetClientSid", WinCapabilityInternetClientSid);
  NODE_DEFINE_CONSTANT2(exports, "CapabilityInternetClientServerSid", WinCapabilityInternetClientServerSid);
  NODE_DEFINE_CONSTANT2(exports, "CapabilityPrivateNetworkClientServerSid", WinCapabilityPrivateNetworkClientServerSid);
  NODE_DEFINE_CONSTANT2(exports, "CapabilityPicturesLibrarySid", WinCapabilityPicturesLibrarySid);
  NODE_DEFINE_CONSTANT2(exports, "CapabilityVideosLibrarySid", WinCapabilityVideosLibrarySid);
  NODE_DEFINE_CONSTANT2(exports, "CapabilityMusicLibrarySid", WinCapabilityMusicLibrarySid);
  NODE_DEFINE_CONSTANT2(exports, "CapabilityDocumentsLibrarySid", WinCapabilityDocumentsLibrarySid);
  NODE_DEFINE_CONSTANT2(exports, "CapabilitySharedUserCertificatesSid", WinCapabilitySharedUserCertificatesSid);
  NODE_DEFINE_CONSTANT2(exports, "CapabilityEnterpriseAuthenticationSid", WinCapabilityEnterpriseAuthenticationSid);
  NODE_DEFINE_CONSTANT2(exports, "CapabilityRemovableStorageSid", WinCapabilityRemovableStorageSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinRDSRemoteAccessServersSid", WinBuiltinRDSRemoteAccessServersSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinRDSEndpointServersSid", WinBuiltinRDSEndpointServersSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinRDSManagementServersSid", WinBuiltinRDSManagementServersSid);
  NODE_DEFINE_CONSTANT2(exports, "UserModeDriversSid", WinUserModeDriversSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinHyperVAdminsSid", WinBuiltinHyperVAdminsSid);
  NODE_DEFINE_CONSTANT2(exports, "AccountCloneableControllersSid", WinAccountCloneableControllersSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinAccessControlAssistanceOperatorsSid", WinBuiltinAccessControlAssistanceOperatorsSid);
  NODE_DEFINE_CONSTANT2(exports, "BuiltinRemoteManagementUsersSid", WinBuiltinRemoteManagementUsersSid);
  NODE_DEFINE_CONSTANT2(exports, "AuthenticationAuthorityAssertedSid", WinAuthenticationAuthorityAssertedSid);
  NODE_DEFINE_CONSTANT2(exports, "AuthenticationServiceAssertedSid", WinAuthenticationServiceAssertedSid);
  NODE_DEFINE_CONSTANT2(exports, "LocalAccountSid", WinLocalAccountSid);
  NODE_DEFINE_CONSTANT2(exports, "LocalAccountAndAdministratorSid", WinLocalAccountAndAdministratorSid);
  NODE_DEFINE_CONSTANT2(exports, "AccountProtectedUsersSid", WinAccountProtectedUsersSid);
}

void InitSidNameUseConstants(v8::Local<v8::Object> exports) {
  v8::Isolate *isolate = exports->GetIsolate();
  v8::HandleScope scope(isolate);

  NODE_DEFINE_CONSTANT2(exports, "User", SidTypeUser);
  NODE_DEFINE_CONSTANT2(exports, "Group", SidTypeGroup);
  NODE_DEFINE_CONSTANT2(exports, "Domain", SidTypeDomain);
  NODE_DEFINE_CONSTANT2(exports, "Alias", SidTypeAlias);
  NODE_DEFINE_CONSTANT2(exports, "WellKnownGroup", SidTypeWellKnownGroup);
  NODE_DEFINE_CONSTANT2(exports, "DeletedAccount", SidTypeDeletedAccount);
  NODE_DEFINE_CONSTANT2(exports, "Computer", SidTypeComputer);
  NODE_DEFINE_CONSTANT2(exports, "Label", SidTypeLabel);
  NODE_DEFINE_CONSTANT2(exports, "Invalid", SidTypeInvalid);
  NODE_DEFINE_CONSTANT2(exports, "Unknown", SidTypeUnknown);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, InitAll)