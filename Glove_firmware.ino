#include <Servo.h>

unsigned long last_time = 0;

// Define servo motors pins
int servoPins[5] = {2, 3, 4, 5, 6};

int collisionStatus[5] = {0, 0, 0, 0, 0};

bool already180[5] = {true, true, true, true, true};

// Define potentiometer pins
int potPins[5] = {A0, A1, A2, A3, A4};

// Initialize servo motors
Servo servos[5];

// Initialize potentiometer values array
int potValues[5];

void setup() {
  // Initialize hardware serial port
  Serial.begin(9600);

  // Attach servo motors to pins
 for (int i = 0; i < 5; i++) {
  servos[i].attach(servoPins[i]);
  servos[i].write(180);
  }
}

void loop() {
  // Read potentiometer values
  for (int i = 0; i < 5; i++) {
    potValues[i] = analogRead(potPins[i]);
  }

  //callibrate all potentiometers' base value to 0
  potValues[1] = abs(potValues[1] - 1023);
  potValues[2] = abs(potValues[2] - 1023);
  potValues[3] = abs(potValues[3] - 1023);


  // Send potentiometer values to Unity
  Serial.print("P");
  Serial.print(potValues[0]);
  Serial.print(",");
  Serial.print(potValues[1]);
  Serial.print(",");
  Serial.print(potValues[2]);
  Serial.print(",");
  Serial.print(potValues[3]);
  Serial.print(",");
  Serial.println(potValues[4]);

  // Check for incoming data from Unity
  if (millis() > last_time + 500)
  {
  if (Serial.available() > 0) {
    //Read incoming data
    String data = Serial.readStringUntil('\n');
    if(data.indexOf('C') == 0)
    {
      data = data.substring(1);
    // Parse collision status array
    int index = 0;
    int start = 0;
    int end = data.indexOf(',');
    while (end != -1) {
     collisionStatus[index] = data.substring(start, end).toInt();
     index++;
     start = end + 1;
     end = data.indexOf(',', start);
    }
    collisionStatus[index] = data.substring(start).toInt();

    // Update servo motors based on collision status array
    for (int i = 0; i < 5; i++) {
      if (collisionStatus[i] == 1) 
      {
        if(already180[i])
        {
          servos[i].write(((potValues[i] / 1000.0f) * 180.0f) + 30.0f);
          already180[i] = false;
        }
        
      } else {
        if(!already180[i])
        {
          servos[i].write(180);
          already180[i] = true;
        }
        
      }
    }
  }

}
  }


}
