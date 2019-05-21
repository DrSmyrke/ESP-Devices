os_timer_t myTimer;

void timerCallback(void *pArg)
{
	//Serial.print("WiFiConnectionState: ");Serial.println( WiFiConnectionState() );
	//Serial.print("WiFi IP: ");Serial.println( WiFi.localIP().toString() );
	if( !WiFiConnectionState() && conf.APWifiSSID != "" ) WiFi.reconnect();
}

void timer_init(void)
{
	os_timer_setfn(&myTimer, timerCallback, NULL);
	os_timer_arm(&myTimer, 3000, true);
}
