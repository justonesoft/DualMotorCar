/*
Program to control a dual motor car using Serial input.
*/

// Definitions for PIN constants
#define  MOTORS_ENABLE_PWM_PIN   9 //cip pin 15
#define  LEFT_MOTOR_A_PIN       11 //cip pin 17
#define  LEFT_MOTOR_B_PIN       10 //cip pin 16
#define  RIGHT_MOTOR_A_PIN      12 //cip pin 18
#define  RIGHT_MOTOR_B_PIN      13 //cip pin 19
#define  MOVE_DURATION_MILLIS    100
#define LEFT_RIGHT_MOVE_DURATION  100
#define  STOP_DURATION_MILLIS    10

#define SERIAL_BAUD_RATE        4800

#define  CMD_STOP               0
#define  CMD_STOP_CHAR          '0'
#define  CMD_GO_FWD             1
#define  CMD_GO_FWD_CHAR        '1'
#define  CMD_GO_BCK             2
#define  CMD_GO_BCK_CHAR        '2'
#define  CMD_GO_LEFT            3
#define  CMD_GO_LEFT_CHAR       '3'
#define  CMD_GO_RIGHT           4
#define  CMD_GO_RIGHT_CHAR      '4'
#define  CMD_NO_OP          255

// responses to be sent back through serial
#define RESP_STOP               "#0"
#define RESP_FWD                "#1"
#define RESP_BCK                "#2"
#define RESP_LEFT               "#3"
#define RESP_RIGHT              "#4"

boolean goingForward  =  false;
boolean goingBack  =  false;
boolean goingRight   =  false;
boolean goingLeft    =  false;
boolean usingSerialLog = true;
byte serialData = CMD_NO_OP;   // for incoming serial data
byte lastCommand = CMD_NO_OP;

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  pinMode(MOTORS_ENABLE_PWM_PIN, OUTPUT); // both motor-enable pins are connected to this
  digitalWrite(MOTORS_ENABLE_PWM_PIN, LOW);
  pinMode(LEFT_MOTOR_A_PIN, OUTPUT);
  pinMode(LEFT_MOTOR_B_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_A_PIN, OUTPUT);
  pinMode(RIGHT_MOTOR_B_PIN, OUTPUT);
  pinMode(8, OUTPUT);
  disableMotors();
  fullStop();
  Serial.println("READY");
  serialData = CMD_NO_OP;
  lastCommand = CMD_NO_OP;
}

void loop() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    serialData = Serial.read();
    // say what you got:
    serialPrint("I received: ");
    serialPrintln(serialData);

    lastCommand = serialData;
  }

  // not sure if to put this switch inside "if" or not
  switch (lastCommand) {
    case CMD_STOP:
    case CMD_STOP_CHAR:
      serialPrint("STOP");
      serialPrint(RESP_STOP);
      fullStop();
      break;
    case CMD_GO_FWD:
    case CMD_GO_FWD_CHAR:
        if (!goingForward) {
          // currently car is not going forward
          // so we need to first stop the car
          fullStop();
        }
        moveForward();
        serialPrint(RESP_FWD);
//          delay(MOVE_DURATION_MILLIS);
        break;
    case CMD_GO_BCK:
    case CMD_GO_BCK_CHAR:
        if (!goingBack) {
          // currently car is not going backwards
          // so we need to first stop the car
          fullStop();
        }
        moveBackwards();
        serialPrint(RESP_BCK);
//         delay(MOVE_DURATION_MILLIS);
        break;
    case CMD_GO_LEFT:
    case CMD_GO_LEFT_CHAR:
        if (!goingLeft) {
          // currently car is not going left
          // so we need to first stop the car
          fullStop();
        }
        turnLeft();
        serialPrint(RESP_LEFT);
//          delay(LEFT_RIGHT_MOVE_DURATION);
        break;
    case CMD_GO_RIGHT:
    case CMD_GO_RIGHT_CHAR:
        if (!goingRight) {
          // currently car is not going left
          // so we need to first stop the car
          fullStop();
        }
        turnRight();
        serialPrint(RESP_RIGHT);
//          delay(LEFT_RIGHT_MOVE_DURATION);
        break;
    case CMD_NO_OP:
        // don't do anything
        break;
    default: 
        fullStop();
        serialPrint("Default: ");
        serialPrintln(lastCommand);
        break;
        
  } //end switch
  
  lastCommand = CMD_NO_OP;
}

void demo() {
  // move car forward, backwards, rotate left and rotate right

  moveForward();
  delay(MOVE_DURATION_MILLIS);

  fullStop();
  
  moveBackwards();
  delay(MOVE_DURATION_MILLIS);
  
  fullStop();
  
  turnLeft();
  delay(MOVE_DURATION_MILLIS);
  
  fullStop();

  turnRight();
  delay(MOVE_DURATION_MILLIS); 
}

