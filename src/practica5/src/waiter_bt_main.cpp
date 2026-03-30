//main del nodo WaiterBT.cpp
#include <memory>
#include <string>

#include "practica5/WaiterBT.hpp"
#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto waiter_bt = std::make_shared<practica5::WaiterBT>();

  rclcpp::Rate rate(10); //10hz

  while (rclcpp::ok()) {
    BT::NodeStatus status = waiter_bt->tick();

    if (status == BT::NodeStatus::SUCCESS) {
      RCLCPP_INFO(waiter_bt->get_logger(), "Mission completed successfully");
      break;
    } else if (status == BT::NodeStatus::FAILURE) {
      RCLCPP_ERROR(waiter_bt->get_logger(), "Mission failed");
      break;
    }
    //si no es ninguno de los dos estados, estamos RUNNING, seguimos con los ticks
    rclcpp::spin_some(waiter_bt);
    rate.sleep();
  }

  rclcpp::shutdown();
  return 0;
}