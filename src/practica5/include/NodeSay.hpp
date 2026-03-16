#ifndef PRACTICA4__NODE_SAY_HPP
#define PRACTICA4__NODE_SAY_HPP

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "nav2_example/simple_navigation_app.hpp"
#include "nav2_example/navigation_client.hpp"

#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/string.hpp"

#include "nav2_msgs/action/navigate_to_pose.hpp"
#include "rclcpp_action/rclcpp.hpp"

#include "rclcpp/rclcpp.hpp"

#include "geometry_msgs/msg/point_stamped.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"

#include "bt_examples/say_text_client_action.hpp"
#include "bt_examples/text_utils.hpp"

// executable='' VERYIMPORTANT

using namespace std::chrono_literals;

namespace practica5 {

class NodeSay : public rclcpp::Node
{
public:
  NodeSay();
private:
  void control_cycle();

  State current_state_;
  int retry_count_;
  bool start_button_pressed_ = false;

  std::shared_ptr<nav2_example::NavigationClient> nav_client_;
  std::vector<geometry_msgs::msg::PoseStamped> waypoints_;
  int current_waypoint_;
  double min_dist_; // = 10.0
  const double OBST_THRESHOLD= 0.5;
};

} //namespace practica5

#endif //PRACTICA4__NODE_SAY_HPP