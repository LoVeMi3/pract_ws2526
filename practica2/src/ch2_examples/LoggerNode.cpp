#include "rclcpp/rclcpp.hpp"
#include "practica2/LoggerNode.hpp"

using namespace std::chrono_literals;

LoggerNode::LoggerNode()
: Node("logger_node")
{
  counter_ = 0;
  timer_ = create_wall_timer(
    500ms, std::bind(&LoggerNode::timer_callback, this));
}

void
LoggerNode::timer_callback()
{
  RCLCPP_INFO(get_logger(), "Hello %d", counter_++);
}

