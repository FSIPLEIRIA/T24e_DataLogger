#include "logger.h"

volatile float CANbusArray[100];  //Array de dados canbus
volatile byte DATA[100];
volatile int ID;
bool Serial_running = true;  //true or false
unsigned long nowTime, nowTime2;
String dataMessage;

/*
volatile uint16_t RPM,    TPS,        IAT,            MAP,     CLT,          //0x600
                  VSPD,   BARO,       OILT,           OILP,    FUELP, BATV,  //0x601
                  IGNANG, DWELL,      LAMBDA,         LAMCORR, EGT1,  EGT2,  //0x602
                  CBUSLd, ECUTEMP,    ETHANOLCONTENT,                        //0x603
                  DBWPOS, DBWTRGT,    BOOSTP,                                //0x604
                  SPOS1,  SPOS2,      WSFR,           WSFL,                  //0x700
                  SPOS3,  SPOS4,      WSRR,           WSRL,                  //0x701
                  Gear,   Gear_Value;                                        //0x607
*/

void ReadCAN() {  //READ CAN BUS DATA

  int packetSize = CAN.parsePacket();
  if (packetSize) {
    ID = CAN.packetId();
    for (int i = 0; i < 8; i++) {
      DATA[i] = CAN.read();
    }
    switch (ID) {
      case 0x600:
        CANbusArray[0] = (DATA[1] << 8) + DATA[0];  //RPM
        CANbusArray[1] = DATA[2];                   //TPS
        CANbusArray[2] = DATA[3];                   //IAT
        CANbusArray[3] = (DATA[5] << 8) + DATA[4];  //MAP
        CANbusArray[4] = (DATA[7] << 8) + DATA[6];  //CLT
        break;

      case 0x601:
        CANbusArray[5] = (DATA[1] << 8) + DATA[0];   //VSPD
        CANbusArray[6] = DATA[2];                    //BARO
        CANbusArray[7] = DATA[3];                    //OILT
        CANbusArray[8] = DATA[4];                    //OILP
        CANbusArray[9] = DATA[5];                    //FUELP
        CANbusArray[10] = (DATA[7] << 8) + DATA[6];  //BATV
        break;

      case 0x602:
        CANbusArray[11] = DATA[0];                   //IGNANG
        CANbusArray[12] = DATA[1];                   //DWELL
        CANbusArray[13] = DATA[2];                   //LAMBDA
        CANbusArray[14] = DATA[3];                   //LAMCORR
        CANbusArray[15] = (DATA[5] << 8) + DATA[4];  //EGT1
        CANbusArray[16] = (DATA[7] << 8) + DATA[6];  //EGT2
        break;

      case 0x603:
        CANbusArray[17] = DATA[0];  //CBUSLd
        CANbusArray[18] = DATA[1];  //ECUTEMP
        CANbusArray[19] = DATA[2];  //ETHANOLCONTENT
        break;

      case 0x604:
        CANbusArray[20] = DATA[0];                   //DBWPOS
        CANbusArray[21] = DATA[1];                   //DBWTRGT
        CANbusArray[22] = (DATA[3] << 8) + DATA[2];  //BOOSTP
        break;

      case 0x607:
        CANbusArray[23] = DATA[0];  //Gear
        CANbusArray[24] = DATA[1];  //Gear_Value
        break;

      case 0x700:
        CANbusArray[25] = DATA[0];  //SPOS1
        CANbusArray[26] = DATA[1];  //SPOS2
        CANbusArray[27] = DATA[2];  //WSFR
        CANbusArray[28] = DATA[3];  //WSFL
        break;

      case 0x701:
        CANbusArray[29] = DATA[0];  //SPOS3
        CANbusArray[30] = DATA[1];  //SPOS4
        CANbusArray[31] = DATA[2];  //WSRR
        CANbusArray[32] = DATA[3];  //WSRL
        break;
    }
  }
}
void InitSD() {  //SETUP SDCARD
  if (!SD_MMC.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD_MMC card attached");
    return;
  }

  File file = SD_MMC.open("/data.txt");
  if (!file) {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD_MMC, "/data.txt", "ola \r\n");
  } else {
    Serial.println("File already exists");
  }
  file.close();
}
void PRINT_CAN() {  //WRITE TO SERIAL MONITOR
  /*
  for (int i = 0, CANbusArray[i] =! 0, i++) {
    Serial.print("Canbus Array [");
    Serial.print(i);
    Serial.print("] ");
    Serial.println(CANbusArray[i]);
  }
  */
  if (Serial_running) {
    printf("\nID:600------ \n RPM:%f \n TPS:%f \n IAT:%f \n MAP:%f \n CLT:%f \n", CANbusArray[0], CANbusArray[1], CANbusArray[2], CANbusArray[3], CANbusArray[4]);
    printf("\nID:601------ \n VSPD:%i \n BARO:%i \n OILT:%i \n OILP:%i \n FUELP:%i \n BATV:%i \n", CANbusArray[5], CANbusArray[6], CANbusArray[7], CANbusArray[8], CANbusArray[9], CANbusArray[10]);
    printf("\nID:602------ \n IGNANG:%i \n DWELL:%i \n LAMBDA:%i \n LAMCORR:%i \n EGT1:%i \n EGT2:%i \n", CANbusArray[11], CANbusArray[12], CANbusArray[13], CANbusArray[14], CANbusArray[15], CANbusArray[16]);
    printf("\nID:603------ \n CBUSLd:%i \n ECUTEMP:%i \n ETHANOLCONTENT:%i \n", CANbusArray[17], CANbusArray[18], CANbusArray[19]);
    printf("\nID:604------ \n DBWPOS:%i \n DBWTRIGT:%i \n BOOSTP:%i \n", CANbusArray[20], CANbusArray[21], CANbusArray[22]);
    printf("\nID:607------ \n Gear:%i \n Gear_Value:%i \n ", CANbusArray[23], CANbusArray[24]);
    printf("\nID:700------ \n SPOS1:%i \n SPOS2:%i \n WSFR:%i \n WSFL:%i \n", CANbusArray[25], CANbusArray[26], CANbusArray[27], CANbusArray[28]);
    printf("\nID:701------ \n SPOS3:%i \n SPOS4:%i \n WSRR:%i \n WSRL:%i \n", CANbusArray[29], CANbusArray[30], CANbusArray[31], CANbusArray[32]);
  }
}
void appendFile(fs::FS &fs, const char *path, const char *message) {  //DON'T MODIFY THIS FUNCTION
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
}
void writeFile(fs::FS &fs, const char *path, const char *message) {  //DON'T MODIFY THIS FUNCTION
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
}
void logSDCard() {  //WRITE TO SD_CARD STRING

  nowTime = millis();
  if ((unsigned long)(millis() - nowTime > 5)) {  //WRITE DATA 200HZ
    for (int j = 1; j <= 32; j++) {
      dataMessage = dataMessage + "," + String(CANbusArray[j]);
    }
    dataMessage = dataMessage + "\r\n";
    //Serial.print("Save data: ");
    //Serial.println(dataMessage);
    appendFile(SD_MMC, "/data.txt", dataMessage.c_str());
    dataMessage = "";
  }
}