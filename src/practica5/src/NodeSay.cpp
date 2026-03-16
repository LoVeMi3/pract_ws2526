#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "practica5/NodeSay.hpp"
#include "nav2_example/simple_navigation_app.hpp"
#include "nav2_example/navigation_client.hpp"

#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/string.hpp"

#include "nav2_msgs/action/navigate_to_pose.hpp"
#include "rclcpp_action/rclcpp.hpp"

#include "rclcpp/rclcpp.hpp"

#include "geometry_msgs/msg/point_stamped.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"

#include "bt_examples/say_text_client_action.hpp"
#include "bt_examples/text_utils.hpp"

// executable='' VERYIMPORTANT

//HRIClient abstrae complejidad, el nodo no gestiona clientes de servicio ROS directamente,
//HRIClient encapsula esa lógica
using namespace std::chrono_literals;

namespace practica5 {

NodeSay::NodeSay() : Node("node_say")
{
  const std::string &name;
  const BT::NodeConfig &config;
  std::shared_ptr<HRIClient> hri_client) : BT::StatefulActionNode(name, config);
  hri_client_(hri_client);
}

//expande variables del blackboard ("hole (name)" se convierte en "hola juan" si
//name="juan" en el blackboard)
std::string
NodeSay::formatText(const std::string &text)
{
  return bt_examples::formatText(text, config().blackboard);
}

//inicia la operación TTS con start_speaking(), onRunning() monitoriza el progreso
//sin blqquear
BT::NodeStatus
NodeSay::onStart()
{
  std::string text;
  if (!getInput("text", text)) {
    RCLCPP_ERROR(hri_client_->get_logger(), "NodeSay: 'text' input port is required");
    return BT::NodeStatus::FAILURE;
  }
  text = formatText(text);
  RCLCPP_INFO(hri_client_->get_logger(), "Saying: '%s'", text.c_str());

  hri_client_->start_speaking(text);
  start_time_ = std::chrono::steady_clock::now();

  return BT::NodeStatus::RUNNING;
}

//implementa un timeout de 30 segundos para evitar bloqueos si el servicio de TTS no responde
//comprueba tanto que la operación terminó como que fue exitosa
BT::NodeStatus
NodeSay::onRunning()
{
  auto elapsed = std::chrono::steady_clock::now() - start_time_;
  if (elapsed > 30s) {
    RCLCPP_ERROR(hri_client_->get_logger(), "TTS operation timeout");
    return BT::NodeStatus::FAILURE;
  }

  if (hri_client_->is_speaking_done()) {
    if (hri_client_->get_speaking_result()) {
      RCLCPP_INFO(hri_client_->get_logger(), "TTS completed successfully");
      return BT::NodeStatus::SUCCESS;
    } else {
      RCLCPP_ERROR(hri_client_->get_logger(), "TTS operation failed");
      return BT::NodeStatus::FAILURE;
    }
  }
  return BT::NodeStatus::RUNNING;
}

void
NodeSay::onHalted()
{
  RCLCPP_WARN(hri_clent_->get_logger(), "TTS action halted");
}