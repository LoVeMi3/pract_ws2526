  * PRÁCTICA 1 

1. Listar nodos en ejecución
   
  /bridge_gz_ros_camera_depth
  
  /bridge_gz_ros_camera_image
  
  /bridge_ros_gz
  
  /fake_bumer_node
  
  /joint_state_publisher
  
  /robot_state_publisher
  
  /ros_gz_sim



2. Identificar topics relevantes (sensores, odometría, comandos)
   
  /clock
  
  /cmd_vel
  
  /events/bumper
  
  /joint_states
  
  /odom
  
  /parameter_events
  
  /rgbd_camera/camera_info
  
  /rgbd_camera/depth_image
  
  /rgbd_camera/depth_image/compressed
  
  /rgbd_camera/depth_image/compressedDepth
  
  /rgbd_camera/depth_image/theora
  
  /rgbd_camera/depth_image/zstd
  
  /rgbd_camera/image
  
  /rgbd_camera/image/compressed
  
  /rgbd_camera/image/compressedDepth
  
  /rgbd_camera/image/theora
  
  /rgbd_camera/image/zstd
  
  /rgbd_camera/points
  
  /robot_description
  
  /rosout
  
  /scan_raw
  
  /tf
  
  /tf_static
  


3. Inspeccionar el tipo de mensajes en los topics principales
   
  /clock >> rosgraph_msgs/msg/Clock

  /cmd_vel >> geometry_msgs/msg/Twist
  
  /events/bumper >> kobuki_ros_interfaces/msg/BumperEvent
  
  /odom >> nav_msgs/msg/Odometry
  
  /parameter_events >> rcl_interfaces/msg/ParameterEvent
  
  /rgbd_camera/camera_info >> sensor_msgs/msg/CameraInfo
  
  /rgbd_camera/depth_image >> sensor_msgs/msg/Image
  
  /rgbd_camera/image >> sensor_msgs/msg/Image
  
  /robot_description >> std_msgs/msg/String
  
  /rosout >> rcl_interfaces/msg/Log
  
  /scan_raw >> sensor_msgs/msg/LaserScan
  
  /tf >> tf2_msgs/msg/TFMessage
  
  /tf_static >> tf2_msgs/msg/TFMessage
  


4. Observar mensajes en tiempo real en al menos dos topics (uno sensorial y otro de estado)
   
  /scan_raw >> 
  
    header:
  
    stamp:
  
      sec: 393
    
      nanosec: 0
    
    frame_id: laser_link
  
    angle_min: -1.5707999467849731

    angle_max: 1.5707999467849731

    angle_increment: 0.008750975131988525

    time_increment: 0.0

    scan_time: 0.0

    range_min: 0.17000000178813934

    range_max: 3.5

    ranges:

    - 2.1051888465881348
  
    - 1.9885499477386475
  
    - 1.8913618326187134
  
    - 1.7902170419692993
  
    - 1.6973944902420044
  
    - 1.5984212160110474
  
    - .inf
  
    ...

  /robot_description >> 
  
    data: '<?xml version="1.0" ?>
  
    <!-- =================================================================================== -->
  
    <!-- |    Th...'
  
    ---

    -->



5. Medir la frecuencia de publicación de un topic (poreje, odometría o láser)
   
  ros2 topic hz /odom >>
  
    average rate: 16.807
  
          min: 0.019s max: 0.100s std dev: 0.03112s window: 18
          
    average rate: 16.849
  
          min: 0.019s max: 0.102s std dev: 0.03109s window: 36
          
    average rate: 16.806
  
          min: 0.019s max: 0.106s std dev: 0.03116s window: 54

  ros2 topic hz /scan_raw >>
  
    average rate: 1.692
  
          min: 0.575s max: 0.608s std dev: 0.01337s window: 3
          
    average rate: 1.698
  
          min: 0.575s max: 0.608s std dev: 0.01167s window: 5
          
    average rate: 1.700
  
          min: 0.570s max: 0.608s std dev: 0.01320s window: 7
          


6. Localizar el topic de comandos de velocidad y publicar un comando de prueba (si procede según el sistema)
   
  lovemi@f-l2009-pc04:~$ ros2 topic info /cmd_vel 
  
      Type: geometry_msgs/msg/Twist
    
      Publisher count: 0
    
      Subscription count: 1
    

  lovemi@f-l2009-pc04:~$ ros2 topic pub /cmd_vel geometry_msgs/msg/Twist "{linear: {x: 0.5}, angular: {z: 0.2}}"
  
      publisher: beginning loop
    
      publishing #1: geometry_msgs.msg.Twist(linear=geometry_msgs.msg.Vector3(x=0.5, y=0.0, z=0.0), angular=geometry_msgs.msg.Vector3(x=0.0, y=0.0, z=0.2))

      publishing #2: geometry_msgs.msg.Twist(linear=geometry_msgs.msg.Vector3(x=0.5, y=0.0, z=0.0), angular=geometry_msgs.msg.Vector3(x=0.0, y=0.0, z=0.2))

      publishing #3: geometry_msgs.msg.Twist(linear=geometry_msgs.msg.Vector3(x=0.5, y=0.0, z=0.0), angular=geometry_msgs.msg.Vector3(x=0.0, y=0.0, z=0.2))


7. Inspeccionar el estado del sistema y visualizar los sensores del robot en RViz2
   
  En la carpeta de la practica 1, a parte del README.md esta la grafica creada del estado del sistema, con ros2 run rqt_graph rqt_graph.
  Y luego hacemos: ros2 run rviz2 rviz2, y he metido en esta práctica una imgaen de lo que se veía en el rviz2, con las pestañas y demás que sirven para añadir topics.
