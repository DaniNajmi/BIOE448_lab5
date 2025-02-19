// Lab 5 Microcontrollers: Bluetooth Communication
// February 19th, 2025
// Daniela Najmias Lang, Matan Blitz, Marc De Guzman

const int trigPin = 8;
const int echoPin = 10;
long duration;
int distanceCm, distanceInch;
long previousMillis = 0;

const int red = 2; // red LED
const int green = 3; // green LED
const int range = 5; // range for changing from green to red based on distance in cm

// Including the libraries
#include <ArduinoBLE.h>
BLEService newService("180A"); // creating the service
BLEByteCharacteristic readChar("2A57", BLERead);
BLEByteCharacteristic writeChar("2A58", BLEWrite);


void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  Serial.begin(9600);

  // Naming the device
  BLE.setLocalName("DanielaMatanMarc");
  BLE.setAdvertisedService(newService);
  newService.addCharacteristic(readChar);
  newService.addCharacteristic(writeChar);
  BLE.addService(newService);

  // Adding loop for Bluetooth
  while(!Serial);
  if (!BLE.begin()){
  Serial.println("Waiting for ArduinoBLE");
  while(1);
  }

  readChar.writeValue(0);
  writeChar.writeValue(0);

  BLE.advertise();
  Serial.println("Bluetooth device active");

}

void loop() {

  BLEDevice central = BLE.central(); // wait for a BLE central

  if (central) {  // if a central is connected to the peripheral
    Serial.print("Connected to central: ");
    
    Serial.println(central.address()); // print the central's BT address
    
    digitalWrite(LED_BUILTIN, HIGH); // turn on the LED to indicate the connection

    while (central.connected()) { // while the central is connected:
      long currentMillis = millis();
      
      if (currentMillis - previousMillis >= 200) { 
        previousMillis = currentMillis;

        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        duration = pulseIn(echoPin, HIGH);
        distanceCm = duration * 0.034 / 2;

        Serial.print("Distance: ");
        Serial.print(distanceCm);
        Serial.println(" cm");

        delay(10);
        readChar.writeValue(distanceCm);
        Serial.println("Distance printed to peripheral");
      }
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}