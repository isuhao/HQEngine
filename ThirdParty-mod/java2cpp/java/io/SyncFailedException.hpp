/*================================================================================
  code generated by: java2cpp
  author: Zoran Angelov, mailto://baldzar@gmail.com
  class: java.io.SyncFailedException
================================================================================*/


#ifndef J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_IO_SYNCFAILEDEXCEPTION_HPP_DECL
#define J2CPP_JAVA_IO_SYNCFAILEDEXCEPTION_HPP_DECL


namespace j2cpp { namespace java { namespace io { class IOException; } } }
namespace j2cpp { namespace java { namespace lang { class String; } } }


#include <java/io/IOException.hpp>
#include <java/lang/String.hpp>


namespace j2cpp {

namespace java { namespace io {

	class SyncFailedException;
	class SyncFailedException
		: public object<SyncFailedException>
	{
	public:

		J2CPP_DECLARE_CLASS

		J2CPP_DECLARE_METHOD(0)

		explicit SyncFailedException(jobject jobj)
		: object<SyncFailedException>(jobj)
		{
		}

		operator local_ref<java::io::IOException>() const;


		SyncFailedException(local_ref< java::lang::String > const&);
	}; //class SyncFailedException

} //namespace io
} //namespace java

} //namespace j2cpp

#endif //J2CPP_JAVA_IO_SYNCFAILEDEXCEPTION_HPP_DECL

#else //J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_IO_SYNCFAILEDEXCEPTION_HPP_IMPL
#define J2CPP_JAVA_IO_SYNCFAILEDEXCEPTION_HPP_IMPL

namespace j2cpp {



java::io::SyncFailedException::operator local_ref<java::io::IOException>() const
{
	return local_ref<java::io::IOException>(get_jobject());
}


java::io::SyncFailedException::SyncFailedException(local_ref< java::lang::String > const &a0)
: object<java::io::SyncFailedException>(
	call_new_object<
		java::io::SyncFailedException::J2CPP_CLASS_NAME,
		java::io::SyncFailedException::J2CPP_METHOD_NAME(0),
		java::io::SyncFailedException::J2CPP_METHOD_SIGNATURE(0)
	>(a0)
)
{
}



J2CPP_DEFINE_CLASS(java::io::SyncFailedException,"java/io/SyncFailedException")
J2CPP_DEFINE_METHOD(java::io::SyncFailedException,0,"<init>","(Ljava/lang/String;)V")

} //namespace j2cpp

#endif //J2CPP_JAVA_IO_SYNCFAILEDEXCEPTION_HPP_IMPL

#endif //J2CPP_INCLUDE_IMPLEMENTATION
