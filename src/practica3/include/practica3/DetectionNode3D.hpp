// el .hpp del detection node 3d

#ifndef PRACTICA3__DETECTION_NODE_3_D_HPP_
#define PRACTICA3__DETECTION_NODE_3_D_HPP_

#include <memory>

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

#include "message_filters/subscriber.h"
#include "message_filters/synchronizer.h"
#include "message_filters/sync_policies/approximate_time.h"

namespace practica3 {

class DetectionNode3D : public rclcpp::Node
{
public:
  DetectionNode3D();
private:
  void sync_callback(const vision_msgs::msg::Detection2D::ConstSharedPtr & vision, const sensor_msgs::msg::Image::ConstSharedPtr & img);
  void img_cam_info_callback(const sensor_msgs::msg::CameraInfo::ConstSharedPtr & info);

  message_filters::Subscriber<vision_msgs::msg::Detection2D> vision_2d_sub_;
  message_filters::Subscriber<sensor_msgs::msg::Image> img_depth_sub_;
  rclcpp::Subscription<sensor_msgs::msg::CameraInfo>::SharedPtr img_cam_info_sub_;

  rclcpp::Publisher<vision_msgs::msg::Detection3D>::SharedPtr vision_3d_pub_;

  typedef message_filters::sync_policies::ApproximateTime<vision_msgs::msg::Detection2D, sensor_msgs::msg::Image> SyncPolicy;
  std::shared_ptr<message_filters::Synchronizer<SyncPolicy>> sync_;

  void publish_vision();

  tf2_ros::Buffer tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
  std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;

  vision_msgs::msg::Detection2D::ConstSharedPtr last_detection_;
  sensor_msgs::msg::Image::ConstSharedPtr last_depth_;
  sensor_msgs::msg::CameraInfo::ConstSharedPtr last_cam_info_;

  float min_distance_ {0.5f};
};

}

#endif //PRACTICA3__DETECTION_NODE_3_D_HPP_