En ROS 2, para un teleoperador reactivo, lo ideal es que un mismo nodo escuche (subscriba) y hable (publique).
El concepto: Percepción vs. Control
El Callback del Bumper (Percepción): No toma decisiones de movimiento. Solo dice: "Oye, me acaban de avisar que el bumper izquierdo se ha pulsado". Guarda ese estado en una variable. El callback toma las decisiones de percepción , en qué estado estamos, en el de 
parar, girar, ir hacia delante, etc

El Timer (Control): Se ejecuta cada 100ms (10 Hz). Mira las variables de estado y dice: "A ver, ¿está el izquierdo pulsado? Sí. Pues publico giro". El timer es el que toma las decisiones de control, el que publica los mensajes 
de velocidad, el que hace los cálculos.

Necesitamos un proceso intermedio que defina lo de los estados, en qué estado 
estamos, el switch.

El mensaje que se recibe del subscriptor, el tipo de mensaje, hay que traducirlo 
a estados del robot, crear un enum de los estados y utilizar un switch para ir 
cambiando de estado, llamar al callback correspondiente y que haga lo que tenga 
que hacer.


ros2 topic info /events/bumper

Type: kobuki_ros_interfaces/msg/BumperEvent
Publisher count: 1
Subscription count: 0
lovemi@f-l3202-pc30:~$ ros2 interface show kobuki_ros_interfaces/msg/BumperEvent
.# Provides a bumper event.
.# This message is generated whenever a particular bumper is pressed or released.
.# Note that, despite bumper field on SensorState messages, state field is not a
.# bitmask, but the new state of a single sensor.

.# bumper
uint8 LEFT   = 0
uint8 CENTER = 1
uint8 RIGHT  = 2

.# state
uint8 RELEASED = 0
uint8 PRESSED  = 1

uint8 bumper
uint8 state


ros2 topic info /cmd_vel

Type: geometry_msgs/msg/Twist
Publisher count: 0
Subscription count: 1
lovemi@f-l3202-pc30:~$ ros2 interface show geometry_msgs/msg/Twist
.# This expresses velocity in free space broken into its linear and angular parts.

Vector3  linear
    float64 x
    float64 y
    float64 z
Vector3  angular
    float64 x
    float64 y
    float64 z
lovemi@f-l3202-pc30:~$ 


=======================
// Copyright 2026 Intelligent Robotics Lab
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
=======================
