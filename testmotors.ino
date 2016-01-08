/*
Program to control a dual motor car using Serial input.
*/

// Definitions for PIN constants
#define  MOTORS_ENABLE_PWM_PIN   9 //cip pin 15
#define  LEFT_MOTOR_A_PIN       11 //cip pin 17
#define  LEFT_MOTOR_B_PIN       10 //cip pin 16
#define  RIGHT_MOTOR_A_PIN      12 //cip pin 18
#define  RIGHT_MOTOR_B_PIN      13 //cip pin 19
#define  MOVE_DURATION_MILLIS    400
#define  STOP_DURATION_MILLIS    300

#define SERIAL_BAUD_RATE        4800

#define  CMD_GO_FWD              '1'
#define  CMD_GO_FWD_NUM           1
#define  CMD_GO_BCK              '2'
#define  CMD_GO_BCK_NUM              2
#define  CMD_GO_LEFT             '3'
#define  CMD_GO_LEFT_NUM            3
#define  CMD_GO_RIGHT            '4'
#define  CMD_GO_RIGHT_NUM            4

boolean goingForward  =  false;
boolean goingBack  =  false;
boolean goingRight   =  false;
boolean goingLeft    =  false;
byte serialData = 0;   // for incoming serial data
byte lastCommand = 0;
unsigned long timeMonitor = 0;
unsigned long timeComparator = 0;

void setup() {
  Serial.begin(4800);
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
  enableMotors();
}

void loop() {
    leftMotorForward();
    rightMotorForward();
    delay(2000);
    disableMotors();
    delay(100);
    leftMotorBackwards();
    rightMotorBackwards();
    enableMotors();
    delay(2000);
  while (Serial.available() > 0) {
    // read the incoming byte:
    serialData = Serial.read();
    // say what you got:
    Serial.print("I received: ");
    Serial.println(serialData, DEC);
    
    
    switch (serialData) {
   //   case CMD_GO_FWD:
      case CMD_GO_FWD_NUM:
          break;
     //case CMD_GO_BCK:
      case CMD_GO_BCK_NUM:
          break;
      //case CMD_GO_LEFT:
      case CMD_GO_LEFT_NUM:
          break;
      //case CMD_GO_RIGHT:
      case CMD_GO_RIGHT_NUM:
          break;
      default: 
          fullStop();
          Serial.print("Default: ");
          Serial.println(serialData, DEC);
          break;
          
    } //end switch
  } // end serial.available
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
  disableRightMotor();
  disableLeftMotor();
  goingForward = false;
  goingBack = false;
  goingLeft = false;
  goingRight = false;
  delay(STOP_DURATION_MILLIS);
}

void blink()
{
  digitalWrite(8, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(300);              // wait for a second
  digitalWrite(8, LOW);    // turn the LED off by making the voltage LOW
  delay(300);              // wait for a second  
}
