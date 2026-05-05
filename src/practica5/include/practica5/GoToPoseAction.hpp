#ifndef PRACTICA5__GO_TO_POSE_ACTION_HPP
#define PRACTICA5__GO_TO_POSE_ACTION_HPP

//PUEDO REUTILIZAR EL QUE YA DAN LOS PROFES

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <string>
#include <map>
#include <array>

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
#include "practica5/WaitForPersonAction.hpp"
#include "practica5/DetectionNode.hpp"

namespace practica5 {

class GoToPoseAction : public BT::StatefulActionNode
{
public:
  using NavigateToPose = nav2_msgs::action::NavigateToPose;
  using GoalHandleNav = rclcpp_action::ClientGoalHandle<NavigateToPose>;

  GoToPoseAction(const std::string &name, const BT::NodeConfig &config);

  static
  BT::PortsList providedPorts()
  {
    return {
      BT::InputPort<std::string>("goal"),
      BT::InputPort<geometry_msgs::msg::PoseStamped>("client_pose")};
  }

  BT::NodeStatus onStart() override;
  BT::NodeStatus onRunning() override;
  void onHalted() override;

private:
  rclcpp::Node::SharedPtr node_;
  rclcpp_action::Client<NavigateToPose>::SharedPtr nav_client_;

  static const std::map<std::string, std::array<double, 3>> waypoints_;
  std::shared_future<GoalHandleNav::SharedPtr> goal_handle_future_;
  GoalHandleNav::SharedPtr goal_handle_;
  bool goal_sent_{false};
};

} // namespace practica5

#endif //PRACTICA5__GO_TO_POSE_ACTION_HPP
