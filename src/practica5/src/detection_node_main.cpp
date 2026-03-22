//main del nodo DetectionNode.cpp
#include <memory>

#include "practica5/DetectionNode.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto detection_node = std::make_shared<practica5::DetectionNode>();

  rclcpp::spin(detection_node);

  rclcpp::shutdown();
  return 0;
}