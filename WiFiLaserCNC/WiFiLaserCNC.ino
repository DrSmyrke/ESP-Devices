#include "pin_define.h"
#include "config.h"
#include "iniConf.h"
#include "data.h"
#include "http.h"
#include "functions.h"
#include "timer.h"

void setup()
{
	Serial.begin(115200);
	delay(3000);
	
	portInit();
	if (SPIFFS.begin()) conf.spiffsActive = true;
	delay(300);
	setDefaultConfig();
	delay(300);
	iniConfPars( CONFFILE );
	delay(100);
	wifi_init();
	
	webServer.on("/", httpHandleIndex);
	webServer.on("/config", httpHandleSettings);
	webServer.on("/admin", httpHandleAdminKa);
	webServer.on("/set", handleSet);
	webServer.on("/gcode", handleGcode);
	webServer.on("/get", handleGet);
	webServer.on("/upload", HTTP_POST, [](){}, handleUpload);
	webServer.on(CONFFILE, httpGetConfig);
	webServer.onNotFound(httpHandleNotFound);
	webServer.begin();

	/*
	
	EEPROM.begin(4096);
	conf.relayNum		= EEPROM.read( EEPROM_ADDRESS_NUM );
	conf.relayState		= EEPROM.read( EEPROM_ADDRESS_STATE );
	conf.relayMode		= EEPROM.read( EEPROM_ADDRESS_MODE );
	
	
	//Serial.print("SPIFFS active:");Serial.println(conf.spiffsActive);

	
  
	*/

	//SPIFFS.format();

	conf.responseStr.reserve( 128 );
	conf.response.reserve( 128 );
	
	
	delay(100);
	timer_init();
}

void loop()
{
	webServer.handleClient();
	
	while( Serial.available() ) {
		char inChar = (char)Serial.read();
		conf.responseStr += inChar;
		if (inChar == '\n') {
			if( conf.responseStr == "ok" ){
				conf.cmdState = 0;
			}else{
				conf.cmdState = 2;
			}
			conf.response = conf.responseStr;
			conf.responseStr = "";
		}
		
    }
}
