//main de nodo DetectionNode3d.cpp
//main del nodo DetectObstacle.cpp
#include <memory>

#include "practica3/DetectionNode3D.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node_det_tresd = std::make_shared<practica3::DetectionNode3D>();

  rclcpp::spin(node_det_tresd);

  rclcpp::shutdown();
  return 0;
}

