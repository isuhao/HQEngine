/*================================================================================
  code generated by: java2cpp
  author: Zoran Angelov, mailto://baldzar@gmail.com
  class: org.apache.http.client.NonRepeatableRequestException
================================================================================*/


#ifndef J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_ORG_APACHE_HTTP_CLIENT_NONREPEATABLEREQUESTEXCEPTION_HPP_DECL
#define J2CPP_ORG_APACHE_HTTP_CLIENT_NONREPEATABLEREQUESTEXCEPTION_HPP_DECL


namespace j2cpp { namespace java { namespace lang { class String; } } }
namespace j2cpp { namespace org { namespace apache { namespace http { class ProtocolException; } } } }


#include <java/lang/String.hpp>
#include <org/apache/http/ProtocolException.hpp>


namespace j2cpp {

namespace org { namespace apache { namespace http { namespace client {

	class NonRepeatableRequestException;
	class NonRepeatableRequestException
		: public object<NonRepeatableRequestException>
	{
	public:

		J2CPP_DECLARE_CLASS

		J2CPP_DECLARE_METHOD(0)
		J2CPP_DECLARE_METHOD(1)

		explicit NonRepeatableRequestException(jobject jobj)
		: object<NonRepeatableRequestException>(jobj)
		{
		}

		operator local_ref<org::apache::http::ProtocolException>() const;


		NonRepeatableRequestException();
		NonRepeatableRequestException(local_ref< java::lang::String > const&);
	}; //class NonRepeatableRequestException

} //namespace client
} //namespace http
} //namespace apache
} //namespace org

} //namespace j2cpp

#endif //J2CPP_ORG_APACHE_HTTP_CLIENT_NONREPEATABLEREQUESTEXCEPTION_HPP_DECL

#else //J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_ORG_APACHE_HTTP_CLIENT_NONREPEATABLEREQUESTEXCEPTION_HPP_IMPL
#define J2CPP_ORG_APACHE_HTTP_CLIENT_NONREPEATABLEREQUESTEXCEPTION_HPP_IMPL

namespace j2cpp {



org::apache::http::client::NonRepeatableRequestException::operator local_ref<org::apache::http::ProtocolException>() const
{
	return local_ref<org::apache::http::ProtocolException>(get_jobject());
}


org::apache::http::client::NonRepeatableRequestException::NonRepeatableRequestException()
: object<org::apache::http::client::NonRepeatableRequestException>(
	call_new_object<
		org::apache::http::client::NonRepeatableRequestException::J2CPP_CLASS_NAME,
		org::apache::http::client::NonRepeatableRequestException::J2CPP_METHOD_NAME(0),
		org::apache::http::client::NonRepeatableRequestException::J2CPP_METHOD_SIGNATURE(0)
	>()
)
{
}



org::apache::http::client::NonRepeatableRequestException::NonRepeatableRequestException(local_ref< java::lang::String > const &a0)
: object<org::apache::http::client::NonRepeatableRequestException>(
	call_new_object<
		org::apache::http::client::NonRepeatableRequestException::J2CPP_CLASS_NAME,
		org::apache::http::client::NonRepeatableRequestException::J2CPP_METHOD_NAME(1),
		org::apache::http::client::NonRepeatableRequestException::J2CPP_METHOD_SIGNATURE(1)
	>(a0)
)
{
}



J2CPP_DEFINE_CLASS(org::apache::http::client::NonRepeatableRequestException,"org/apache/http/client/NonRepeatableRequestException")
J2CPP_DEFINE_METHOD(org::apache::http::client::NonRepeatableRequestException,0,"<init>","()V")
J2CPP_DEFINE_METHOD(org::apache::http::client::NonRepeatableRequestException,1,"<init>","(Ljava/lang/String;)V")

} //namespace j2cpp

#endif //J2CPP_ORG_APACHE_HTTP_CLIENT_NONREPEATABLEREQUESTEXCEPTION_HPP_IMPL

#endif //J2CPP_INCLUDE_IMPLEMENTATION
