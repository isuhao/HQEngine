/*================================================================================
  code generated by: java2cpp
  author: Zoran Angelov, mailto://baldzar@gmail.com
  class: java.nio.channels.SelectableChannel
================================================================================*/


#ifndef J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_NIO_CHANNELS_SELECTABLECHANNEL_HPP_DECL
#define J2CPP_JAVA_NIO_CHANNELS_SELECTABLECHANNEL_HPP_DECL


namespace j2cpp { namespace java { namespace lang { class Object; } } }
namespace j2cpp { namespace java { namespace nio { namespace channels { class SelectionKey; } } } }
namespace j2cpp { namespace java { namespace nio { namespace channels { namespace spi { class AbstractInterruptibleChannel; } } } } }
namespace j2cpp { namespace java { namespace nio { namespace channels { namespace spi { class SelectorProvider; } } } } }
namespace j2cpp { namespace java { namespace nio { namespace channels { class Channel; } } } }
namespace j2cpp { namespace java { namespace nio { namespace channels { class Selector; } } } }


#include <java/lang/Object.hpp>
#include <java/nio/channels/Channel.hpp>
#include <java/nio/channels/SelectionKey.hpp>
#include <java/nio/channels/Selector.hpp>
#include <java/nio/channels/spi/AbstractInterruptibleChannel.hpp>
#include <java/nio/channels/spi/SelectorProvider.hpp>


namespace j2cpp {

namespace java { namespace nio { namespace channels {

	class SelectableChannel;
	class SelectableChannel
		: public object<SelectableChannel>
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
		J2CPP_DECLARE_METHOD(9)

		explicit SelectableChannel(jobject jobj)
		: object<SelectableChannel>(jobj)
		{
		}

		operator local_ref<java::nio::channels::spi::AbstractInterruptibleChannel>() const;
		operator local_ref<java::nio::channels::Channel>() const;


		local_ref< java::lang::Object > blockingLock();
		local_ref< java::nio::channels::SelectableChannel > configureBlocking(jboolean);
		jboolean isBlocking();
		jboolean isRegistered();
		local_ref< java::nio::channels::SelectionKey > keyFor(local_ref< java::nio::channels::Selector >  const&);
		local_ref< java::nio::channels::spi::SelectorProvider > provider();
		local_ref< java::nio::channels::SelectionKey > register_(local_ref< java::nio::channels::Selector >  const&, jint);
		local_ref< java::nio::channels::SelectionKey > register_(local_ref< java::nio::channels::Selector >  const&, jint, local_ref< java::lang::Object >  const&);
		jint validOps();
	}; //class SelectableChannel

} //namespace channels
} //namespace nio
} //namespace java

} //namespace j2cpp

#endif //J2CPP_JAVA_NIO_CHANNELS_SELECTABLECHANNEL_HPP_DECL

#else //J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_NIO_CHANNELS_SELECTABLECHANNEL_HPP_IMPL
#define J2CPP_JAVA_NIO_CHANNELS_SELECTABLECHANNEL_HPP_IMPL

