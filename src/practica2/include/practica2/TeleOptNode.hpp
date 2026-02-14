#ifndef PRACTICA2__TELEOPT_NODE_HPP_
#define PRACTICA2__TELEOPT_NODE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "kobuki_ros_interfaces/msg/bumper_event.hpp"
#include "geometry_msgs/msg/twist.hpp"

class TeleOptNode : public rclcpp::Node
{
public:
  TeleOptNode();

private:
  void bumper_callback(const kobuki_ros_interfaces::msg::BumperEvent::SharedPtr msg);
  void pub_speed();

  rclcpp::Subscription<kobuki_ros_interfaces::msg::BumperEvent>::SharedPtr bumper_sub_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr vel_pub_;
  rclcpp::TimerBase::SharedPtr timer_;

  //estados bumper
  bool left_pressed_;
  bool mid_pressed_;
  bool right_pressed_;

  //valores velocidad
  double linear_vel_ = 0.2;
  double angular_vel_ = 0.5;
};

#endif // PRACTICA2__TELEOPT_NODE_HPP_
