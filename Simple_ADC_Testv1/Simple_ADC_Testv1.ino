#define BATTERY_ADC_PIN 34

void setup() {
  Serial.begin(115200);

  // GPIO34 is ADC1
  analogSetPinAttenuation(BATTERY_ADC_PIN, ADC_11db);

  Serial.println("Battery ADC Test");
}

void loop() {

  // Read calibrated voltage at ADC pin
  uint32_t adcVoltage = analogReadMilliVolts(BATTERY_ADC_PIN);

  // 100k + 100k voltage divider
  float batteryVoltage = (adcVoltage * 2.0) / 1000.0;
  // After applying correction factor 
  batteryVoltage =  batteryVoltage * 0.9845;
  Serial.print("ADC Voltage: ");
  Serial.print(adcVoltage);
  Serial.print(" mV");

  Serial.print(" | Battery Voltage: ");
  Serial.print(batteryVoltage, 2);
  Serial.println(" V");

  delay(1000);
}