
#include "rclcpp/rclcpp.hpp"

#include "practica2/PublisherNode.hpp"
#include "practica2/SubscriberNode.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto pub_node = std::make_shared<PublisherNode>();
  auto sub_node = std::make_shared<SubscriberNode>();

  rclcpp::executors::SingleThreadedExecutor executor;
  executor.add_node(pub_node);
  executor.add_node(sub_node);
  executor.spin();

  rclcpp::shutdown();
  return 0;
}
