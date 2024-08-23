#include <TimerOne.h>
#include <Arduino.h>

#define pinOut 3
#define zeroCrossPin 2
volatile int cnt = 0;
volatile bool ZeroCrossingFlag = false;
int time_itr = 100;
int dim = 0;

void zeroCrossing()
{
	ZeroCrossingFlag = true;
	cnt = 0;
	digitalWrite(pinOut, LOW);
}

void dimmer()
{
  if (ZeroCrossingFlag == true)
  {
    if (cnt > dim)
    {
      digitalWrite(pinOut, HIGH);
      cnt = 0;
      ZeroCrossingFlag = false;
    }
    else
    {
      cnt++;
    } 
  }
}

void setup()
{
	Serial.begin(115200);
	pinMode(3, OUTPUT);
  pinMode(13, OUTPUT);
	attachInterrupt(digitalPinToInterrupt(2), zeroCrossing, FALLING);
  Timer1.initialize(time_itr);
  Timer1.attachInterrupt(dimmer, time_itr);
  //
  Serial.print("Set value: ");
}

bool led_state = true;
void loop()
{
  static uint32_t timer_blink = millis();
  if (Serial.available())
  {
    while (Serial.available() > 0)
    {
        int power = Serial.parseInt();
        power = constrain(power, 0, 100);
        dim = 100 - power;
        Serial.print(power);
        Serial.println('%'); 
    }
    //
    Serial.print("Set value: ");
  }
  //Blink led
  if (millis() - timer_blink > 1000)
  {
    digitalWrite(13, led_state);
    led_state = !led_state;
    timer_blink = millis();
  }
}