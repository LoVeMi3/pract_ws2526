//main del nodo Detection2DNode.cpp
#include <memory>

#include "practica3/Detection2DNode.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node_detector = std::make_shared<practica3::Detection2DNode>();

  rclcpp::spin(node_detector);

  rclcpp::shutdown();
  return 0;
}
