#ifndef config_h
#define config_h

/*
 * Pin Defines
 */

// Analog
#define PIN_FAR_IR                  3
#define PIN_CLOSE_IR                4
#define PIN_FOOT_IR                 5
#define PIN_LIGHT_SENSOR_1          9
#define PIN_LIGHT_SENSOR_2          10
#define PIN_LIGHT_SENSOR_3          11
#define PIN_LIGHT_SENSOR_4          12

// Digital
#define PIN_ENCODER_OUT_LEFT_0      2
#define PIN_ENCODER_OUT_LEFT_1      3
#define BIT_R_1                     5
#define BIT_R_2                     4
#define PIN_TURRET                  9
#define BIT_L_1			   13
#define BIT_L_2                     12
#define PIN_WEAPON                  14

#define PIN_ENABLE_LEFT_MOTOR       15
#define PIN_ENABLE_RIGHT_MOTOR      16

#define PIN_ENCODER_OUT_RIGHT_1     20
#define PIN_ENCODER_OUT_RIGHT_0     21

float value_long, value_short, temp_long, temp_short;
int time, milliseconds;

//////////////////////////Wireless Communication//////////////
int orderReceived = 0;
//recevie message
//int Directions = 0; //0=forward,1=Backward, 2=left, 3=right
//int move = 0; //0=stop, 1=move
int turnTr = 0; // 0 = not move, 1 = left, 3 = right
int Attact = 0; //0=not attact, 1 = attact
int Manual = 0; //0 = manual, 1 = auto
//Send Message
int sector, dist_short, dist_long;
double x_axis = 0, y_axis = 0;   //x, y postion 2438 mm max Mycar
int TrAng_temp = 0;
int TrAng = 0;  //max 16 bit
int common_angle = 0; //max 16 bit

int Enemy_x = 1222;
int Enemy_y = 406;


int AttCount = 0;  //max 16 bit
int OtherInfo = 0; //max 8 bit, [Light Sensor 1|LS2|LS3|LS4|Too Close 0|0|0|0]

/* The below sets up timer5 to be used to time subroutines to determine their 
strain on the CPU */

void setup_timer5 ()
{
  TCCR5A = _BV(COM5A1) | _BV(COM5B1) | _BV(COM5C1);
  TCCR5B = _BV(CS51) | _BV(CS50) |_BV(WGM52);

  OCR5A = 125*2;

  TIMSK5 = _BV(OCIE5A);
  TIFR5 = _BV(OCF5A);
}

void setup_timer5_disable ()
{
  TIMSK5 = _BV(!OCIE5A);
}

void setup_timer5_enable ()
{
  TIMSK5 = _BV(OCIE5A);
}

// HENRY: Timer 4 is for Kevin's motor correction ISR
void setup_timer4 ()
{
  TCCR4A = _BV(COM4A1) | _BV(COM4B1) | _BV(COM4C1);
  // TCCR4B = _BV(CS41) | _BV(CS40) |_BV(WGM42);
  TCCR4B = _BV(CS42) | _BV(CS40) | _BV(WGM42);

  //OCR4A = 125*2;
  OCR4A = 125*10000;


  TIMSK4 = _BV(OCIE4A);
  TIFR4 = _BV(OCF4A);
}

void setup_timer4_disable ()
{
  TIMSK4 = _BV(!OCIE4A);
}

void setup_timer4_enable ()
{
  TIMSK4 = _BV(OCIE4A);
}


/* Timer 2 info:
 * 1. OCR2A corresponds to pin 10 (pwm)
 * 2. OCR2B corresponds to pin 9 (pwm)
 * 3. PWM Phase Correct Mode runs the timer up and down from 0 to 255 and
 * based on COM2x1:0 will either be high until OCR2x and then low or vice
 * versa
 */  
void setup_timer2_for_pwm ()
{
  TCCR2A = _BV(COM2A1) | _BV(WGM20);
  TCCR2B = _BV(CS22) | _BV(CS21);
  
  TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(COM2B0) | _BV(WGM20);
  TCCR2B = _BV(CS22) | _BV(CS21);
}

#endif
