#include <WiFiNINA.h>
#include "time_magic_numbers.hpp"
#include "wifi_settings.hpp"
#include "auto_generated_data.hpp"

// Arduino will get stuck if debug is enabled but the board is not connected to a pc
#define DEBUG_OUTPUT

// #define LED_TEST
// #define BUTTON_TEST

#define BUTTON_PIN                ( 20 )

#define MAX_CONNECT_ATTEMPTS      ( 3 )
#define MAX_GET_EPOCH_ATTEMPS     ( 50 )

// Delay in seconds between using WiFi to check the time
#define TIME_DOWNLOAD_PERIOD      ( SECONDS_IN_TEN_HOURS )
// Delay between when the program should check if it's bin day
#define BINDAY_CHECK_DELAY        ( SECONDS_PER_TEN_MINUTES )
// Button poll frequency in milliseconds
#define MAIN_REFRESH_DELAY        ( 100 )
#define SLOW_BLINK_DELAY          ( 2000 )
#define FAST_BLINK_DELAY          ( 1000 )

// Global Variables
uint64_t downloadedUnixTime;              // The value returned WiFi.getTime(), in seconds
uint64_t currentUnixTime;                 // Unix time which is calculated by WiFi.getTime + time since calling Wifi.getTime
uint32_t wifiGetTimeMillis;               // The value of millis() last time WiFi.getTime was called
uint64_t lastBinDayCheckUnix;             // Unix time last time the bin data was checked
uint64_t lastButtonPressUnix;             // Unix time last time the button was pressed
uint16_t blinkSpeed[NUMBER_OF_DATASETS];  // Led blink speed in milliseconds, 0 means off and not blinking

void setup()
{
  #ifdef DEBUG_OUTPUT
  // Start Serial
  Serial.begin( 9600 );
  // Wait for Serial to start
  while ( !Serial )
  {
    delay( 100 );
  }
  #endif

  // Setup output pins
  pinMode( LED_BUILTIN, OUTPUT );
  pinMode( BUTTON_PIN, INPUT );
  for ( int dataSetNumber = 0; dataSetNumber < NUMBER_OF_DATASETS; dataSetNumber++ )
  {
    pinMode( setArray[dataSetNumber].ledPin, OUTPUT );
  }

  // --- Setup tests

  #ifdef LED_TEST
  // Test the LEDs
  digitalWrite( LED_BUILTIN, HIGH );
  delay(500);
  digitalWrite( LED_BUILTIN, LOW );
  for ( int dataSetNumber = 0; dataSetNumber < NUMBER_OF_DATASETS; dataSetNumber++ )
  {
    digitalWrite( setArray[dataSetNumber].ledPin, HIGH );
    delay(500);
    digitalWrite( setArray[dataSetNumber].ledPin, LOW );
  }
  #endif

  #ifdef BUTTON_TEST
  bool buttonState;
  while ( true )
  {
    // Read the input pin
    buttonState = digitalRead( BUTTON_PIN );
    // Write to the built in led
    digitalWrite( LED_BUILTIN, buttonState );
    delay(10);
  }
  #endif
}

void loop() 
{
  // Get unix time from WiFi if needed
  refreshTime();

  // Calculate actual time
  currentUnixTime = downloadedUnixTime + ( ( millis() - wifiGetTimeMillis ) / 1000UL );

  // Check if it's bin day
  refreshBinDay();

  // A delay which pretty much only affects 
  delay( MAIN_REFRESH_DELAY );
}

// --- Functions relating to fetching time from WiFi

void refreshTime()
{
  if ( ( downloadedUnixTime == 0U ) || ( (millis() - wifiGetTimeMillis) / 1000UL > TIME_DOWNLOAD_PERIOD ) )
  {
    // Built in LED to indicate when WiFi is being accessed
    digitalWrite( LED_BUILTIN, HIGH );
    downloadedUnixTime = getUnixFromWifi();
    wifiGetTimeMillis = millis();
    if ( downloadedUnixTime == 0U )
    {
      // Time was not acquired from WiFi, wait 10 minutes and then try again
      delay( MILLISECONDS_PER_TEN_MINUTES );
      return;
    }
    digitalWrite( LED_BUILTIN, LOW );
  }
}

