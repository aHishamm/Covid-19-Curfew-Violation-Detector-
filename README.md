### Project Idea ###
* Detection of lockdown curfews, we can send the nodeMCU some location coordinates to define a domain, the gps module would continuously check the current location of the person, if the person goes out of that geolocation domain, he would be alerted that he’s violating a lockdown curfew.
* 4 location coordinates will be send to the NodeMCU over wifi, the nodeMCU will check the current location through the Neo 6M GPS module and compare it against the 4 location coordinates received through wifi, if the current location is outside the geolocation domain determined by those 4 location coordinates, then the NodeMCU will send an alert saying the user is outside the geolocation domain.
### Project Description ###
* The NodeMCU is connected to the Neo 6M GPS module, current GPS location coordinates are fed to the NodeMCU through SoftwareSerial (ports 4 and 5), the information retrieved is then displayed on a generated HTML page. additionally we send the NodeMCU 4 domain coordinates (NW,NE,SW,SE) to specify a domain, if the current location coordinates is outside that domain, a message is shown to indicate that the person is outside the curfew domain, otherwise it shows that the person is within the curfew domain. The 4 domain coordinates are sent over MQTT to the Arduino from any MQTT broker over TCP, the server is "broker.mqtt-dashboard.com" and we named the topic we used "CurfewProject". The nodeMCU listens to all the messages sent on that topic and checks the first character of the string. 
    * Axx.xxxxxx --> saved as north west latitude coordinate 
    * Bxx.xxxxxx --> saved as north west longitude coordinate 
    * Cxx.xxxxxx --> saved as north east latitude coordinate 
    * Dxx.xxxxxx --> saved as north east longitude coordinate 
    * Exx.xxxxxx --> saved as south west latitude coordinate 
    * Fxx.xxxxxx --> saved as south west longitude coordinate 
    * Gxx.xxxxxx --> saved as south east latitude coordinate 
    * Hxx.xxxxxx --> saved as south east longitude coordinate 
* Additionally, the 4 domain coordinates and the message is displayed on that generated HTML page, a button is shown where it redirects the user to Google Maps with the current location pinned on the map.
### Hardware Components ###
* NEO 6M
* Node MCU


<img src = "https://i.imgur.com/PkqkBzY.png" width="83" height="170">
<img src = "https://imgur.com/sQ3rlXH.png" width="212" height="349">
