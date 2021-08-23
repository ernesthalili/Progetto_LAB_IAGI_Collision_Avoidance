// Generated by gencpp from file collision_avoidance/Istruzione.msg
// DO NOT EDIT!


#ifndef COLLISION_AVOIDANCE_MESSAGE_ISTRUZIONE_H
#define COLLISION_AVOIDANCE_MESSAGE_ISTRUZIONE_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace collision_avoidance
{
template <class ContainerAllocator>
struct Istruzione_
{
  typedef Istruzione_<ContainerAllocator> Type;

  Istruzione_()
    : linear_velocity(0.0)
    , angular_velocity(0.0)  {
    }
  Istruzione_(const ContainerAllocator& _alloc)
    : linear_velocity(0.0)
    , angular_velocity(0.0)  {
  (void)_alloc;
    }



   typedef float _linear_velocity_type;
  _linear_velocity_type linear_velocity;

   typedef float _angular_velocity_type;
  _angular_velocity_type angular_velocity;





  typedef boost::shared_ptr< ::collision_avoidance::Istruzione_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::collision_avoidance::Istruzione_<ContainerAllocator> const> ConstPtr;

}; // struct Istruzione_

typedef ::collision_avoidance::Istruzione_<std::allocator<void> > Istruzione;

typedef boost::shared_ptr< ::collision_avoidance::Istruzione > IstruzionePtr;
typedef boost::shared_ptr< ::collision_avoidance::Istruzione const> IstruzioneConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::collision_avoidance::Istruzione_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::collision_avoidance::Istruzione_<ContainerAllocator> >::stream(s, "", v);
return s;
}


template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator==(const ::collision_avoidance::Istruzione_<ContainerAllocator1> & lhs, const ::collision_avoidance::Istruzione_<ContainerAllocator2> & rhs)
{
  return lhs.linear_velocity == rhs.linear_velocity &&
    lhs.angular_velocity == rhs.angular_velocity;
}

template<typename ContainerAllocator1, typename ContainerAllocator2>
bool operator!=(const ::collision_avoidance::Istruzione_<ContainerAllocator1> & lhs, const ::collision_avoidance::Istruzione_<ContainerAllocator2> & rhs)
{
  return !(lhs == rhs);
}


} // namespace collision_avoidance

namespace ros
{
namespace message_traits
{





template <class ContainerAllocator>
struct IsFixedSize< ::collision_avoidance::Istruzione_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::collision_avoidance::Istruzione_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::collision_avoidance::Istruzione_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::collision_avoidance::Istruzione_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::collision_avoidance::Istruzione_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::collision_avoidance::Istruzione_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::collision_avoidance::Istruzione_<ContainerAllocator> >
{
  static const char* value()
  {
    return "830d49d85ef543fb78a32609382932d6";
  }

  static const char* value(const ::collision_avoidance::Istruzione_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x830d49d85ef543fbULL;
  static const uint64_t static_value2 = 0x78a32609382932d6ULL;
};

template<class ContainerAllocator>
struct DataType< ::collision_avoidance::Istruzione_<ContainerAllocator> >
{
  static const char* value()
  {
    return "collision_avoidance/Istruzione";
  }

  static const char* value(const ::collision_avoidance::Istruzione_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::collision_avoidance::Istruzione_<ContainerAllocator> >
{
  static const char* value()
  {
    return "float32 linear_velocity\n"
"float32 angular_velocity\n"
;
  }

  static const char* value(const ::collision_avoidance::Istruzione_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::collision_avoidance::Istruzione_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.linear_velocity);
      stream.next(m.angular_velocity);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct Istruzione_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::collision_avoidance::Istruzione_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::collision_avoidance::Istruzione_<ContainerAllocator>& v)
  {
    s << indent << "linear_velocity: ";
    Printer<float>::stream(s, indent + "  ", v.linear_velocity);
    s << indent << "angular_velocity: ";
    Printer<float>::stream(s, indent + "  ", v.angular_velocity);
  }
};

} // namespace message_operations
} // namespace ros

#endif // COLLISION_AVOIDANCE_MESSAGE_ISTRUZIONE_H
