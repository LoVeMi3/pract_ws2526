/* main del nodo de orientación */

#include <memory>

#include "practica3/OrientationNode.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node_orientation = std::make_shared<practica3::OrientationNode>();

  rclcpp::spin(node_orientation);

  rclcpp::shutdown();
  return 0;
}
