/*================================================================================
  code generated by: java2cpp
  author: Zoran Angelov, mailto://baldzar@gmail.com
  class: javax.net.ssl.SSLKeyException
================================================================================*/


#ifndef J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVAX_NET_SSL_SSLKEYEXCEPTION_HPP_DECL
#define J2CPP_JAVAX_NET_SSL_SSLKEYEXCEPTION_HPP_DECL


namespace j2cpp { namespace javax { namespace net { namespace ssl { class SSLException; } } } }
namespace j2cpp { namespace java { namespace lang { class String; } } }


#include <java/lang/String.hpp>
#include <javax/net/ssl/SSLException.hpp>


namespace j2cpp {

namespace javax { namespace net { namespace ssl {

	class SSLKeyException;
	class SSLKeyException
		: public object<SSLKeyException>
	{
	public:

		J2CPP_DECLARE_CLASS

		J2CPP_DECLARE_METHOD(0)

		explicit SSLKeyException(jobject jobj)
		: object<SSLKeyException>(jobj)
		{
		}

		operator local_ref<javax::net::ssl::SSLException>() const;


		SSLKeyException(local_ref< java::lang::String > const&);
	}; //class SSLKeyException

} //namespace ssl
} //namespace net
} //namespace javax

} //namespace j2cpp

#endif //J2CPP_JAVAX_NET_SSL_SSLKEYEXCEPTION_HPP_DECL

#else //J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVAX_NET_SSL_SSLKEYEXCEPTION_HPP_IMPL
#define J2CPP_JAVAX_NET_SSL_SSLKEYEXCEPTION_HPP_IMPL

namespace j2cpp {



javax::net::ssl::SSLKeyException::operator local_ref<javax::net::ssl::SSLException>() const
{
	return local_ref<javax::net::ssl::SSLException>(get_jobject());
}


javax::net::ssl::SSLKeyException::SSLKeyException(local_ref< java::lang::String > const &a0)
: object<javax::net::ssl::SSLKeyException>(
	call_new_object<
		javax::net::ssl::SSLKeyException::J2CPP_CLASS_NAME,
		javax::net::ssl::SSLKeyException::J2CPP_METHOD_NAME(0),
		javax::net::ssl::SSLKeyException::J2CPP_METHOD_SIGNATURE(0)
	>(a0)
)
{
}



J2CPP_DEFINE_CLASS(javax::net::ssl::SSLKeyException,"javax/net/ssl/SSLKeyException")
J2CPP_DEFINE_METHOD(javax::net::ssl::SSLKeyException,0,"<init>","(Ljava/lang/String;)V")

} //namespace j2cpp

#endif //J2CPP_JAVAX_NET_SSL_SSLKEYEXCEPTION_HPP_IMPL

#endif //J2CPP_INCLUDE_IMPLEMENTATION
