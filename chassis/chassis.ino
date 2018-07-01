// Code for a robot chassis with three sound sensors
// Three sensors triangulate sound to determine the direction of the loudest noise
// Then chassis chases after that noise

// Initialize volume variables, which will be used later
int vol_1 = 0;
int vol_2 = 0;
int vol_3 = 0;

// Initialize volume input received by sound sensor
// Later, we will analogRead these values and set them as volume variables initialized above
int volInput_1 = A5;
int volInput_2 = A4;
int volInput_3 = A3;

// Left and right wheels on chassis are controlled separatedly, attached to pins 3 and 5
int leftWheel = 3;
int rightWheel = 5;

// Trig and Echo pins are part of the sonar sensor and are used to control it
// Sonar is required so chassis knows when it's close to hitting an object and move away
int trigPin = 13;
int echoPin = 12;

// Initialize ambient noise level. This value should be adjusted based on chassis' environment
int ambNoise = 20;

// Initialize how long the wheels should run in order for chassis to turn x degrees
// Eg. To turn 180 degrees, one of the wheels must run for 1050 milliseconds
int turn180 = 1050;
int turn135 = turn180 * 3/4;
int turn90 = turn180 * 1/2;
int turn45 = turn180 * 1/4;
int turn23 = turn180 * 1/8;
int turn113 = turn180 * 5/8;

// Initialize how long the chassis should move forward for 
int forwardTime = 1000;

// Initialize delay time
int delayTime = 400;

// Define whether variables are inputs or outputs
// The only inputs are the volume inputs from the sound sensor and the echoPin from the sonar sensor
void setup() {
  pinMode(volInput_1, INPUT);
  pinMode(volInput_2, INPUT);
  pinMode(volInput_3, INPUT);
  pinMode(leftWheel, OUTPUT);
  pinMode(rightWheel, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(38400);
}

// Create a moveForward function that causes chassis to move forward
// This function will be used later
void moveForward() {
    // DigitalWrite both wheels to cause chassis to move forward
    digitalWrite(leftWheel, HIGH);
    digitalWrite(rightWheel, HIGH);

    // Code that allows sonar to receive input
    // We need this code in the moveForward function to prevent chassis from crashing into objects
    long duration, distance;
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(2); 
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); 
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) / 29.1;
    
    // If the chassis is less than 15 cm away from hitting something in front of it, turn 180 degrees
    if (distance < 15) {  
      digitalWrite(leftWheel, HIGH); 
      delay(turn180);
      digitalWrite(leftWheel,LOW);
    }
    delay(1000);
    digitalWrite(leftWheel, LOW);
    digitalWrite(rightWheel, LOW);

  }

void loop() {
  // Read the value from the sound sensor and set as vol_1, vol_2, vol_3 variables
  vol_1 = analogRead(volInput_1); 
  vol_2 = analogRead(volInput_2);
  vol_3 = analogRead(volInput_3);

  // Nested if statements below
  // Here we determine which sensor is receiving the loudest, second loudest, and quietest input

  // The outer if statement determines if all three volume inputs are above ambient noise
  // Only if that is the case, will we proceed. This ensures that the chassis will not chase 
  // after any conversational din in the room
  
  if (vol_1 > ambNoise && vol_2 > ambNoise && vol_3 > ambNoise) { 
    if (vol_1 > vol_2 && vol_1 > vol_3) {   // If vol_1 is the loudest
      if (vol_2 > vol_3) {                  // If vol_2 is louder than 3 
        // In this case, vol_1 > vol_2 > vol_3
        // Therefore, the noise is 135 degrees 
        // clockwise from the front of the chassis
        digitalWrite(leftWheel, HIGH);   
        delay(turn135);                     // Let left wheel run so chassis will turn 135 degrees
        digitalWrite(leftWheel, LOW);
        moveForward();
        delay(delayTime);
        
      } else if (vol_2 < vol_3) {           // vol_3 louder than 2
        // In this case, vol_1 > vol_2 > vol_3
        // Therefore, the noise is 135 degrees 
        // counterclockwise from the front of the chassis
        digitalWrite(rightWheel, HIGH);
        delay(turn135);
        digitalWrite(rightWheel, LOW);
        moveForward();
        delay(delayTime);
        
      } else if (vol_2 == vol_3) {          // same volume
        digitalWrite(leftWheel, HIGH);
        delay(turn180);
        digitalWrite(leftWheel, LOW);
        moveForward();
        delay(delayTime);
      }
      
    } else if (vol_2 > vol_3 && vol_2 > vol_1) {  // vol_2 loudest
      if (vol_3 > vol_1) {                        // vol_3 louder than 1
        digitalWrite(leftWheel, HIGH);
        delay(turn23);
        digitalWrite(leftWheel, LOW);
        moveForward();
        delay(delayTime);
        
      } else if (vol_3 < vol_1) {                 // vol_1 louder than 3
        digitalWrite(leftWheel, HIGH);
        delay(turn90);
        digitalWrite(leftWheel, LOW);
        moveForward();
        delay(delayTime);
        
      } else if (vol_3 == vol_1) {                // same volume
        digitalWrite(leftWheel, HIGH);
        delay(turn45);
        digitalWrite(leftWheel, LOW);
        moveForward();
        delay(delayTime);
      }
      
    } else if (vol_3 > vol_1 && vol_3 > vol_2) {  // vol_3 loudest
      if (vol_1 > vol_2) {                        // vol_3 louder than 1
        digitalWrite(rightWheel, HIGH);
        delay(turn90);
        digitalWrite(rightWheel, LOW);
        moveForward();
        delay(delayTime);
        
      } else if (vol_1 < vol_2) {                 // vol_1 louder than 3
        digitalWrite(rightWheel, HIGH);
        delay(turn23);
        digitalWrite(rightWheel, LOW);
        moveForward();
        delay(delayTime);
        
      } else if (vol_1 == vol_2) {                // same volume
        digitalWrite(rightWheel, HIGH);
        delay(turn45);
        digitalWrite(rightWheel, LOW);
        moveForward();
        delay(delayTime);
      }
    } // end volume conditions
  } // end ambNoise condition
} // end void loop

