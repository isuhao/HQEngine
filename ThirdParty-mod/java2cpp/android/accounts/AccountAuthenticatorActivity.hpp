/*================================================================================
  code generated by: java2cpp
  author: Zoran Angelov, mailto://baldzar@gmail.com
  class: android.accounts.AccountAuthenticatorActivity
================================================================================*/


#ifndef J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_ANDROID_ACCOUNTS_ACCOUNTAUTHENTICATORACTIVITY_HPP_DECL
#define J2CPP_ANDROID_ACCOUNTS_ACCOUNTAUTHENTICATORACTIVITY_HPP_DECL


namespace j2cpp { namespace android { namespace app { class Activity; } } }
namespace j2cpp { namespace android { namespace os { class Bundle; } } }


#include <android/app/Activity.hpp>
#include <android/os/Bundle.hpp>


namespace j2cpp {

namespace android { namespace accounts {

	class AccountAuthenticatorActivity;
	class AccountAuthenticatorActivity
		: public object<AccountAuthenticatorActivity>
	{
	public:

		J2CPP_DECLARE_CLASS

		J2CPP_DECLARE_METHOD(0)
		J2CPP_DECLARE_METHOD(1)
		J2CPP_DECLARE_METHOD(2)
		J2CPP_DECLARE_METHOD(3)

		explicit AccountAuthenticatorActivity(jobject jobj)
		: object<AccountAuthenticatorActivity>(jobj)
		{
		}

		operator local_ref<android::app::Activity>() const;


		AccountAuthenticatorActivity();
		void setAccountAuthenticatorResult(local_ref< android::os::Bundle >  const&);
		void finish();
	}; //class AccountAuthenticatorActivity

} //namespace accounts
} //namespace android

} //namespace j2cpp

#endif //J2CPP_ANDROID_ACCOUNTS_ACCOUNTAUTHENTICATORACTIVITY_HPP_DECL

#else //J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_ANDROID_ACCOUNTS_ACCOUNTAUTHENTICATORACTIVITY_HPP_IMPL
#define J2CPP_ANDROID_ACCOUNTS_ACCOUNTAUTHENTICATORACTIVITY_HPP_IMPL

namespace j2cpp {



android::accounts::AccountAuthenticatorActivity::operator local_ref<android::app::Activity>() const
{
	return local_ref<android::app::Activity>(get_jobject());
}


android::accounts::AccountAuthenticatorActivity::AccountAuthenticatorActivity()
: object<android::accounts::AccountAuthenticatorActivity>(
	call_new_object<
		android::accounts::AccountAuthenticatorActivity::J2CPP_CLASS_NAME,
		android::accounts::AccountAuthenticatorActivity::J2CPP_METHOD_NAME(0),
		android::accounts::AccountAuthenticatorActivity::J2CPP_METHOD_SIGNATURE(0)
	>()
)
{
}


void android::accounts::AccountAuthenticatorActivity::setAccountAuthenticatorResult(local_ref< android::os::Bundle > const &a0)
{
	return call_method<
		android::accounts::AccountAuthenticatorActivity::J2CPP_CLASS_NAME,
		android::accounts::AccountAuthenticatorActivity::J2CPP_METHOD_NAME(1),
		android::accounts::AccountAuthenticatorActivity::J2CPP_METHOD_SIGNATURE(1), 
		void
	>(get_jobject(), a0);
}


void android::accounts::AccountAuthenticatorActivity::finish()
{
	return call_method<
		android::accounts::AccountAuthenticatorActivity::J2CPP_CLASS_NAME,
		android::accounts::AccountAuthenticatorActivity::J2CPP_METHOD_NAME(3),
		android::accounts::AccountAuthenticatorActivity::J2CPP_METHOD_SIGNATURE(3), 
		void
	>(get_jobject());
}


J2CPP_DEFINE_CLASS(android::accounts::AccountAuthenticatorActivity,"android/accounts/AccountAuthenticatorActivity")
J2CPP_DEFINE_METHOD(android::accounts::AccountAuthenticatorActivity,0,"<init>","()V")
J2CPP_DEFINE_METHOD(android::accounts::AccountAuthenticatorActivity,1,"setAccountAuthenticatorResult","(Landroid/os/Bundle;)V")
J2CPP_DEFINE_METHOD(android::accounts::AccountAuthenticatorActivity,2,"onCreate","(Landroid/os/Bundle;)V")
J2CPP_DEFINE_METHOD(android::accounts::AccountAuthenticatorActivity,3,"finish","()V")

} //namespace j2cpp

#endif //J2CPP_ANDROID_ACCOUNTS_ACCOUNTAUTHENTICATORACTIVITY_HPP_IMPL

#endif //J2CPP_INCLUDE_IMPLEMENTATION
