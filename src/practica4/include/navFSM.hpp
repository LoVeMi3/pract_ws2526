#ifndef PRACTICA4__NAV_FSM_HPP_
#define PRACTICA4__NAV_FSM_HPP_

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

//#include "sensor_msgs/msg/image.hpp"
//#include "sensor_msgs/msg/camera_info.hpp"
//#include "vision_msgs/msg/detection2_d.hpp"
//#include "vision_msgs/msg/detection3_d.hpp"

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp.hpp"

//#include "tf2/exceptions.h"
//#include "tf2/time.h"
//#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"

//#include "tf2/LinearMath/Transform.h"
//#include "tf2/transform_datatypes.h"
//#include "tf2_ros/transform_broadcaster.h"
//#include "tf2_ros/transform_listener.h"

#include "geometry_msgs/msg/point_stamped.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"

// executable='' VERYIMPORTANT

using namespace std::chrono_literals;

namespace practica4 {

class navFSM : public rclcpp::Node
{
public:
  enum class State { INIT, WAIT_SERVER, SEND_GOAL, NAVIGATING, CHECK_RESULT };
  navFSM();
private:
  void control_cycle();

  State current_state_;
  int retry_count_;
  bool start_button_pressed_ = false;

  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr state_pub_;
  rclcpp::TimerBase::SharedPtr timer_;

  std::shared_ptr<nav2_example::NavigationClient> nav_client_;
  std::vector<geometry_msgs::msg::PoseStamped> waypoints_;
  int current_waypoint_;
  double min_dist_; // = 10.0
  const double OBST_THRESHOLD= 0.5;
};

} //namespace practica4

#endif //PRACTICA4__NAV_FSM_HPP_