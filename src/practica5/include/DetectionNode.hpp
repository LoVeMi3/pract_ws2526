//declaraciones del nodo DetectionNode.cpp

#ifndef PRACTICA5__DETECTION_NODE_HPP_
#define PRACTICA5__DETECTION_NODE_HPP_

#include <memory>

#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_msgs/msg/bool.hpp"
#include "rclcpp/rclcpp.hpp"

namespace practica3 {

class DetectionNode : public rclcpp::Node
{
public:
  DetectionNode();
private:
  void laser_callback(const sensor_msgs::msg::LaserScan::ConstSharedPtr & scan);

  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub_;
  rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr obstacle_pub_;

  float min_distance_ {0.5f};
};

}

#endif //PRACTICA5__DETECTION_NODE_HPP_