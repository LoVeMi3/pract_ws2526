/* implementar una FSM que coordina uan tarea de patrullaje mediante la 
   capacidad de navegación validada en el paso2 con nav2
   INIT > WAIT_SERVER > SEND_GOAL > NAVIGATING > CHECK_RESULT > SEND_GOAL (siguiente waypoint)
*/

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "practica4/navFSM.hpp"
#include "nav2_example/simple_navigation_app.hpp"
#include "nav2_example/navigation_client.hpp"

#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/string.hpp"

#include "nav2_msgs/action/navigate_to_pose.hpp"
#include "rclcpp_action/rclcpp.hpp"

//#include "sensor_msgs/msg/image.hpp"
//#include "sensor_msgs/msg/camera_info.hpp"
//#include "vision_msgs/msg/detection2_d.hpp"
//#include "vision_msgs/msg/detection3_d.hpp"

#include "rclcpp/rclcpp.hpp"

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

navFSM::navFSM() : Node("nav_FSM")
{
  //using NavigateToPose = nav2_msgs::action::NavigateToPose;
  //using GoalHandleNav = rclcpp_action::ClientGoalHandle<NavigateToPose>;

  current_state_ = State::INIT;
  current_waypoint_ = 0;
  retry_count_ = 0;

  state_pub_ = create_publisher<std_msgs::msg::String>("fsm_state", 10);
  nav_client_ = std::make_shared<nav2_example::NavigationClient>(this);

  waypoints_.push_back(nav_client_->create_pose_stamped(2.0, 1.0, 0.0));
  waypoints_.push_back(nav_client_->create_pose_stamped(4.0, 1.0, 0.0));
  waypoints_.push_back(nav_client_->create_pose_stamped(4.0, -2.0, 0.0));

  timer_ = create_wall_timer(200ms, std::bind(&navFSM::control_cycle, this));
}

void
navFSM::control_cycle()
{
  std_msgs::msg::String msg;
  switch (current_state_) {
  case State::INIT:
    //detener motores y esperar
    msg.data = "INIT";
    state_pub_->publish(msg);
    current_state_ = State::WAIT_SERVER;
    break;
  case State::WAIT_SERVER:
    //detener motores y esperar
    if (nav_client_->wait_for_action_server(std::chrono::seconds(1))) {
      RCLCPP_INFO(get_logger(), "Servidor listo");
      RCLCPP_INFO(get_logger(), "Waypoint actual: %d", current_waypoint_);
      msg.data = "WAITING SERVER";
      state_pub_->publish(msg);
      current_state_ = State::SEND_GOAL;
    }
    break;
  case State::SEND_GOAL:
    RCLCPP_INFO(get_logger(), "Enviando waypoint %d", current_waypoint_);
    nav_client_->send_goal(waypoints_[current_waypoint_]);
    msg.data = "SENDING GOAL";
    state_pub_->publish(msg);
    current_state_ = State::NAVIGATING;
    break;
  case State::NAVIGATING:
    if (!nav_client_->is_goal_done()) {
      return;
    }
    RCLCPP_INFO(get_logger(), "Waypoint actual: %d", current_waypoint_);
    msg.data = "NAVIGATING";
    state_pub_->publish(msg);
    current_state_ = State::CHECK_RESULT;
    break;
  case State::CHECK_RESULT:
    if (nav_client_->was_goal_successful()) {
      current_waypoint_++;
      if (current_waypoint_ >= waypoints_.size()) {
        current_waypoint_ = 0;
      }
      msg.data = "CHECKING RESULT";
      state_pub_->publish(msg);
      current_state_ = State::SEND_GOAL;
    } else {
      retry_count_++;
      if (retry_count_ < 3) {
        current_state_ = State::SEND_GOAL;
      } else {
        retry_count_ = 0;
        current_waypoint_++;
        msg.data = "CHECKING RESULT";
        state_pub_->publish(msg);
        current_state_ = State::SEND_GOAL;
      }
    }
    break;
  default:
    //nidea
    break;
  }
}

} //namespace practica 4