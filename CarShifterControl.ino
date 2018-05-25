const int controlPin = 8;  // Digital pin 8 will be called 'pin8'
const int sensorPin = A0;  // Analog input pin that senses Vout
int sensorValue = 0;       // sensorPin default value
float Vin = 5;             // Input voltage
float Vout = 0;            // Vout default value
float Rref = 999;          // Reference resistor's value in ohms (you can give this value in kiloohms or megaohms - the resistance of the tested resistor will be given in the same units)
float R = 0;               // Tested resistors default value
float plusValue = 9000;    // Value of resistor when press shifters +
float subtractValue = 5000;// Value of resistor when press shifters -
unsigned long start = 0;
unsigned long timeout = 15000; // Manual shifters will be off after 15 seconds

void setup ()
{
  Serial.begin(9600);      // Initialize serial communications at 9600 bps
}

void loop ()
{
  sensorValue = analogRead(sensorPin);  // Read Vout on analog input pin A0 (Arduino can sense from 0-1023, 1023 is 5V)
  Vout = (Vin * sensorValue) / 1023;    // Convert Vout to volts
  R = Rref * (1 / ((Vin / Vout) - 1));  // Formula to calculate tested resistor's value
  Serial.print("R: ");
  Serial.println(R);                    // Give calculated resistance in Serial Monitor
  delay(1000);                          // Delay in milliseconds between reeds
  //check if pess shifters +/-
  if (R >= plusValue || R <= subtractValue) {
    digitalWrite(controlPin, LOW);
  }
  //check timeout
  if (start > 0) {
    long now = millis();
    long elapsed = now - start;
    if (elapsed > timeout) {
      digitalWrite(controlPin, HIGH);
    }
  }
}

