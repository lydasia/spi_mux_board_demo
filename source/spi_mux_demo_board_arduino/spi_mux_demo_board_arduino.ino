#include <SPI.h>
#include <Ethernet.h>
#include <avr/wdt.h>

#define P0_EN   2
#define P0_IN   3
#define P1_EN   4
#define P1_IN   5
#define P2_EN   6
#define P2_IN   7
#define P3_EN   8
#define P3_IN   9
#define P4_EN   16
#define P4_IN   17
#define P5_EN   14
#define P5_IN   15
#define FLA_RST 18
#define MCU_LED 19

#define MUX_DISC  0
#define MUX_BOARD 1
#define MUX_PROG  2

#define CHIP_MODEL_0 "MT25QL512"
#define CHIP_MODEL_1 "MX25L512"
#define CHIP_MODEL_2 "MX66L1G"

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(172, 16, 4, 6);
String readString;
int muxStatus[3][2] = {{MUX_DISC, MUX_DISC}, {MUX_DISC, MUX_DISC}, {MUX_DISC, MUX_DISC}};
EthernetServer server(80);

// void setMux()
// {
//     for (int i = 0; i < 3; i++)
//     {
        
//         for (int j = 0; j < 2; j++)
//         {
//         }
//     }
// }

void setup() {
  wdt_enable(WDTO_8S);

  pinMode(P0_EN, OUTPUT);
  pinMode(P0_IN, OUTPUT);
  pinMode(P1_EN, OUTPUT);
  pinMode(P1_IN, OUTPUT);
  pinMode(P2_EN, OUTPUT);
  pinMode(P2_IN, OUTPUT);
  pinMode(P3_EN, OUTPUT);
  pinMode(P3_IN, OUTPUT);
  pinMode(P4_EN, OUTPUT);
  pinMode(P4_IN, OUTPUT);
  pinMode(P5_EN, OUTPUT);
  pinMode(P5_IN, OUTPUT);
  pinMode(FLA_RST, OUTPUT);
  pinMode(MCU_LED, OUTPUT);
    
  digitalWrite(P0_EN, HIGH);
  digitalWrite(P0_IN, HIGH);
  digitalWrite(P1_EN, HIGH);
  digitalWrite(P1_IN, HIGH);
  digitalWrite(P2_EN, HIGH);
  digitalWrite(P2_IN, HIGH);
  digitalWrite(P3_EN, HIGH);
  digitalWrite(P3_IN, HIGH);
  digitalWrite(P4_EN, HIGH);
  digitalWrite(P4_IN, HIGH);
  digitalWrite(P5_EN, HIGH);
  digitalWrite(P5_IN, HIGH);
  digitalWrite(FLA_RST, HIGH);
  digitalWrite(MCU_LED, HIGH);

  Ethernet.begin(mac, ip);
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {    
    while (true) {
      delay(1); 
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
  }
  server.begin();
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
          
          char c = client.read();
          if (readString.length() < 100){
              readString = readString + c;// Store characters to string
          }
          if (c == '\n' && currentLineIsBlank) {
          
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println("Refresh: 2");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<head>");
          client.println("<meta charset='utf-8'>");
          client.println("<title>Demo</title>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h3>SPI Multiplexer Board</h3>");
          client.println("<p>A Quad SPI NOR Flash Switcher</p>");
          client.println("<p>Supports up to Six QSPI Flashes</p><br>");
          client.println("<table border='5'>");
          client.println("<caption>Flash Array</caption>");
          for (int i = 0; i < 3; i++){
            client.println("<tr>");
            for (int j = 0; j < 2; j++){
              client.println("<td>");
              // <p>MT25QL512 #A</p>
              client.print("<p>");
              switch(i) {
                case 0:
                  client.print(CHIP_MODEL_0);
                  client.print(" #");
                  break;
                case 1:
                  client.print(CHIP_MODEL_1);
                  client.print(" #");
                  break;
                case 2:
                  client.print(CHIP_MODEL_2);
                  client.print(" #");
                  break;
              }
              switch(j) {
                case 0:
                  client.println("A</p>");
                  break;
                case 1:
                  client.println("B</p>");
                  break;
              }
              // <p>Status:  <font color="Grey">Disconnect</font></p>
              client.print("<p>Status:  <font color=");
              switch(muxStatus[i][j]) {
                case MUX_DISC:
                  client.println("'Grey'>Disconnect</font></p>");
                  break;
                case MUX_BOARD:
                  client.println("'Green'>Board</font></p>");
                  break;
                case MUX_PROG:
                  client.println("'Red'>Prog</font></p>");
                  break;
              }
              client.print("<input type=button value='Disc.' onmousedown=location.href='/?d");
              client.print(i);
              client.print(j);
              client.println("'>");
              client.print("<input type=button value='Board' onmousedown=location.href='/?b");
              client.print(i);
              client.print(j);
              client.println("'>");
              client.print("<input type=button value='Prog' onmousedown=location.href='/?p");
              client.print(i);
              client.print(j);
              client.println("'>");
              client.println("</td>");
            }
            client.println("</tr>");
          }
          client.println("</table>");
          client.println("<p></p>");
          client.println("<input type=button value='Disconnect All' onmousedown=location.href='/?da'>");
          client.println("<input type=button value='Hard Reset All' onmousedown=location.href='/?ra'>");
          client.println("<input type=button value='Toggle LED' onmousedown=location.href='/?tl'>");
          client.println("<br><br><p>©2022 Made by Lv, Yuedong in Nanjing.</p>");
          client.println("</body>");

          
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop(); 
    
    if(readString.indexOf("/?d00") > 0){
      digitalWrite(P0_EN, HIGH);
      digitalWrite(P0_IN, HIGH);
      muxStatus[0][0] = MUX_DISC;
    }
    if(readString.indexOf("/?d01") > 0){
      digitalWrite(P1_EN, HIGH);
      digitalWrite(P1_IN, HIGH);
      muxStatus[0][1] = MUX_DISC;
    }
    if(readString.indexOf("/?d10") > 0){
      digitalWrite(P2_EN, HIGH);
      digitalWrite(P2_IN, HIGH);
      muxStatus[1][0] = MUX_DISC;
    }
    if(readString.indexOf("/?d11") > 0){
      digitalWrite(P3_EN, HIGH);
      digitalWrite(P3_IN, HIGH);
      muxStatus[1][1] = MUX_DISC;
    }
    if(readString.indexOf("/?d20") > 0){
      digitalWrite(P4_EN, HIGH);
      digitalWrite(P4_IN, HIGH);
      muxStatus[2][0] = MUX_DISC;
    }
    if(readString.indexOf("/?d21") > 0){
      digitalWrite(P5_EN, HIGH);
      digitalWrite(P5_IN, HIGH);
      muxStatus[2][1] = MUX_DISC;
    }
    
    if(readString.indexOf("/?b00") > 0){
      digitalWrite(P0_EN, LOW);
      digitalWrite(P0_IN, HIGH);
      muxStatus[0][0] = MUX_BOARD;
    }
    if(readString.indexOf("/?b01") > 0){
      digitalWrite(P1_EN, LOW);
      digitalWrite(P1_IN, HIGH);
      muxStatus[0][1] = MUX_BOARD;
    }
    if(readString.indexOf("/?b10") > 0){
      digitalWrite(P2_EN, LOW);
      digitalWrite(P2_IN, HIGH);
      muxStatus[1][0] = MUX_BOARD;
    }
    if(readString.indexOf("/?b11") > 0){
      digitalWrite(P3_EN, LOW);
      digitalWrite(P3_IN, HIGH);
      muxStatus[1][1] = MUX_BOARD;
    }
    if(readString.indexOf("/?b20") > 0){
      digitalWrite(P4_EN, LOW);
      digitalWrite(P4_IN, HIGH);
      muxStatus[2][0] = MUX_BOARD;
    }
    if(readString.indexOf("/?b21") > 0){
      digitalWrite(P5_EN, LOW);
      digitalWrite(P5_IN, HIGH);
      muxStatus[2][1] = MUX_BOARD;
    }

    if(readString.indexOf("/?p00") > 0){
      digitalWrite(P0_EN, LOW);
      digitalWrite(P0_IN, LOW);
      muxStatus[0][0] = MUX_PROG;
    }
    if(readString.indexOf("/?p01") > 0){
      digitalWrite(P1_EN, LOW);
      digitalWrite(P1_IN, LOW);
      muxStatus[0][1] = MUX_PROG;
    }
    if(readString.indexOf("/?p10") > 0){
      digitalWrite(P2_EN, LOW);
      digitalWrite(P2_IN, LOW);
      muxStatus[1][0] = MUX_PROG;
    }
    if(readString.indexOf("/?p11") > 0){
      digitalWrite(P3_EN, LOW);
      digitalWrite(P3_IN, LOW);
      muxStatus[1][1] = MUX_PROG;
    }
    if(readString.indexOf("/?p20") > 0){
      digitalWrite(P4_EN, LOW);
      digitalWrite(P4_IN, LOW);
      muxStatus[2][0] = MUX_PROG;
    }
    if(readString.indexOf("/?p21") > 0){
      digitalWrite(P5_EN, LOW);
      digitalWrite(P5_IN, LOW);
      muxStatus[2][1] = MUX_PROG;
    }

    if(readString.indexOf("/?da") > 0){
      digitalWrite(P0_EN, HIGH);
      digitalWrite(P0_IN, HIGH);
      digitalWrite(P1_EN, HIGH);
      digitalWrite(P1_IN, HIGH);
      digitalWrite(P2_EN, HIGH);
      digitalWrite(P2_IN, HIGH);
      digitalWrite(P3_EN, HIGH);
      digitalWrite(P3_IN, HIGH);
      digitalWrite(P4_EN, HIGH);
      digitalWrite(P4_IN, HIGH);
      digitalWrite(P5_EN, HIGH);
      digitalWrite(P5_IN, HIGH);
      for (int i = 0; i < 3; i++)
      {
          for (int j = 0; j < 2; j++){
              muxStatus[i][j] = 0;
          }
      }
    }

    if(readString.indexOf("/?ra") > 0){
      digitalWrite(FLA_RST, LOW);
      delay(1000);
      digitalWrite(FLA_RST, HIGH);
    }

    if(readString.indexOf("/?tl") > 0){
      if(digitalRead(MCU_LED)) {
        digitalWrite(MCU_LED, LOW);
      }
      else {
        digitalWrite(MCU_LED, HIGH);
      }
    }

    readString = "";
  } 
  wdt_reset();
}
