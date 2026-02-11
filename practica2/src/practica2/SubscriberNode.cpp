#include <iostream>
#include <cstdio>

namespace include {

SubscriberNode::SubscriberNode()
: Node("subscriber_node")
{
	counter_subscription_ = create_subscription<kobuki_ros_interfaces/msg/BumperEvent>("counter", 10, std::bind(&SubscriberNode::subscription_callback, this, std::placeholders::_1);
}

}
