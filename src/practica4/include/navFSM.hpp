#ifndef PRACTICA4__NAV_FSM_HPP_
#define PRACTICA4__NAV_FSM_HPP_

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "std_msgs/msg/bool.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "sensor_msgs/msg/camera_info.hpp"
#include "vision_msgs/msg/detection2_d.hpp"
#include "vision_msgs/msg/detection3_d.hpp"

#include "rclcpp/rclcpp.hpp"

#include "tf2/exceptions.h"
#include "tf2/time.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"

#include "tf2/LinearMath/Transform.h"
#include "tf2/transform_datatypes.h"
#include "tf2_ros/transform_broadcaster.h"
#include "tf2_ros/transform_listener.h"

#include "geometry_msgs/msg/point_stamped.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"

// executable='' VERYIMPORTANT

using namespace std::chrono_literals;

namespace practica4 {

class navFSM : public rclcpp::Node
{
public:
  enum class State { IDLE, MOVING, OBST_DETECTED, STOPPED };
  navFSM();
  void simulate_start_button();
private:
  State current_state_;
  double min_dist_; // = 10.0
  const double OBST_THRESHOLD= 0.5;
  bool start_button_pressed_ = false;
  rclcpp::Time stopped_time_;

  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr vel_pub_;

  rclcpp::TimerBase::SharedPtr timer_;

  void laser_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg);
  void control_cycle();
  void publish_velocity(double linear, double angular);
};

} //namespace practica4

#endif //PRACTICA4__NAV_FSM_HPP_