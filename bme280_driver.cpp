// Driver for retrieving calibrated bme280 data from the calibrate and sensor data
//
// Compile for
// - Linux Native (Intel)
// - [Cross] Embedded Linux - V6 / V7 - Raspberry PI) platforms.
//
// Created:  28/11/2019, Ward Dewerchin
// Modified:

// Setting for cross compile - prefix: arm-linux-gnueabihf- & -std=c++17 -> runtime not available on RPi2 (is on RPi3)
//                                                            -std=c++11 & /home/wd/RPiKernel/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin
//
// To include cpp and h file from another location or project (e.g. BME280 - shared items), go to: properties->new->folders->advanced->link to alternate location (linked folders)
//


#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

#include <string.h>

#include "../BME280/BME280.h"
#include "../Tools/Tools.h"

#define DATALENGTH 9
#define CALLENGTH 96

int main (int argc, char **argv)
{
   double T, P, H;
   int    R;
   unsigned char spDataBuffer[DATALENGTH];
   unsigned char spCalBuffer [CALLENGTH];
   fstream ifBme;

   try
   {
      if (argc != 3) throw ("Usage: command cal data");

      memset(spDataBuffer, 0x00, DATALENGTH);
      memset(spCalBuffer,  0x00, CALLENGTH);

      // Read calibration data
      ifBme.open(argv[1], ios::in|ios::binary);
      if (ifBme.fail()) throw ("Error reading calibration data");
      ifBme.read((char *) spCalBuffer, CALLENGTH);
      ifBme.close();

      // Read sensor data
      ifBme.open(argv[2], ios::in|ios::binary);
      if (ifBme.fail()) throw ("Error reading sensor data");
      ifBme.read((char *) spDataBuffer, DATALENGTH);
      ifBme.close();

      cout << "Calibration Data:" << endl;
      HexDump(spCalBuffer, CALLENGTH);
      cout << "Sensor Data:" << endl;
      HexDump(spDataBuffer, DATALENGTH);

      // Convert raw data to calibrated T, P, H
      bme280((unsigned char *) spDataBuffer, (unsigned char *) spCalBuffer, T, P, H);
      // show 2 decimals
      cout << showpoint;
      cout.precision(int (log10(T) + 1) + 2); cout << "T(C):   " << T << endl;
      cout.precision(int (log10(P) + 1) + 2); cout << "P(hPa): " << P << endl;
      cout.precision(int (log10(H) + 1) + 2); cout << "H(%):   " << H << endl;

      R = (int8_t &) *(spDataBuffer + 8);
      if (R != 0) { cout.precision(2);        cout << "dBm:    " << R << endl; }

      cout << noshowpoint;
   }
   catch (const char * spMessage)
   {
      cout << spMessage << endl;
   }
   catch (...)
   {
      cout << "Undefined Error" << endl;
   }
   return (0);
}
