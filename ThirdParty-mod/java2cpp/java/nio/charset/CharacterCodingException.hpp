/*================================================================================
  code generated by: java2cpp
  author: Zoran Angelov, mailto://baldzar@gmail.com
  class: java.nio.charset.CharacterCodingException
================================================================================*/


#ifndef J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_NIO_CHARSET_CHARACTERCODINGEXCEPTION_HPP_DECL
#define J2CPP_JAVA_NIO_CHARSET_CHARACTERCODINGEXCEPTION_HPP_DECL


namespace j2cpp { namespace java { namespace io { class IOException; } } }


#include <java/io/IOException.hpp>


namespace j2cpp {

namespace java { namespace nio { namespace charset {

	class CharacterCodingException;
	class CharacterCodingException
		: public object<CharacterCodingException>
	{
	public:

		J2CPP_DECLARE_CLASS

		J2CPP_DECLARE_METHOD(0)

		explicit CharacterCodingException(jobject jobj)
		: object<CharacterCodingException>(jobj)
		{
		}

		operator local_ref<java::io::IOException>() const;


		CharacterCodingException();
	}; //class CharacterCodingException

} //namespace charset
} //namespace nio
} //namespace java

} //namespace j2cpp

#endif //J2CPP_JAVA_NIO_CHARSET_CHARACTERCODINGEXCEPTION_HPP_DECL

#else //J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_NIO_CHARSET_CHARACTERCODINGEXCEPTION_HPP_IMPL
#define J2CPP_JAVA_NIO_CHARSET_CHARACTERCODINGEXCEPTION_HPP_IMPL

namespace j2cpp {



java::nio::charset::CharacterCodingException::operator local_ref<java::io::IOException>() const
{
	return local_ref<java::io::IOException>(get_jobject());
}


java::nio::charset::CharacterCodingException::CharacterCodingException()
: object<java::nio::charset::CharacterCodingException>(
	call_new_object<
		java::nio::charset::CharacterCodingException::J2CPP_CLASS_NAME,
		java::nio::charset::CharacterCodingException::J2CPP_METHOD_NAME(0),
		java::nio::charset::CharacterCodingException::J2CPP_METHOD_SIGNATURE(0)
	>()
)
{
}



J2CPP_DEFINE_CLASS(java::nio::charset::CharacterCodingException,"java/nio/charset/CharacterCodingException")
J2CPP_DEFINE_METHOD(java::nio::charset::CharacterCodingException,0,"<init>","()V")

} //namespace j2cpp

#endif //J2CPP_JAVA_NIO_CHARSET_CHARACTERCODINGEXCEPTION_HPP_IMPL

#endif //J2CPP_INCLUDE_IMPLEMENTATION