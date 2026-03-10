#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "practica4/OOPFSMRobot.hpp"
#include "practica4/StateMachine.hpp"

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
#include "geometry_msgs/msg/twist.hpp"

// executable='' VERYIMPORTANT

using namespace std::chrono_literals;

namespace practica4 {

class State
{
public:
  virtual void on_entry() {}
  virtual void on_do() = 0;
  virtual void on_exit() {}
  virtual State* check_transitions() = 0;
  virtual ~State() = default;

  virtual std::string get_name() const = 0;
};

class StateMachine
{
  State* current_state_;
  rclcpp::Logger logger_;

public:
  StateMachine(State* initial_state, rclcpp::Logger logger) : current_state_(initial_state), logger_(logger)
  {
  RCLCPP_INFO(logger_, "FSM iniciada en estado: %s", current_state_->get_anme().c_str());
  current_state->on_entry();
  }

  void
  step()
  {
    //ejecutar lógica del estado actual
    current_state_->on_do();

    //verificar trnasiciones
    State* new_state = current_state->check_transitions();

    //si hay transición, ejecutar salica, cambiar y ejecutar entrada
    if (next_state != nullptr) {
      RCLCPP_INFO(logger_, "Transición: &s -> %s", current_state_->get_name().c_str(), next_state->get_name().c_str());

      current_state_->on_exit();
      delete current_state_;
      current_state_ = next_state;
      currect_state_->on_entry();
    }
  }
};

class IdleState : pubic State
{
  OOPFSMRobot* robot_;
public:
  explicit IdleState(OOPFSMRobot* r) : robot_(r) {}

  void
  on_entry() override
  {
    RCLCPP_INFO(robot->get_logger(), "[IDLE] Entrando al estado - Deteniendo motores");
    robot_->pulish_velocity(0.0, 0.0);
  }

  void
  on_do() override
  {
    //no hace nada, simplemente espera
  }

  State*
  check_transitions() override
  {
    if (robot->is_start_pressed()) {
      robot_->clear_start_button();
      return new MovingState(robot_);
    }
    return nullptr; //no hay transición
  }

  void
  on_exit() override
  {
    RCLCPP_INFO(robot->get_logger(), "[IDLE] Saliendo del estado - Preparando para moverse");
  }

  std::string
  get_name() const override {
    return "IDLE";
  }
};

class MovingState : public State
{
  OOPFSMRobot* robot_;
public:
  explicit MovingState(OOPFSMRobot* r) : robot_(r) {}

  void
  on_entry() override
  {
    RCLCPP_INFO(robot_->get_logger(), "[MOVING] Entrando al estado - Iniciando movimiento");
  }

  void
  on_do() override
  {
    //ejecutar accción continua: avanzar
    robot_->publish_velocity(0.3, 0.0);
  }

  State* MovingState::check_transitions() override
  {
    if (robot_->get_min_distance() < robot_->get_obstacle_threshold()) {
      RCLCPP_WARN(robot_->get_logger(), "[MOVING] Obstáculo detectado a %.2f m!", robot_->get_min_distance());
      return new StoppedState(robot_);
    }
    return nullptr;
  }

  void
  on_exit() override
  {
    RCLCPP_INFO(robot_->get_logger(), "[MOVING] Saliendo del estado - Iniciando frenado");
    //detener motores antes de cambiar de estado
    robot_->publish_velocity(0.0, 0.0);
  }

  std::string
  get_name() const override
  {
    return "MOVING";
  }
};

}