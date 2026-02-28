/* .hpp del orentationNode.cpp, declaración de este nodo */

#ifndef PRACTICA3__ORIENTATION_NODE_HPP_
#define PRACTICA3__ORIENTATION_NODE_HPP_

#include <memory>

#include "practica3/PIDController.hpp"

#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/transform_broadcaster.h"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/point_stamped.hpp"

#include "sensor_msgs/msg/image.hpp"
#include "sensor_msgs/msg/camera_info.hpp"
#include "vision_msgs/msg/detection2_d.hpp"
#include "vision_msgs/msg/detection3_d.hpp"
#include "std_msgs/msg/bool.hpp"
#include "rclcpp/rclcpp.hpp"

namespace practica3 {

class OrientationNode : public rclcpp::Node
{
public:
  OrientationNode();
private:
  void vision_3d_callback(const vision_msgs::msg::Detection3D::ConstSharedPtr & vision);
  void nearest_obstacle_callback(const geometry_msgs::msg::PointStamped::ConstSharedPtr & obstacle);

  rclcpp::Subscription<vision_msgs::msg::Detection3D>::SharedPtr vision_3d_sub_;
  rclcpp::Subscription<geometry_msgs::msg::PointStamped>::SharedPtr obstacle_sub_;

  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr vel_publisher_;

  void control_cycle();

  tf2_ros::Buffer tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
  std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;

  vision_msgs::msg::Detection3D::ConstSharedPtr last_detection_;
  geometry_msgs::msg::PointStamped::ConstSharedPtr last_obstacle_;
  rclcpp::TimerBase::SharedPtr timer_;

  PIDController vlin_pid_, vrot_pid_;
};

}

#endif //PRACTICA3__ORIENTATION_NODE_HPP_
