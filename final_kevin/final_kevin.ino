#include <avr/io.h>
// #define DEBUG 1
#ifndef DEBUG
#define DEBUG 0
#endif

// #define TIMER5_FOR_PERFORMANCE


#include "C:\Users\Kevin\Dropbox\Porcessing folder\finalfinal_Fight\config_h\config_h.ino"
#include "C:\Users\Kevin\Dropbox\Porcessing folder\finalfinal_Fight\robot_h\robot_h.ino"
//#include "D:\Dropbox\Porcessing folder\finalfinal\wheel_v6\wheel_v6.ino"


/*
-----------------------------------MAIN CODE------------------------------------
*/

Robot robot;

#ifdef TIMER5_FOR_PERFORMANCE
ISR(TIMER5_COMPA_vect)
{
  if (milliseconds > 1000)
  {
   // if (DEBUG) Serial.print("One Second\n");
    
    milliseconds = 0;
  }
  milliseconds++;
}
#else
ISR(TIMER5_COMPA_vect)
{
  milliseconds++;
  temp_long = analogRead(PIN_FAR_IR);
  temp_short = analogRead(PIN_CLOSE_IR);

  if (temp_long > value_long)
  {
    value_long = temp_long;
    time = milliseconds;
  } 
  
  if (temp_short > value_short)
  {
    value_short = temp_short;
    time = milliseconds;
  } 

}
#endif

void setup()

{
  Serial.begin(38400); 
  
  cli();
  setup_timer5();
  setup_timer2_for_pwm();
  sei();
  setup_timer5_disable();
  
  pinMode(31, OUTPUT);
  pinMode(32, OUTPUT);
  pinMode(50, OUTPUT);  //temp
  
  digitalWrite(31, HIGH);
  digitalWrite(32, HIGH);  

  setup_timer4();
  setup_timer4_disable();

  //sweeping();
     //Light sensor initialization- initial reading
/*
	  int_light_sensor_FL = analogRead(9);
	  int_light_sensor_FR = analogRead(8);
	  int_light_sensor_BL = analogRead(11);
	  int_light_sensor_BR = analogRead(10);
*/
}


#define SPEED_SLOW 180


void sweeping ()
{
  sector = robot.sweep_search();
  dist_long = access_long_range_IR_sensor(value_long);
  dist_short = access_close_range_IR_sensor(value_short);
  /*
  Serial.println(sector);
  Serial.print("Long: ");
  Serial.println(dist_long);
  Serial.print("Short: ");
  Serial.println(dist_short);
  Serial.print("Time: ");
  Serial.println(time);
  */
}

void seeing ()
{
  robot.access_turret(42);
 Serial.println(access_close_range_IR_sensor(analogRead(PIN_CLOSE_IR)));
 Serial.println(access_long_range_IR_sensor(analogRead(PIN_FAR_IR)));
 Serial.println(access_long_range_IR_sensor(analogRead(PIN_FOOT_IR)));
}

void found_adversary()
{
  switch (sector) 
  {
  case 1:
    robot.access_motor(MOTOR_TURN, 90, SPEED_SLOW);
    robot.access_motor(MOTOR_FORWARDS, dist_long * 5, SPEED_SLOW);
    break;
  case 2:
    robot.access_motor(MOTOR_TURN, 45, SPEED_SLOW);
    robot.access_motor(MOTOR_FORWARDS, dist_long * 5, SPEED_SLOW);
    break;
  case 3:
    if (dist_long)
    {
      if (dist_long < 70)
        robot.access_weapon(1);  
      robot.access_motor(MOTOR_FORWARDS, dist_long, SPEED_SLOW);
    }
    break;
  case 4:
    robot.access_motor(MOTOR_TURN, 315, SPEED_SLOW);
    robot.access_motor(MOTOR_FORWARDS, dist_long * 5, SPEED_SLOW);
    break;
  case 5:
    robot.access_motor(MOTOR_TURN, 270, SPEED_SLOW);
    robot.access_motor(MOTOR_FORWARDS, dist_long * 5, SPEED_SLOW);
    break;
  default:
  //  Serial.println("default");
    break;
  }
}

void blind_search ()
{
  robot.access_weapon(0); 
  robot.access_motor(MOTOR_FORWARDS, 100, SPEED_SLOW);
  robot.access_motor(MOTOR_TURN, 180, SPEED_SLOW);

/*
  if (slap_check())
  {
    robot.access_motor(MOTOR_BACKWARDS, 60, SPEED_SLOW);
    return;
  }
  */
  sweeping();
  if (dist_long == 0)
  {
    robot.access_motor(MOTOR_TURN, 180, SPEED_SLOW);
    return;
  }
  else
    found_adversary();
}

int slap_check()
{
  int close_on_the_left = access_long_range_IR_sensor(
      analogRead(PIN_FOOT_IR)) < 40;
  int close_on_the_right = access_close_range_IR_sensor(
      analogRead(PIN_CLOSE_IR));

  if ( close_on_the_left && !close_on_the_right)
  {
    for (int i = 0; i < 5; i++)
    {
      // move turret to the left
      OCR2B = 71;
      // quickly move turret back to center
      OCR2B = 42;
      // short delay
      delay(100);
    }
    return 1;
  }

  if (close_on_the_right)
  {
    for (int i = 0; i < 5; i++)
    {
      // move turret to the right
      OCR2B = 0;
      // quickly move turret back to center
      OCR2B = 42;
      // short delay
      delay(100);
    }
    return 1;
  }
  return 0;
}

void artificial_intelligence()
{
  /*
  if (slap_check())
  {
    robot.access_motor(MOTOR_BACKWARDS, 60, SPEED_SLOW);
  }
  */
  sweeping();

  if (dist_long == 0)
  {
    blind_search();
    TrAng = TrAng_temp;
  }
  
  else
    found_adversary();
}

void serial_control()
{
  if (Serial.available())
  {
    switch ((char)Serial.read())
    {
    case 'a':
      sweeping();
      break;
    case 'b':
      seeing();
      break;
    case 'c':
      robot.access_motor(MOTOR_FORWARDS, 200, 120);      
      break;
    case 'd':
      robot.access_motor(MOTOR_FORWARDS, 200, 160);
      break;
    case 'e':
      robot.access_motor(MOTOR_BACKWARDS, 200, 120);
      break;
    case 'f':
      robot.access_motor(MOTOR_TURN, 90, 120);
      break;
    case 'g':
      robot.access_weapon(1);
      delay(2000);
      robot.access_weapon(0);
      break;
    case 'h':
      robot.access_light_sensor();
      Serial.println();
      break;
    case 'i':
      blind_search();
      break;
    case 'j':
      slap_check();
      break;
    case 'k':
      for (int i = 0; i < 5; i++)
      {
        // move turret to the left
        OCR2B = 255 - 0;
        delay(70);
        // quickly move turret back to center
        OCR2B = 255 - 42;
        // short delay
        delay(150);
      }
      break;
    case 'l':
      OCR2B = 0;
      break;
    case 'z':
      artificial_intelligence();
      break;
    default:
      Serial.println("Invalid Input");
      break;
    }
  }
}
int T_degree_temp = 0;
void loop()
{
  /*
 Serial.write(" \n TrAng: ");
  Serial.print(TrAng);
  Serial.write("  common_angle: ");
  Serial.print(common_angle);      
 */
  AttCount = 0;
   artificial_intelligence();
   convert();
    //serial_control();
   sendMessage();
}


