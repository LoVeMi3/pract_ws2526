//main del nodo navFSM.cpp
#include <memory>

#include "practica4/navFSM.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node_det_obs = std::make_shared<practica4::navFSM>();

  rclcpp::spin(node_det_obs);

  rclcpp::shutdown();
  return 0;
}
