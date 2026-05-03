//el WaiterBT.cpp

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

#include "practica5/WaiterBT.hpp"
#include "practica5/GoToPoseAction.hpp"
#include "practica5/WaitForPersonAction.hpp"
#include "practica5/DetectionNode.hpp"
#include "practica5/GetClientPoseAction.hpp"

// executable='waiter_bt' VERYIMPORTANT

using namespace std::chrono_literals;

namespace practica5 {

WaiterBT::WaiterBT()
: Node("waiter_bt")
{
  blackboard_ = BT::Blackboard::create();
}

void
WaiterBT::init()
{
  blackboard_->set("node", shared_from_this());
 
  hri_client_ = std::make_shared<HRIClient::HRIClient>(shared_from_this());
  blackboard_->set("hri_client", hri_client_);
 
  register_nodes();
  create_tree();
}

void
WaiterBT::register_nodes()
{
  factory_.registerNodeType<SayTextClientAction>("SayTextClient");
  factory_.registerNodeType<ListenTextClientAction>("ListenTextClient");
  factory_.registerNodeType<ExtractInfoClientAction>("ExtractInfoClient");

  factory_.registerNodeType<GoToPoseAction>("GoToPose");
  factory_.registerNodeType<WaitForPersonAction>("WaitForPerson");
  factory_.registerNodeType<GetClientPoseAction>("GetClientPoseAction");
}

void
WaiterBT::create_tree()
{
  std::string xml_path = ament_index_cpp::get_package_share_directory("practica5") + "/config/waiter_mission.xml";
  RCLCPP_INFO(get_logger(), "Loading BT from: %s", xml_path.c_str());

  tree_ = factory_.createTreeFromFile(xml_path, blackboard_);
}

BT::NodeStatus
WaiterBT::tick()
{
  return tree_.tickOnce();
}

bool
WaiterBT::isRunning() const
{
  return tree_.rootNode()->status() == BT::NodeStatus::RUNNING;
}

} //namespace practica5
