#ifndef PRACTICA5__WAIT_FOR_PERSON_ACTION_HPP
#define PRACTICA5__WAIT_FOR_PERSON_ACTION_HPP

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
#include "bt_examples/bt_nodes/SayTextClientAction.hpp"
#include "bt_examples/bt_nodes/ListenTextClientAction.hpp"
#include "bt_examples/bt_nodes/ExtractInfoClientAction.hpp"
#include "bt_examples/bt_nodes/bt_node_registration.hpp"

#include "practica5/GoToPoseAction.hpp"
#include "practica5/DetectionNode.hpp"
#include "practica5/WaiterBT.hpp"

namespace practica5 {

class WaitForPersonAction : public BT::StatefulActionNode
{
public:
  WaitForPersonAction(const std::string &name, const BT::NodeConfig &config)
  : BT::StatefulActionNode(name, config), person_detected_(false)
  {
    if (!config.blackboard->get("node", node_)) {
      throw BT::RuntimeError("WaitForPersonAction: missing 'node' in blackboard");
    }

    sub_ = node_->create_subscription<std_msgs::msg::Bool>("/person_detected", 10, [this](const std_msgs::msg::Bool::SharedPtr msg) {person_detected_ = msg->data;});
  }

  static BT::PortsList providedPorts() { return {};}

  BT::NodeStatus onStart() override
  {
    person_detected_ = false;
    RCLCPP_INFO(node_->get_logger(), "WaitForPersonAction: waiting for person...");
    return BT::NodeStatus::RUNNING;
  }
  BT::NodeStatus onRunning() override
  {
    if (person_detected_) {
      RCLCPP_INFO(node_->get_logger(), "WaitForPersonAction: person detected");
      return BT::NodeStatus::SUCCESS;
    }
    return BT::NodeStatus::RUNNING;
  }
  void onHalted() override
  {
    RCLCPP_WARN(node_->get_logger(), "WaitForPersonAction: halted");
  }

private:
  rclcpp::Node::SharedPtr node_;
  rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr sub_;
  bool person_detected_;
};

} //namespace practica5

#endif //PRACTICA5__WAIT_FOR_PERSON_ACTION_HPP