namespace j2cpp {



java::nio::channels::SelectableChannel::operator local_ref<java::nio::channels::spi::AbstractInterruptibleChannel>() const
{
	return local_ref<java::nio::channels::spi::AbstractInterruptibleChannel>(get_jobject());
}

java::nio::channels::SelectableChannel::operator local_ref<java::nio::channels::Channel>() const
{
	return local_ref<java::nio::channels::Channel>(get_jobject());
}


local_ref< java::lang::Object > java::nio::channels::SelectableChannel::blockingLock()
{
	return call_method<
		java::nio::channels::SelectableChannel::J2CPP_CLASS_NAME,
		java::nio::channels::SelectableChannel::J2CPP_METHOD_NAME(1),
		java::nio::channels::SelectableChannel::J2CPP_METHOD_SIGNATURE(1), 
		local_ref< java::lang::Object >
	>(get_jobject());
}

local_ref< java::nio::channels::SelectableChannel > java::nio::channels::SelectableChannel::configureBlocking(jboolean a0)
{
	return call_method<
		java::nio::channels::SelectableChannel::J2CPP_CLASS_NAME,
		java::nio::channels::SelectableChannel::J2CPP_METHOD_NAME(2),
		java::nio::channels::SelectableChannel::J2CPP_METHOD_SIGNATURE(2), 
		local_ref< java::nio::channels::SelectableChannel >
	>(get_jobject(), a0);
}

jboolean java::nio::channels::SelectableChannel::isBlocking()
{
	return call_method<
		java::nio::channels::SelectableChannel::J2CPP_CLASS_NAME,
		java::nio::channels::SelectableChannel::J2CPP_METHOD_NAME(3),
		java::nio::channels::SelectableChannel::J2CPP_METHOD_SIGNATURE(3), 
		jboolean
	>(get_jobject());
}

jboolean java::nio::channels::SelectableChannel::isRegistered()
{
	return call_method<
		java::nio::channels::SelectableChannel::J2CPP_CLASS_NAME,
		java::nio::channels::SelectableChannel::J2CPP_METHOD_NAME(4),
		java::nio::channels::SelectableChannel::J2CPP_METHOD_SIGNATURE(4), 
		jboolean
	>(get_jobject());
}

local_ref< java::nio::channels::SelectionKey > java::nio::channels::SelectableChannel::keyFor(local_ref< java::nio::channels::Selector > const &a0)
{
	return call_method<
		java::nio::channels::SelectableChannel::J2CPP_CLASS_NAME,
		java::nio::channels::SelectableChannel::J2CPP_METHOD_NAME(5),
		java::nio::channels::SelectableChannel::J2CPP_METHOD_SIGNATURE(5), 
		local_ref< java::nio::channels::SelectionKey >
	>(get_jobject(), a0);
}

local_ref< java::nio::channels::spi::SelectorProvider > java::nio::channels::SelectableChannel::provider()
{
	return call_method<
		java::nio::channels::SelectableChannel::J2CPP_CLASS_NAME,
		java::nio::channels::SelectableChannel::J2CPP_METHOD_NAME(6),
		java::nio::channels::SelectableChannel::J2CPP_METHOD_SIGNATURE(6), 
		local_ref< java::nio::channels::spi::SelectorProvider >
	>(get_jobject());
}

local_ref< java::nio::channels::SelectionKey > java::nio::channels::SelectableChannel::register_(local_ref< java::nio::channels::Selector > const &a0, jint a1)
{
	return call_method<
		java::nio::channels::SelectableChannel::J2CPP_CLASS_NAME,
		java::nio::channels::SelectableChannel::J2CPP_METHOD_NAME(7),
		java::nio::channels::SelectableChannel::J2CPP_METHOD_SIGNATURE(7), 
		local_ref< java::nio::channels::SelectionKey >
	>(get_jobject(), a0, a1);
}

local_ref< java::nio::channels::SelectionKey > java::nio::channels::SelectableChannel::register_(local_ref< java::nio::channels::Selector > const &a0, jint a1, local_ref< java::lang::Object > const &a2)
{
	return call_method<
		java::nio::channels::SelectableChannel::J2CPP_CLASS_NAME,
		java::nio::channels::SelectableChannel::J2CPP_METHOD_NAME(8),
		java::nio::channels::SelectableChannel::J2CPP_METHOD_SIGNATURE(8), 
		local_ref< java::nio::channels::SelectionKey >
	>(get_jobject(), a0, a1, a2);
}

jint java::nio::channels::SelectableChannel::validOps()
{
	return call_method<
		java::nio::channels::SelectableChannel::J2CPP_CLASS_NAME,
		java::nio::channels::SelectableChannel::J2CPP_METHOD_NAME(9),
		java::nio::channels::SelectableChannel::J2CPP_METHOD_SIGNATURE(9), 
		jint
	>(get_jobject());
}


J2CPP_DEFINE_CLASS(java::nio::channels::SelectableChannel,"java/nio/channels/SelectableChannel")
J2CPP_DEFINE_METHOD(java::nio::channels::SelectableChannel,0,"<init>","()V")
J2CPP_DEFINE_METHOD(java::nio::channels::SelectableChannel,1,"blockingLock","()Ljava/lang/Object;")
J2CPP_DEFINE_METHOD(java::nio::channels::SelectableChannel,2,"configureBlocking","(Z)Ljava/nio/channels/SelectableChannel;")
J2CPP_DEFINE_METHOD(java::nio::channels::SelectableChannel,3,"isBlocking","()Z")
J2CPP_DEFINE_METHOD(java::nio::channels::SelectableChannel,4,"isRegistered","()Z")
J2CPP_DEFINE_METHOD(java::nio::channels::SelectableChannel,5,"keyFor","(Ljava/nio/channels/Selector;)Ljava/nio/channels/SelectionKey;")
J2CPP_DEFINE_METHOD(java::nio::channels::SelectableChannel,6,"provider","()Ljava/nio/channels/spi/SelectorProvider;")
J2CPP_DEFINE_METHOD(java::nio::channels::SelectableChannel,7,"register","(Ljava/nio/channels/Selector;I)Ljava/nio/channels/SelectionKey;")
J2CPP_DEFINE_METHOD(java::nio::channels::SelectableChannel,8,"register","(Ljava/nio/channels/Selector;ILjava/lang/Object;)Ljava/nio/channels/SelectionKey;")
J2CPP_DEFINE_METHOD(java::nio::channels::SelectableChannel,9,"validOps","()I")

} //namespace j2cpp

#endif //J2CPP_JAVA_NIO_CHANNELS_SELECTABLECHANNEL_HPP_IMPL

#endif //J2CPP_INCLUDE_IMPLEMENTATION