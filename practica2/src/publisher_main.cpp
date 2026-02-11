#include "rclcpp/rclcpp.hpp"
#include "practica2/PublisherNode.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<PublisherNode>();

  rclcpp::spin(node);

  rclcpp::shutdown();
  return 0;
}
