El mensaje que se recibe del subscriptor, el tipo de mensaje, hay que traducirlo 
a estados del robot, crear un enum de los estados y utilizar un switch para ir 
cambiando de estado, llamar al calback correspondiente y que haga lo que tenga 
que hacer.

El timer es el que toma las decisiones de control, el que publica los mensajes 
de velocidad, el que hace los cálculos.
Necesitamos un proceso intermedio que defina lo de los estados, en qué estado 
estamos, el switch.
El callback toma las deciosiones de percepción , en qué estado estamos, en el de 
parar, girar, ir hacia delante, etc.


ros2 topic info /events/bumper

Type: kobuki_ros_interfaces/msg/BumperEvent
Publisher count: 1
Subscription count: 0
lovemi@f-l3202-pc30:~$ ros2 interface show kobuki_ros_interfaces/msg/BumperEvent
# Provides a bumper event.
# This message is generated whenever a particular bumper is pressed or released.
# Note that, despite bumper field on SensorState messages, state field is not a
# bitmask, but the new state of a single sensor.

# bumper
uint8 LEFT   = 0
uint8 CENTER = 1
uint8 RIGHT  = 2

# state
uint8 RELEASED = 0
uint8 PRESSED  = 1

uint8 bumper
uint8 state


ros2 topic info /cmd_vel

Type: geometry_msgs/msg/Twist
Publisher count: 0
Subscription count: 1
lovemi@f-l3202-pc30:~$ ros2 interface show geometry_msgs/msg/Twist
# This expresses velocity in free space broken into its linear and angular parts.

Vector3  linear
	float64 x
	float64 y
	float64 z
Vector3  angular
	float64 x
	float64 y
	float64 z
lovemi@f-l3202-pc30:~$ 

