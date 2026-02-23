//main del nodo DetectObstacle.cpp
#include <memory>

#include "practica3/DetectObstacle.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node_det_obs = std::make_shared<practica3::DetectObstacle>();

  rclcpp::spin(node_det_obs);

  rclcpp::shutdown();
  return 0;
}
