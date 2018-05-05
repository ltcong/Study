#include <WiFi.h>
#include <WiFiUdp.h>
#include <coap.h>

#define COAP_SERVER_IP	"192.168.16.29"

IPAddress server_ip;
char* ssid     = "TIC";
const char* password = "T1cVi3tn@m";

// CoAP client response callback
void callback_response(CoapPacket &packet, IPAddress ip, int port);

// UDP and CoAP class
WiFiUDP udp;
Coap coap(udp);

// CoAP client response callback
void callback_response(CoapPacket &packet, IPAddress ip, int port) {
  Serial.println("[Coap Response got]");

  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;

  Serial.println(p);
}

void setup() {
  Serial.begin(115200);
  server_ip.fromString(COAP_SERVER_IP);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // client response callback.
  // this endpoint is single callback.
  Serial.println("Setup Response Callback");
  coap.response(callback_response);

  // start coap server/client
  Serial.println("CoAP client starting......");
  coap.start();
}

void loop() {
  // send GET or PUT coap request to CoAP server.
  // To test, use libcoap, microcoap server...etc
  int msgid1 = coap.put(IPAddress(server_ip), 5683, "light", "0");
  Serial.println("Send Request");
  //int msgid2 = coap.get(IPAddress(server_ip), 5683, "light");
  delay(2000);
  int msgid3 = coap.put(IPAddress(server_ip), 5683, "light", "1");
  Serial.println("Send Request");
  //int msgid4 = coap.get(IPAddress(server_ip), 5683, "light");
  delay(2000);
  coap.loop();
}
