/**
 * Athena motion control (Vehicle side)
 * 2019/03/01
 * Copyright (c) Cen XIN
 */

// For parsing commands
String cmd = "";
char car_direction = 'x';
int car_speed = 0;
int car_distance = 0;
int car_degree = 0;
int is_moving = 0;
int is_turning = 0;

// For controlling wheels
int E1 = 5;   // Pin 5 = PWM control for Motor 2
int M1 = 4;   // Pin 4 = Direction control for Motor 2
int E2 = 6;   // Pin 6 = PWM control for Motor 1                     
int M2 = 7;   // Pin 7 = Direction control for Motor 1

// For recording distances (ISR)
const byte interruptPin_left = 2;
const byte interruptPin_right = 3;
volatile int state_left = 0;
volatile int state_right = 0;

void change_left() {
  state_left++;
}

void change_right() {
  state_right++;
}

void parse_cmd() {
  int i, j;
  int dist_digit, temp = 1;
  
  car_direction = cmd[0];
  if (cmd.length() <= 0) {
    return;
  } else if (cmd.length() == 1) {
    switch(car_direction) {
      case 'w':
      case 's':
        car_speed = 2;
        car_distance = 20;
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
    switch(car_direction) {
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

void clear_state() {
  car_direction = 0;
  car_speed = 0;
  car_distance = 0;
  car_degree = 0;
  state_left = 0;
  state_right = 0;  
  is_moving = 0;  
  is_turning = 0;    
}

void setup() {
  // initialize both serial ports:
  Serial.begin(9600);
  Serial3.begin(9600);

  pinMode(interruptPin_left, INPUT_PULLUP);
  pinMode(interruptPin_right, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin_left), change_left, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptPin_right), change_right, CHANGE);
}

void loop() {
  while(Serial3.available() > 0) {
    cmd += char(Serial3.read());
    delay(2);
  }
  
  if (cmd.length() > 0) {
    parse_cmd();
    cmd = "";

    switch(car_direction) {
      case 'w':
        is_moving = 1;
        is_turning = 0;
        digitalWrite(M1, LOW);
        digitalWrite(M2, LOW);
        switch(car_speed) {
          case 0:
            analogWrite(E1, 0);
            analogWrite(E2, 0);
            break;
          case 1:
            analogWrite(E1, 150);
            analogWrite(E2, 150);
            break;          
          case 2:
            analogWrite(E1, 200);
            analogWrite(E2, 200);
            break;          
          case 3:
            analogWrite(E1, 255);
            analogWrite(E2, 255);
            break;          
          default:
            analogWrite(E1, 200);
            analogWrite(E2, 200);
            break;          
        }
        break;
      case 's':
        is_moving = 1;
        is_turning = 0;
        digitalWrite(M1, HIGH);
        digitalWrite(M2, HIGH);
        switch(car_speed) {
          case 0:
            analogWrite(E1, 0);
            analogWrite(E2, 0);
            break;
          case 1:
            analogWrite(E1, 150);
            analogWrite(E2, 150);
            break;          
          case 2:
            analogWrite(E1, 200);
            analogWrite(E2, 200);
            break;          
          case 3:
            analogWrite(E1, 255);
            analogWrite(E2, 255);
            break;          
          default:
            analogWrite(E1, 200);
            analogWrite(E2, 200);
            break;          
        }
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

  if (is_moving == 1 && ((state_left/384.0)*20.7 > car_distance || (state_right/384.0)*20.7 > car_distance)) {
    analogWrite(E1, 0);
    analogWrite(E2, 0);
    clear_state();
  } else {
    if (is_turning == 1 && ((state_left/384.0)*180.0 > car_degree || (state_right/384.0)*180.0 > car_degree)) {
      delay(100);
      analogWrite(E1, 0);
      analogWrite(E2, 0);
      clear_state();   
    }
  }
  if (is_moving == 0 && is_turning == 0) {
    if (state_left != 0 || state_right != 0) {
      clear_state();
    }
  }
  delay(100);
}
