#line 1 "E:\\GitHub\\SmartWebServer\\legacy\\SmartWebServer\\src\\lib\\axis\\motor\\servo\\feedback\\FeedbackBase.cpp"
// -----------------------------------------------------------------------------------
// servo motor feedback

#include "FeedbackBase.h"

#ifdef SERVO_MOTOR_PRESENT

// initialize feedback control and parameters
void Feedback::init(uint8_t axisNumber, ServoControl *control) {
  this->axisNumber = axisNumber;

  this->control = control;
  control->in = 0;
  control->set = 0;

  ready = true;
}

#endif