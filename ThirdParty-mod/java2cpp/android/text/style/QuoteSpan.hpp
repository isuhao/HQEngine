/*================================================================================
  code generated by: java2cpp
  author: Zoran Angelov, mailto://baldzar@gmail.com
  class: android.text.style.QuoteSpan
================================================================================*/


#ifndef J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_ANDROID_TEXT_STYLE_QUOTESPAN_HPP_DECL
#define J2CPP_ANDROID_TEXT_STYLE_QUOTESPAN_HPP_DECL


namespace j2cpp { namespace java { namespace lang { class Object; } } }
namespace j2cpp { namespace java { namespace lang { class CharSequence; } } }
namespace j2cpp { namespace android { namespace graphics { class Paint; } } }
namespace j2cpp { namespace android { namespace graphics { class Canvas; } } }
namespace j2cpp { namespace android { namespace text { class Layout; } } }
namespace j2cpp { namespace android { namespace text { namespace style { class LeadingMarginSpan; } } } }
namespace j2cpp { namespace android { namespace text { class ParcelableSpan; } } }
namespace j2cpp { namespace android { namespace os { class Parcel; } } }


#include <android/graphics/Canvas.hpp>
#include <android/graphics/Paint.hpp>
#include <android/os/Parcel.hpp>
#include <android/text/Layout.hpp>
#include <android/text/ParcelableSpan.hpp>
#include <android/text/style/LeadingMarginSpan.hpp>
#include <java/lang/CharSequence.hpp>
#include <java/lang/Object.hpp>


namespace j2cpp {

namespace android { namespace text { namespace style {

	class QuoteSpan;
	class QuoteSpan
		: public object<QuoteSpan>
	{
	public:

		J2CPP_DECLARE_CLASS

		J2CPP_DECLARE_METHOD(0)
		J2CPP_DECLARE_METHOD(1)
		J2CPP_DECLARE_METHOD(2)
		J2CPP_DECLARE_METHOD(3)
		J2CPP_DECLARE_METHOD(4)
		J2CPP_DECLARE_METHOD(5)
		J2CPP_DECLARE_METHOD(6)
		J2CPP_DECLARE_METHOD(7)
		J2CPP_DECLARE_METHOD(8)

		explicit QuoteSpan(jobject jobj)
		: object<QuoteSpan>(jobj)
		{
		}

		operator local_ref<java::lang::Object>() const;
		operator local_ref<android::text::style::LeadingMarginSpan>() const;
		operator local_ref<android::text::ParcelableSpan>() const;


		QuoteSpan();
		QuoteSpan(jint);
		QuoteSpan(local_ref< android::os::Parcel > const&);
		jint getSpanTypeId();
		jint describeContents();
		void writeToParcel(local_ref< android::os::Parcel >  const&, jint);
		jint getColor();
		jint getLeadingMargin(jboolean);
		void drawLeadingMargin(local_ref< android::graphics::Canvas >  const&, local_ref< android::graphics::Paint >  const&, jint, jint, jint, jint, jint, local_ref< java::lang::CharSequence >  const&, jint, jint, jboolean, local_ref< android::text::Layout >  const&);
	}; //class QuoteSpan

} //namespace style
} //namespace text
} //namespace android

} //namespace j2cpp

#endif //J2CPP_ANDROID_TEXT_STYLE_QUOTESPAN_HPP_DECL

#else //J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_ANDROID_TEXT_STYLE_QUOTESPAN_HPP_IMPL
#define J2CPP_ANDROID_TEXT_STYLE_QUOTESPAN_HPP_IMPL

