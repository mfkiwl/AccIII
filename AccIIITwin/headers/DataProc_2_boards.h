#pragma once
//------------------------------------------------------------------------------
#include<string>
#define ExpFs 1298 // Expected sampling frequency
#define AccBusNum 23 // Totally 23 buses each connecting two accelerometers
#define DataByteNum 24 //Each bus connects two accelerometer each with 3 axies [High + Low] bytes

#include <queue>  
#include <fstream> 
#include <iostream>
#include <iomanip>
#include "ftd2xx.h"

#pragma comment (lib, "ftd2xx.lib")

bool SetupConnect(FT_HANDLE ftHandle, UCHAR LatencyTimer, LPVOID lpBuffer, DWORD dwBytesToWrite, LPDWORD lpBytesWritten);

void USBReadData(FT_HANDLE ftHandle, DWORD readBytes, long* dwSum, int dataNum, unsigned char fileBuffer[]);

void SaveDataResult(long dwSum, unsigned char fileBuffer[], std::string fileName);

void SaveNum(float inputValue, std::string fileName);

void SaveTickTime(std::queue<float> inputStack, float tickFreq, std::string fileName);