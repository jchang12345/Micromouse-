
// ================================================== Movement Functions ==================================================================================
void alignWithFrontWall();
void halt()
{
  analogWrite(leftForward, 0);
  analogWrite(rightForward, 0);
  analogWrite(leftBackward, 0);
  analogWrite(rightBackward, 0);
}

void forward(int cells)
{
  left.write(0);
  right.write(0);
  int targetCount = cells * CELL_COUNT;

  while(left.read() < targetCount) {
    switch (pidMode) {
      case 0: // 0
        analogWrite(leftForward, leftSpeed);// DEFAULT_SPEED - irCorrection);
        analogWrite(rightForward, rightSpeed);//DEFAULT_SPEED + irCorrection);
      break;
      case 1: 
          analogWrite(leftForward, DEFAULT_SPEED - leftCorrection);
          analogWrite(rightForward, DEFAULT_SPEED + leftCorrection);
        break;
      case 2:
        analogWrite(leftForward, DEFAULT_SPEED + rightCorrection);
        analogWrite(rightForward, DEFAULT_SPEED - rightCorrection);
      case 3: // 3
        analogWrite(leftForward, DEFAULT_SPEED - correction);
        analogWrite(rightForward, DEFAULT_SPEED + correction);
      break;
      default:
        halt();
        alignWithFrontWall();
        return;
      break;    
    }
  }
//    int time = millis();
//    int lDistError = targetCount - left.read();
//    int rDistError = targetCount - right.read();
//    int prevLDistError = 0;
//    int prevRDistError = 0;
//    int deltaLDistError = lDistError - prevLDistError;
//    int deltaRDistError = rDistError - prevRDistError;
//    int leftInput, rightInput;
//    while (millis() < time + 1200) {
//      if(lDistError > 0) {
//        analogWrite(leftForward, distKp * lDistError);
//        analogWrite(leftBackward, 0);
//      }
//      else {
//        analogWrite(leftBackward, distKp * lDistError);
//        analogWrite(leftForward, 0);
//      }
//      if(rDistError > 0) {
//        analogWrite(rightForward, distKp * rDistError);
//        analogWrite(rightBackward, 0);
//      }
//      else {
//        analogWrite(rightBackward, distKp * rDistError);
//        analogWrite(rightForward, 0);    
//      }
//     }
//  }
  halt();
}

void left90()
{
  left.write(0);
  right.write(0);
  int time = millis();
  int lWheelError = 0;
  int rWheelError = 0;
  
  int deltaLWheel = 0;
  int deltaRWheel = 0;
  int prevLError = 0;
  int prevRError = 0;
  int leftCorrection;
  int rightCorrection;
  int rightInput;
  int leftInput;

  while (millis() < time + 1300)
  {
    lWheelError = TURN_COUNT - (-left.read());
    rWheelError = TURN_COUNT - right.read();
    deltaLWheel = lWheelError - prevLError;
    deltaRWheel = rWheelError - prevRError;
    prevLError = lWheelError;
    prevRError = rWheelError;
   
    if (millis() < time + 400)
      turnKp = 0.8;
    else 
      turnKp = 0.12;
    
    if(TURN_COUNT >= right.read()) {
      rightCorrection = TURN_SPEED + turnKp * rWheelError + turnKd * deltaRWheel;
      rightInput = rightCorrection > TURN_SPEED_LIMIT ? TURN_SPEED_LIMIT : rightCorrection;
      analogWrite(rightForward, rightInput);
      analogWrite(rightBackward, 0);
    }
    else if (right.read() > TURN_COUNT) {
      rightCorrection = TURN_SPEED + turnKp * -rWheelError + turnKd * -deltaRWheel;
      rightInput = rightCorrection > TURN_SPEED_LIMIT ? TURN_SPEED_LIMIT : rightCorrection;
      analogWrite(rightBackward, rightInput);
      analogWrite(rightForward, 0);
    }
    if (TURN_COUNT >= -left.read()) {
      leftCorrection = TURN_SPEED + 5 + turnKp * lWheelError + turnKd * deltaLWheel;
      leftInput = leftCorrection > TURN_SPEED_LIMIT ? TURN_SPEED_LIMIT : leftCorrection;
      analogWrite(leftBackward, leftInput);
      analogWrite(leftForward, 0);
    }
    else {
      leftCorrection = TURN_SPEED + 5 + turnKp * -lWheelError + turnKd * - deltaLWheel;
      leftInput = leftCorrection > TURN_SPEED_LIMIT ? TURN_SPEED_LIMIT : leftCorrection;
      analogWrite(leftForward, leftInput);
      analogWrite(leftBackward, 0);  
    }
  }
  
  halt();
  delay(400);
}

