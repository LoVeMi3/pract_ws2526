# PRÁCTICA 5 #

ACORDARSE DE COMPILAR EL ASR Y HACER source/install/setup.bash DE ESTE

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

_3._ y _4._ creo que tendré que hacerlos en el lab porque no tengo Nav2 y no lo
voy a descargar ahora. Prefiero ponerme a organizar los nodos y tal.

### Paso 2: capacidad de diálogo y detección de presencia ###
- Opción 1: publicar sintéticamente en un topic /person_detected la existencia
  de una detección de persona (tipo std_msgs/Bool)

### Paso 3: behaviour tree completo de la misión
Único Behaviour Tree
