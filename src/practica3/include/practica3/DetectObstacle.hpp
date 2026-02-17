//declaraciones del nodo DetectObstacle.cpp

#ifndef PRACTICA3__DETECT_OBSTACLE_HPP_
#define PRACTICA3__DETECT_OBSTACLE_HPP_

#include <memory>

#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"

#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_msgs/msg/bool.hpp"
#include "rclcpp/rclcpp.hpp"

class DetectObstacle : public rclcpp::Node
{
public:
  DetectObstacle();
//protected? herencia?
private:
  void laser_callback(const sensor_msgs::msg::LaserScan::ConstSharedPtr & scan);
  bool is_obstacle(const sensor_msgs::msg::LaserScan & scan, float dist_thrld);

  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub_;
  rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr obstacle_pub_;

  tf2_ros::Buffer tf_buffer_;
  tf2_ros::TransformListener tf_listener_;

  float min_distance_ {0.5f};
};

#endif //PRACTICA3__DETECT_OBSTACLE_HPP_