void right90()
{
  left.write(0);
  right.write(0);
  int time = millis();
  int lWheelError = 0;
  int rWheelError = 0;
  
  int deltaLWheel = 0;
  int deltaRWheel = 0;
  int prevLError = 0;
  int prevRError = 0;
  int leftCorrection;
  int rightCorrection;
  int rightInput;
  int leftInput;

  while (millis() < time + 1300)
  {
    rWheelError = TURN_COUNT - (-right.read());
    lWheelError = TURN_COUNT - left.read();
    deltaRWheel = rWheelError - prevRError;
    deltaLWheel = lWheelError - prevLError;
    prevRError = rWheelError;
    prevLError = lWheelError;
   
    if (millis() < time + 400)
      turnKp = 0.8;
    else 
      turnKp = 0.12;
    
    if(TURN_COUNT >= left.read()) {
      leftCorrection = TURN_SPEED + 5 + turnKp * lWheelError + turnKd * deltaLWheel;
      leftInput = leftCorrection > TURN_SPEED_LIMIT ? TURN_SPEED_LIMIT : leftCorrection;
      analogWrite(leftForward, leftInput);
      analogWrite(leftBackward, 0);
    }
    else if (left.read() > TURN_COUNT) {
      leftCorrection = TURN_SPEED + 5 + turnKp * -lWheelError + turnKd * -deltaLWheel;
      leftInput = leftCorrection > TURN_SPEED_LIMIT ? TURN_SPEED_LIMIT : leftCorrection;
      analogWrite(leftBackward, leftInput);
      analogWrite(leftForward, 0);
    }
    if (TURN_COUNT >= -right.read()) {
      rightCorrection = TURN_SPEED + turnKp * rWheelError + turnKd * deltaRWheel;
      rightInput = rightCorrection > TURN_SPEED_LIMIT ? TURN_SPEED_LIMIT : rightCorrection;
      analogWrite(rightBackward, rightInput);
      analogWrite(rightForward, 0);
    }
    else {
      rightCorrection = TURN_SPEED + turnKp * -rWheelError + turnKd * - deltaRWheel;
      rightInput = rightCorrection > TURN_SPEED_LIMIT ? TURN_SPEED_LIMIT : rightCorrection;
      analogWrite(rightForward, rightInput);
      analogWrite(rightBackward, 0);  
    }
  }
  
  halt();
  delay(400);
}

void uTurn(){
  left90();
  left90();
  delay(400);
}

void alignWithFrontWall()
{
  int time = millis();
  while(millis() < time + 1250) {
    if(frontREC1 <= frontWallDetect){
     analogWrite(leftForward, 0.3 *(frontWallDetect - frontREC1));
     analogWrite(leftBackward, 0);
    }
    if(frontREC1 > frontWallDetect){
     analogWrite(leftBackward, 0.8 *(frontREC1 - frontWallDetect));
     analogWrite(leftForward, 0);
    }
    if(frontREC2 <= frontWallDetect) {
     analogWrite(rightForward, 0.3 * (frontWallDetect - frontREC2));
     analogWrite(rightBackward, 0);
    }
    if(frontREC2 > frontWallDetect){
     analogWrite(rightBackward, 0.8 * (frontREC2 - frontWallDetect));
     analogWrite(rightForward, 0);
    }
  }
  halt();
}
// -----------------------------------------------------------------------------------------------------------------------------


