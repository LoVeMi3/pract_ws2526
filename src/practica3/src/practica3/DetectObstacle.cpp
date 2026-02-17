/* nodo que reciba del láser, calcule posición del obstáculo más cercano, 
calcular posición 2D, publicar el resultado y publicar la tf asociada al obstáculo */

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "practica3/DetectObstacle.hpp"

#include "geometry_msgs/msg/point_stamped.hpp"

#include "sensor_msgs/msg/laser_scan.hpp"
#include "std_msgs/msg/bool.hpp"

#include "rclcpp/rclcpp.hpp"

#include "tf2/exceptions.h"
#include "tf2/time.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"

#include "tf2/LinearMath/Transform.h"
#include "tf2/transform_datatypes.h"
#include "tf2_ros/transform_broadcaster.h"

using namespace std::chrono_literals;

DetectObstacle::DetectObstacle()
: Node("detect_obstacle"), tf_buffer_(this->get_clock()), tf_listener_(tf_buffer_), rd_(), generator_(rd_())
{
  declare_parameter("min_distance", min_distance_);
  get_parameter("min_distance", min_distance_);

  laser_sub_ = create_subscription<sensor_msgs::msg::LaserScan>("input_scan", rclcpp::SensorDataQoS().reliable(), std::bind(&DetectObstacle::laser_callback, this, _1));
  obstacle_pub_ = create_publisher<geometry_msgs::msg::PointStamped>("/nearest_obstacle", 100);

  timer_generate_ = create_wall_timer(20s, std::bind(&TFPublisherNode::generate_tf, this));
  timer_publish_ = create_wall_timer(50ms, std::bind(&TFPublisherNode::publish_tf, this));

  tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(*this);

  generate_tf();
}

bool
DetectObstacle::is_obstacle(const sensor_msgs::msg::LaserScan & scan, float dist_thrld)
{
  float distance_min = std::numeric_limits<float>::infinity();
  for (size_t i = 0; i < scan.ranges.size(); ++i) {
    const float range = scan.ranges[i];
    if (!std::isfinite(range)) {
      continue;
    }
    if (range < scan.range_min || range > scan.range_max) {
      continue;
    }
    distance_min = std::min(distance_min, range);
  }

  if (!std::isfinite(distance_min)) {
    return false;
  }

  return distance_min < dist_thrld;
}

void
DetectObstacle::laser_callback(const sensor_msgs::msg::LaserScan::ConstSharedPtr & scan)
{
  std_msgs::msg::Bool obstacle_msg;
  obstacle_msg.data = is_obstacle(*scan, min_distance_);

  if (obstacle_msg.data) {
    print_obstacle_info(*scan, min_distance_);
  }

  obstacle_pub_->publish(obstacle_msg);
}

void
DetectObstacle::generate_tf()
{
  std::uniform_real_distribution<double> pos_x(-5.0, 5.0);
  std::uniform_real_distribution<double> pos_y(-5.0, 5.0);

  transform_.header.stamp = now();
  transform_.header.frame_id = "odom";
  transform_.child_frame_id = "target";

  transform_.transform.translation.x = pos_x(generator_);
  transform_.transform.translation.y = pos_y(generator_);
  transform_.transform.translation.z = 0.0;
}

void
DetectObstacle::publish_tf()
{
  transform_.header.stamp = now();
  tf_broadcaster_->sendTransform(transform_);
}
