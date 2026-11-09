#include <Arduino.h>
#include <WorkshopAirWatch.h>
// Simulated ppm from a potentiometer; NOT a gas sensor or safety alarm.
// LED output is only an educational indicator.
#if defined(ARDUINO_ARCH_ESP32)
const int PPM_POT=34, WARNING_LED=18;
const double ADC_MAX=4095.0;
#else
const int PPM_POT=A0, WARNING_LED=8;
const double ADC_MAX=1023.0;
#endif
airwatch::Monitor monitor;
void setup() {
  Serial.begin(115200);
  pinMode(WARNING_LED,OUTPUT);
  digitalWrite(WARNING_LED,LOW);
}
void loop() {
  double synthetic_ppm=analogRead(PPM_POT)*3000.0/ADC_MAX;
  airwatch::Report r=monitor.ingest(millis()/1000.0,synthetic_ppm);
  digitalWrite(WARNING_LED,r.ventilation_recommended?HIGH:LOW);
  Serial.print("synthetic_ppm="); Serial.print(synthetic_ppm);
  Serial.print(" valid="); Serial.print(r.valid);
  Serial.print(" level_code="); Serial.println(static_cast<int>(r.level));
  delay(1000);
}
