/**
 * Athena Vehicle Node
 * For controlling the robotic car 
 * with the odometry data sent to 
 * the Raspberry Pi Node
 * 2019/03/22
 * Copyright (c) Cen XIN 
 */

#include <ros.h>
#include <ros/time.h>
#include <tf/tf.h>
#include <tf/transform_broadcaster.h>

#define DIST_PER_TICK ((PI * 0.065) / 384.0)
#define DIST_BETWEEN_WHEELS 0.146

// ROS parameters
ros::NodeHandle  nh;
geometry_msgs::TransformStamped t;
tf::TransformBroadcaster broadcaster;

// ROS topics
char base_link[] = "/base_link";
char odom[] = "/odom";

// Initial position
double x      = 0.0;
double y      = 0.0;
double theta  = 0.0;

// Vehicle status
String cmd = "";
char car_direction = 'x';
int car_speed = 0;
int car_distance = 0;
int car_degree = 0;
int is_moving = 0;
int is_turning = 0;

// Motor pins
int E1 = 5;   // Right wheel power
int M1 = 4;   // Right wheel direction
int E2 = 6;   // Left wheel power
int M2 = 7;   // Left wheel direction

// Interrupt pins
const byte interruptPin_left = 2;
const byte interruptPin_right = 3;

// Wheel encoder information
volatile int state_left = 0;    // Ticks of every movement
volatile int state_right = 0;
volatile long left_ticks = 0;   // Total ticks
volatile long right_ticks = 0;
long prev_left_ticks = 0;       // Total ticks of last time
long prev_right_ticks = 0;

// Speed of master motor and slave motor
int master_speed = 0;
int slave_speed = master_speed;

/**
 * Utilities
 */

/**
 * Monitor the changes on
 * left encoder
 */
void changeLeft();

/**
 * Monitor the changes on
 * right encoder
 */
void changeRight();

/**
 * Parse the command received
 * from the UART port (from 
 * Raspberry Pi Node)
 */
void parseCommand();

/**
 * Write the speed parameter (
 * range from 0 to 9) to the
 * motors
 */
void writeSpeed(int car_speed);

/**
 * Write the direction information
 * to the motors
 */
void writeDirection(int car_direction, int car_speed);

/**
 * Check whether the vehicle
 * runs a specified distance
 */
void checkDistance();

/**
 * Check whether the status 
 * of the vehicle is cleared
 */
void checkClearance();

/**
 * Clear the status of the
 * vehicle
 */
void clearStatus();

/**
 * Compute odometry data
 * (need to be modified according
 * to different vehicles)
 */
void computeOdom(long delta_left_ticks, long delta_right_ticks, double &dx, double &dtheta);

void setup() {
  // Setup ROS
  nh.initNode();
  broadcaster.init(nh);
  // Initialize both serial ports:
  Serial3.begin(9600);
  // Setup ISR pins
  pinMode(interruptPin_left, INPUT_PULLUP);
  pinMode(interruptPin_right, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin_left), changeLeft, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptPin_right), changeRight, CHANGE);
}

void loop() {
  double dtheta = 0.0;
  double dx = 0.0;
  int now_left_ticks;
  int now_right_ticks;

  while (Serial3.available() > 0) {
    cmd += char(Serial3.read());
    delay(2);
  }

  if (cmd.length() > 0) {
    // Get direction and speed from command
    parseCommand();
    // Clear the command buffer
    cmd = "";
    // Write direction and speed into motors
    writeDirection(car_direction, car_speed);
  }

  // Check distance
  checkDistance();
  // Check if clearance is finished, otherwise clear again
  checkClearance();

  // Calculate odometry data
  now_left_ticks = left_ticks;
  now_right_ticks = right_ticks;
  computeOdom((now_left_ticks - prev_left_ticks), (now_right_ticks - prev_right_ticks), dx, dtheta);
  prev_left_ticks = now_left_ticks;
  prev_right_ticks = now_right_ticks;
  x += cos(theta) * dx;
  y += sin(theta) * dx;
  theta += dtheta;
  while (theta > 3.14)
    theta -= 3.14;

  // tf odom->base_link
  t.header.frame_id = odom;
  t.child_frame_id = base_link;

  t.transform.translation.x = x;
  t.transform.translation.y = y;

  t.transform.rotation = tf::createQuaternionFromYaw(theta);
  t.header.stamp = nh.now();

  broadcaster.sendTransform(t);
  nh.spinOnce();
    
  delay(10);
}

void changeLeft() {
  state_left++;
  left_ticks++;
}

void changeRight() {
  state_right++;
  right_ticks++;
}

