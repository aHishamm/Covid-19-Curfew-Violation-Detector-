#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
TinyGPSPlus gps;  // The TinyGPS++ object

SoftwareSerial ss(4, 5); // The serial connection to the GPS device

const char* ssid = "ssid";
const char* password = "pass";
const char* mqtt_server = "broker.mqtt-dashboard.com";
float latitude , longitude;
String curfewStatus;
char charPayload;
int year , month , date, hour , minute , second;
String date_str , time_str , lat_str , lng_str;
String NWLAT,NWLOG,NELAT,NELOG,SWLAT,SWLOG,SELAT,SELOG;
int pm;
WiFiClient espClient;
PubSubClient client(espClient);
WiFiServer server(4269);
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println();
  if ((char)payload[0] == 'A') {
    NWLAT = (char*)payload;
    NWLAT = NWLAT.substring(1,10);
  } else if((char)payload[0] == 'B'){
    NWLOG = (char*)payload; 
    NWLOG = NWLOG.substring(1,10); 
  }
  else if((char)payload[0] == 'C'){
    NELAT = (char*)payload; 
    NELAT = NELAT.substring(1,10); 
  }
  else if((char)payload[0] == 'D'){
    NELOG = (char*)payload; 
    NELOG = NELOG.substring(1,10); 
  }
  else if((char)payload[0] == 'E'){
    SWLAT = (char*)payload; 
    SWLAT = SWLAT.substring(1,10); 
  }
  else if((char)payload[0] == 'F'){
    SWLOG = (char*)payload; 
    SWLOG = SWLOG.substring(1,10); 
  }
  else if((char)payload[0] == 'G'){
    SELAT = (char*)payload; 
    SELAT = SELAT.substring(1,10); 
  }
  else if((char)payload[0] == 'H'){
    SELOG = (char*)payload; 
    SELOG = SELOG.substring(1,10); 
  }
}

void setup()
{
  Serial.begin(115200);
  ss.begin(9600);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());

}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
    Serial.print("North west latitude: ");
    Serial.println(NWLAT);
    Serial.print("North west longitude: "); 
    Serial.println(NWLOG);
    Serial.print("North east latitude: "); 
    Serial.println(NELAT); 
    Serial.print("North east longitude: "); 
    Serial.println(NELOG); 
    Serial.print("South west latitude: "); 
    Serial.println(SWLAT); 
    Serial.print("South west longitude: "); 
    Serial.println(SWLOG); 
    Serial.print("South east latitude: "); 
    Serial.println(SELAT); 
    Serial.print("South east longitude: "); 
    Serial.println(SELOG); 
    curfewStatus = geoDomainTest();
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
    {
       lat_str = "INVALID";
       lng_str = "INVALID";
      if (gps.location.isValid())
      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
      }

      
      if (gps.date.isValid())
      {
        date_str = "";
        date = gps.date.day();
        month = gps.date.month();
        year = gps.date.year();

        if (date < 10)
          date_str = '0';
        date_str += String(date);

        date_str += " / ";

        if (month < 10)
          date_str += '0';
        date_str += String(month);

        date_str += " / ";

        if (year < 10)
          date_str += '0';
        date_str += String(year);
      }
     
      if (gps.time.isValid())
      {
        time_str = "";
        hour = gps.time.hour();
        minute = gps.time.minute();
        second = gps.time.second();

        minute = (minute + 30);
        if (minute > 59)
        {
          minute = minute - 60;
          hour = hour + 1;
        }
        hour = (hour + 5) ;
        if (hour > 23)
          hour = hour - 24;

        if (hour >= 12)
          pm = 1;
        else
          pm = 0;

        hour = hour % 12;

        if (hour < 10)
          time_str = '0';
        time_str += String(hour);

        time_str += " : ";

        if (minute < 10)
          time_str += '0';
        time_str += String(minute);

        time_str += " : ";

        if (second < 10)
          time_str += '0';
        time_str += String(second);

        if (pm == 1)
          time_str += " PM ";
        else
          time_str += " AM ";

      }

    }
  // Check if a client has connected
  WiFiClient Sclient = server.available();
  if (!Sclient)
  {
    return;
  }

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n <!DOCTYPE html> <html> <head> <title>Covid-19 Curfew Violation Detector</title> <style>";
  s += "a:link {background-color: YELLOW;text-decoration: none;}";
  s += "table, th, td {border: 1px solid black;} </style> </head> <body> <h1  style=";
  s += "font-size:300%;";
  s += " ALIGN=CENTER> GPS Interfacing with NodeMCU</h1>";
  s += "<p ALIGN=CENTER style=""font-size:150%;""";
  s += "> <b>Location Details</b></p> <table ALIGN=CENTER style=";
  s += "width:50%";
  s += "> <tr> <th>Latitude</th>";
  s += "<td ALIGN=CENTER >";
  s += lat_str;
  s += "</td> </tr> <tr> <th>Longitude</th> <td ALIGN=CENTER >";
  s += lng_str;
  s += "</td> </tr> <tr>  <th>Date</th> <td ALIGN=CENTER >";
  s += date_str;
  s += "</td></tr> <tr> <th>Time</th> <td ALIGN=CENTER >";
  s += time_str;
  s += "</td></tr> <tr> <th>North West Coordinates</th> <td ALIGN=CENTER >";
  s += NWLAT+","+NWLOG;
  s += "</td></tr> <tr> <th>North East Coordinates</th> <td ALIGN=CENTER >";
  s += NELAT+","+NELOG;
  s += "</td></tr> <tr> <th>South West Coordinates</th> <td ALIGN=CENTER >";
  s += SWLAT+","+SWLOG;
  s += "</td></tr> <tr> <th>South East Coordinates</th> <td ALIGN=CENTER >";
  s += SELAT+","+SELOG;
  s += "</td></tr> <tr> <th>Curfew Violation Status</th> <td ALIGN=CENTER >";
  s += curfewStatus;
  s += "</td>  </tr> </table> ";
 
  
  if (gps.location.isValid())
  {
     s += "<p align=center><a style=""color:RED;font-size:125%;"" href=""http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=";
    s += lat_str;
    s += "+";
    s += lng_str;
    s += """ target=""_top"">Click here!</a> To check the location in Google maps.</p>";
  }

  s += "</body> </html> \n";

  Sclient.print(s);
  delay(50);
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("CurfewProject");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
String geoDomainTest(){
    if( lat_str <= NWLAT and lat_str >= SWLAT and lng_str >= NWLOG and lng_str <= SELOG ) { 
        return "Within Curfew Domain";
        delay(500);
      }
    else{
        return "WARNING!! OUTSIDE CURFEW DOMAIN!!";
        delay(500); 
      }
   Serial.println();
      }
