#include <ThreeWire.h>  // RTC
#include <RtcDS1302.h>  // RTC
#include <Stepper.h>    // motor

ThreeWire myWire(4, 5, 2);  // RTC
RtcDS1302<ThreeWire> Rtc(myWire); // RTC
#define countof(a) (sizeof(a) / sizeof(a[0]))   // RTC

const int one_circle_counter_clockwise = 2048;                      // motor
const int one_circle_clockwise = -one_circle_counter_clockwise;     // motor
Stepper myStepper = Stepper(one_circle_counter_clockwise, 8, 10, 9, 11);  // motor

const int feed_time_hour = 12;
const int feed_time_minutes = 26;
const int check_time_interval = 1000;

void setup() {
  Serial.begin(9600);

  Rtc.Begin();  // RTC
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  if (!Rtc.IsDateTimeValid()) {
    // Common Causes:
    //    1) first time you ran and the device wasn't running yet
    //    2) the battery on the device is low or even missing

    Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }

  if (Rtc.GetIsWriteProtected()) {
    Serial.println("RTC was write protected, enabling writing now");
    Rtc.SetIsWriteProtected(false);
  }

  if (!Rtc.GetIsRunning()) {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  } else if (now > compiled) {
    Serial.println("RTC is newer than compile time. (this is expected)");
  } else if (now == compiled) {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }
  
  myStepper.setSpeed(5);  // motor
}

void loop() {
  RtcDateTime now = Rtc.GetDateTime();
  
  if (!now.IsValid()) {
    // Common Causes:
    //    1) the battery on the device is low or even missing and the power line was disconnected
    Serial.println("RTC lost confidence in the DateTime!");
  }

  if ((now.Hour() == feed_time_hour) && (now.Minute() == feed_time_minutes)) {
    Serial.println("time is up\n");
    myStepper.step(one_circle_clockwise);
  }
  delay(check_time_interval); 
}



void printUnformattedTime(const RtcDateTime& dt) {
  char datestring[5];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u%02u"),
             dt.Hour(),
             dt.Minute());
  Serial.print(datestring);
}
/*
void printDateTime(const RtcDateTime& dt) {
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Month(),
             dt.Day(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second());
  Serial.print(datestring);
}*/
