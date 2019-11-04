#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h> 

int led = 4;
Servo panServo; 
Servo tiltServo;
int panPos = 0; 
int tiltPos=0;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };   //physical mac address
byte ip[] = { 192, 168, 1, 178 };                      // ip in lan (that's what you need to use in your browser. ("192.168.1.178")
byte gateway[] = { 192, 168, 1, 1 };                   // internet access via router
byte subnet[] = { 255, 255, 255, 0 };                  //subnet mask
EthernetServer server(80);                             //server port     
String readString;

void setup() {
  
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  pinMode(led, OUTPUT);
  panServo.attach(9);
  tiltServo.attach(8);
}


void loop() {
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {   
      if (client.available()) {
        char c = client.read();
     
        //read char by char HTTP request
        if (readString.length() < 100) {
          //store characters to string
          readString += c;
          //Serial.print(c);
         }

         //if HTTP request has ended
         if (c == '\n') {          
           Serial.println(readString); //print to serial monitor for debuging
     
           client.println("HTTP/1.1 200 OK"); //send new page
           client.println("Content-Type: text/html");
           client.println();     
           client.println("<HTML>");
           client.println("<HEAD>");
           client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
           client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
           client.println("<link rel='stylesheet' type='text/css' href='https://github.com/Vasu-Baldwa/ArduinoInternetGimble/blob/master/GremlinsGimble.css' />");
           client.println("<TITLE>Dixie's Camera</TITLE>");
           client.println("</HEAD>");
           client.println("<BODY>");
           client.println("<H1>Control the Gremlin's Gimble</H1>");
           client.println("<hr />");
           client.println("<br />");  
           client.println("<H2>Fall 2019: A Christmas Carol</H2>");
           client.println("<br />");  
           client.println("<a href=\"/?button1on\"\">Rotate Left</a>");
           client.println("<a href=\"/?button1off\"\">Rotate Right</a><br />");   
           client.println("<br />");     
           client.println("<br />"); 
           client.println("<a href=\"/?button2on\"\">Pan Left</a>");
           client.println("<a href=\"/?button2off\"\">Pan Right</a><br />"); 
           client.println("<br />"); 
           client.println("</BODY>");
           client.println("</HTML>");
     
           delay(1);
           //stopping client
           client.stop();
           //controls the Arduino if you press the buttons
           if (readString.indexOf("?button1on") >0){
               for(tiltPos = 0; tiltPos < 180; tiltPos += 3)  // goes from 0 degrees to 180 degrees 
                {                                  // in steps of 1 degree 
                  tiltServo.write(tiltPos);              // tell servo to go to position in variable 'pos' 
                  delay(15);                       // waits 15ms for the servo to reach the position 
                } 
           }
           if (readString.indexOf("?button1off") >0){
               for(tiltPos = 0; tiltPos < 180; tiltPos += 3)  // goes from 0 degrees to 180 degrees 
                {                                  // in steps of 1 degree 
                  tiltServo.write(tiltPos);              // tell servo to go to position in variable 'pos' 
                  delay(15);                       // waits 15ms for the servo to reach the position 
                } 
           }
           if (readString.indexOf("?button2on") >0){
                for(panPos = 0; panPos < 180; panPos += 3)  // goes from 0 degrees to 180 degrees 
                {                                  // in steps of 1 degree 
                  panServo.write(panPos);              // tell servo to go to position in variable 'pos' 
                  delay(15);                       // waits 15ms for the servo to reach the position 
                } 
           }
           if (readString.indexOf("?button2off") >0){
                for(panPos = 180; panPos>=1; panPos-=3)     // goes from 180 degrees to 0 degrees 
                {                                
                  panServo.write(panPos);              // tell servo to go to position in variable 'pos' 
                  delay(15);                       // waits 15ms for the servo to reach the position 
                } 
           }
            //clearing string for next read
            readString="";  
           
         }
       }
    }
}
}
