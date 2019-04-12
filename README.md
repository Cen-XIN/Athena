# Athena

This project is to draw a comparison of different SLAM algorithms on several aspects. The experiment is conducted on a distributed system with a laptop as the master, a Raspberry Pi as the slave, and an Arduino as the control center under ROS.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisites

#### Hardware

* Master machine (your laptop or desktop)

    * Intel(R) i5-3320M CPU or higher

    * Total memory 8GB or more

* Slave machine (your robotic car)

    * Raspberry Pi 3B (or 3B+)

    * Arduino Mega 2560

    * DFRobot 2x1A DC Motor Shied for Arduino
    
    * SLAMTEC RPLidar A1
    
    * SparkFun Hobby Motor and Encoder Kit
    
    * SparkFun Logic Level Converter - Bi-Directional
    
* Battery, power bank, and jump cables, tec...

#### Software

* Ubuntu 14.04.6 LTS, Trusty Tahr (on your master machine)

* Raspbian GNU/Linux 8 Jessie (on your RPi)

* ROS indigo (on both of your machine)

* Arduino IDE 1.8.8 or higher (for uploading codes to your board)

### Installing

* First change the swap size on your RPi in case suffering "out of memory" while compiling ROS.

```shell
sudo sed 's/^CONF_SWAPSIZE =.*/CONF_SWAPSIZE = 1024/g' /etc/dphys-swapfile
sudo /etc/init.d/dphys-swapfile restart
```

* Install [ROS Indigo](http://wiki.ros.org/ROSberryPi/Installing%20ROS%20Indigo%20on%20Raspberry%20Pi) on your RPi. Better to install "ROS-Comm", or your RPi may crash if installing "Desktop Full" :-P

* Install [ROS Indigo Desktop Full](http://wiki.ros.org/indigo/Installation/Ubuntu) on your master machine.

* Set up the [catkin workspace](http://wiki.ros.org/catkin/Tutorials/create_a_workspace) on both of your machines.

* Git clone my repo to both of your machines.

* Put "arduino/vehicle_node" to your Arduino sketch folder. Put "laptop/catkin_ws/src" to your catkin workspace on your master machine. Put "pi/catkin_ws/src" to your catkin workspace on your RPi. Put "pi/catkin_ws/pi_node.py" to your home directory.

* Set up [ROS Serial Arduino](http://wiki.ros.org/rosserial_arduino/Tutorials) on both of your machines. Please remember your should install "ros_lib" on the Arduino IDE so as to complie my code successfully.

## Running the tests

TODO...

## Deployment

TODO...

## License

This project is licensed under the MIT License.

## Acknowledgments

TODO...

