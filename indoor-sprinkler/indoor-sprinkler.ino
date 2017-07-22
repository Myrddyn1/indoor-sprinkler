// Pins definition
#define PIN_WATERPUMP 7
#define PIN_SENSOR_MOISTURE 5
#define PIN_LED_SENSOR_MOISTURE 4
#define PIN_SENSOR_WATERLEVEL 6
#define PIN_LED_WATERLEVELALERT 3
#define PIN_BUTTON_SENSOR_MOISTURE_TUNING 2

#define CHECKING_PERIOD_IN_HOURS 6 // Elapsed time in between 2 moisture checks (in min)
#define WATERING_PERIOD_IN_SEC 5 // Duration of each watering (in sec)

void setup() {
  // Settings the PIN modes
  pinMode(PIN_WATERPUMP, OUTPUT);
  pinMode(PIN_SENSOR_MOISTURE, INPUT);
  pinMode(PIN_LED_SENSOR_MOISTURE, OUTPUT);
  pinMode(PIN_SENSOR_WATERLEVEL, INPUT_PULLUP);
  pinMode(PIN_LED_WATERLEVELALERT, OUTPUT);
  pinMode(PIN_BUTTON_SENSOR_MOISTURE_TUNING, INPUT_PULLUP);

  // Debug console
  Serial.begin(9600);
  Serial.println("System ON");
}

void loop() {
  // If "moisture sensor tuning" mode on, light up the LED when the threshold is reached
  if (IsTuningMode())
  {
    // Reset LED
    Serial.println("Tuning mode ON");
    if (IsEnoughMoisture())
    {
      Serial.println("Water not needed");
      digitalWrite(PIN_LED_SENSOR_MOISTURE, LOW);
    }
    else
    {
      Serial.println("Level where we should water");
      digitalWrite(PIN_LED_SENSOR_MOISTURE, HIGH);
    }
  }
  else
  {
    // If water level is enough, continue or else, disable the system
    if (IsEnoughWater())
    {
      // Check moisture level
      // If level low, then water
      if (!IsEnoughMoisture())
      {
        Serial.println("Not enough moisture");
        ActivateWaterPump(WATERING_PERIOD_IN_SEC);
      }
      // Wait for the next loop
      Serial.println("Delay checking");
      delay(CHECKING_PERIOD_IN_HOURS * 3600 * 1000);
    }
  }
}

bool IsTuningMode()
{
  return !digitalRead(PIN_BUTTON_SENSOR_MOISTURE_TUNING);
}

bool IsEnoughWater()
{
  if (!digitalRead(PIN_SENSOR_WATERLEVEL))
  {
    Serial.println("Enough water in tank");
    digitalWrite(PIN_LED_WATERLEVELALERT, LOW);
    return true;
  }
  else
  {
    Serial.println("Not enough water in tank");
    // Light on the LED for water level alert
    digitalWrite(PIN_LED_WATERLEVELALERT, HIGH);
    return false;
  }
}

bool IsEnoughMoisture()
{
  return (!digitalRead(PIN_SENSOR_MOISTURE));
}

void ActivateWaterPump(int duration_in_sec)
{
  // Start watering during the requested period in sec, then stop
  Serial.println("Start watering");
  digitalWrite(PIN_LED_SENSOR_MOISTURE, HIGH);
  digitalWrite(PIN_WATERPUMP, HIGH);
  delay(duration_in_sec * 1000);
  digitalWrite(PIN_WATERPUMP, LOW);
  digitalWrite(PIN_LED_SENSOR_MOISTURE, LOW);
  Serial.println("Stop watering");
}
