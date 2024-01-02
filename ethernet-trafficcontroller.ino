#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; //physical mac address
byte ip[] = { 192, 168, 1, 150 }; // IP address in LAN 
byte gateway[] = { 192, 168, 1, 1 }; // internet access via router
byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
EthernetServer server(80); //server port

String readString;
int greenLed = 2;
int redLed = 3;

void setup(){

    pinMode(greenLed, OUTPUT); //pin selected to control
    pinMode(redLed, OUTPUT); //pin selected to control
  
    // Test LEDs
    digitalWrite(redLed, HIGH); // set pin high
    delay(500);
    digitalWrite(redLed, LOW); // set pin low
    delay(500);
    digitalWrite(greenLed, HIGH); // set pin high
    delay(500);
    digitalWrite(greenLed, LOW); // set pin low
    delay(500);

    Serial.begin(9600);
    //start Ethernet
    Ethernet.begin(mac, ip, gateway, subnet);
    server.begin();
    Serial.print("Server is at: ");
    Serial.print(Ethernet.localIP());
}

void loop(){
    // Create a client connection
    EthernetClient client = server.available();
    if (client) {
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.print(c);
                //read char by char HTTP request
                if (readString.length() < 100) {

                    //store characters to string
                    readString += c;
                }
              
                //if HTTP request has ended– 0x0D is Carriage Return \n ASCII
                if (c == 0x0D) {
                    client.println("HTTP/1.1 200 OK"); //send new page
                    client.println("Content-Type: text/html");
                    client.println();

                    client.println("<HTML>");
                    client.println("<HEAD>");
                    client.println("<TITLE> ARDUINO ETHERNET SHIELD</TITLE>"); // website title
                    client.println("</HEAD>");
                    client.println("<BODY>");
                    client.println("<style>");
                    client.println("body { text-align: center; }"); // Center-align the body content
                    client.println("h1 { color: green; }"); // Style for the H1 title
                    client.println("form { display: inline-block; }"); // center-align them
                    client.println("</style>");
                    client.println("<br>");
                    client.println("<H1 style=\"color:green;\">ARDUINO ETHERNET SHIELD: SIMPLE TRAFFIC LIGHT CONTROLLER</H1>");
                    client.println("<hr>");
                    client.println("<br>");
                    //style for buttons stop and go 
                    client.println("<style>");
                    client.println(".greenbutton {");
                    client.println("  background-color: white;");
                    client.println("  border: 2px solid green;");
                    client.println("  color: black;");
                    client.println("  border-radius: 8px;");
                    client.println("  width: 300px;");
                    client.println("  padding: 15px 32px;");
                    client.println("  text-align: center;");
                    client.println("  text-decoration: none;");
                    client.println("  display: inline-block;");
                    client.println("  font-size: 16px;");
                    client.println("  margin: 4px 2px;");
                    client.println("}");

                    client.println(".redbutton {");
                    client.println("  background-color: white;");
                    client.println("  border: 2px solid red;");
                    client.println("  color: black;");
                    client.println("  border-radius: 8px;");
                    client.println("  width: 300px;");
                    client.println("  padding: 15px 32px;");
                    client.println("  text-align: center;");
                    client.println("  text-decoration: none;");
                    client.println("  display: inline-block;");
                    client.println("  font-size: 16px;");
                    client.println("  margin: 4px 2px;");
                    client.println("}");

                    client.println("</style>");
                    //create go button
                    client.println("<H2><a href=\"/?LEDON\"><button class='greenbutton'>GO: ON</button></a><br></H2>");
                    client.println("<H2><a href=\"/?LEDOFF\"><button class='greenbutton'>GO: OFF</button></a><br></H2>");
                    //create stop button
                    client.println("<H2><a href=\"/?LED1ON\"><button class='redbutton'>STOP: ON</button></a><br></H2>");
                    client.println("<H2><a href=\"/?LED1OFF\"><button class='redbutton'>STOP: OFF</button></a><br></H2>");

                    client.println("</BODY>");
                    client.println("</HTML>");

                    delay(10);
                    //stopping client
                    client.stop();

                    // control arduino pins
                    if(readString.indexOf("?LEDON")>0){
                      digitalWrite(greenLed, HIGH);
                      Serial.println("GREEN LED: ON");
                    }

                    if(readString.indexOf("?LEDOFF")>0){
                      digitalWrite(greenLed, LOW);
                      Serial.println("GREEN LED: OFF");
                    }

                    if(readString.indexOf("?LED1ON")>0){
                      digitalWrite(redLed, HIGH);
                      Serial.println("RED LED: ON");
                    }

                    if(readString.indexOf("?LED1OFF")>0){
                      digitalWrite(redLed, LOW);
                      Serial.println("RED LED: OFF");
                    }
                  
                    //clearing string for next read
                    readString="";

                }
            }
        }
        Serial.println("");
    }
}