#include <WiFiNINA.h>
#include "time_magic_numbers.hpp"
#include "wifi_settings.hpp"
#include "auto_generated_data.hpp"

// Arduino will get stuck if debug is enabled but the board is not connected to a pc
// #define DEBUG_OUTPUT

// #define LED_TEST
// #define BUTTON_TEST

#define BUTTON_PIN                ( 20 )

#define MAX_CONNECT_ATTEMPTS      ( 3 )
#define MAX_GET_EPOCH_ATTEMPS     ( 50 )

// Delay in seconds between using WiFi to check the time
#define TIME_DOWNLOAD_PERIOD      ( SECONDS_IN_TEN_HOURS )
// Button poll frequency in milliseconds
#define BUTTON_POLL_FREQUENCY_MS  ( 50 )
// LED toggle period in milliseconds. E.g. if value is 500, LED will toggle every 500 ms
#define LED_TOGGLE_PERIOD         ( 1000 )

// Global Variables
uint64_t currentTime;               // Seconds since 1970
uint64_t timeSinceTimeDownload = 0; // Seconds since WiFi.getTime() was last called
uint64_t flashIntervalThing[NUMBER_OF_DATASETS];
bool isItBinDay[NUMBER_OF_DATASETS];

void setup() 
{
  #ifdef DEBUG_OUTPUT
  // Start Serial
  Serial.begin( 9600 );
  // Wait for Serial to start
  while ( !Serial );
  #endif

  // Setup output pins
  pinMode( LED_BUILTIN, OUTPUT );
  pinMode( BUTTON_PIN, INPUT );
  for ( int dataSetNumber = 0; dataSetNumber < NUMBER_OF_DATASETS; dataSetNumber++ )
  {
    pinMode( setArray[dataSetNumber].ledPin, OUTPUT );
  }

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
  // Get unix time from WiFi
  currentTime = getUnixFromWifi();
  Serial.println( currentTime );

  checkForBinDay();

  /*
  Get the time from wifi every X hours
  Blink the built-in led if there is no wifi
  Blink an led if its bin day, until the button is pressed


  */
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
  uint8_t connectAttempts = 0;
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
  uint8_t getEpochAttempts = 0;
  while ( 0 == unixEpoch )
  {
    delay( 200 );
    if ( MAX_GET_EPOCH_ATTEMPS == getEpochAttempts++ )
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
  Serial.print( ++getEpochAttempts );
  Serial.println( " attempts" );
  #endif

  return unixEpoch;
}

void checkForBinDay()
{
  //
}