namespace j2cpp {



android::text::style::QuoteSpan::operator local_ref<java::lang::Object>() const
{
	return local_ref<java::lang::Object>(get_jobject());
}

android::text::style::QuoteSpan::operator local_ref<android::text::style::LeadingMarginSpan>() const
{
	return local_ref<android::text::style::LeadingMarginSpan>(get_jobject());
}

android::text::style::QuoteSpan::operator local_ref<android::text::ParcelableSpan>() const
{
	return local_ref<android::text::ParcelableSpan>(get_jobject());
}


android::text::style::QuoteSpan::QuoteSpan()
: object<android::text::style::QuoteSpan>(
	call_new_object<
		android::text::style::QuoteSpan::J2CPP_CLASS_NAME,
		android::text::style::QuoteSpan::J2CPP_METHOD_NAME(0),
		android::text::style::QuoteSpan::J2CPP_METHOD_SIGNATURE(0)
	>()
)
{
}



android::text::style::QuoteSpan::QuoteSpan(jint a0)
: object<android::text::style::QuoteSpan>(
	call_new_object<
		android::text::style::QuoteSpan::J2CPP_CLASS_NAME,
		android::text::style::QuoteSpan::J2CPP_METHOD_NAME(1),
		android::text::style::QuoteSpan::J2CPP_METHOD_SIGNATURE(1)
	>(a0)
)
{
}



android::text::style::QuoteSpan::QuoteSpan(local_ref< android::os::Parcel > const &a0)
: object<android::text::style::QuoteSpan>(
	call_new_object<
		android::text::style::QuoteSpan::J2CPP_CLASS_NAME,
		android::text::style::QuoteSpan::J2CPP_METHOD_NAME(2),
		android::text::style::QuoteSpan::J2CPP_METHOD_SIGNATURE(2)
	>(a0)
)
{
}


jint android::text::style::QuoteSpan::getSpanTypeId()
{
	return call_method<
		android::text::style::QuoteSpan::J2CPP_CLASS_NAME,
		android::text::style::QuoteSpan::J2CPP_METHOD_NAME(3),
		android::text::style::QuoteSpan::J2CPP_METHOD_SIGNATURE(3), 
		jint
	>(get_jobject());
}

jint android::text::style::QuoteSpan::describeContents()
{
	return call_method<
		android::text::style::QuoteSpan::J2CPP_CLASS_NAME,
		android::text::style::QuoteSpan::J2CPP_METHOD_NAME(4),
		android::text::style::QuoteSpan::J2CPP_METHOD_SIGNATURE(4), 
		jint
	>(get_jobject());
}

void android::text::style::QuoteSpan::writeToParcel(local_ref< android::os::Parcel > const &a0, jint a1)
{
	return call_method<
		android::text::style::QuoteSpan::J2CPP_CLASS_NAME,
		android::text::style::QuoteSpan::J2CPP_METHOD_NAME(5),
		android::text::style::QuoteSpan::J2CPP_METHOD_SIGNATURE(5), 
		void
	>(get_jobject(), a0, a1);
}

jint android::text::style::QuoteSpan::getColor()
{
	return call_method<
		android::text::style::QuoteSpan::J2CPP_CLASS_NAME,
		android::text::style::QuoteSpan::J2CPP_METHOD_NAME(6),
		android::text::style::QuoteSpan::J2CPP_METHOD_SIGNATURE(6), 
		jint
	>(get_jobject());
}

jint android::text::style::QuoteSpan::getLeadingMargin(jboolean a0)
{
	return call_method<
		android::text::style::QuoteSpan::J2CPP_CLASS_NAME,
		android::text::style::QuoteSpan::J2CPP_METHOD_NAME(7),
		android::text::style::QuoteSpan::J2CPP_METHOD_SIGNATURE(7), 
		jint
	>(get_jobject(), a0);
}

void android::text::style::QuoteSpan::drawLeadingMargin(local_ref< android::graphics::Canvas > const &a0, local_ref< android::graphics::Paint > const &a1, jint a2, jint a3, jint a4, jint a5, jint a6, local_ref< java::lang::CharSequence > const &a7, jint a8, jint a9, jboolean a10, local_ref< android::text::Layout > const &a11)
{
	return call_method<
		android::text::style::QuoteSpan::J2CPP_CLASS_NAME,
		android::text::style::QuoteSpan::J2CPP_METHOD_NAME(8),
		android::text::style::QuoteSpan::J2CPP_METHOD_SIGNATURE(8), 
		void
	>(get_jobject(), a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
}


J2CPP_DEFINE_CLASS(android::text::style::QuoteSpan,"android/text/style/QuoteSpan")
J2CPP_DEFINE_METHOD(android::text::style::QuoteSpan,0,"<init>","()V")
J2CPP_DEFINE_METHOD(android::text::style::QuoteSpan,1,"<init>","(I)V")
J2CPP_DEFINE_METHOD(android::text::style::QuoteSpan,2,"<init>","(Landroid/os/Parcel;)V")
J2CPP_DEFINE_METHOD(android::text::style::QuoteSpan,3,"getSpanTypeId","()I")
J2CPP_DEFINE_METHOD(android::text::style::QuoteSpan,4,"describeContents","()I")
J2CPP_DEFINE_METHOD(android::text::style::QuoteSpan,5,"writeToParcel","(Landroid/os/Parcel;I)V")
J2CPP_DEFINE_METHOD(android::text::style::QuoteSpan,6,"getColor","()I")
J2CPP_DEFINE_METHOD(android::text::style::QuoteSpan,7,"getLeadingMargin","(Z)I")
J2CPP_DEFINE_METHOD(android::text::style::QuoteSpan,8,"drawLeadingMargin","(Landroid/graphics/Canvas;Landroid/graphics/Paint;IIIIILjava/lang/CharSequence;IIZLandroid/text/Layout;)V")

} //namespace j2cpp

#endif //J2CPP_ANDROID_TEXT_STYLE_QUOTESPAN_HPP_IMPL

#endif //J2CPP_INCLUDE_IMPLEMENTATION