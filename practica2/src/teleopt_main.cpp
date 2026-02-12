#include "rclcpp/rclcpp.hpp"
#include "practica2/TeleOptNodeNode.hpp"

int main(int argc, char *argv[])
{
	rclcpp::init(argc, argv);
	auto node = std::make_shared<TeleOptNode>();
	rclcpp::spin(node);
	rclcpp::shutdown();
	return 0;
}