uint64_t getUnixFromWifi()
{
  /*
  Connects to wifi, gets unix time, and disconnects from wifi
  Returns 0 on failure
  Makes MAX_CONNECT_ATTEMPTS attempts to connect to wifi
  */

  // Check the WiFi module
  if ( WiFi.status() == WL_NO_MODULE )
  {
    digitalWrite( LED_BUILTIN, HIGH );
    #ifdef DEBUG_OUTPUT
    Serial.println( "Error: WiFi module not found" );
    #endif
    while ( true )
    {
      delay(10000);
    }
  }

  // Connect to WiFi
  int64_t status = WL_IDLE_STATUS;
  uint8_t connectAttempts = 0U;
  while ( WL_CONNECTED != status )
  {
    if ( connectAttempts++ == MAX_CONNECT_ATTEMPTS )
    {
      #ifdef DEBUG_OUTPUT
      Serial.println( "Error: Failed to connect to WiFi" );
      #endif
      return 0;
    }
    status = WiFi.begin( SSID, PASSWORD );
  }

  #ifdef DEBUG_OUTPUT
  Serial.println( "Connected to WiFi sucessfully" );
  #endif

  // Get unix epoch, may take several attempts before WiFi is properly initialised
  uint64_t unixEpoch = WiFi.getTime();
  uint8_t epochFetchAttempts = 0U;
  while ( 0 == unixEpoch )
  {
    delay( 200 );
    if ( MAX_GET_EPOCH_ATTEMPS == epochFetchAttempts++ )
    {
      #ifdef DEBUG_OUTPUT
      Serial.println( "Took too many attempts to get epoch" );
      Serial.println( "Disconnecting WiFI" );
      #endif
      WiFi.end();
      return 0;
    }
    unixEpoch = WiFi.getTime();
  }

  #ifdef DEBUG_OUTPUT
  Serial.print( "Fetched the unix epoch time in " );
  Serial.print( ++epochFetchAttempts );
  Serial.println( " attempts" );
  #endif

  return unixEpoch;
}

// --- Functions relating to checking if it's bin day

void refreshBinDay()
{

}

void checkForBinDay()
{
  uint64_t actualTime = downloadedUnixTime + ( ( millis() - wifiGetTimeMillis ) / 1000UL );
  uint64_t* arrayPtr;

  #ifdef DEBUG_OUTPUT
  Serial.println(" --- ");
  Serial.println("actualTime = %d");
  #endif

  // For each data set (For recyling and for landfill)
  for ( uint8_t dataSetNumber = 0; dataSetNumber < NUMBER_OF_DATASETS; dataSetNumber++ )
  {
    #ifdef DEBUG_OUTPUT
    Serial.println("dataSetNumber = ");
    #endif
    // For each bin day unix time
    arrayPtr = (uint64_t*) setArray[dataSetNumber].arrayStart;
    for ( uint16_t arrayIndex = 0; arrayIndex < setArray[dataSetNumber].arrayLength; arrayIndex++ )
    {

      // Check if the button has been pressed within this bin day
      if ( ( *arrayPtr ) || ( *arrayPtr ) )
      {
        #ifdef DEBUG_OUTPUT
        Serial.println("User has pressed button near this bin day");
        #endif
        blinkSpeed[dataSetNumber] = 0;
        break;
      }
      // Check if bin day is tomorrow (slow blink)
      // else if ()
      // {

      // }
      // // Check if bin day is today (fast blink)
      // else if ()
      // {

      // }
      // Increment
      arrayPtr++;
    }
  }
}

// --- Button functions

bool readButton()
{
  bool buttonState = digitalRead( BUTTON_PIN );
  if ( buttonState )
  {
    processButtonPress();
  }
  return buttonState;
}

void processButtonPress()
{
  lastButtonPressUnix = downloadedUnixTime + ( ( millis() - wifiGetTimeMillis ) / 1000UL );
}

// --- LED functions

void blinkHandler()
{

}
