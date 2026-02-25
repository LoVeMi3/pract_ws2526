/* nodo de control que recibe una vision de detection 3d, genera velocidad 
	angular para orientar robot hacia objetivo, y publique comandos de velocidad */

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "practica3/OrientationNode.hpp"
#include "practica3/PIDController.hpp"

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

// executable='orientation' VERYIMPORTANT

using namespace std::chrono_literals;

namespace practica3 {

OrientationNode::OrientationNode()
: Node("orientation_node"), tf_buffer_(this->get_clock()), vlin_pid_(0.0, 1.0, 0.0, 0.7), vrot_pid_(0.0, 1.0, 0.3, 1.0)
{
  vision_3d_sub_ = create_subscription<vision_msgs::msg::Detection3D>("output_detection_3d", rclcpp::SensorDataQoS().reliable(), std::bind(&OrientationNode::vision_3d_callback, this, std::placeholders::_1));

  vel_publisher_ = create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
  timer_ = create_wall_timer(50ms, std::bind(&OrientationNode::control_cycle, this));

  tf_listener_ = std::make_shared<tf2_ros::TransformListener>(tf_buffer_);
  tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);
}

void
OrientationNode::vision_3d_callback(const vision_msgs::msg::Detection3D::ConstSharedPtr & vision)
{
  last_detection_ = vision;

  if (!last_detection_) {
    RCLCPP_WARN(get_logger(), "Error en la detección de la visión 3D.");
    //el robot debe girar hasta encontrar la visión
    find_vision();
    return;
  }

  control_cycle();
}

void
OrientationNode::control_cycle()
{
  tf2::Stamped<tf2::Transform> bf2target;
  std::string error;

  if (tf_buffer_.canTransform("base_footprint", "target", tf2::TimePointZero, &error)) {
    auto bf2target_msg = tf_buffer_.lookupTransform(
    "base_footprint", "target", tf2::TimePointZero);

    tf2::fromMsg(bf2target_msg, bf2target);

    double x = bf2target.getOrigin().x();
    double y = bf2target.getOrigin().y();

    double angle = atan2(y, x);
    double dist = sqrt(x * x + y * y);

    double vel_rot = std::clamp(vrot_pid_.get_output(angle), -2.0, 2.0);
    double vel_lin = std::clamp(vlin_pid_.get_output(dist - 1.0), -1.0, 1.0);

    geometry_msgs::msg::Twist twist;
    twist.linear.x = vel_lin;
    twist.angular.z = vel_rot;

    vel_publisher_->publish(twist);

  } else {
    //no existe el frame target, habrá que buscarlo
    find_vision();
  }
}

void
OrientationNode::find_vision()
{
  geometry_msgs::msg::Twist twist;
  twist.linear.x = 0.03;
  twist.angular.z = 0.5;

  vel_publisher_->publish(twist);
  
  RCLCPP_INFO_THROTTLE(get_logger(), *get_clock(), 2000, "Buscando objetivo... vroom... vroom");
}
} //namespace practica3

