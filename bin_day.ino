#include <WiFiNINA.h>
#include "wifi_settings.hpp"

#define DEBUG_OUTPUT

#define MAX_CONNECT_ATTEMPTS  ( 3 )
#define MAX_GET_EPOCH_ATTEMPS ( 50 )

void setup() 
{
  #ifdef DEBUG_OUTPUT
  // Start Serial
  Serial.begin( 9600 );
  // Wait for Serial to start
  while ( !Serial );
  #endif

  pinMode( LED_BUILTIN, OUTPUT );
}

void loop() 
{
  // Get unix time from WiFi
  uint64_t theTime = 0;
  theTime = getUnixFromWifi();
  Serial.println(theTime);

  delay( 1000 );
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
    errorFlash();
    #ifdef DEBUG_OUTPUT
    Serial.println( "Error: WiFi module not found" );
    #endif
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

void errorFlash()
{
  bool state = false;
  for ( ;; )
  {
    digitalWrite( LED_BUILTIN, state );
    state = !state;
    delay( 500 );
  }
}
