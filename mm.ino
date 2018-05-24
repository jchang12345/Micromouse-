#include <LedDisplay.h>
#include <Encoder.h>
#include <SoftwareSerial.h>
//#include <StackArray.h>

// =========================================================== Pin Declarations ==================================================
#define leftForward       3
#define leftBackward      4
#define rightForward      10
#define rightBackward     9
#define BUTTON            22
#define FLED1             0
#define FLED2             6
#define LLED              21
#define RLED              17
#define FREC1             23
#define FREC2             A14
#define LREC              20
#define RREC              19
SoftwareSerial            bt(7,8);
Encoder left(1,2);
Encoder right(12,11);

LedDisplay mydisplay =    LedDisplay(15, 16, 5, 14, 13, 4);
// --------------------------------------------------------------------------------------------------------------------------------





// ====================================================== Constants and Thresholds ================================================
#define DEFAULT_SPEED     48
#define SPEED_LIMIT       68


#define TURN_SPEED        0
#define TURN_SPEED_LIMIT  45

#define TURN_COUNT        685
#define rightWallDetect   510
#define leftWallDetect    510

#define leftWallThresh    390
#define rightWallThresh   390
#define frontWallDetect   820
#define frontWallThresh   820

#define CELL_COUNT        2190
// -------------------------------------------------------------------------------------------------------------------------------





// ============================================================ PID Variables =====================================================
double kp = 0.4;
double kd = 2;

double irKp = 0.0000085;
double irKd = 37;

double rightKpIR = 0.15;
double rightKdIR = 3.6;

double leftKpIR = 0.15;
double leftKdIR = 3.6;

double turnKp = 0.22;
double turnKd = 3.2;

double distKp = 0.5;
double distKd = 2;

// Encoder Counts
volatile bool led = false;
volatile int encError = 0;
volatile int deltaEncError = 0;
volatile int correction = 0;
volatile int prevEncError = 0;

volatile int irError = 0;
volatile int deltaIRError = 0;
volatile int irCorrection = 0;
volatile int prevIRError = 0;

// IR READINGS
volatile int leftREC = 0;
volatile int rightREC = 0;
volatile int frontREC1 = 0;
volatile int frontREC2 = 0;

volatile int rightError = 0;
volatile int deltaRightError = 0;
volatile int rightCorrection = 0;
volatile int prevRightError = 0;

volatile int leftError = 0;
volatile int deltaLeftError = 0;
volatile int leftCorrection = 0;
volatile int prevLeftError = 0;

volatile int pidMode = 0; // 0: Both Walls  1: Left Wall  2: Right Wall   3: No walls

// Encoder errors during turns
volatile int L_Error;
volatile int R_Error;

volatile int leftSpeed = 0;
volatile int rightSpeed = 0;

// ---------------------------------------------------------------------------------------------------------------------------------







// ============================================== Interrupt Handlers ===============================================================
volatile bool buttonPress = 0;
void buttonISR()
{
  buttonPress = 1;
}

