/*================================================================================
  code generated by: java2cpp
  author: Zoran Angelov, mailto://baldzar@gmail.com
  class: java.nio.InvalidMarkException
================================================================================*/


#ifndef J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_NIO_INVALIDMARKEXCEPTION_HPP_DECL
#define J2CPP_JAVA_NIO_INVALIDMARKEXCEPTION_HPP_DECL


namespace j2cpp { namespace java { namespace lang { class IllegalStateException; } } }


#include <java/lang/IllegalStateException.hpp>


namespace j2cpp {

namespace java { namespace nio {

	class InvalidMarkException;
	class InvalidMarkException
		: public object<InvalidMarkException>
	{
	public:

		J2CPP_DECLARE_CLASS

		J2CPP_DECLARE_METHOD(0)

		explicit InvalidMarkException(jobject jobj)
		: object<InvalidMarkException>(jobj)
		{
		}

		operator local_ref<java::lang::IllegalStateException>() const;


		InvalidMarkException();
	}; //class InvalidMarkException

} //namespace nio
} //namespace java

} //namespace j2cpp

#endif //J2CPP_JAVA_NIO_INVALIDMARKEXCEPTION_HPP_DECL

#else //J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_NIO_INVALIDMARKEXCEPTION_HPP_IMPL
#define J2CPP_JAVA_NIO_INVALIDMARKEXCEPTION_HPP_IMPL

namespace j2cpp {



java::nio::InvalidMarkException::operator local_ref<java::lang::IllegalStateException>() const
{
	return local_ref<java::lang::IllegalStateException>(get_jobject());
}


java::nio::InvalidMarkException::InvalidMarkException()
: object<java::nio::InvalidMarkException>(
	call_new_object<
		java::nio::InvalidMarkException::J2CPP_CLASS_NAME,
		java::nio::InvalidMarkException::J2CPP_METHOD_NAME(0),
		java::nio::InvalidMarkException::J2CPP_METHOD_SIGNATURE(0)
	>()
)
{
}



J2CPP_DEFINE_CLASS(java::nio::InvalidMarkException,"java/nio/InvalidMarkException")
J2CPP_DEFINE_METHOD(java::nio::InvalidMarkException,0,"<init>","()V")

} //namespace j2cpp

#endif //J2CPP_JAVA_NIO_INVALIDMARKEXCEPTION_HPP_IMPL

#endif //J2CPP_INCLUDE_IMPLEMENTATION