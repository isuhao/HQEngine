/*================================================================================
  code generated by: java2cpp
  author: Zoran Angelov, mailto://baldzar@gmail.com
  class: java.util.InputMismatchException
================================================================================*/


#ifndef J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_UTIL_INPUTMISMATCHEXCEPTION_HPP_DECL
#define J2CPP_JAVA_UTIL_INPUTMISMATCHEXCEPTION_HPP_DECL


namespace j2cpp { namespace java { namespace io { class Serializable; } } }
namespace j2cpp { namespace java { namespace lang { class String; } } }
namespace j2cpp { namespace java { namespace util { class NoSuchElementException; } } }


#include <java/io/Serializable.hpp>
#include <java/lang/String.hpp>
#include <java/util/NoSuchElementException.hpp>


namespace j2cpp {

namespace java { namespace util {

	class InputMismatchException;
	class InputMismatchException
		: public object<InputMismatchException>
	{
	public:

		J2CPP_DECLARE_CLASS

		J2CPP_DECLARE_METHOD(0)
		J2CPP_DECLARE_METHOD(1)

		explicit InputMismatchException(jobject jobj)
		: object<InputMismatchException>(jobj)
		{
		}

		operator local_ref<java::util::NoSuchElementException>() const;
		operator local_ref<java::io::Serializable>() const;


		InputMismatchException();
		InputMismatchException(local_ref< java::lang::String > const&);
	}; //class InputMismatchException

} //namespace util
} //namespace java

} //namespace j2cpp

#endif //J2CPP_JAVA_UTIL_INPUTMISMATCHEXCEPTION_HPP_DECL

#else //J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_UTIL_INPUTMISMATCHEXCEPTION_HPP_IMPL
#define J2CPP_JAVA_UTIL_INPUTMISMATCHEXCEPTION_HPP_IMPL

namespace j2cpp {



java::util::InputMismatchException::operator local_ref<java::util::NoSuchElementException>() const
{
	return local_ref<java::util::NoSuchElementException>(get_jobject());
}

java::util::InputMismatchException::operator local_ref<java::io::Serializable>() const
{
	return local_ref<java::io::Serializable>(get_jobject());
}


java::util::InputMismatchException::InputMismatchException()
: object<java::util::InputMismatchException>(
	call_new_object<
		java::util::InputMismatchException::J2CPP_CLASS_NAME,
		java::util::InputMismatchException::J2CPP_METHOD_NAME(0),
		java::util::InputMismatchException::J2CPP_METHOD_SIGNATURE(0)
	>()
)
{
}



java::util::InputMismatchException::InputMismatchException(local_ref< java::lang::String > const &a0)
: object<java::util::InputMismatchException>(
	call_new_object<
		java::util::InputMismatchException::J2CPP_CLASS_NAME,
		java::util::InputMismatchException::J2CPP_METHOD_NAME(1),
		java::util::InputMismatchException::J2CPP_METHOD_SIGNATURE(1)
	>(a0)
)
{
}



J2CPP_DEFINE_CLASS(java::util::InputMismatchException,"java/util/InputMismatchException")
J2CPP_DEFINE_METHOD(java::util::InputMismatchException,0,"<init>","()V")
J2CPP_DEFINE_METHOD(java::util::InputMismatchException,1,"<init>","(Ljava/lang/String;)V")

} //namespace j2cpp

#endif //J2CPP_JAVA_UTIL_INPUTMISMATCHEXCEPTION_HPP_IMPL

#endif //J2CPP_INCLUDE_IMPLEMENTATION
