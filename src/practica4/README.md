## README ##

lovemi@f-l3202-pc30:~$ ros2 topic list

    /clicked_point
    /clock
    /cmd_vel                    #topic que nos interesa
    /events/bumper
    /goal_pose
    /initialpose
    /joint_states
    /odom                   #topic que nos interesa
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
    /scan_raw                   #topic que nos interesa
    /tf
    /tf_static

lovemi@f-l3202-pc30:~$ ros2 topic info /scan_raw

    Type: sensor_msgs/msg/LaserScan
    Publisher count: 1
    Subscription count: 2
    lovemi@f-l3202-pc30:~$ 

lovemi@f-l3202-pc30:~$ ros2 topic info /odom

    Type: nav_msgs/msg/Odometry
    Publisher count: 1
    Subscription count: 1
    lovemi@f-l3202-pc30:~$ 

lovemi@f-l3202-pc30:~$ ros2 topic info /cmd_vel

    Type: geometry_msgs/msg/Twist
    Publisher count: 0
    Subscription count: 1
    lovemi@f-l3202-pc30:~$ 

### Adjuntado un pdf con el grafo de las transformadas dónde encontramos la presencia ###
### de los marcos de _odom_, _base-link_ y _base-scan_. Aunque tienen distinto nombre. ###