/*
Desc: Move forward with no PWM. Motors will have full power from external source.
*/
void moveForward()
{
  // Move forward with no PWM. Motors will have full power from external source

  if (goingForward) {
    serialPrintln("Already FWD");
    // already moving full speed forward so do nothing
    return;
  }
  serialPrintln("Go FWD");
  enableMotors();
  
  // Start left motor
  leftMotorForward();
  
  // Start right motor
  rightMotorForward();
  
  goingLeft = false;
  goingForward = true;
  goingBack = false;
  goingRight = false;
}

/*
Desc: Move backwards with no PWM. Motors will have full power from external source.
*/
void moveBackwards()
{
  // Move forward with no PWM. Motors will have full power from external source

  if (goingBack) {
    serialPrintln("Already BCK");
    // already moving full speed backwards so do nothing
    return;
  }
  serialPrintln("Go BCK");
  enableMotors();
  
  // Start left motor
  leftMotorBackwards();
  
  // Start right motor
  rightMotorBackwards();
  
  goingLeft = false;
  goingForward = false;
  goingBack = true;
  goingRight = false;
}

void turnLeft()
{  // Left motor forward, Right motor backwards
  if (goingLeft) {
    serialPrintln("Already LFT");
    return;
  }
  serialPrintln("Go LFT");

  enableMotors();

  leftMotorBackwards();
  rightMotorForward();
  goingLeft = true;
  goingForward = false;
  goingBack = false;
  goingRight = false;
}

void turnRight()
{  // Left motor backwards, Right motor forward
  if (goingRight) {
    serialPrintln("Already RGT");
    return;
  }
  serialPrintln("Go RGT");

  enableMotors();

  leftMotorForward();
  rightMotorBackwards();
  goingLeft = false;
  goingForward = false;
  goingBack = false;
  goingRight = true;
}

void leftMotorForward()
{
  // Spin left motor forward
  digitalWrite(LEFT_MOTOR_A_PIN, LOW);
  digitalWrite(LEFT_MOTOR_B_PIN, HIGH);
}

void leftMotorBackwards()
{
  // Spin left motor backwards
  digitalWrite(LEFT_MOTOR_A_PIN, HIGH);
  digitalWrite(LEFT_MOTOR_B_PIN, LOW);
}

void rightMotorForward()
{
  // Spin right motor forward
  digitalWrite(RIGHT_MOTOR_A_PIN, LOW);
  digitalWrite(RIGHT_MOTOR_B_PIN, HIGH);
}

void rightMotorBackwards()
{
  // Spin right motor backwards
  digitalWrite(RIGHT_MOTOR_A_PIN, HIGH);
  digitalWrite(RIGHT_MOTOR_B_PIN, LOW);
}

void enableMotors()
{
  // Give motors the full power from external source. No PWM
  digitalWrite(MOTORS_ENABLE_PWM_PIN, HIGH);  
}

void disableMotors()
{
  // First disable both motor-enable pins
  // This should be enough to stop both motors
  digitalWrite(MOTORS_ENABLE_PWM_PIN, LOW);
  disableLeftMotor();
  disableRightMotor();
}

void disableLeftMotor()
{
  // Sets both control pins for left motor to LOW
  digitalWrite(LEFT_MOTOR_A_PIN, LOW);
  digitalWrite(LEFT_MOTOR_B_PIN, LOW);
}

void disableRightMotor()
{
  // Sets both control pins for right motor to LOW
  digitalWrite(RIGHT_MOTOR_A_PIN, LOW);
  digitalWrite(RIGHT_MOTOR_B_PIN, LOW);  
}

void fullStop()
{
  // Now set both motor-control pins to LOW. There will be 4 pins to set LOW
  disableMotors();
  goingForward = false;
  goingBack = false;
  goingLeft = false;
  goingRight = false;
}

void blink()
{
  digitalWrite(8, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(300);              // wait for a second
  digitalWrite(8, LOW);    // turn the LED off by making the voltage LOW
  delay(300);              // wait for a second  
}

void serialPrint(char* message) {
  if (usingSerialLog) {
    Serial.print(message);  
  }
}

void serialPrintln(char* message) {
  if (usingSerialLog) {
    Serial.println(message);  
  }
}

void serialPrint(byte val) {
  if (usingSerialLog) {
    Serial.print(val, DEC);  
  }
}

void serialPrintln(byte val) {
  if (usingSerialLog) {
    Serial.println(val, DEC);  
  }
}

