//PUEDO REUTILIZAR EL QUE YA DAN LOS PROFES

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <string>
#include <map>

#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/behavior_tree.h"
#include "behaviortree_cpp/loggers/bt_cout_logger.h"
#include "ament_index_cpp/get_package_share_directory.hpp"

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_msgs/msg/bool.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "nav2_msgs/action/navigate_to_pose.hpp"

#include "practica5/WaiterBT.hpp"
#include "practica5/GoToPoseAction.hpp"
#include "practica5/WaitForPersonAction.hpp"
#include "practica5/DetectionNode.hpp"

// executable='go_to_pose' VERYIMPORTANT

using namespace std::chrono_literals;

namespace practica5 {

const std::map<std::string, std::array<double, 3>> GoToPoseAction::waypoints_ = {
  {"kitchen",  {2.0,  1.0, 1.0}},
  {"client",   {0.5, -1.0, 1.0}},
  {"waiting",  {0.0,  0.0, 1.0}},
};

GoToPoseAction::GoToPoseAction (
  const std::string &name, const BT::NodeConfig &config)
: BT::StatefulActionNode(name, config)
{
  if (!config.blackboard->get("node", node_)) {
    throw BT::RuntimeError("GoToPoseAction: missing 'node' in blackboard");
  }

  nav_client_ = rclcpp_action::create_client<NavigateToPose>(node_, "navigate_to_pose");
}

BT::NodeStatus
GoToPoseAction::onStart()
{
  std::string goal_name;
  if (!getInput("goal", goal_name)) {
    RCLCPP_ERROR(node_->get_logger(), "GoToPoseAction: 'goal' port not found");
    return BT::NodeStatus::FAILURE;
  }

  auto there = waypoints_.find(goal_name);
  if (there == waypoints_.end()) {
    RCLCPP_ERROR(node_->get_logger(), "GoToPoseAction: unknown goal '%s' ", goal_name.c_str());
    return BT::NodeStatus::FAILURE;
  }

  if (!nav_client_->wait_for_action_server(std::chrono::seconds(5))) {
    RCLCPP_ERROR(node_->get_logger(), "GoToPoseAction: Nav2 action server not available");
    return BT::NodeStatus::FAILURE;
  }

  NavigateToPose::Goal goal_msg;
  goal_msg.pose.header.frame_id = "map";
  goal_msg.pose.header.stamp = node_->get_clock()->now();
  
  double goal_x = there->second[0];
  double goal_y = there->second[1];
  double goal_w = there->second[2];

  if (goal_name == "client") {
    geometry_msgs::msg::PoseStamped dynamic_pose;
    if (getInput("client_pose", dynamic_pose)) {
      goal_msg.pose.pose = dynamic_pose.pose;
      goal_x = dynamic_pose.pose.position.x;
      goal_y = dynamic_pose.pose.position.y;
      RCLCPP_INFO(node_->get_logger(), "GoToPoseAction: usando pose dinámica del cliente (%.2f, %.2f)", goal_x, goal_y);
    } else {
      goal_msg.pose.pose.position.x    = goal_x;
      goal_msg.pose.pose.position.y    = goal_y;
      goal_msg.pose.pose.position.z    = 0.0;
      goal_msg.pose.pose.orientation.w = goal_w;
      goal_msg.pose.pose.orientation.z = 0.0;
      RCLCPP_WARN(node_->get_logger(), "GoToPoseAction: sin pose dinámica, usando coordenada estática del cliente");
    }
  } else {
    goal_msg.pose.pose.position.x    = goal_x;
    goal_msg.pose.pose.position.y    = goal_y;
    goal_msg.pose.pose.position.z    = 0.0;
    goal_msg.pose.pose.orientation.w = goal_w;
    goal_msg.pose.pose.orientation.z = 0.0;
  }

  RCLCPP_INFO(node_->get_logger(), "GoToPoseAction: navigating to '%s' (%.2f, %.2f)", goal_name.c_str(), goal_x, goal_y);

  goal_handle_future_ = nav_client_->async_send_goal(goal_msg);
  goal_sent_ = true;

  return BT::NodeStatus::RUNNING;
}

BT::NodeStatus
GoToPoseAction::onRunning()
{
  if (!goal_sent_) {
    return BT::NodeStatus::FAILURE;
  }

  if (!goal_handle_) {
    if (goal_handle_future_.wait_for(0ms) == std::future_status::ready) {
      goal_handle_ = goal_handle_future_.get();
      if (!goal_handle_) {
        RCLCPP_ERROR(node_->get_logger(), "GoToPoseAction: goal was rejected");
        return BT::NodeStatus::FAILURE;
      }
    }
    return BT::NodeStatus::RUNNING;
  }

  auto result_future = nav_client_->async_get_result(goal_handle_);
  if (result_future.wait_for(0ms) == std::future_status::ready) {
    auto result = result_future.get();
    goal_handle_ = nullptr;
    goal_sent_ = false;

    if (result.code == rclcpp_action::ResultCode::SUCCEEDED) {
      RCLCPP_INFO(node_->get_logger(), "GoToPoseAction: goal reached");
      return BT::NodeStatus::SUCCESS;
    } else {
      RCLCPP_ERROR(node_->get_logger(), "GoToPoseAction: navigation failed");
      return BT::NodeStatus::FAILURE;
    }
  }

  return BT::NodeStatus::RUNNING;
}

void
GoToPoseAction::onHalted()
{
  if (goal_handle_) {
    nav_client_->async_cancel_goal(goal_handle_);
    goal_handle_ = nullptr;
  }

  goal_sent_ = false;
  RCLCPP_WARN(node_->get_logger(), "GoToPoseAction: halted, goal cancelled");
}

} //namespace practica5
