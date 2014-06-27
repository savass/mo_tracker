#include <iostream>
#include <string>
#include <unistd.h>
#include "libraries/BlackLib/BlackLib.cpp"
#include "libraries/eQEP/eqep.cpp"


using namespace std;

/*FOR eQEO definations  see readme file
 * At this point we need to be sure that these encoders are loaded in the device three
 */

//See & change libraries/eQEP/eqep.h for eQEP definations..
#define TILT_ENC_PATH eQEP1							//eQEP1 (A=P8_35 B=P8_33)
#define TILT_DIR_PIN 	GPIO_48							//GPIO1_16 = (1x32) + 16 = 48 (P9_15)
#define TILT_PWMOUT_PIN	P9_14							//See BlackLib documentation

#define PAN_ENC_PATH 	eQEP2							//eQEP2 (A=P8_41  B=P8_42)
#define PAN_DIR_PIN 	GPIO_60							//GPIO1_28 = (1x32) + 28 = 60 (P9_12)
#define PAN_PWMOUT_PIN	P9_16							//See Blacklib documentation

#define PWM_OUT_DCYCLE	5000000							//in nanoseconds

#define PAN 1
#define TILT 0

#define CW high									//clockwise
#define CCW low									//Conter clockwise

#define PAN_ENC_COUNTS	942803							//number of encoder counts per turn (This value is valid for orion teletrack tracker)
#define TILT_ENC_COUNTS	942803							//number of encoder counts per turn

#define GOTO_TOLERANCE	1							//

//initialization of control pins
eQEP TiltEncoder(TILT_ENC_PATH, eQEP::eQEP_Mode_Absolute);
BlackGPIO	TILT_DIR_GPIO(TILT_DIR_PIN,output);
BlackPWM 	TILT_PWM_GPIO(TILT_PWMOUT_PIN); // initialization pwm with second led

eQEP PanEncoder(PAN_ENC_PATH, eQEP::eQEP_Mode_Absolute);
BlackGPIO	PAN_DIR_GPIO(PAN_DIR_PIN,output);
BlackPWM	PAN_PWM_GPIO(PAN_PWMOUT_PIN); // initialization pwm with second led
//Read axis position
int32_t AxisPosition;

int32_t read_angle_pos(int axis) {
  if ( axis == TILT ) return TiltEncoder.get_position(false);
  else if ( axis == PAN ) return PanEncoder.get_position(false);
  else return -1;
}

int set_axis_dir(int axis, gpio_value direction) {
  if ( axis == TILT ) {
    TILT_DIR_GPIO.setValue(direction);
    return 0;
  }
  else if ( axis == PAN ) {
    PAN_DIR_GPIO.setValue(direction);
    return 0;
  }
  return -1;
}

int set_motor_speed(int axis,float speed) {
  if ( axis == TILT ) {
    TILT_PWM_GPIO.setDutyPercent(speed);
    return 0;
  }
  else if ( axis == PAN ) {
    PAN_PWM_GPIO.setDutyPercent(speed);
    return 0;
  }
  return -1;
}

int go_to(int axis, float angle, float speed) {

  //read position of axis

  int32_t RequiredPosition;
  int32_t AxisMargin;

  AxisPosition= read_angle_pos(axis);

  if ( axis == 0 ) {
    //Convert angle to encoder count
    RequiredPosition= (TILT_ENC_COUNTS/360)*angle;
    //Axis margin
    AxisMargin = (TILT_ENC_COUNTS/360) * GOTO_TOLERANCE;
  }
  else if ( axis==1 ) {
    //Convert angle to encoder count
    RequiredPosition= (PAN_ENC_COUNTS/360)*angle;
    //Axis margin
    AxisMargin = (PAN_ENC_COUNTS/360) * GOTO_TOLERANCE;
  }
  else {return -1;}

  while ( !( (AxisPosition <= (RequiredPosition+AxisMargin) ) && (AxisPosition >= (RequiredPosition-AxisMargin) ) ) )
  {

    //Set motor direction
    if (AxisPosition>RequiredPosition) set_axis_dir(axis,CW);
    else set_axis_dir(axis,CCW);

    //Set motor speed
    set_motor_speed(axis,speed);

  AxisPosition= read_angle_pos(axis);
  //std::cout << "[eQEP " << axis << "] Position = " << AxisPosition << std::endl;
  }

//Stop motor
set_motor_speed(axis,0);
return 0;
}

int main(int argc, char *argv[]){



  //Initial setup of control pins
  TILT_PWM_GPIO.setPeriodTime(PWM_OUT_DCYCLE);
  TILT_PWM_GPIO.setDutyPercent(0);
  TILT_DIR_GPIO.setValue(low);

  PAN_PWM_GPIO.setPeriodTime(PWM_OUT_DCYCLE);
  PAN_PWM_GPIO.setDutyPercent(0);
  PAN_DIR_GPIO.setValue(low);

  int goto_axis;
  float goto_speed;
  float goto_angle;

   while(true)
    {
      cout << "Enter axis number: (Pan=1,Tilt=0 , -1 to exit)";
      cin >> goto_axis;
	if (goto_axis <0 ) {
	  return 0;
	  PAN_PWM_GPIO.setDutyPercent(0);
	  TILT_PWM_GPIO.setDutyPercent(0);
	}

      cout << "Enter goto angle:";
      cin >> goto_angle;

      cout << "Enter goto speed (1-100):";
      cin >> goto_speed;

      set_motor_speed(goto_axis,goto_speed);
      go_to(goto_axis,goto_angle,goto_speed);

    }


  return 0;
}

