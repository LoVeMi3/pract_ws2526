#ifndef CH2_EXAMPLES__PUBLISHER_NODE_HPP_
#define CH2_EXAMPLES__PUBLISHER_NODE_HPP_

#include "geometry_msgs/msg/Twist.hpp"

#include "rclcpp/rclcpp.hpp"

class PublisherNode : public rclcpp::Node
{
public:
  PublisherNode();

  void timer_callback();

private:
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<geometry_msgs/msg/Twist>::SharedPtr publisher_;
  geometry_msgs::msg::Twist counter_message_;
};

#endif // CH2_EXAMPLES__PUBLISHER_NODE_HPP_
