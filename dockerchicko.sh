#! /bin/bash

xhost +local:root

# export SOFA_ROOT=/sofa/build
export IMAGE=lakehanne/sofa:IAB

if [[ "$(uname -s)" == 'Linux'  ]]; then
    echo -e "OS: Linux. Running docker on $IMAGE\n"
    docker run -ti --rm -v /tmp/.X11-unix:/tmp/.X11-unix:ro \
      -e DISPLAY=$DISPLAY \
      -v ~/catkin_ws/src/superchicko:/root/superchicko:rw \
      $IMAGE
      # -v ~/Documents/superchicko:/opt/ros2_ws/src/superchicko:rw \
elif [[ "$(uname -s)" == 'Darwin' ]]; then
  export ip=$(ifconfig en1 | grep inet | awk '$1=="inet" {print $2}')
  # echo -e "ip: ${ip}"
  xhost + $ip
  docker run --privileged -it --rm -e DISPLAY=${ip}:1 -v /tmp/.X11-unix:/tmp/.X11-unix \
        -v ~/Documents/superchicko:/root/superchicko:rw $IMAGE
  # docker run --privileged -it --rm -e DISPLAY=${ip}:0 -v /tmp/.X11-unix:/tmp/.X11-unix \
  #       -v ~/Documents/superchicko:/root/superchicko jess/firefox
  # docker run -d --name IAB DISPLAY=$ip:0 -v /tmp/.X11-unix:/tmp/.X11-unix ~/Documents/superchicko:/opt/ros_ws/ lakehanne/sofa:IAB

fi

