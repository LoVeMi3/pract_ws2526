#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "practica4/StateMachine.hpp"
#include "practica4/OOPFSMRobot.hpp"

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

class OOPFSMRobot : public rclcpp::Node
{
  StateMachine* fsm;

  //variables de estado, memoria compartida entre estados
  double min_distance_ = 10.0;
  const double OBSTACLE_THRESHOLD = 0.5;
  bool start_button_pressed_ = false;

  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr vel_pub_;
  rclcpp::TimerBase::SharedPtr timer_;

public:
  OOPFSMRobot() : Node("oop_fsm_robot")
  {
    //suscripciones a sensores
    laser_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>("/scan", 10, std::bind(&OOPFSMRobot::laser_callback, this, std::placeholder::_1));
    //publicadores de comandos
    vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
    //timer ciclo de contro, 10Hz
    timer_ = this->create_wall_timer(100ms, std::bind(&OOPFSMRobot::control_cycle, this));
    //inicializar FSM en estado IDLE
    fsm_ = new StateMachine(new IdleState(this), this->get_logger());
  }
private:
  void
  laser_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
  {
    if (!msg->ranges.empty()) {
      min_distance_ = *std::min_element(msg->ranges.begin(), msg->ranges.end());
    }
  }

  void
  control_cycle()
  {
    //aquí la FSM toma decisiones
    fsm_->step();
  }

  //interfaz públic para que los estados del robot acceden a la información del robot
  double
  get_min_min_distance() const
  {
    return min_distance_;
  }
  double
  get_obstacle_threshold() const
  {
    return OBSTACLE_THRESHOLD;
  }
  bool
  is_start_pressed() const
  {
    return start_button_pressed_;
  }
  void
  publish_velocity(double linear, double angular);
};

}