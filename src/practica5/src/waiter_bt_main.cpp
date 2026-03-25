//main del nodo WaiterBT.cpp
#include <memory>
#include <string>

#include "practica5/WaiterBT.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto waiter_bt = std::make_shared<practica5::WaiterBT>();

  rclcpp::spin(waiter_bt);

  rclcpp::shutdown();
  return 0;
}