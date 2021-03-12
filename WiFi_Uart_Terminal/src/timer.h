os_timer_t myTimer;

void timerCallback(void *pArg)
{
	
	//Serial.print("WiFiConnectionState: ");Serial.println( WiFiConnectionState() );
	//Serial.print("WiFi IP: ");Serial.println( WiFi.localIP().toString() );
	//Serial.print("WiFi SSID: ");Serial.println( String(conf.clientWifiSSID) );
	
	if( !WiFiConnectionState() ){
		digitalWrite(LED_BUILTIN, HIGH);
		if( strlen(conf.clientWifiSSID) == 0 ){
			wifi_ap_init();
			return;
		}
		if( conf.softAPen ){
			wifi_init();
			return;
		}
		WiFi.reconnect();
		//Serial.print("WiFi reconnect\n");
		
	}else{
		digitalWrite(LED_BUILTIN, LOW);
		WiFi.softAPdisconnect( true );
		//Serial.print("WiFi SOFT AP disconnect\n");
	}
}

void timer_init(void)
{
	os_timer_setfn(&myTimer, timerCallback, NULL);
	os_timer_arm(&myTimer, 10000, true);
}
