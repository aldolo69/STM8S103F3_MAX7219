//STM8S103F3 board and MAX7219 led without external lib
//C only. no C++ required.
//seeduino core
//https://github.com/tenbaht/sduino/raw/master/package_sduino_stm8_index.json


// LED matrix modules with 64 LEDs
// per module.
// SPI interface is through the SPI
// library and so uses the native hardware
// interface pin for the target MCU.
#include "Arduino.h"
#include "SPI.h"

// Define the number of modules in the
// display and rows/columns per module
#define NUM_MODULES  1
#define MAX_ROWS 8

// Define the SS pin
const uint8_t pinSS = 12;

// Opcodes for the MAX7221 and MAX7219
const uint16_t OP_NOOP = 0;
const uint16_t OP_DIGIT0 = 1;
// note: all OP_DIGITn are +n offsets from OP_DIGIT0
const uint16_t OP_DIGIT1 = 2;
const uint16_t OP_DIGIT2 = 3;
const uint16_t OP_DIGIT3 = 4;
const uint16_t OP_DIGIT4 = 5;
const uint16_t OP_DIGIT5 = 6;
const uint16_t OP_DIGIT6 = 7;
const uint16_t OP_DIGIT7 = 8;
const uint16_t OP_DECODEMODE = 9;
const uint16_t OP_INTENSITY = 10;
const uint16_t OP_SCANLIMIT = 11;
const uint16_t OP_SHUTDOWN = 12;
const uint16_t OP_DISPLAYTEST = 15;


uint8_t moduleData[NUM_MODULES][MAX_ROWS];
#define SPI_PARAMETER SPISettings(500000, MSBFIRST, SPI_MODE0)

void sendCmd(uint16_t cmd, uint8_t data)
// Send a simple command to the MAX7219
// chain using the hardware SPI interface
{
  SPI_beginTransaction(SPI_PARAMETER);
  digitalWrite(pinSS, LOW);
  for (uint8_t i = 0; i < NUM_MODULES; i++)
  {
    SPI_transfer(cmd);
    SPI_transfer(data);
  }
  digitalWrite(pinSS, HIGH);
  SPI_endTransaction();
}

void sendData(void)
// Send display data the MAX7219 chain
// using the hardware SPI interface
{
  SPI_beginTransaction(SPI_PARAMETER);
  for (uint8_t j = 0; j < MAX_ROWS; j++)
  {
    digitalWrite(pinSS, LOW);
    SPI_transfer(OP_DIGIT0+j);
    for (uint8_t i = 0; i < NUM_MODULES; i++)
    {
      SPI_transfer(moduleData[i][j]);
    }
    digitalWrite(pinSS, HIGH);
  }
  SPI_endTransaction();
}


void setup(void)
{
  pinMode(pinSS, OUTPUT);
  SPI_begin();

  // Set the MAX7219 parameters
  sendCmd(OP_SHUTDOWN, 1);
  sendCmd(OP_DECODEMODE, 0);
  sendCmd(OP_INTENSITY, 7);
  sendCmd(OP_SCANLIMIT , MAX_ROWS-1);


  // turn on all LED for a short time
  sendCmd(OP_DISPLAYTEST, 1);
  delay(500);
  sendCmd(OP_DISPLAYTEST, 0);
  delay(500);
  sendCmd(OP_DISPLAYTEST, 1);
  delay(500);
  sendCmd(OP_DISPLAYTEST, 0);

moduleData[0][0]=1;
moduleData[0][1]=2;
moduleData[0][2]=4;
moduleData[0][3]=8;
moduleData[0][4]=1;
moduleData[0][5]=2;
moduleData[0][6]=4;
moduleData[0][7]=8;

 sendData();
}

void loop(void)
{
}
