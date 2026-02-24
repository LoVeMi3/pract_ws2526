/* implementará un nodo que publique: una vision_msgs/msg/Detection3D 
(por ejemplo, en /detection_3d). Una TF propia asociada al objeto/persona detectado */

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "practica3/DetectionNode3D.hpp"

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

// executable='detectNode3d' VERYIMPORTANT

using namespace std::chrono_literals;

namespace practica3 {

DetectionNode3D::DetectionNode3D()
: Node("detection_node_3_d"), tf_buffer_(this->get_clock())
{
  declare_parameter("min_distance", min_distance_);
  get_parameter("min_distance", min_distance_);

  vision_2d_sub_ = create_subscription<vision_msgs::msg::Detection2D>("input_detection_2d", rclcpp::SensorDataQoS().reliable(), std::bind(&DetectionNode3D::vision_2d_callback, this, std::placeholders::_1));
  img_depth_sub_ = create_subscription<sensor_msgs::msg::Image>("input_depth", rclcpp::SensorDataQoS().reliable(), std::bind(&DetectionNode3D::img_depth_callback, this, std::placeholders::_1));
  img_cam_info_sub_ = create_subscription<sensor_msgs::msg::CameraInfo>("camera_info", rclcpp::SensorDataQoS().reliable(), std::bind(&DetectionNode3D::img_cam_info_callback, this, std::placeholders::_1));

  vision_3d_pub_ = create_publisher<vision_msgs::msg::Detection3D>("/detection_3d", 10);

  tf_listener_ = std::make_shared<tf2_ros::TransformListener>(tf_buffer_);
  tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(this);
}

void
DetectionNode3D::vision_2d_callback(const vision_msgs::msg::Detection2D::ConstSharedPtr & vision)
{
  last_detection_ = vision;

  if (!last_depth_ || !last_cam_info_) {
    RCLCPP_WARN(this->get_logger(), "No recibidos depth o camera_info de imagen 2D");
    return;
  }

  publish_vision();
}

void
DetectionNode3D::img_depth_callback(const sensor_msgs::msg::Image::ConstSharedPtr & img)
{
  last_depth_ = img;
}

void
DetectionNode3D::img_cam_info_callback(const sensor_msgs::msg::CameraInfo::ConstSharedPtr & info)
{
  last_cam_info_ = info;
}
/*
Centro del bounding box 2D → (u, v)
Profundidad en ese píxel → Z
Intrínsecos de cámara → fx, fy, cx, cy

X = (u - cx) * Z / fx
Y = (v - cy) * Z / fy
Z = Z

K = [fx  0 cx]
    [0  fy cy]
    [0   0  1]
*/
void
DetectionNode3D::publish_vision() {
  float u = last_detection_->bbox.center.position.x;
  float v = last_detection_->bbox.center.position.y;

  int width = last_depth_->width;
  int x = static_cast<int>(u);
  int y = static_cast<int>(v);

  float * depth_data = (float *) &last_depth_->data[0];
  float Z = depth_data[y * width + x];

  if (std::isnan(Z) || Z < min_distance_) {
    return;
  }

  float fx = last_cam_info_->k[0];
  float fy = last_cam_info_->k[4];
  float cx = last_cam_info_->k[2];
  float cy = last_cam_info_->k[5];

  float X = ((u - cx) * Z) / fx;
  float Y = ((v - cy) * Z) / fy;

  //publicar la imagen detectada en 3d
  vision_msgs::msg::Detection3D det_3d;

  det_3d.header = last_detection_->header;
  det_3d.results = last_detection_->results;

  det_3d.bbox.center.position.x = X;
  det_3d.bbox.center.position.y = Y;
  det_3d.bbox.center.position.z = Z;

  det_3d.bbox.center.orientation.w = 1.0;  // sin rotación

  det_3d.bbox.size.x = 0.2;
  det_3d.bbox.size.y = 0.2;
  det_3d.bbox.size.z = 0.2;

  vision_3d_pub_->publish(det_3d);

  //publicar la tf asociada a la visión
  geometry_msgs::msg::TransformStamped tf2_msg;

  tf2_msg.header.stamp = last_detection_->header.stamp;
  tf2_msg.header.frame_id = last_detection_->header.frame_id;
  tf2_msg.child_frame_id = "target";

  tf2_msg.transform.translation.x = X;
  tf2_msg.transform.translation.y = Y;
  tf2_msg.transform.translation.z = Z;

  tf2_msg.transform.rotation.x = 0.0;
  tf2_msg.transform.rotation.y = 0.0;
  tf2_msg.transform.rotation.z = 0.0;
  tf2_msg.transform.rotation.w = 1.0;

  tf_broadcaster_->sendTransform(tf2_msg);
}

} //namespace practica3
