#define BATTERY_ADC_PIN 34

float readBatteryVoltage() {

  const int samples = 10;
  uint32_t total = 0;

  for (int i = 0; i < samples; i++) {
    total += analogReadMilliVolts(BATTERY_ADC_PIN);
    delay(5);
  }

  float adcVoltage = (float)total / samples;

  // 100kΩ / 100kΩ voltage divider
  float batteryVoltage = (adcVoltage * 2.0) / 1000.0;
  // After applying correction factor 
  batteryVoltage =  batteryVoltage * 0.9845;
  return batteryVoltage;
}

void setup() {

  Serial.begin(115200);

  analogSetPinAttenuation(BATTERY_ADC_PIN, ADC_11db);

  Serial.println("ESP32 Battery Monitor");
}

void loop() {

  float batteryVoltage = readBatteryVoltage();
  int percentage = batteryPercentage(batteryVoltage);

  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage, 2);
  Serial.println(" V");
  Serial.print(percentage);
  Serial.println("%");
  delay(1000);
}
int batteryPercentage(float voltage) {

  if (voltage >= 4.20)
    return 100;

  if (voltage <= 3.20)
    return 0;

  return (int)((voltage - 3.20) * 100.0 / (4.20 - 3.20));
}