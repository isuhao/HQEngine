/*================================================================================
  code generated by: java2cpp
  author: Zoran Angelov, mailto://baldzar@gmail.com
  class: java.io.OptionalDataException
================================================================================*/


#ifndef J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_IO_OPTIONALDATAEXCEPTION_HPP_DECL
#define J2CPP_JAVA_IO_OPTIONALDATAEXCEPTION_HPP_DECL


namespace j2cpp { namespace java { namespace io { class ObjectStreamException; } } }


#include <java/io/ObjectStreamException.hpp>


namespace j2cpp {

namespace java { namespace io {

	class OptionalDataException;
	class OptionalDataException
		: public object<OptionalDataException>
	{
	public:

		J2CPP_DECLARE_CLASS

		J2CPP_DECLARE_METHOD(0)
		J2CPP_DECLARE_FIELD(0)
		J2CPP_DECLARE_FIELD(1)

		explicit OptionalDataException(jobject jobj)
		: object<OptionalDataException>(jobj)
		, eof(jobj)
		, length(jobj)
		{
		}

		operator local_ref<java::io::ObjectStreamException>() const;



		field< J2CPP_CLASS_NAME, J2CPP_FIELD_NAME(0), J2CPP_FIELD_SIGNATURE(0), jboolean > eof;
		field< J2CPP_CLASS_NAME, J2CPP_FIELD_NAME(1), J2CPP_FIELD_SIGNATURE(1), jint > length;
	}; //class OptionalDataException

} //namespace io
} //namespace java

} //namespace j2cpp

#endif //J2CPP_JAVA_IO_OPTIONALDATAEXCEPTION_HPP_DECL

#else //J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_IO_OPTIONALDATAEXCEPTION_HPP_IMPL
#define J2CPP_JAVA_IO_OPTIONALDATAEXCEPTION_HPP_IMPL

namespace j2cpp {



java::io::OptionalDataException::operator local_ref<java::io::ObjectStreamException>() const
{
	return local_ref<java::io::ObjectStreamException>(get_jobject());
}




J2CPP_DEFINE_CLASS(java::io::OptionalDataException,"java/io/OptionalDataException")
J2CPP_DEFINE_METHOD(java::io::OptionalDataException,0,"<init>","()V")
J2CPP_DEFINE_FIELD(java::io::OptionalDataException,0,"eof","Z")
J2CPP_DEFINE_FIELD(java::io::OptionalDataException,1,"length","I")

} //namespace j2cpp

#endif //J2CPP_JAVA_IO_OPTIONALDATAEXCEPTION_HPP_IMPL

#endif //J2CPP_INCLUDE_IMPLEMENTATION
