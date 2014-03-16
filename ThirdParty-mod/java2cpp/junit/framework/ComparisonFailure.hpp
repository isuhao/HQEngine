/*================================================================================
  code generated by: java2cpp
  author: Zoran Angelov, mailto://baldzar@gmail.com
  class: junit.framework.ComparisonFailure
================================================================================*/


#ifndef J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JUNIT_FRAMEWORK_COMPARISONFAILURE_HPP_DECL
#define J2CPP_JUNIT_FRAMEWORK_COMPARISONFAILURE_HPP_DECL


namespace j2cpp { namespace junit { namespace framework { class AssertionFailedError; } } }
namespace j2cpp { namespace java { namespace lang { class String; } } }


#include <java/lang/String.hpp>
#include <junit/framework/AssertionFailedError.hpp>


namespace j2cpp {

namespace junit { namespace framework {

	class ComparisonFailure;
	class ComparisonFailure
		: public object<ComparisonFailure>
	{
	public:

		J2CPP_DECLARE_CLASS

		J2CPP_DECLARE_METHOD(0)
		J2CPP_DECLARE_METHOD(1)

		explicit ComparisonFailure(jobject jobj)
		: object<ComparisonFailure>(jobj)
		{
		}

		operator local_ref<junit::framework::AssertionFailedError>() const;


		ComparisonFailure(local_ref< java::lang::String > const&, local_ref< java::lang::String > const&, local_ref< java::lang::String > const&);
		local_ref< java::lang::String > getMessage();
	}; //class ComparisonFailure

} //namespace framework
} //namespace junit

} //namespace j2cpp

#endif //J2CPP_JUNIT_FRAMEWORK_COMPARISONFAILURE_HPP_DECL

#else //J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JUNIT_FRAMEWORK_COMPARISONFAILURE_HPP_IMPL
#define J2CPP_JUNIT_FRAMEWORK_COMPARISONFAILURE_HPP_IMPL

namespace j2cpp {



junit::framework::ComparisonFailure::operator local_ref<junit::framework::AssertionFailedError>() const
{
	return local_ref<junit::framework::AssertionFailedError>(get_jobject());
}


junit::framework::ComparisonFailure::ComparisonFailure(local_ref< java::lang::String > const &a0, local_ref< java::lang::String > const &a1, local_ref< java::lang::String > const &a2)
: object<junit::framework::ComparisonFailure>(
	call_new_object<
		junit::framework::ComparisonFailure::J2CPP_CLASS_NAME,
		junit::framework::ComparisonFailure::J2CPP_METHOD_NAME(0),
		junit::framework::ComparisonFailure::J2CPP_METHOD_SIGNATURE(0)
	>(a0, a1, a2)
)
{
}


local_ref< java::lang::String > junit::framework::ComparisonFailure::getMessage()
{
	return call_method<
		junit::framework::ComparisonFailure::J2CPP_CLASS_NAME,
		junit::framework::ComparisonFailure::J2CPP_METHOD_NAME(1),
		junit::framework::ComparisonFailure::J2CPP_METHOD_SIGNATURE(1), 
		local_ref< java::lang::String >
	>(get_jobject());
}


J2CPP_DEFINE_CLASS(junit::framework::ComparisonFailure,"junit/framework/ComparisonFailure")
J2CPP_DEFINE_METHOD(junit::framework::ComparisonFailure,0,"<init>","(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V")
J2CPP_DEFINE_METHOD(junit::framework::ComparisonFailure,1,"getMessage","()Ljava/lang/String;")

} //namespace j2cpp

#endif //J2CPP_JUNIT_FRAMEWORK_COMPARISONFAILURE_HPP_IMPL

#endif //J2CPP_INCLUDE_IMPLEMENTATION