void parseCommand() {
  int i, j;
  int dist_digit, temp = 1;

  car_direction = cmd[0];
  if (cmd.length() <= 0) {
    return;
  } else if (cmd.length() == 1) {
    switch (car_direction) {
      case 'w':
      case 's':
        car_speed = 2;
        car_distance = 20;
        break;
      case 'q':
      case 'e':
        car_distance = 30;
        break;
      case 'a':
      case 'd':
        car_degree = 10;
        break;
      default:
        car_speed = 2;
        car_distance = 30;
        break;
    }
  } else if ((car_direction == 'w' || car_direction == 's') && cmd.length() == 3) {
    car_speed = int(cmd[2] - 48);
    car_distance = 20;
  } else {
    switch (car_direction) {
      case 'w':
      case 's':
        dist_digit = cmd.length() - 4;
        car_speed = int(cmd[2] - 48);
        for (i = 4; cmd[i] != '\0'; i++) {
          cmd[i] -= 48;
          for (j = 0; j < dist_digit - 1; j++) {
            temp *= 10;
          }
          temp *= int(cmd[i]);
          car_distance += temp;
          temp = 1;
          dist_digit--;
        }
        break;
      case 'a':
      case 'd':
        dist_digit = cmd.length() - 2;
        for (i = 0; cmd[i] != '\0'; i++) {
          if (i >= 2) {
            cmd[i] -= 48;
            for (j = 0; j < dist_digit - 1; j++) {
              temp *= 10;
            }
            temp *= int(cmd[i]);
            car_degree += temp;
            temp = 1;
            dist_digit--;
          }
        }
        break;
      default:
        car_direction = 'x';
        car_degree = 0;
        car_speed = 0;
        car_distance = 0;
        break;
    }
  }
}

void writeSpeed(int car_speed) {
  switch (car_speed) {
    case 0:
      master_speed = 165;
      break;
    case 1:
      master_speed = 175;
      break;
    case 2:
      master_speed = 185;
      break;
    case 3:
      master_speed = 195;
      break;
    case 4:
      master_speed = 205;
      break;
    case 5:
      master_speed = 215;
      break;
    case 6:
      master_speed = 225;
      break;
    case 7:
      master_speed = 235;
      break;
    case 8:
      master_speed = 245;
      break;
    case 9:
      master_speed = 255;
      break;
    default:
      master_speed = 255;
      break;
  }
  slave_speed = master_speed - 20;
  analogWrite(E2, master_speed);
  analogWrite(E1, slave_speed);
}

void writeDirection(int car_direction, int car_speed) {
  switch (car_direction) {
    case 'w':
      is_moving = 1;
      is_turning = 0;
      digitalWrite(M1, LOW);
      digitalWrite(M2, LOW);
      writeSpeed(car_speed);
      break;
    case 's':
      is_moving = 1;
      is_turning = 0;
      digitalWrite(M1, HIGH);
      digitalWrite(M2, HIGH);
      writeSpeed(car_speed);
      break;
    case 'q':
      is_moving = 1;
      is_turning = 0;
      digitalWrite(M1, LOW);
      digitalWrite(M2, LOW);
      analogWrite(E1, 225);
      analogWrite(E2, 225);
      break;    
    case 'e':
      is_moving = 1;
      is_turning = 0;
      digitalWrite(M1, LOW);
      digitalWrite(M2, LOW);
      analogWrite(E1, 195);
      analogWrite(E2, 255);
      break;        
    case 'a':
      is_turning = 1;
      is_moving = 0;
      digitalWrite(M1, LOW);
      digitalWrite(M2, HIGH);
      analogWrite(E1, 255);
      analogWrite(E2, 255);
      break;
    case 'd':
      is_turning = 1;
      is_moving = 0;
      digitalWrite(M1, HIGH);
      digitalWrite(M2, LOW);
      analogWrite(E1, 255);
      analogWrite(E2, 255);
      break;
    case 'x':
      is_moving = 0;
      is_turning = 0;
      digitalWrite(M1, LOW);
      digitalWrite(M2, LOW);
      analogWrite(E1, 0);
      analogWrite(E2, 0);
      break;
    default:
      is_moving = 0;
      is_turning = 0;
      digitalWrite(M1, LOW);
      digitalWrite(M2, LOW);
      analogWrite(E1, 0);
      analogWrite(E2, 0);
      break;
  }
}

void clearStatus() {
  car_direction = 0;
  car_speed = 0;
  car_distance = 0;
  car_degree = 0;
  state_left = 0;
  state_right = 0;
  is_moving = 0;
  is_turning = 0;
  master_speed = 0;
  slave_speed = 0;
}

void computeOdom(long delta_left_ticks, long delta_right_ticks, double &dx, double &dtheta) {
  double left_dist = delta_left_ticks * DIST_PER_TICK;
  double right_dist = delta_right_ticks * DIST_PER_TICK;
  dtheta = fabs(left_dist - right_dist) / DIST_BETWEEN_WHEELS;
  dx = (left_dist + right_dist) / 2.0;
}

void checkDistance() {
  if (is_moving == 1 && ((state_left / 384.0) * 20.7 > car_distance || (state_right / 384.0) * 20.7 > car_distance)) {
    analogWrite(E1, 0);
    analogWrite(E2, 0);
    clearStatus();
  } else {
    if (is_turning == 1 && ((state_left / 384.0) * 180.0 > car_degree || (state_right / 384.0) * 180.0 > car_degree)) {
      delay(100);
      analogWrite(E1, 0);
      analogWrite(E2, 0);
      clearStatus();
    }
  }
}

void checkClearance() {
  if (is_moving == 0 && is_turning == 0) {
    if (state_left != 0 || state_right != 0) {
      clearStatus();
    }
  }
}
