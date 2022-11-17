#ifndef LOGGER_H
#define LOGGER_H

#include <CAN.h>
#include <Arduino.h>
#include "FS.h"
#include "SD_MMC.h"


void ReadCAN();
void InitSD();
void PRINT_CAN();
void appendFile(fs::FS &fs, const char *path, const char *message);
void writeFile(fs::FS &fs, const char *path, const char *message);
void logSDCard();

#endif