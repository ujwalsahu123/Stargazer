# now , do sf , then motor code (motor moves to a particular alt az range)
# write arduino sf and motor code . 
# and now , we select a object in sterellium and then it send the alt az of that object . 
# arduino catches the alt az and moves to motor to point the rpy values to that alt az . 

# one thing you can do here before sending the alt az is -> increase the values by +or- 0.02 or etc 
# every second the 1st 2nd decimal value changes . so i send a value now , then motor run may it takes 3,4 second then
# unacurracy .  ek bar think on this ..









# Arduino checkup code
# // Altitude and Azimuth values
# float altitude = 0;
# float azimuth = 0;

# void setup() {
#   // Start serial communication
#   Serial.begin(9600);
  
#   // Set the onboard LED as output (usually on pin 13)
#   pinMode(LED_BUILTIN, OUTPUT);
# }

# void loop() {
#   // Check if data is available on Serial
#   if (Serial.available() > 0) {
#     // Read incoming data as a string
#     String data = Serial.readStringUntil('\n');
    
#     // Parse the received data
#     int commaIndex = data.indexOf(',');
#     if (commaIndex > 0) {
#       altitude = data.substring(0, commaIndex).toFloat();
#       azimuth = data.substring(commaIndex + 1).toFloat();
      
#       // Print the received values
#       Serial.print("Altitude: ");
#       Serial.print(altitude);
#       Serial.print(" | Azimuth: ");
#       Serial.println(azimuth);
      
#       // Blink the onboard LED 2 times
#       for (int i = 0; i < 2; i++) {
#         digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on
#         delay(500);                       // Wait for 500 ms
#         digitalWrite(LED_BUILTIN, LOW);    // Turn the LED off
#         delay(500);                       // Wait for 500 ms
#       }
#     }
#   }
# }

