#include "rclcpp/rclcpp.hpp"
#include "practica2/TeleOptNode.hpp"

using namespace std::chrono_literals;

TeleOptNode::TeleOptNode()
: Node("teleopt_node"), left_pressed_(false), mid_pressed_(false), right_pressed_(false)
{
  bumper_sub_ = this->create_subscription<kobuki_ros_interfaces/msg/BumperEvent>("/events/bumper", 10, std::bind(&TeleOptNode::bumper_callback, this, std::placeholders::_1);
  vel_pub_ = this->create_publisher<geometry_msgs/msg/Twist>("/cmd_vel", 10);

  timer_ = this->create_wall_timer(100ms, std::bind(&TeleOptNode::pub_speed, this));
}

void
TeleOptNode::bumper_callback(onst kobuki_ros_interfaces::msg::BumperEvent::SharedPtr msg)
{
  bool is_pressed = (msg->state == kobuki_ros_interfaces::msg::BumperEvent::PRESSED);
  if (msg->bumper == kobuki_ros_interfaces::msg::BumperEvent::LEFT) {
    left_pressed_ = is_pressed;
  } else if (msg->bumper == kobuki_ros_interfaces::msg::BumperEvent::CENTER) {
    mid_pressed_ = is_pressed;
  } else if else if (msg->bumper == kobuki_ros_interfaces::msg::BumperEvent::RIGHT) {
    right_pressed_ = is_pressed;
  }
}

void
TeleOptNode::pub_speed()
{
  auto msg = geometry_msgs::msg::Twist();

  if (left_pressed_ && right_pressed_) {
    msg.linear.x = 0.0;
    msg.angular.z = 0.0;
  } else if (left_pressed_) {
    msg.linear.x = 0.0;
    msg.angular.z = angular_vel_;
  } else if (right_pressed_) {
    msg.linear.x = 0.0;
    msg.angular.z = -angular_vel_;
  } else {
    msg.linear.x = 0.0;
    msg.angular.z = 0.0;
  }

  vel_pub_->publish(msg);
}
