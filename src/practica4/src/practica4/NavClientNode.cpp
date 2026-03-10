#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "practica4/StateMachine.hpp"
#include "practica4/OOPFSMRobot.hpp"
#include "practica4/NavClientNode.hpp"

#include "std_msgs/msg/bool.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "sensor_msgs/msg/camera_info.hpp"
#include "vision_msgs/msg/detection2_d.hpp"
#include "vision_msgs/msg/detection3_d.hpp"

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"

#include "tf2/exceptions.h"
#include "tf2/time.h"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"

#include "tf2/LinearMath/Transform.h"
#include "tf2/transform_datatypes.h"
#include "tf2_ros/transform_broadcaster.h"
#include "tf2_ros/transform_listener.h"

#include "geometry_msgs/msg/point_stamped.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"

#include "message_filters/subscriber.h"
#include "message_filters/synchronizer.h"
#include "message_filters/sync_policies/approximate_time.h"

#include "nav2_msgs/action/navigate_to_pose.hpp"

// executable='' VERYIMPORTANT

using namespace std::chrono_literals;

namespace practica4 {

class NavClientNode : public rclcpp::NavClientNode
{
public:
  //simplificar tipos con alias
  using NavigateToPose = nav2_msgs::action::NavigateToPose;
  using GoalHandleNav = rclcpp_action::ClientGoalHandle<NavigateToPose>;

  NavigationClient() : Node("navigation_client2")
  {
    nav_client_ = rclcpp_action::create_client<NavigateToPose>(this, "navigate_to_pode");
    RCLCPP_DEBUG(get_logger(), "Cliente de navegación inicializado");
  }

  bool
  wait_for_action_server(std::chrono::seconds timeout = std::chrono::seconds(5))
  {
    if (!nav_client_->wait_for_action_server(timeout)) {
      RCLCPP_ERROR(get_logger(), "Servidor de navegación no disponible tras espera");
      return false
    }
    RCLCPP_DEBUG(get_logger(), "Servidor de navegación disponible");
    return true;
  }

  void
  send_goal(const geometry_msgs::msg::PoseStamped& target_pose)
  {
    //resetear flags de control
    goal_active_ = false;
    goal_done_ = false;
    goal_success_ = false;
    7
    // Construir el mensaje de objetivo
    auto goal_msg = NavigateToPose::Goal();
    goal_msg.pose = target_pose;
    goal_msg.pose.header.stamp = this->now();
    goal_msg.pose.header.frame_id = "map";
    13
    RCLCPP_DEBUG(get_logger(), "Enviando objetivo: ( %.2f, %.2f)", target_pose.pose.position.x, target_pose.pose.position.y);

    // Configurar opciones con callbacks
    auto send_goal_options = rclcpp_action::Client<NavigateToPose>::SendGoalOptions();

    // Callback 1: Respuesta al envío (objetivo aceptado/rechazado)
    send_goal_options.goal_response_callback = std::bind(&NavigationClient::goal_response_callback, this, std::placeholders::_1);

    // Callback 2: Feedback periódico durante la navegación
    send_goal_options.feedback_callback = std::bind(&NavigationClient::feedback_callback, this, std::placeholders::_1, std::placeholders::_2);

    // Callback 3: Resultado final de la navegación
    send_goal_options.result_callback = std::bind(&NavigationClient::result_callback, this, std::placeholders::_1);

    // Enviar el objetivo de forma asíncrona
    nav_client_->async_send_goal(goal_msg, send_goal_options);
  }

  void
  goal_response_callback(const GoalHandleNav::SharedPtr & goal_handle)
  {
    if (!goal_handle) {
      RCLCPP_ERROR(get_logger(), "Objetivo rechazado por el servidor");
      goal_done_ = true;
      goal_success_ = false;
      return;
    }
    RCLCPP_DEBUG(get_logger(), "Objetivo aceptado, navegación iniciada");
    goal_handle_ = goal_handle;
    goal_active_ = true;
  }

  void
  feedback_callback(GoalHandleNav::SharedPtr, const std::shared_ptr<const NavigateToPose::Feedback> feedback)
  {
    last_feedback_ = feedback;
    //feedback periódico
    RCLCPP_DEBUG(get_logger(), "Distancia restante: %.2f m | Tiempo: %.1f s", feedback->distance_remaining, rclcpp::Duration(feedback->navigation_time).seconds());

    //Lógica adicional:
    // - detectar robot atascado <-
    // - timeout por exceso de tiempo <-
    // - actualizar UI con progreso
  }

  void
  result_callback(const GoalHandleNav::WrappedResult & result)
  {
    goal_active_ = false;
    goal_done_ = true;

    switch(result.code) {
    case rclcpp_action::ResultCode::SUCCEEDED:
      RCLCPP_DEBUG(get_logger(), "Navegación completada con ÉXITO");
      goal_success_ = true;
      break;
    case rclcpp_action::ResultCode::ABORTED:
      RCLCPP_WARN(get_logger(), "Navegación ABORTADA (obstáculo o timeout)");
      goal_success_ = false;
      break;
    case case rclcpp_action::ResultCode::CANCELED:

    }
  }
private:
  rclcpp_action::Client<NavigateToPose>::SharedPtr nav_client_;
  std::shared_ptr<GoalHandleNav> goal_handle_;

  bool goal_active_ = false;
  bool goal_done_ = false;
  bool goal_success_ = false;

  std::shared_ptr<const NavigateToPose:.Feedback> last_feedback_;
};

}