/* подключение лоры
 * 3.3v - 3.3v
 * GND - GND
 * SCK - PA5 
 * MISO - PA6
 * MOSI - PA7
 * NSS - PA4
 * NRESET - PC13
 * DIO0 - PA1
 * DIO1 - PB13
 * DIO2 - PB12
 */
 
#include <SPI.h>
#include <LoRa.h>

#define DEBAG 0
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  LoRa.begin(433E6);
}


uint8_t chr_count = 0;//k123456s 
char buf[10];
int16_t packetSize;

void loop() { // не работает если ключ разделён на два покета 
  if (DEBAG) {
    packetSize = 1;
  } else {
    packetSize = LoRa.parsePacket();
  }

  if (packetSize) {
    bool ysl;

    do {

      if (DEBAG) {
        ysl = Serial.available();
      } else {
        ysl = LoRa.available();
      }

      if (chr_count == 8) {
        if (buf[0] == 'k' and buf[7] == 's') {
          bool is_dig = 1;

          for (uint8_t i = 1; i < 7; i++) {
            if (!(buf[i] - '0' >= 0 && buf[i] - '0' <= 9)) is_dig = 0;
          }

          if (is_dig) {
            if (DEBAG) {
              for (uint8_t i = 0; i < 8; i++) Serial.print(buf[i]);
              Serial.println();
            }

            uint16_t dig[2] = {0, 0};

            for (uint8_t dg = 1; dg < 7; dg++) {
              dig[(dg - 1) / 3] += (buf[dg] - '0') * pow (10, (2 - (dg - 1) % 3));

              if (DEBAG) {
                Serial.print(buf[dg] - '0');
                Serial.print("   ");
                Serial.print((dg - 1) / 3);
                Serial.print("   ");
                Serial.println(pow (10, (2 - (dg - 1) % 3)));
              }
            }

            chr_count = 0;

            Serial.print(dig[0цывц]);
            Serial.print("   ");
            Serial.println(dig[1]);
          } else {
            for (uint8_t i = 0; i < 7; i++) {
              buf[i] = buf[i + 1];
            }

            chr_count--;
          }
        } else {
          for (uint8_t i = 0; i < 7; i++) {
            buf[i] = buf[i + 1];
          }

          chr_count--;
        }
      } else {
        if (DEBAG) {
          buf[chr_count] = (char)Serial.read();
        } else {
          buf[chr_count] = (char)LoRa.read();
        }

        chr_count++;
        //Serial.println(chr_count);
      }
    }while (ysl);
    if (DEBAG) {
      for (uint8_t i = 0; i < 10; i++) {
        Serial.print(buf[i]);
        //buf[i] = 0;
      }
      Serial.println();
      delay(1000);
    }
  }
}



//loop для приёма в Serial если я правильно понял, то Lora.available() будет работать даже если RSSI покет пришёл не до конца. Если моё не верно, то лучше заменить Serial.available() и Serial.read() ниже на LoRa.available() и LoRa.read()
//данный код не останавливает выполнение основного и нормально работает если ключ был отправлен раздельно
/*void loop() {
  if (chr_count == 8) {
    if (buf[0] == 'k' and buf[7] == 's') {
      bool is_dig = 1;

      for (uint8_t i = 1; i < 7; i++) {
        if (!(buf[i] - '0' >= 0 && buf[i] - '0' <= 9)) is_dig = 0;
      }

      if (is_dig) {
        if (DEBAG) {
          for (uint8_t i = 0; i < 8; i++) Serial.print(buf[i]);
          Serial.println();
        }

        uint16_t dig[2] = {0, 0};

        for (uint8_t dg = 1; dg < 7; dg++) {
          dig[(dg - 1) / 3] += (buf[dg] - '0') * pow (10, (2 - (dg - 1) % 3));

          if (DEBAG) {
            Serial.print(buf[dg] - '0');
            Serial.print("   ");
            Serial.print((dg - 1) / 3);
            Serial.print("   ");
            Serial.println(pow (10, (2 - (dg - 1) % 3)));
          }
        }

        chr_count = 0;

        Serial.print(dig[0]);
        Serial.print("   ");
        Serial.println(dig[1]);
      } else {
        for (uint8_t i = 0; i < 7; i++) {
          buf[i] = buf[i + 1];
        }
        chr_count--;
      }
    } else {
      for (uint8_t i = 0; i < 7; i++) {
        buf[i] = buf[i + 1];
      }
      chr_count--;
    }
  } else {
    if (Serial.available()) {
      buf[chr_count] = (char)Serial.read();
      chr_count++;
    }
  }
}*/
