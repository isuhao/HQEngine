/*================================================================================
  code generated by: java2cpp
  author: Zoran Angelov, mailto://baldzar@gmail.com
  class: java.util.IllegalFormatConversionException
================================================================================*/


#ifndef J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_UTIL_ILLEGALFORMATCONVERSIONEXCEPTION_HPP_DECL
#define J2CPP_JAVA_UTIL_ILLEGALFORMATCONVERSIONEXCEPTION_HPP_DECL


namespace j2cpp { namespace java { namespace io { class Serializable; } } }
namespace j2cpp { namespace java { namespace lang { class Class; } } }
namespace j2cpp { namespace java { namespace lang { class String; } } }
namespace j2cpp { namespace java { namespace util { class IllegalFormatException; } } }


#include <java/io/Serializable.hpp>
#include <java/lang/Class.hpp>
#include <java/lang/String.hpp>
#include <java/util/IllegalFormatException.hpp>


namespace j2cpp {

namespace java { namespace util {

	class IllegalFormatConversionException;
	class IllegalFormatConversionException
		: public object<IllegalFormatConversionException>
	{
	public:

		J2CPP_DECLARE_CLASS

		J2CPP_DECLARE_METHOD(0)
		J2CPP_DECLARE_METHOD(1)
		J2CPP_DECLARE_METHOD(2)
		J2CPP_DECLARE_METHOD(3)

		explicit IllegalFormatConversionException(jobject jobj)
		: object<IllegalFormatConversionException>(jobj)
		{
		}

		operator local_ref<java::util::IllegalFormatException>() const;
		operator local_ref<java::io::Serializable>() const;


		IllegalFormatConversionException(jchar, local_ref< java::lang::Class > const&);
		local_ref< java::lang::Class > getArgumentClass();
		jchar getConversion();
		local_ref< java::lang::String > getMessage();
	}; //class IllegalFormatConversionException

} //namespace util
} //namespace java

} //namespace j2cpp

#endif //J2CPP_JAVA_UTIL_ILLEGALFORMATCONVERSIONEXCEPTION_HPP_DECL

#else //J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_UTIL_ILLEGALFORMATCONVERSIONEXCEPTION_HPP_IMPL
#define J2CPP_JAVA_UTIL_ILLEGALFORMATCONVERSIONEXCEPTION_HPP_IMPL

namespace j2cpp {



java::util::IllegalFormatConversionException::operator local_ref<java::util::IllegalFormatException>() const
{
	return local_ref<java::util::IllegalFormatException>(get_jobject());
}

java::util::IllegalFormatConversionException::operator local_ref<java::io::Serializable>() const
{
	return local_ref<java::io::Serializable>(get_jobject());
}


java::util::IllegalFormatConversionException::IllegalFormatConversionException(jchar a0, local_ref< java::lang::Class > const &a1)
: object<java::util::IllegalFormatConversionException>(
	call_new_object<
		java::util::IllegalFormatConversionException::J2CPP_CLASS_NAME,
		java::util::IllegalFormatConversionException::J2CPP_METHOD_NAME(0),
		java::util::IllegalFormatConversionException::J2CPP_METHOD_SIGNATURE(0)
	>(a0, a1)
)
{
}


local_ref< java::lang::Class > java::util::IllegalFormatConversionException::getArgumentClass()
{
	return call_method<
		java::util::IllegalFormatConversionException::J2CPP_CLASS_NAME,
		java::util::IllegalFormatConversionException::J2CPP_METHOD_NAME(1),
		java::util::IllegalFormatConversionException::J2CPP_METHOD_SIGNATURE(1), 
		local_ref< java::lang::Class >
	>(get_jobject());
}

jchar java::util::IllegalFormatConversionException::getConversion()
{
	return call_method<
		java::util::IllegalFormatConversionException::J2CPP_CLASS_NAME,
		java::util::IllegalFormatConversionException::J2CPP_METHOD_NAME(2),
		java::util::IllegalFormatConversionException::J2CPP_METHOD_SIGNATURE(2), 
		jchar
	>(get_jobject());
}

local_ref< java::lang::String > java::util::IllegalFormatConversionException::getMessage()
{
	return call_method<
		java::util::IllegalFormatConversionException::J2CPP_CLASS_NAME,
		java::util::IllegalFormatConversionException::J2CPP_METHOD_NAME(3),
		java::util::IllegalFormatConversionException::J2CPP_METHOD_SIGNATURE(3), 
		local_ref< java::lang::String >
	>(get_jobject());
}


J2CPP_DEFINE_CLASS(java::util::IllegalFormatConversionException,"java/util/IllegalFormatConversionException")
J2CPP_DEFINE_METHOD(java::util::IllegalFormatConversionException,0,"<init>","(CLjava/lang/Class;)V")
J2CPP_DEFINE_METHOD(java::util::IllegalFormatConversionException,1,"getArgumentClass","()Ljava/lang/Class;")
J2CPP_DEFINE_METHOD(java::util::IllegalFormatConversionException,2,"getConversion","()C")
J2CPP_DEFINE_METHOD(java::util::IllegalFormatConversionException,3,"getMessage","()Ljava/lang/String;")

} //namespace j2cpp

#endif //J2CPP_JAVA_UTIL_ILLEGALFORMATCONVERSIONEXCEPTION_HPP_IMPL

#endif //J2CPP_INCLUDE_IMPLEMENTATION
