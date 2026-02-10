#ifndef CH2_EXAMPLES__SUBSCRIBER_NODE_HPP_
#define CH2_EXAMPLES__SUBSCRIBER_NODE_HPP_

#include "kobuki_ros_interfaces/msg/BumperEvent.hpp"

#include "rclcpp/rclcpp.hpp"

class SubscriberNode : public rclcpp::Node
{
public:
  SubscriberNode();

  void subscription_callback(const kobuki_ros_interfaces::msg::BumperEvent::SharedPtr msg);

private:
  rclcpp::Subscription<kobuki_ros_interfaces/msg/BumperEvent>::SharedPtr counter_subscription_;
};

#endif // CH2_EXAMPLES__SUBSCRIBER_NODE_HPP_
