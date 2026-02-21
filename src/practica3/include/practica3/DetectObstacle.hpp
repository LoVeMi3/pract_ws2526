//declaraciones del nodo DetectObstacle.cpp

#ifndef PRACTICA3__DETECT_OBSTACLE_HPP_
#define PRACTICA3__DETECT_OBSTACLE_HPP_

#include <memory>

#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"

#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_msgs/msg/bool.hpp"
#include "rclcpp/rclcpp.hpp"

namespace practica3 {

class DetectObstacle : public rclcpp::Node
{
public:
  DetectObstacle();
private:
  void laser_callback(const sensor_msgs::msg::LaserScan::ConstSharedPtr & scan);

  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub_;
  rclcpp::Publisher<geometry_msgs::msg::PointStamped>::SharedPtr obstacle_pub_;

  tf2_ros::Buffer tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
  std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;

  float min_distance_ {0.5f};
};

}

#endif //PRACTICA3__DETECT_OBSTACLE_HPP_