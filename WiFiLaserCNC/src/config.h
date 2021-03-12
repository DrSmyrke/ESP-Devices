#include <EEPROM.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include "FS.h"

#define setPlus(reg,bit) reg |= (1<<bit)
#define setZero(reg,bit) reg &= ~(1<<bit)
#define ibi(reg,bit) reg ^= (1<<bit)
#define CheckBit(reg,bit) (reg&(1<<bit))

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


#define UPDFILE				"main.bin"
#define CONFFILE			"/settings.cfg"
#define DEFAULT_AP_SSID		"WiFiLaserCNC"
#define DEFAULT_AP_PASS		"12345678"
#define DEFAULT_LOGIN		"admin"
#define DEFAULT_PASS		"admin"

enum{
	upload_type_file		= 1,
	upload_type_firmware,
};

struct MyConf{
	uint8_t APip[4]					= { 0, 0, 0, 0 };
	uint8_t APgw[4]					= { 0, 0, 0, 0 };
	uint8_t APmask[4]				= { 0, 0, 0, 0 };
	char APWifiSSID[ WIFI_SSID_BUFF_SIZE ];
	char APWifiPass[ WIFI_PASS_BUFF_SIZE ];
	char clientWifiSSID[ WIFI_SSID_BUFF_SIZE ];
	char clientWifiPass[ WIFI_PASS_BUFF_SIZE ];
	char sendUrl[ SEND_URL_SIZE ];
	char devLogin[ PARAM_SIZE ];
	char devPass[ PARAM_SIZE ];
	char devName[ PARAM_SIZE ];
	String version					= "0.2";
	uint8_t uploadType				= 0;
	bool spiffsActive				= false;
	uint16_t lineNum				= 0;
	uint32_t lastPosition			= 0;
	bool execute					= false;
	bool softAPen					= false;
	uint8_t cmdState				= 0;
	String responseStr				= "";
	String response					= "";
	String execFile					= "";
	char buff[32];
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
void saveCurrentLine()
{
	EEPROM.write( EEPROM_ADDRESS_NUM, conf.lineNum );
	EEPROM.commit();
}

void portInit(void)
{
	pinMode(LED_BUILTIN, OUTPUT);
}
