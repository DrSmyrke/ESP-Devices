#include <EEPROM.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include "FS.h"

//#define RELAY_HIGH_LEVEL
#define RELAY_LOW_LEVEL

#define setPlus(reg,bit) reg |= (1<<bit)
#define setZero(reg,bit) reg &= ~(1<<bit)
#define ibi(reg,bit) reg ^= (1<<bit)
#define CheckBit(reg,bit) (reg&(1<<bit))

#define PORT_NAME_SIZE							16
#define WIFI_SSID_BUFF_SIZE						16
#define WIFI_PASS_BUFF_SIZE						16
#define PARAM_SIZE								16
#define SEND_URL_SIZE							32
#define	EEPROM_ADDRESS_NUM						0
#define	EEPROM_ADDRESS_STATE					1
#define	EEPROM_ADDRESS_MODE						2
#define PORT1									D1
#define PORT2									D2
#define PORT3									D3
#define PORT4									D5
#define PORT5									D6
#define PORT6									D7



#ifdef RELAY_LOW_LEVEL
	#define PORT_ON				0
	#define PORT_OFF			1
	#define RELAY_ON(num)		digitalWrite(RELAY_PORT,num,PORT_ON)
	#define RELAY_OFF(num)		digitalWrite(RELAY_PORT,num,PORT_OFF)
	#define PORT_STATE_MASK		0b11111111
#else
	#ifdef RELAY_HIGH_LEVEL
		#define PORT_ON				1
		#define PORT_OFF			0
		#define RELAY_ON(num)	digitalWrite(RELAY_PORT,num,PORT_ON)
		#define RELAY_OFF(num)	digitalWrite(RELAY_PORT,num,PORT_OFF)
		#define PORT_STATE_MASK	0b00000000
	#endif
#endif



#define UPDFILE				"main.bin"
#define CONFFILE			"/settings.cfg"
#define DEFAULT_AP_SSID		"MyDEV"
#define DEFAULT_AP_PASS		"12345678"
#define DEFAULT_LOGIN		"admin"
#define DEFAULT_PASS		"admin"

enum{
	upload_type_file		= 1,
	upload_type_firmware,
};

struct MyConf{
	uint8_t APip[4]					= { 192, 168, 4, 1 };
	uint8_t APgw[4]					= { 0, 0, 0, 0 };
	uint8_t APmask[4]				= { 255, 255, 255, 0 };
	char APWifiSSID[ WIFI_SSID_BUFF_SIZE ];
	char APWifiPass[ WIFI_PASS_BUFF_SIZE ];
	char clientWifiSSID[ WIFI_SSID_BUFF_SIZE ];
	char clientWifiPass[ WIFI_PASS_BUFF_SIZE ];
	char sendUrl[ SEND_URL_SIZE ];
	char devLogin[ PARAM_SIZE ];
	char devPass[ PARAM_SIZE ];
	char devName[ PARAM_SIZE ];
	String version					= "0.6";
	uint8_t uploadType				= 0;
	bool spiffsActive				= false;
	uint8_t relayNum				= 2;
	uint8_t relayState				= 0;
	uint8_t relayMode				= 0;
	char port1name[ PORT_NAME_SIZE ];
	char port2name[ PORT_NAME_SIZE ];
	char port3name[ PORT_NAME_SIZE ];
	char port4name[ PORT_NAME_SIZE ];
	char port5name[ PORT_NAME_SIZE ];
	char port6name[ PORT_NAME_SIZE ];
	char port7name[ PORT_NAME_SIZE ];
	char port8name[ PORT_NAME_SIZE ];
};

MyConf conf;
File fsUploadFile;

ESP8266WebServer webServer(80);
HTTPClient HTTPclient;

void setDefaultConfig(void);
void saveConfToMem(void);
void readPages(void);
bool WiFiConnectionState(void);



//---------------------------------------------------------
void portReScan()
{
	conf.relayNum = 8;
	//TODO: Сделать рескан
	EEPROM.write( EEPROM_ADDRESS_NUM, conf.relayNum );
	EEPROM.commit();
}

void alarm()
{
	//TODO: Сделать мигание лампочками
}

void portSetMode( uint8_t mode )
{
	conf.relayMode = mode;
	EEPROM.write( EEPROM_ADDRESS_MODE, conf.relayMode );
	EEPROM.commit();
}

void portSetState( )
{
	digitalWrite( PORT1, ( CheckBit( conf.relayState, 0 ) ) ? PORT_ON : PORT_OFF );
	digitalWrite( PORT2, ( CheckBit( conf.relayState, 1 ) ) ? PORT_ON : PORT_OFF );
	digitalWrite( PORT3, ( CheckBit( conf.relayState, 2 ) ) ? PORT_ON : PORT_OFF );
	digitalWrite( PORT4, ( CheckBit( conf.relayState, 3 ) ) ? PORT_ON : PORT_OFF );
	digitalWrite( PORT5, ( CheckBit( conf.relayState, 4 ) ) ? PORT_ON : PORT_OFF );
	digitalWrite( PORT6, ( CheckBit( conf.relayState, 5 ) ) ? PORT_ON : PORT_OFF );

	EEPROM.write( EEPROM_ADDRESS_STATE, conf.relayState );
	EEPROM.commit();
}

void portRestoreState(void)
{
	conf.relayState &= conf.relayMode;
	portSetState();
}

void portOn( uint8_t num )
{
	setPlus( conf.relayState, num );
	portSetState();
}

void portOff( uint8_t num )
{
	setZero( conf.relayState, num );
	portSetState();
}

void portInit(void)
{
	pinMode( PORT1, OUTPUT );
	pinMode( PORT2, OUTPUT );
	pinMode( PORT3, OUTPUT );
	pinMode( PORT4, OUTPUT );
	pinMode( PORT5, OUTPUT );
	pinMode( PORT6, OUTPUT );
}
