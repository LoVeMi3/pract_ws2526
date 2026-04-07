#ifndef PRACTICA5__WAITER_BT_HPP
#define PRACTICA5__WAITER_BT_HPP

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <string>

#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/behavior_tree.h"
#include "behaviortree_cpp/loggers/bt_cout_logger.h"
#include "ament_index_cpp/get_package_share_directory.hpp"

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_msgs/msg/bool.hpp"

#include "hri_client/hri_client.hpp"
#include "bt_examples/bt_nodes/say_text_client_action.hpp"
#include "bt_examples/bt_nodes/listen_text_client_action.hpp"
#include "bt_examples/bt_nodes/extract_info_client_action.hpp"
#include "bt_examples/bt_nodes/bt_node_registration.hpp"
//#include "bt_examples/bt_nodes/SayTextClientAction.hpp"
//#include "bt_examples/bt_nodes/ListenTextClientAction.hpp"
//#include "bt_examples/bt_nodes/ExtractInfoClientAction.hpp"
//#include "bt_examples/bt_nodes/bt_node_registration.hpp"

#include "practica5/GoToPoseAction.hpp"
#include "practica5/WaitForPersonAction.hpp"
#include "practica5/DetectionNode.hpp"

namespace practica5 {

class WaiterBT : public rclcpp::Node
{
public:
  WaiterBT();
  BT::NodeStatus tick(); //el tick de un estado al otro en el tree

  bool isRunning() const; //tree running

private:
  BT::BehaviorTreeFactory factory_;
  BT::Tree tree_;
  BT::Blackboard::Ptr blackboard_;

  rclcpp::shared_ptr<HRIClient::HRIClient> hri_client_;
  //estos cliente de aquí abajo se crean en el hri_client.hpp y .cpp, nodo de los profes
  //rclcpp::Client<std_srvs::srv::SetBool>::SharedPtr stt_client_;
  //rclcpp::Client<bt_examples::bt_nodes::SayTextClientAction>::SharedPtr stt_client_;

  //rclcpp::Client<simple_hri_interfaces::srv::Speech>::SharedPtr tts_client_;
  //rclcpp::Client<bt_examples::bt_nodes::ListenTextClientAction>::SharedPtr tts_client_;

  //rclcpp::Client<simple_hri_interfaces::srv::Extract>::SharedPtr extract_client_;
  //rclcpp::Client<bt_examples::bt_nodes::ExtractInfoClientAction>::SharedPtr extract_client_;

  void register_nodes();
  void create_tree();
};

} //namespace practica5

#endif //PRACTICA5__WAITER_BT_HPP
