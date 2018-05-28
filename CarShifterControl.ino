const int controlPin = 8;  // Digital pin 8 will be called 'pin8'
const int forceStopByP = A1;  // Analog input pin will be called 'A1'
const int forceStopByUnlock = A2;  // Analog input pin will be called 'A2'
const int forceStopByN = A3;  // Analog input pin will be called 'A3'
const int sensorPin = A0;  // Analog input pin that senses Vout
int sensorValue = 0;       // sensorPin default value
float Vin = 5;             // Input voltage
float Vout = 0;            // Vout default value
float Rref = 1000000000;          // Reference resistor's value in ohms (you can give this value in kiloohms or megaohms - the resistance of the tested resistor will be given in the same units)
float R = 0;               // Tested resistors default value
float minValue = 0;    // Value of resistor when press shifters +
float maxValue = 0;// Value of resistor when press shifters -
unsigned long started = 0;
unsigned long timeout = 15000; // Manual shifters will be off after 15 seconds
boolean inited = false;
void setup ()
{
  Serial.begin(9600);      // Initialize serial communications at 9600 bps
  pinMode(controlPin, OUTPUT);
  pinMode(forceStopByP, INPUT_PULLUP);
  pinMode(forceStopByUnlock, INPUT_PULLUP);
  pinMode(forceStopByN, INPUT_PULLUP);
  digitalWrite(controlPin, HIGH);
}

float getResistor() {
  sensorValue = analogRead(sensorPin);  // Read Vout on analog input pin A0 (Arduino can sense from 0-1023, 1023 is 5V)
  Serial.println("analogRead >> ");
  Serial.println(sensorValue);
  Vout = (Vin * sensorValue) / 1023;    // Convert Vout to volts
  if (Vout != Vin) {
    float r = Rref * (1 / ((Vin / Vout) - 1));  // Formula to calculate tested resistor's value
    Serial.print("r: ");
    Serial.println(r);                    // Give calculated resistance in Serial Monitor
    return r;
  }
  return 0;
}

void getStarting() {
  delay(1000); //delay one seconds before get max and min value
  Serial.println("delay one seconds before get max and min value");
  for (int i = 0; i < 100; i++) {
    float r = getResistor();
    if (maxValue == 0 || maxValue < r) {
      maxValue = r;
    }
    if (minValue == 0 || minValue > r) {
      minValue = r;
    }
    delay(50);
  }
  Serial.println("maxValue >> ");
  Serial.println(maxValue);
  Serial.println("minValue >> ");
  Serial.println(minValue);
  delay(500); // Delay in 500(1/2s) milliseconds before processing
  inited = true;
}

void loop ()
{
  if (inited == false) {
    getStarting();
  }
  R = getResistor();
  Serial.print("R in loop ");
  Serial.println(R);
  
  Serial.print("Control pin state is: ");
  Serial.println(digitalRead(controlPin));
  //check if pess shifters +/-
  if (R != 0 && (R <= minValue || R >= maxValue)) {
    Serial.println("Start control");
    started = millis();
    digitalWrite(controlPin, LOW);
  }
  //check timeout
  if (started > 0) {
    float forceStop1 = analogRead(forceStopByP);
    Serial.print("Stop 1 >> ");
    Serial.println(forceStop1);
    float forceStop2 = analogRead(forceStopByUnlock);
    Serial.print("Stop 2 >> ");
    Serial.println(forceStop2);
    float forceStop3 = analogRead(forceStopByN);
    Serial.print("Stop 3 >> ");
    Serial.println(forceStop3);

    long now = millis();
    long elapsed = now - started;
    if (elapsed > timeout) {
      Serial.println("Stop control");
      digitalWrite(controlPin, HIGH);
      started = 0;
    }
  }
  delay(100); // Delay in milliseconds between reeds
}
