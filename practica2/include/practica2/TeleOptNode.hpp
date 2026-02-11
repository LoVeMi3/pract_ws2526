#include "rclcpp/rclcpp.hpp"

class TeleOptNode : publis rclcpp::Node
{
public:
	LoggerNode();
	void timer_callback();
	
private:
	rclcpp::TimerBase::SharedPtr timer_;
	int counter_;
}
