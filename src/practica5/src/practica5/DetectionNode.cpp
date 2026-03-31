//el DetectionNode.cpp

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_msgs/msg/bool.hpp"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"

// executable='detection' VERYIMPORTANT

using namespace std::chrono_literals;

namespace practica5 {

DetectionNode::DetectionNode()
: Node("detection_node")
{
  declare_parameter("min_distance", min_distance_);
  get_parameter("min_distance", min_distance_);

  laser_sub_ = create_subscription<sensor_msgs::msg::LaserScan>("input_scan", rclcpp::SensorDataQoS().reliable(), std::bind(&DetectionNode::laser_callback, this, std::placeholders::_1));
  person_pub_ = create_publisher<std_msgs::msg::Bool>("/person_detected", 10);
  pose_pub_ = create_publisher<geometry_msgs::msg::PoseStamped>("/person_pose", 10);

  tf_buffer_ = std::make_shared<tf2_ros::Buffer>(get_clock());
  tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
}

void
DetectionNode::laser_callback(const sensor_msgs::msg::LaserScan::ConstSharedPtr & scan)
{
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
      min_index = static_cast<int>(i);
    }
  }

  std_msgs::msg::Bool person_msg;
  if (min_index == -1) {
    person_msg.data = false;
  } else {
    person_msg.data = (min_range < min_distance_);
  }

  person_pub_->publish(person_msg);

  if (person_msg.data && min_index != -1) {
    double angle = scan->angle_min + min_index * scan->angle_increment;

    geometry_msgs::msg::PoseStamped pose_laser;
    pose_laser.header.frame_id = scan->header.frame_id;
    pose_laser.header.stamp    = scan->header.stamp;
    pose_laser.pose.position.x = min_range * std::cos(angle);
    pose_laser.pose.position.y = min_range * std::sin(angle);
    pose_laser.pose.position.z = 0.0;
    pose_laser.pose.orientation.w = 1.0;

    try {
      auto pose_map = tf_buffer_->transform(pose_laser, "map", tf2::durationFromSec(0.2));
      pose_pub_->publish(pose_map);
      RCLCPP_INFO(get_logger(), "Person detected at map (%.2f, %.2f), dist=%.2f m", pose_map.pose.position.x, pose_map.pose.position.y, min_range);
    } catch (const tf2::TransformException &ex) {
      RCLCPP_WARN(get_logger(), "TF transform failed: %s", ex.what());
    }
  }
}

} //namespace practica5