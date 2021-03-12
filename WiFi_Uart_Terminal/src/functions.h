void setDefaultConfig(void)
{
	strlcpy(conf.APWifiSSID,		DEFAULT_AP_SSID,		sizeof(conf.APWifiSSID));
	strlcpy(conf.APWifiPass,		DEFAULT_AP_PASS,		sizeof(conf.APWifiPass));
	strlcpy(conf.clientWifiSSID,	"",						sizeof(conf.clientWifiSSID));
	strlcpy(conf.clientWifiPass,	"",						sizeof(conf.clientWifiPass));
	strlcpy(conf.devLogin,			DEFAULT_LOGIN,			sizeof(conf.devLogin));
	strlcpy(conf.devPass,			DEFAULT_PASS,			sizeof(conf.devPass));
	strlcpy(conf.devName,			DEFAULT_AP_SSID,		sizeof(conf.devName));

	conf.APip[0] = 192;
	conf.APip[1] = 168;
	conf.APip[2] = 4;
	conf.APip[3] = 1;
	conf.APgw[0] = 0;
	conf.APgw[1] = 0;
	conf.APgw[2] = 0;
	conf.APgw[3] = 0;
	conf.APmask[0] = 255;
	conf.APmask[1] = 255;
	conf.APmask[2] = 255;
	conf.APmask[3] = 0;
}

void saveConfToMem(void)
{
	iniConfSaveInit( CONFFILE );

	if(fd){
		iniConfSetComment("AP Settings");
		iniConfSet("APWifiSSID", conf.APWifiSSID);
		iniConfSet("APWifiPass", conf.APWifiPass);
		iniConfSetComment("Client Settings");
		iniConfSet("clientWifiSSID", conf.clientWifiSSID);
		iniConfSet("clientWifiPass", conf.clientWifiPass);
		iniConfSetComment("Other Settings");
		iniConfSet("devName", conf.devName);
		iniConfSet("devLogin", conf.devLogin);
		iniConfSet("devPass", conf.devPass);
		iniConfSetComment("IP Settings");
		iniConfSet("APip1", conf.APip[0]);
		iniConfSet("APip2", conf.APip[1]);
		iniConfSet("APip3", conf.APip[2]);
		iniConfSet("APip4", conf.APip[3]);
		iniConfSet("APmask1", conf.APmask[0]);
		iniConfSet("APmask2", conf.APmask[1]);
		iniConfSet("APmask3", conf.APmask[2]);
		iniConfSet("APmask4", conf.APmask[3]);
		iniConfSet("APgw1", conf.APgw[0]);
		iniConfSet("APgw2", conf.APgw[1]);
		iniConfSet("APgw3", conf.APgw[2]);
		iniConfSet("APgw4", conf.APgw[3]);
	}

	iniConfSaveDeInit();
}

bool WiFiConnectionState(void)
{
	bool state = false;
	if( WiFi.status() == WL_CONNECTED ) state = true;
	if( WiFi.status() == WL_CONNECTED && WiFi.localIP().toString() == "0.0.0.0" ) state = false;
	return state;
}

void wifi_init(void)
{
	conf.softAPen = false;
	WiFi.hostname(conf.devName);
	WiFi.setAutoReconnect(false);
	WiFi.setAutoConnect(false);
	WiFi.mode(WiFiMode_t::WIFI_STA);
	
	WiFi.begin(conf.clientWifiSSID, conf.clientWifiPass);
	WiFi.softAPdisconnect( true );
}

void wifi_ap_init(void)
{
	if( conf.softAPen ) return;
	
	IPAddress	apIP( conf.APip );
	IPAddress	apGW( conf.APgw );
	IPAddress	apMASK( conf.APmask );
	
	WiFi.mode(WiFiMode_t::WIFI_AP);
	WiFi.softAPConfig(apIP, apGW, apMASK);
	conf.softAPen = WiFi.softAP(conf.APWifiSSID, conf.APWifiPass);
	//Serial.print("WiFi SOFT AP Started\n");
	
	IPAddress myIP = WiFi.softAPIP();
}

void start_execution(void)
{
	if( !conf.execute ) return;
	if( !conf.spiffsActive ) return;
	if( !SPIFFS.exists( conf.execFile ) ) return;
	
	File fd = SPIFFS.open( conf.execFile, "r" );
	
	uint16_t ch = 0;
	bool find = false;
	
	while( fd.available() ){
		String line = fd.readStringUntil('\n');
		if( ch == conf.lineNum ){
			Serial.println( line );
			find = true;
			conf.lineNum++;
			conf.lastPosition = fd.position();
			break;
		}
		ch++;
	}
	
	if( !find ){
		conf.execute		= false;
		conf.lineNum		= 0;
		conf.lastPosition	= 0;
		conf.execFile		= "";
	}
	
	fd.close();
}

char symToHex(const char sym)
{
	char val = 0;

	if( sym >= '0' && sym <= '9' ){
		val += sym - '0';
	}else{
		if( sym >= 'A' && sym <= 'F' ){
			val += sym - 'A' + 10;
		}else{
			if( sym >= 'a' && sym <= 'f' ){
				val += sym - 'a' + 10;
			}
		}
	}

	return val;
}
