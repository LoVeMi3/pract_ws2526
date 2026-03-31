# PRÁCTICA 5 #

ACORDARSE DE COMPILAR EL ASR Y HACER source/install/setup.bash DE ESTE

Actualizar CMakeLists.txt

Añadir GoToPoseAction.cpp y WaiterBT.cpp a las fuentes del ejecutable waiter, 
y añadir nav2_msgs y rclcpp_action como dependencias.


[detection] nodo ROS  →  publica /person_detected (Bool)

[waiter_bt] nodo ROS  →  WaitForPerson suscribe a /person_detected



1. 📍 ¿Qué pasa si los waypoints son inventados?
   El robot intentará ir a coordenadas que no existen en el mapa real — Nav2 puede aceptar el goal pero el robot acabará en un sitio incorrecto, chocará, o el path planner fallará. Tienes que obtener las coordenadas reales.
   Cómo obtener las coordenadas correctas con RViz2:

Lanza el simulador + Nav2 con tu mapa
En RViz2, usa el botón "2D Pose Estimate" para establecer la pose inicial     Angle: -0.00580479 ; -5.41 ; 0.0
Mueve el robot manualmente con Nav2 o teleop hasta la zona de cocina          Angle: 4.36921 ; -8.41 ; -3.56
En una terminal aparte:

bashros2 topic echo /amcl_pose --once
Anota position.x, position.y y orientation.w

Repite para la zona de cliente y home
Sustituye en el .hpp:

cppstd::map<std::string, std::array<double, 3>> waypoints_ = {
{"kitchen", {X_REAL, Y_REAL, 1.0}},
{"client",  {X_REAL, Y_REAL, 1.0}},
{"home",    {X_REAL, Y_REAL, 1.0}}
};

Para una práctica, orientation.w = 1.0 (sin rotación) es suficiente en todos los waypoints.



### Paso 1: preparación del entorno y capacidad de nevegación ###
lorea@Baymax6:~$ ros2 topic list

    /clock
    /cmd_vel
    /events/bumper
    /joint_states
    /odom                     #odometría
    /parameter_events
    /rgbd_camera/camera_info
    /rgbd_camera/depth_image
    /rgbd_camera/image
    /rgbd_camera/points
    /robot_description
    /rosout
    /scan_raw                 #láser
    /tf
    /tf_static

_Confirmar_ _que_ _se_ _dispone_ _de_ _un_ _mapa_ _del_ _entorno_ _navegable_

ros2 launch nav2_bringup navigation_launch.py map:=/practica5/map/aws_bookstore.yaml
ros2 launch kobuki simulation.launch.py

Node(
package='nav2_map_server',
executable='map_server',
name='map_server',
parameters=[{'yaml_filename': '/ruta/a/map.yaml'}]
)

_3._ y _4._ creo que tendré que hacerlos en el lab porque no tengo Nav2 y no lo
voy a descargar ahora. Prefiero ponerme a organizar los nodos y tal.

### Paso 2: capacidad de diálogo y detección de presencia ###
- Opción 1: publicar sintéticamente en un topic /person_detected la existencia
  de una detección de persona (tipo std_msgs/Bool)

### Paso 3: behaviour tree completo de la misión
Único Behaviour Tree


ros2 launch simple_hri free_simple_hri.launch.py

ros2 run hri_examples simple_hri_app
