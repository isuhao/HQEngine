/*================================================================================
  code generated by: java2cpp
  author: Zoran Angelov, mailto://baldzar@gmail.com
  class: java.util.EmptyStackException
================================================================================*/


#ifndef J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_UTIL_EMPTYSTACKEXCEPTION_HPP_DECL
#define J2CPP_JAVA_UTIL_EMPTYSTACKEXCEPTION_HPP_DECL


namespace j2cpp { namespace java { namespace lang { class RuntimeException; } } }


#include <java/lang/RuntimeException.hpp>


namespace j2cpp {

namespace java { namespace util {

	class EmptyStackException;
	class EmptyStackException
		: public object<EmptyStackException>
	{
	public:

		J2CPP_DECLARE_CLASS

		J2CPP_DECLARE_METHOD(0)

		explicit EmptyStackException(jobject jobj)
		: object<EmptyStackException>(jobj)
		{
		}

		operator local_ref<java::lang::RuntimeException>() const;


		EmptyStackException();
	}; //class EmptyStackException

} //namespace util
} //namespace java

} //namespace j2cpp

#endif //J2CPP_JAVA_UTIL_EMPTYSTACKEXCEPTION_HPP_DECL

#else //J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_UTIL_EMPTYSTACKEXCEPTION_HPP_IMPL
#define J2CPP_JAVA_UTIL_EMPTYSTACKEXCEPTION_HPP_IMPL

namespace j2cpp {



java::util::EmptyStackException::operator local_ref<java::lang::RuntimeException>() const
{
	return local_ref<java::lang::RuntimeException>(get_jobject());
}


java::util::EmptyStackException::EmptyStackException()
: object<java::util::EmptyStackException>(
	call_new_object<
		java::util::EmptyStackException::J2CPP_CLASS_NAME,
		java::util::EmptyStackException::J2CPP_METHOD_NAME(0),
		java::util::EmptyStackException::J2CPP_METHOD_SIGNATURE(0)
	>()
)
{
}



J2CPP_DEFINE_CLASS(java::util::EmptyStackException,"java/util/EmptyStackException")
J2CPP_DEFINE_METHOD(java::util::EmptyStackException,0,"<init>","()V")

} //namespace j2cpp

#endif //J2CPP_JAVA_UTIL_EMPTYSTACKEXCEPTION_HPP_IMPL

#endif //J2CPP_INCLUDE_IMPLEMENTATION
