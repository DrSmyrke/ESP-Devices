os_timer_t myTimer;
uint8_t timerCounter;

void timerCallback(void *pArg)
{
	Serial.print("Speed: ");Serial.print(pktCounter);Serial.print(" pkt/s	");
	Serial.print("CPU: ");Serial.print(tickCounter);Serial.print(" tps	");
	Serial.print("Data: ");Serial.print(DataPktCounter);Serial.print(" pkt/s           \r");
	pktCounter = tickCounter = DataPktCounter = 0;

/*
	if( timerCounter++ < 10 ) return;
	Serial.println(F("Type:   /-------MAC------/-----WiFi Access Point SSID-----/  /----MAC---/  Chnl  RSSI"));
	Serial.println("\n-------------------------------------------------------------------------------------\n");
	for (int u = 0; u < clientsCount; u++) print_client(clients_known[u]);
	for (int u = 0; u < apsCount; u++) print_beacon(aps_known[u]);
	Serial.println("\n-------------------------------------------------------------------------------------\n");
	timerCounter = 0;
*/
}

void timer_init(void)
{
	os_timer_setfn(&myTimer, timerCallback, NULL);
	os_timer_arm(&myTimer, 1000, true);
}
