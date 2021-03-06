/*================================================================================
  code generated by: java2cpp
  author: Zoran Angelov, mailto://baldzar@gmail.com
  class: java.lang.EnumConstantNotPresentException
================================================================================*/


#ifndef J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_LANG_ENUMCONSTANTNOTPRESENTEXCEPTION_HPP_DECL
#define J2CPP_JAVA_LANG_ENUMCONSTANTNOTPRESENTEXCEPTION_HPP_DECL


namespace j2cpp { namespace java { namespace lang { class Class; } } }
namespace j2cpp { namespace java { namespace lang { class RuntimeException; } } }
namespace j2cpp { namespace java { namespace lang { class String; } } }


#include <java/lang/Class.hpp>
#include <java/lang/RuntimeException.hpp>
#include <java/lang/String.hpp>


namespace j2cpp {

namespace java { namespace lang {

	class EnumConstantNotPresentException;
	class EnumConstantNotPresentException
		: public object<EnumConstantNotPresentException>
	{
	public:

		J2CPP_DECLARE_CLASS

		J2CPP_DECLARE_METHOD(0)
		J2CPP_DECLARE_METHOD(1)
		J2CPP_DECLARE_METHOD(2)

		explicit EnumConstantNotPresentException(jobject jobj)
		: object<EnumConstantNotPresentException>(jobj)
		{
		}

		operator local_ref<java::lang::RuntimeException>() const;


		EnumConstantNotPresentException(local_ref< java::lang::Class > const&, local_ref< java::lang::String > const&);
		local_ref< java::lang::Class > enumType();
		local_ref< java::lang::String > constantName();
	}; //class EnumConstantNotPresentException

} //namespace lang
} //namespace java

} //namespace j2cpp

#endif //J2CPP_JAVA_LANG_ENUMCONSTANTNOTPRESENTEXCEPTION_HPP_DECL

#else //J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_LANG_ENUMCONSTANTNOTPRESENTEXCEPTION_HPP_IMPL
#define J2CPP_JAVA_LANG_ENUMCONSTANTNOTPRESENTEXCEPTION_HPP_IMPL

namespace j2cpp {



java::lang::EnumConstantNotPresentException::operator local_ref<java::lang::RuntimeException>() const
{
	return local_ref<java::lang::RuntimeException>(get_jobject());
}


java::lang::EnumConstantNotPresentException::EnumConstantNotPresentException(local_ref< java::lang::Class > const &a0, local_ref< java::lang::String > const &a1)
: object<java::lang::EnumConstantNotPresentException>(
	call_new_object<
		java::lang::EnumConstantNotPresentException::J2CPP_CLASS_NAME,
		java::lang::EnumConstantNotPresentException::J2CPP_METHOD_NAME(0),
		java::lang::EnumConstantNotPresentException::J2CPP_METHOD_SIGNATURE(0)
	>(a0, a1)
)
{
}


local_ref< java::lang::Class > java::lang::EnumConstantNotPresentException::enumType()
{
	return call_method<
		java::lang::EnumConstantNotPresentException::J2CPP_CLASS_NAME,
		java::lang::EnumConstantNotPresentException::J2CPP_METHOD_NAME(1),
		java::lang::EnumConstantNotPresentException::J2CPP_METHOD_SIGNATURE(1), 
		local_ref< java::lang::Class >
	>(get_jobject());
}

local_ref< java::lang::String > java::lang::EnumConstantNotPresentException::constantName()
{
	return call_method<
		java::lang::EnumConstantNotPresentException::J2CPP_CLASS_NAME,
		java::lang::EnumConstantNotPresentException::J2CPP_METHOD_NAME(2),
		java::lang::EnumConstantNotPresentException::J2CPP_METHOD_SIGNATURE(2), 
		local_ref< java::lang::String >
	>(get_jobject());
}


J2CPP_DEFINE_CLASS(java::lang::EnumConstantNotPresentException,"java/lang/EnumConstantNotPresentException")
J2CPP_DEFINE_METHOD(java::lang::EnumConstantNotPresentException,0,"<init>","(Ljava/lang/Class;Ljava/lang/String;)V")
J2CPP_DEFINE_METHOD(java::lang::EnumConstantNotPresentException,1,"enumType","()Ljava/lang/Class;")
J2CPP_DEFINE_METHOD(java::lang::EnumConstantNotPresentException,2,"constantName","()Ljava/lang/String;")

} //namespace j2cpp

#endif //J2CPP_JAVA_LANG_ENUMCONSTANTNOTPRESENTEXCEPTION_HPP_IMPL

#endif //J2CPP_INCLUDE_IMPLEMENTATION
