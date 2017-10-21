#include <EtherCard.h>  // Подключаем скачанную библиотеку. https://yadi.sk/d/R57sVoglbhTRN
#include <classLamp.h>
 
// MAC Address должен быть уникальным в вашей сети. Можно менять. 
static byte mymac[] = { 
  0x5A,0x5A,0x5A,0x5A,0x5A,0x5A };
 
// ip статический / постоянный Address нашей Web страницы.  
static byte myip[] = { 
  192,168,1,177 };
 
// Буфер, чем больше данных на Web странице, тем больше понадобится значения буфера.
byte Ethernet::buffer[900];
BufferFiller bfill;

const char http_OK[] PROGMEM =
"HTTP/1.0 200 OK\r\n"
"Content-Type: text/html\r\n"
"Pragma: no-cache\r\n\r\n";
 
const char http_Found[] PROGMEM =
"HTTP/1.0 302 Found\r\n"
"Location: /\r\n\r\n";
 
const char http_Unauthorized[] PROGMEM =
"HTTP/1.0 401 Unauthorized\r\n"
"Content-Type: text/html\r\n\r\n"
"<h1>401 Unauthorized</h1>";

Lamp lampA(2,A2);
Lamp lampB(3,A3);

//byte lampAStatus = LOW;
//byte lampBStatus = LOW;

void homePage()
{
  bfill.emit_p(PSTR("$F"),http_OK);
  bfill.emit_p(PSTR(
    "<link rel=\"shortcut icon\" type=\"image/x-icon\" href=\"http://arduino.cc/en/favicon.png\" />"
    "<head><meta charset=UTF-8></head>\n"
    "<title>ArduinoPIN Webserver</title>\n" 
    "<table>\n"
      "<tbody>\n"
        "<tr>\n"
          "<td>Выключатель 1</td>\n"
          "<td>Выключатель 2</td>\n"
        "</tr>\n"
        "<tr>\n"
          "<td><a href=\"?lampa=$F\"><img src=\"https://www.iconexperience.com/_img/v_collection_png/512x512/shadow/$F.png\" alt=\"$F\" width=\"100\" height=\"100\" /></td>\n"
          "<td><a href=\"?lampb=$F\"><img src=\"https://www.iconexperience.com/_img/v_collection_png/512x512/shadow/$F.png\" alt=\"$F\" width=\"100\" height=\"100\" /></td>\n"
          "</tr>\n"
      "</tbody>\n"
    "</table>\n"),
          lampA.getLampStatus() ? PSTR("off") : PSTR("on"),
          lampA.getLampStatus() ? PSTR("lightbulb_on") : PSTR("lightbulb"),
          lampA.getLampStatus() ? PSTR("ON") : PSTR("OFF"),
          lampB.getLampStatus() ? PSTR("off") : PSTR("on"),
          lampB.getLampStatus() ? PSTR("lightbulb_on") : PSTR("lightbulb"),
          lampB.getLampStatus() ? PSTR("ON") : PSTR("OFF")
  );
}

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  //Serial.println(F("Start setup"));
  // По умолчанию в Библиотеке "ethercard" (CS-pin) = № 8.
  // if (ether.begin(sizeof Ethernet::buffer, mymac) == 0).
  // and change it to: Меняем (CS-pin) на 10.
  // if (ether.begin(sizeof Ethernet::buffer, mymac, 10) == 0).
 
  if (ether.begin(sizeof Ethernet::buffer, mymac,10) == 0);
 
  if (!ether.dhcpSetup()); 
 
  // Выводим в Serial монитор IP адрес который нам автоматический присвоил наш Router. 
  // Динамический IP адрес, это не удобно, периодический наш IP адрес будет меняться. 
  // Нам придётся каждый раз узнавать кой адрес у нашей страницы.
  ether.printIp("My Router IP: ", ether.myip); // Выводим в Serial монитор IP адрес который нам присвоил Router. 
 
  // Здесь мы подменяем наш динамический IP на статический / постоянный IP Address нашей Web страницы.
  // Теперь не важно какой IP адрес присвоит нам Router, автоматический будем менять его, например на "192.168.1.222". 
  ether.staticSetup(myip);
  //Serial.println(F("Finish setup"));
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1); // Дёргаем микроконтроллер.
 
  word len = ether.packetReceive();   // check for ethernet packet / проверить ethernet пакеты.
  word pos = ether.packetLoop(len);   // check for tcp packet / проверить TCP пакеты.
 
  if (pos) {
    bfill = ether.tcpOffset();
    char *data = (char *) Ethernet::buffer + pos;
Serial.println(data);    
    if (strncmp("GET /", data, 5) != 0) {
      bfill.emit_p(http_Unauthorized);
    }
    else {
 
      data += 5;
      if (data[0] == ' ') {       
        homePage(); // Return home page Если обнаружено изменения на станице, запускаем функцию.
      } else if (strncmp("?lampa=on ", data, 10) == 0) {
        lampA.setLampStatus(HIGH);
        //Serial.println(F("Lamp A turned ON"));
        bfill.emit_p(http_Found);
      }  else if (strncmp("?lampa=off ", data, 11) == 0) {
        lampA.setLampStatus(LOW);
        //Serial.println(F("Lamp A turned OFF"));
        bfill.emit_p(http_Found);
      } else if (strncmp("?lampb=on ", data, 10) == 0) {
        lampB.setLampStatus(HIGH);
        //Serial.println(F("Lamp B turned ON"));
        bfill.emit_p(http_Found);
      } else if (strncmp("?lampb=off ", data, 11) == 0) {
        lampB.setLampStatus(LOW);
        //Serial.println(F("Lamp B turned OFF"));
        bfill.emit_p(http_Found);
      } else if (strncmp("?lampa=status ", data, 14) == 0) {
        bfill.emit_p(PSTR("LAMP_A_IS_$F\n"),
          lampA.getLampStatus() ? PSTR("ON"):PSTR("OFF")
          );
        //Serial.println(F("Lamp A status"));
        bfill.emit_p(http_Found);
      } else if (strncmp("?lampb=status ", data, 14) == 0) {
        bfill.emit_p(PSTR("LAMP_B_IS_$F\n"),
          lampB.getLampStatus() ? PSTR("ON"):PSTR("OFF")
          );
        //Serial.println(F("Lamp B status"));
        bfill.emit_p(http_Found);
      }  else if (strncmp("?lampa=amperage ", data, 16) == 0) {
        bfill.emit_p(PSTR("LAMP_A_AMPERAGE="));
        static char outstr[15];
        dtostrf(lampA.getLampAmperage(),7, 3, outstr);
        bfill.emit_raw(outstr,7);
        bfill.emit_p(PSTR("\n"));
        //Serial.println(F("Lamp A amperage"));
        bfill.emit_p(http_Found);
      } else if (strncmp("?lampb=amperage ", data, 16) == 0) {
        bfill.emit_p(PSTR("LAMP_B_AMPERAGE="));
        static char outstr[15];
        bfill.emit_raw(outstr,7);
        bfill.emit_p(PSTR("\n"));
        //Serial.println(F("Lamp B amperage"));
        dtostrf(lampB.getLampAmperage(),7, 3, outstr);
        bfill.emit_p(http_Found);
      } else {
        // Page not found
        bfill.emit_p(http_Unauthorized);
      }
    }
    ether.httpServerReply(bfill.position());    // send http response
  }
}
