#include "rclcpp/rclcpp.hpp"
#include "practica2/TeleOptNode.hpp"

using namespace std::chrono_literals;

TeleOptNode::TeleOptNode()
: Node("teleopt_node")
{
  counter_ = 0;
  timer_ = create_wall_timer(
    500ms, std::bind(&TeleOptNode::timer_callback, this));
}

void
TeleOptNode::timer_callback()
{
  RCLCPP_INFO(get_logger(), "Hello %d", counter_++);
}

