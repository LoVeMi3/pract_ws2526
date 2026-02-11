#include "rclcpp/rclcpp.hpp"
#include "ch2_examples/PublisherNode.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<PublisherNode>();

  rclcpp::spin(node);

  rclcpp::shutdown();
  return 0;
}
