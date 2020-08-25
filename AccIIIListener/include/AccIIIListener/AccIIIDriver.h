/*
 * AccIIIDriver.h
 *
 *  Created on: Aug 16, 2020
 *      Author: Basil Duvernoy
 */

#ifndef ACCIIIDRIVER_H_
#define ACCIIIDRIVER_H_

#include <algorithm>    // std::copy
#include <errno.h>
#include <iostream>
#include <limits>
#include <limits>       // std::numeric_limits
#include <queue>        // std::queue
#include <stdio.h>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef linux
#include <pthread.h>
#endif

#include "AccIIIDriver_defines.h"
#include "../../libs/ftd2xx.h"

typedef unsigned char Byte;
typedef std::vector< std::vector<int16_t> > vector2D_int;
typedef std::vector< vector2D_int > vector3D_int;

class AccIIIDriver {
private:

    // communication variables
    FT_HANDLE ftHandle; // container for FT_open object
    FT_STATUS ftStatus; // status of ftHandle after calling functions

    // required variables for FT_read()
    DWORD EventDWord;
    DWORD TxBytes;
    DWORD RxBytes;
    DWORD BytesReceived;

    // Short-term buffer for FT_read()
    Byte* RxBuffer;
    int RxBuffer_length;
    // Infinite resizable buffer of RxBuffer
    std::deque< Byte > receivedBytes;
    // Organized data
    vector3D_int accData;

    /**
     * @brief main read called by other functions
     * @return the value of RxBytes (number of bytes received)
     */
    long read_raw();

    /**
     * @brief store RxBuffer values into receivedBytes queue
     */
    bool storeRxBuffer();

    /**
     * @brief clear receivedBytes
     */
    bool initReceivedBytes();

    /**
     * @brief decode byteQueue by frame, sensors and axis in a 3D vector
     * @param byteQueue The queue of byte that has to be converted
     * @return the 3-D vector with X,Y,Z values of each sensor for each frame
     */
    vector3D_int decode(std::deque<Byte> byteQueue);

    /**
     * @brief decode byteQueue for each axis of each accelerometer for one frame
     * based on the manner FTD2XX device communication protocol.
     * @param dataFrame Pointer to a 2-D vector to get the results
     * @param byteQueue Queue of the size of a frame
     */
    bool decode_once(vector2D_int* dataFrame, std::deque<Byte> byteQueue_frame);

    bool pop();
    bool pop_once(int offset = 0);

    /**
     * @brief store decoded ReceivedBytes values into acc_values queue
     */
    bool storeDecodedBytes();


protected:
    /**------ Protected for Unit Test (Mock class) -------**/

    /**------ ReceivedBytes Modifiers --------------------**/
    void addtoReceivedBytes(Byte* bp, long length = -1);
    void addtoReceivedBytes(Byte b);
    void setReceivedBytes(std::deque<Byte> ByteQueue);

    /**------ AccData Modifiers --------------------------**/
    void addtoAccData(std::deque<Byte> ByteQueue);

    /**------ Type Converters ----------------------------**/
    int16_t uint16toint16(uint16_t i);
    uint16_t bytes2uint16(Byte h, Byte l);
    uint8_t byte2uint8(Byte b);
    Byte uint2byte(uint8_t i);


public:

	AccIIIDriver();
	virtual ~AccIIIDriver();

    /**
     * @brief Opening communication with the ftd2xx device.
     * @param Mask Mask for FT_SetBitMode function. Default is 0xFF (output mode)
     * @param Mode Mode for FT_SetBitMode function. Default is 0x00 (reset mode)
     * @param LatencyTimer Latency in millisecond for FT_SetLatencyTimer function. Default is 2 (ms)
     * @param TxBuffer Input value for FT_Write function. Needed to test one time to write into the device. Default value is 0x55
     */
    bool ft_open(UCHAR Mask = 0xff, UCHAR Mode = 0x00, UCHAR LatencyTimer = 2, const char TxBuffer = 0x55 );

    /**
     * @brief close communication with the ftd2xx device.
     */
    bool ft_close();

    /**
     * @brief read until receiving a stop signal.
     */
    bool read_infinite();

    /**
     * @brief close communication with the ftd2xx device.
     * @param time_limit value in millisecond that the program reads the ftd2xx device.
     */
    bool read_for(int time_limit);

    /**
     * @brief read the ftd2xx device once.
     */
    bool read_once();

    /**------ PUBLIC --- GETTERS -------------------------**/
    std::deque<Byte> getReceivedBytes();
    vector3D_int getAccData();
};

#endif /* ACCIIIDRIVER_H_ */