IntervalTimer sysTimer;
void tickISR()
{  
  digitalWrite(FLED1, HIGH);
  delayMicroseconds(60);
  frontREC1 = analogRead(FREC1) - 83;
  delayMicroseconds(60);
  digitalWrite(FLED1, LOW);
  
  digitalWrite(FLED2, HIGH);
  delayMicroseconds(60);
  frontREC2 = analogRead(FREC2) - 70;
  delayMicroseconds(75);
  digitalWrite(FLED2, LOW);
  
  digitalWrite(RLED, HIGH);
  delayMicroseconds(60);
  rightREC = analogRead(RREC) - 4;
  delayMicroseconds(75);
  digitalWrite(RLED, LOW);
   
  digitalWrite(LLED, HIGH);
  delayMicroseconds(60);
  leftREC = analogRead(LREC) - 95;
  delayMicroseconds(75);
  digitalWrite(LLED, LOW);
  
  if ((rightREC > rightWallThresh) && (leftREC > leftWallThresh) &&  ((frontREC1 < frontWallDetect) || (frontREC2 < frontWallDetect))) { // Both Side Walls
    pidMode = 0;
    irError = rightREC - leftREC;
    deltaIRError = irError - prevIRError;
    irCorrection = irKp * irError + irKd * deltaIRError; 
    prevIRError = irError;
    leftSpeed = ((DEFAULT_SPEED - irCorrection > SPEED_LIMIT) ? SPEED_LIMIT : DEFAULT_SPEED - irCorrection);
    rightSpeed = ((DEFAULT_SPEED + irCorrection > SPEED_LIMIT) ? SPEED_LIMIT : DEFAULT_SPEED + irCorrection);
  }
  else if ((rightREC < rightWallThresh) && (leftREC > leftWallThresh) &&  ((frontREC1 < frontWallDetect) || (frontREC2 < frontWallDetect))) { // Left Wall Only
    pidMode = 1;
    leftError = leftWallDetect - leftREC;
    deltaLeftError = leftError - prevLeftError;
    leftCorrection = leftKpIR * leftError + leftKdIR * deltaLeftError;
    prevLeftError = leftError;
  }
  else if ((rightREC > rightWallThresh) && (leftREC < leftWallThresh) &&  ((frontREC1 < frontWallDetect) || (frontREC2 < frontWallDetect))) { // Right Wall Only
    pidMode = 2;
    rightError = rightWallDetect - rightREC;
    deltaRightError = rightError - prevRightError;
    rightCorrection = rightKpIR * rightError + rightKdIR * deltaRightError;
    prevRightError = rightError;
  }
  else if ((rightREC < rightWallThresh) && (leftREC < leftWallThresh) &&  ((frontREC1 < frontWallDetect) || (frontREC2 < frontWallDetect))) { // No Side Walls Only
    pidMode = 3;
    encError = left.read() - right.read();
    deltaEncError = encError - prevEncError;
    correction = kp * encError + kd * deltaEncError;
    prevEncError = encError;
  }
  else if (((frontREC1 > frontWallDetect) || (frontREC2 > frontWallDetect)))
    pidMode = 4;
}
// --------------------------------------------------------------------------------------------------------------------------------------------------













// ====================================================== MAIN ========================================================
char userInput;

void setup() {
  pinMode(BUTTON, INPUT);
  sysTimer.begin(tickISR, 1000);
  sysTimer.priority(10);
  mydisplay.begin();
  mydisplay.setBrightness(15);
  mydisplay.home();
  attachInterrupt(BUTTON, buttonISR, RISING);
  Serial.begin(9600);
  pinMode(FLED1, OUTPUT);
  pinMode(FLED2, OUTPUT);
  pinMode(LLED, OUTPUT);
  pinMode(RLED, OUTPUT);
  pinMode(FREC1, INPUT);
  pinMode(FREC2, INPUT);
  pinMode(LREC, INPUT);
  pinMode(RREC, INPUT);
  bt.begin(9600);
  //forward(3);
//  delay(300);
  //uTurn();
//  delay(300);
// // forward(14);
  initializeMaze();
  //advance();
  //forward(4);
  //left90();
  //left90();
  //left90();
  //right90();
  //forward(6);
  advance();
}

void loop() {
  if (!buttonPress) {
//      left90();
//      delay(150);
//      right90();
//      delay(150);
//      uTurn();
//      delay(150);
       
  }
  else {
    halt();
     bt.print(frontREC1);
     bt.print("      ");
     bt.print(leftREC);
     bt.print("      ");
     bt.print(rightREC);
     bt.print("      ");
     bt.print(frontREC2);
     bt.print("                  ");
     bt.print(left.read());
     bt.print("      ");
     bt.print(right.read());
     bt.print("      ");
     bt.println(pidMode);   
  }
  //delay(1000);
}

// -----------------------------------------------------------------------------------------------------------------------------
