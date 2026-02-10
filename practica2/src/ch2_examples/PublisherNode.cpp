#include <iostream>
#include <cstdio>

#include "PublisherNode.hpp"

namespace include {

PublisherNode::PublisherNode()
: Node("publisher_node")
{
	publisher = create_publisher<geometry_msgs/msg/Twist>("counter", 10)
}
//mirar mensajes de nodos y topics, comprobar qué tipos de datos se publican
//los estados

void PublisherNode::timer_callback()
{
	RCLCPP_INFO(get_logger(), "Publishing %d", counter_message_.data++);
	publisher_->publish(counter_message_);
}

}
