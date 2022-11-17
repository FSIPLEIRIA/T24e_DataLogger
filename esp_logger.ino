#include "logger.h"

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  CAN.setPins(22, 21);       //(rx,tx);
  if (!CAN.begin(1000E3)) {  //1 000 000
    printf("Starting CAN failed!\r\n");
    while (1)
      ;
  }
  printf("\n------------------------\n-------ESP_LOGGER-------\n------------------------\r\n");
                                  
  InitSD();  //SETUP SDCARD
}

void loop() {
  logSDCard();  //WRITE DATA 20HZ
  ReadCAN();    //READ CAN (LOOP SPEED)
  PRINT_CAN();  //PRINT CAN BUS VALUES IF TRUE
}