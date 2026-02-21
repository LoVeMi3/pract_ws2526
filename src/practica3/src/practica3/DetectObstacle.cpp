/* nodo que reciba del láser, calcule posición del obstáculo más cercano, 
calcular posición 2D, publicar el resultado y publicar la tf asociada al obstáculo */

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "practica3/DetectObstacle.hpp"

#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_msgs/msg/bool.hpp"

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

// executable='detect_obstacle' VERYIMPORTANT

using namespace std::chrono_literals;

namespace practica3 {

DetectObstacle::DetectObstacle()
: Node("detect_obstacle"), tf_buffer_(this->get_clock())
{
  declare_parameter("min_distance", min_distance_);
  get_parameter("min_distance", min_distance_);

  laser_sub_ = create_subscription<sensor_msgs::msg::LaserScan>("input_scan", rclcpp::SensorDataQoS().reliable(), std::bind(&DetectObstacle::laser_callback, this, std::placeholders::_1));
  obstacle_pub_ = create_publisher<geometry_msgs::msg::PointStamped>("/nearest_obstacle", 10);

  tf_listener_ = std::make_shared<tf2_ros::TransformListener>(tf_buffer_);
  tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);
}

void
DetectObstacle::laser_callback(const sensor_msgs::msg::LaserScan::ConstSharedPtr & scan)
{
  std_msgs::msg::Bool obstacle_msg;

  float min_range = std::numeric_limits<float>::infinity();
  int min_index = -1;

  for (size_t i = 0; i < scan->ranges.size(); ++i) {
    float r = scan->ranges[i];

    if (!std::isfinite(r)) {
      continue;
    }
    if (r < scan->range_min || r > scan->range_max) {
      continue;
    }
    if (r < min_range) {
      min_range = r;
      min_index = i;
    }
  }

  if (min_index == -1) {
    return;
  }
  
  float angle = scan->angle_min + min_index * scan->angle_increment;
  float x_laser = min_range * std::cos(angle);
  float y_laser = min_range * std::sin(angle);

  //publicar el pointStamped del obstáculo
  geometry_msgs::msg::PointStamped point_laser;
  point_laser.header.stamp = scan->header.stamp;
  point_laser.header.frame_id = scan->header.frame_id;

  point_laser.point.x = x_laser;
  point_laser.point.y = y_laser;
  point_laser.point.z = 0.0;

  geometry_msgs::msg::PointStamped point_robot;
  geometry_msgs::msg::TransformStamped transform;
  try {
    point_robot = tf_buffer_.transform(point_laser, "base_link", tf2::durationFromSec(0.1));
    transform = tf_buffer_.lookupTransform("base_link", scan->header.frame_id, scan->header.stamp, rclcpp::Duration::from_seconds(0.1));
    tf2::doTransform(point_laser, point_robot, transform);
  } catch (tf2::TransformException & ex) {
    RCLCPP_WARN(this->get_logger(), "TF error: %s", ex.what());
    return;
  }
  
  obstacle_pub_->publish(point_robot);

  //publicar tf del obstáculo
  geometry_msgs::msg::TransformStamped tf2_msg;

  tf2_msg.header.stamp = scan->header.stamp;
  tf2_msg.header.frame_id = "base_link";
  tf2_msg.child_frame_id = "nearest_obstacle";

  tf2_msg.transform.translation.x = point_robot.point.x;
  tf2_msg.transform.translation.y = point_robot.point.y;
  tf2_msg.transform.translation.z = 0.0;

  tf2_msg.transform.rotation.x = 0.0;
  tf2_msg.transform.rotation.y = 0.0;
  tf2_msg.transform.rotation.z = 0.0;
  tf2_msg.transform.rotation.w = 1.0;

  tf_broadcaster_->sendTransform(tf2_msg);
}

} //namespace practica3
