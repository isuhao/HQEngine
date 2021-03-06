/*================================================================================
  code generated by: java2cpp
  author: Zoran Angelov, mailto://baldzar@gmail.com
  class: android.net.ParseException
================================================================================*/


#ifndef J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_ANDROID_NET_PARSEEXCEPTION_HPP_DECL
#define J2CPP_ANDROID_NET_PARSEEXCEPTION_HPP_DECL


namespace j2cpp { namespace java { namespace lang { class RuntimeException; } } }
namespace j2cpp { namespace java { namespace lang { class String; } } }


#include <java/lang/RuntimeException.hpp>
#include <java/lang/String.hpp>


namespace j2cpp {

namespace android { namespace net {

	class ParseException;
	class ParseException
		: public object<ParseException>
	{
	public:

		J2CPP_DECLARE_CLASS

		J2CPP_DECLARE_METHOD(0)
		J2CPP_DECLARE_FIELD(0)

		explicit ParseException(jobject jobj)
		: object<ParseException>(jobj)
		, response(jobj)
		{
		}

		operator local_ref<java::lang::RuntimeException>() const;



		field< J2CPP_CLASS_NAME, J2CPP_FIELD_NAME(0), J2CPP_FIELD_SIGNATURE(0), local_ref< java::lang::String > > response;
	}; //class ParseException

} //namespace net
} //namespace android

} //namespace j2cpp

#endif //J2CPP_ANDROID_NET_PARSEEXCEPTION_HPP_DECL

#else //J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_ANDROID_NET_PARSEEXCEPTION_HPP_IMPL
#define J2CPP_ANDROID_NET_PARSEEXCEPTION_HPP_IMPL

namespace j2cpp {



android::net::ParseException::operator local_ref<java::lang::RuntimeException>() const
{
	return local_ref<java::lang::RuntimeException>(get_jobject());
}




J2CPP_DEFINE_CLASS(android::net::ParseException,"android/net/ParseException")
J2CPP_DEFINE_METHOD(android::net::ParseException,0,"<init>","()V")
J2CPP_DEFINE_FIELD(android::net::ParseException,0,"response","Ljava/lang/String;")

} //namespace j2cpp

#endif //J2CPP_ANDROID_NET_PARSEEXCEPTION_HPP_IMPL

#endif //J2CPP_INCLUDE_IMPLEMENTATION
