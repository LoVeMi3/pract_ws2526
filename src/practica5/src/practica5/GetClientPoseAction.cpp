// nodo GetClientPoseAction

#include "practica5/GetClientPoseAction.hpp"

#include "rclcpp/rclcpp.hpp"
#include "behaviortree_cpp/behavior_tree.h"
#include "geometry_msgs/msg/pose_stamped.hpp"

namespace practica5 {

GetClientPoseAction::GetClientPoseAction(const std::string & name, const BT::NodeConfig & config)
: BT::SyncActionNode(name, config)
{
  if (!config.blackboard->get("node", node_)) {
    throw BT::RuntimeError("GetClientPoseAction: missing 'node' in blackboard");
  }
  
  sub = node_->create_subscription<geometry_msgs::msg::PoseStamped>(
  "/person_pose", 10, [this](const geometry_msgs::msg::PoseStamped::SharedPtr msg) {
    last_pose_ = *msg;
    pose_received_ = true;
    });
}

BT::NodeStatus 
GetClientPoseAction::tick()
{
  pose_received_ = false;
  
  rclcpp::Rate rate(20);
  auto deadline = node_->get_clock()->now() + rclcpp::Duration::from_seconds(3.0);

  while (rclcpp::ok() && !pose_received_) {
    rclcpp::spin_some(node_);
    if (node_->get_clock()->now() > deadline) break;
    rate.sleep();
  }

  if (!pose_received_) {
    RCLCPP_WARN(node_->get_logger(), "GetClientPoseAction: no pose received within timeout");
    return BT::NodeStatus::FAILURE;
  }

  setOutput("client_pose", last_pose_);
  RCLCPP_INFO(node_->get_logger(),
    "GetClientPoseAction: client pose saved (%.2f, %.2f)",
    last_pose_.pose.position.x, last_pose_.pose.position.y);

  return BT::NodeStatus::SUCCESS;
}

} // namespace practica5
