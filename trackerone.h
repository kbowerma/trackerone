//defines
#define FILENAME "trackerone.cpp"
#define MYVERSION "0.0.3. build 1219"

//declarations
// Set whether you want the device to publish data to the internet by default here.
  // 1 will Particle.publish AND Serial.print, 0 will just Serial.print
  // Extremely useful for saving data while developing close enough to have a cable plugged in.
  // You can also change this remotely using the Particle.function "tmode" defined in setup()
int transmittingData = 0;

// Used to keep track of the last time we published data
long lastPublish = 0;

// How many minutes minimum between publishes? 10+ recommended!
int delayMinutes = 10;

// Threshold to trigger a publish
// 9000 is VERY sensitive, 12000 will still detect small bumps
int accelThreshold = 12000;

unsigned long lastTime = 0;

// Creating an AssetTracker named 't' for us to reference
AssetTracker t = AssetTracker();

// A FuelGauge named 'fuel' for checking on the battery state
FuelGauge fuel;

// Prototypes
 int accelThresholder(String command);
 int transmitMode(String command);
 int batteryStatus(String command);
 int gpsPublish(String command);
