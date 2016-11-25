// This #include statement was automatically added by the Particle IDE.
#include "application.h"
 #include "lib/AssetTracker/firmware/AssetTracker.h"
 #include "lib/streaming/firmware/spark-streaming.h"
 #include "trackerone.h"


/* -----------------------------------------------------------
See
https://github.com/spark/AssetTracker/blob/master/firmware/examples/2_Accelerometer.cpp
for more details where this was take from
  ---------------------------------------------------------------*/


// All declations moved to trackerone.h


// setup() and loop() are both required. setup() runs once when the device starts
// and is used for registering functions and variables and initializing things
void setup() {
    // Sets up all the necessary AssetTracker bits
//    t.begin();

    // Enable the GPS module. Defaults to off to save power.
    // Takes 1.5s or so because of delays.
    // NOTE:   this seems to prevent the readXYZ or loop from owrking
    //t.gpsOn();

    // Opens up a Serial port so you can listen over USB
    Serial.begin(9600);

    // These three functions are useful for remote diagnostics. Read more below.
    Particle.function("aThresh",accelThresholder);
    Particle.function("tmode", transmitMode);
    Particle.function("batt", batteryStatus);
}

// loop() runs continuously
void loop() {

  if(millis()%5000 < 20 ) {
        Serial.print("doing the 30 loop readXYZmagnitude ");
  }

//  t.updateGPS(); // You'll need to run this every loop to capture the GPS output
    // Check if there's been a big acceleration


    if(t.readXYZmagnitude() > accelThreshold ){
        // Create a nice string with commas between x,y,z
        String pubAccel = String::format("%d,%d,%d",t.readX(),t.readY(),t.readZ());

        // Send that acceleration to the serial port where it can be read by USB
        //Serial.println(pubAccel);

        Serial << MYVERSION << " Z: " << t.readX()  << " Y: " << t.readY() << " Z: " << t.readZ();
        Serial << " XYZ Magnitude  " << t.readXYZmagnitude() << endl;
        //Serial.println("have a good day");

        // If it's set to transmit AND it's been at least delayMinutes since the last one...
        if(transmittingData && ((millis()-lastPublish) > (delayMinutes*60*1000))){
            lastPublish = millis();
            Particle.publish("A", pubAccel, 60, PRIVATE);
        }

    }
    


}

// Remotely change the trigger threshold!
int accelThresholder(String command){
    accelThreshold = atoi(command);
    return 1;
}

// Allows you to remotely change whether a device is publishing to the cloud
  // or is only reporting data over Serial. Saves data when using only Serial!
  // Change the default at the top of the code.
int transmitMode(String command){
    transmittingData = atoi(command);
    return 1;
}

// Lets you remotely check the battery status by calling the function "batt"
  // Triggers a publish with the info (so subscribe or watch the dashboard)
  // and also returns a '1' if there's >10% battery left and a '0' if below
int batteryStatus(String command){
    // Publish the battery voltage and percentage of battery remaining
      // if you want to be really efficient, just report one of these
      // the String::format("%f.2") part gives us a string to publish,
      // but with only 2 decimal points to save space
    Particle.publish("B",
          "v:" + String::format("%f.2",fuel.getVCell()) +
          ",c:" + String::format("%f.2",fuel.getSoC()),
          60, PRIVATE
    );
    // if there's more than 10% of the battery left, then return 1
    if(fuel.getSoC()>10){ return 1;}
    // if you're running out of battery, return 0
    else { return 0;}
}
