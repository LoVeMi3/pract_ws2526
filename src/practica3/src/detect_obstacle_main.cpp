//main del nodo DetectObstacle.cpp
#include <memory>

#include "practica3/DetectObstacle.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node_detector = std::make_shared<practica3::DetectObstacle>();

  rclcpp::spin(node_detector);

  rclcpp::shutdown();
  return 0;
}
