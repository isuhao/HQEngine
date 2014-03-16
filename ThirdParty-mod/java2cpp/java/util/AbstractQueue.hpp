/*================================================================================
  code generated by: java2cpp
  author: Zoran Angelov, mailto://baldzar@gmail.com
  class: java.util.AbstractQueue
================================================================================*/


#ifndef J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_UTIL_ABSTRACTQUEUE_HPP_DECL
#define J2CPP_JAVA_UTIL_ABSTRACTQUEUE_HPP_DECL


namespace j2cpp { namespace java { namespace lang { class Object; } } }
namespace j2cpp { namespace java { namespace util { class AbstractCollection; } } }
namespace j2cpp { namespace java { namespace util { class Queue; } } }
namespace j2cpp { namespace java { namespace util { class Collection; } } }


#include <java/lang/Object.hpp>
#include <java/util/AbstractCollection.hpp>
#include <java/util/Collection.hpp>
#include <java/util/Queue.hpp>


namespace j2cpp {

namespace java { namespace util {

	class AbstractQueue;
	class AbstractQueue
		: public object<AbstractQueue>
	{
	public:

		J2CPP_DECLARE_CLASS

		J2CPP_DECLARE_METHOD(0)
		J2CPP_DECLARE_METHOD(1)
		J2CPP_DECLARE_METHOD(2)
		J2CPP_DECLARE_METHOD(3)
		J2CPP_DECLARE_METHOD(4)
		J2CPP_DECLARE_METHOD(5)

		explicit AbstractQueue(jobject jobj)
		: object<AbstractQueue>(jobj)
		{
		}

		operator local_ref<java::util::AbstractCollection>() const;
		operator local_ref<java::util::Queue>() const;


		jboolean add(local_ref< java::lang::Object >  const&);
		jboolean addAll(local_ref< java::util::Collection >  const&);
		local_ref< java::lang::Object > remove();
		local_ref< java::lang::Object > element();
		void clear();
	}; //class AbstractQueue

} //namespace util
} //namespace java

} //namespace j2cpp

#endif //J2CPP_JAVA_UTIL_ABSTRACTQUEUE_HPP_DECL

#else //J2CPP_INCLUDE_IMPLEMENTATION

#ifndef J2CPP_JAVA_UTIL_ABSTRACTQUEUE_HPP_IMPL
#define J2CPP_JAVA_UTIL_ABSTRACTQUEUE_HPP_IMPL

namespace j2cpp {



java::util::AbstractQueue::operator local_ref<java::util::AbstractCollection>() const
{
	return local_ref<java::util::AbstractCollection>(get_jobject());
}

java::util::AbstractQueue::operator local_ref<java::util::Queue>() const
{
	return local_ref<java::util::Queue>(get_jobject());
}


jboolean java::util::AbstractQueue::add(local_ref< java::lang::Object > const &a0)
{
	return call_method<
		java::util::AbstractQueue::J2CPP_CLASS_NAME,
		java::util::AbstractQueue::J2CPP_METHOD_NAME(1),
		java::util::AbstractQueue::J2CPP_METHOD_SIGNATURE(1), 
		jboolean
	>(get_jobject(), a0);
}

jboolean java::util::AbstractQueue::addAll(local_ref< java::util::Collection > const &a0)
{
	return call_method<
		java::util::AbstractQueue::J2CPP_CLASS_NAME,
		java::util::AbstractQueue::J2CPP_METHOD_NAME(2),
		java::util::AbstractQueue::J2CPP_METHOD_SIGNATURE(2), 
		jboolean
	>(get_jobject(), a0);
}

local_ref< java::lang::Object > java::util::AbstractQueue::remove()
{
	return call_method<
		java::util::AbstractQueue::J2CPP_CLASS_NAME,
		java::util::AbstractQueue::J2CPP_METHOD_NAME(3),
		java::util::AbstractQueue::J2CPP_METHOD_SIGNATURE(3), 
		local_ref< java::lang::Object >
	>(get_jobject());
}

local_ref< java::lang::Object > java::util::AbstractQueue::element()
{
	return call_method<
		java::util::AbstractQueue::J2CPP_CLASS_NAME,
		java::util::AbstractQueue::J2CPP_METHOD_NAME(4),
		java::util::AbstractQueue::J2CPP_METHOD_SIGNATURE(4), 
		local_ref< java::lang::Object >
	>(get_jobject());
}

void java::util::AbstractQueue::clear()
{
	return call_method<
		java::util::AbstractQueue::J2CPP_CLASS_NAME,
		java::util::AbstractQueue::J2CPP_METHOD_NAME(5),
		java::util::AbstractQueue::J2CPP_METHOD_SIGNATURE(5), 
		void
	>(get_jobject());
}


J2CPP_DEFINE_CLASS(java::util::AbstractQueue,"java/util/AbstractQueue")
J2CPP_DEFINE_METHOD(java::util::AbstractQueue,0,"<init>","()V")
J2CPP_DEFINE_METHOD(java::util::AbstractQueue,1,"add","(Ljava/lang/Object;)Z")
J2CPP_DEFINE_METHOD(java::util::AbstractQueue,2,"addAll","(Ljava/util/Collection;)Z")
J2CPP_DEFINE_METHOD(java::util::AbstractQueue,3,"remove","()Ljava/lang/Object;")
J2CPP_DEFINE_METHOD(java::util::AbstractQueue,4,"element","()Ljava/lang/Object;")
J2CPP_DEFINE_METHOD(java::util::AbstractQueue,5,"clear","()V")

} //namespace j2cpp

#endif //J2CPP_JAVA_UTIL_ABSTRACTQUEUE_HPP_IMPL

#endif //J2CPP_INCLUDE_IMPLEMENTATION