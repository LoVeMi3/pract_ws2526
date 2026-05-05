// GetClientPoseAction.hpp
#ifndef PRACTICA5__GET_CLIENT_POSE_ACTION_HPP
#define PRACTICA5__GET_CLIENT_POSE_ACTION_HPP

#include "rclcpp/rclcpp.hpp"
#include "behaviortree_cpp/behavior_tree.h"
#include "geometry_msgs/msg/pose_stamped.hpp"

namespace practica5 {

class GetClientPoseAction : public BT::SyncActionNode
{
public:
  GetClientPoseAction(const std::string & name, const BT::NodeConfig & config);

  static BT::PortsList providedPorts()
  {
    return { BT::OutputPort<geometry_msgs::msg::PoseStamped>("client_pose") };
  }

  BT::NodeStatus tick() override;

private:
  rclcpp::Node::SharedPtr node_;
  rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr sub_;
  geometry_msgs::msg::PoseStamped last_pose_;
  bool pose_received_{false};
};

} // namespace practica5

#endif //PRACTICA5__GET_CLIENT_POSE_ACTION_HPP
