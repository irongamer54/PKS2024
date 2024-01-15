#include <SPI.h>
#include <LoRa.h>


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //LoRa.begin(433E6);
}

bool pars_flag=1;
uint8_t chr_count=0;
void loop() {
  // put your main code here, to run repeatedly:
  //int packetSize = LoRa.parsePacket();
  //if (packetSize) {
  if (Serial.available()>=8 and pars_flag==0){
    pars_flag=1;
    chr_count=0;
  }
    
  if (pars_flag) {
      char c = (char)Serial.read();
      if (c == 'k') {
        char buf[9];
        buf[0]=c;
        for (uint8_t count=1; count<7; count++){
          
        }
      }
    }
  //}
  
}
