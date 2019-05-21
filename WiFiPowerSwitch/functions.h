void setDefaultConfig(void)
{
	strlcpy(conf.APWifiSSID,		DEFAULT_AP_SSID,		sizeof(conf.APWifiSSID));
	strlcpy(conf.APWifiPass,		DEFAULT_AP_PASS,		sizeof(conf.APWifiPass));
	strlcpy(conf.clientWifiSSID,	"",						sizeof(conf.clientWifiSSID));
	strlcpy(conf.clientWifiPass,	"",						sizeof(conf.clientWifiPass));
	strlcpy(conf.sendUrl,			"",						sizeof(conf.sendUrl));
	strlcpy(conf.devLogin,			DEFAULT_LOGIN,			sizeof(conf.devLogin));
	strlcpy(conf.devPass,			DEFAULT_PASS,			sizeof(conf.devPass));
	strlcpy(conf.devName,			DEFAULT_AP_SSID,		sizeof(conf.devName));
	strlcpy(conf.port1name,			"Port 1",				sizeof(conf.port1name));
	strlcpy(conf.port2name,			"Port 2",				sizeof(conf.port2name));
	strlcpy(conf.port3name,			"Port 3",				sizeof(conf.port3name));
	strlcpy(conf.port4name,			"Port 4",				sizeof(conf.port4name));
	strlcpy(conf.port5name,			"Port 5",				sizeof(conf.port5name));
	strlcpy(conf.port6name,			"Port 6",				sizeof(conf.port6name));
	strlcpy(conf.port7name,			"Port 7",				sizeof(conf.port7name));
	strlcpy(conf.port8name,			"Port 8",				sizeof(conf.port8name));

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
		iniConfSet("sendUrl", conf.sendUrl);
		iniConfSet("devName", conf.devName);
		iniConfSet("devLogin", conf.devLogin);
		iniConfSet("devPass", conf.devPass);
		iniConfSetComment("Ports Names");
		iniConfSet("port1name", conf.port1name);
		iniConfSet("port2name", conf.port2name);
		iniConfSet("port3name", conf.port3name);
		iniConfSet("port4name", conf.port4name);
		iniConfSet("port5name", conf.port5name);
		iniConfSet("port6name", conf.port6name);
		iniConfSet("port7name", conf.port7name);
		iniConfSet("port8name", conf.port8name);
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
	IPAddress	apIP( conf.APip );
	IPAddress	apGW( conf.APgw );
	IPAddress	apMASK( conf.APmask );

	WiFi.hostname(conf.devName);
	WiFi.setAutoReconnect(false);
	WiFi.softAPConfig(apIP, apGW, apMASK);
	WiFi.softAP(conf.APWifiSSID, conf.APWifiPass);
	WiFi.begin(conf.clientWifiSSID, conf.clientWifiPass);
	IPAddress myIP = WiFi.softAPIP();
}
