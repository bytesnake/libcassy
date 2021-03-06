#ifndef CA_CONSTANTS_H
#define CA_CONSTANTS_H

#include "libcassy.h"

//
// macros
//

#ifdef CA_LOCAL

#define CA_NEXTBLOCK( length, blocksize ) ((length / blocksize + 1) * blocksize )

#endif

//
// compiler flags
//

// #define CA_LOCAL
// #define CA_DEVNODES
// #define CA_LIBUSB

// #define CA_DEBUG_PRINT

//
// internal constants
//

#ifdef CA_LOCAL

#define CA_INOUT_NORESPONSE 0

#define CA_STREAM_CHUNKSIZE 256

#endif

//
// external constants
//

#define CA_OSTATUS_RUNNING 0
#define CA_OSTATUS_WAITING 1
#define CA_OSTATUS_STOPPED 2

#define CA_OMASK_4MMSOCKA 1
#define CA_OMASK_4MMSOCKB 2
#define CA_OMASK_SENSORA 4
#define CA_OMASK_SENSORB 8

#define CA_OTRIG_VOLTTOGGLE 128
#define CA_OTRIG_RELAYTOGGLE 64
#define CA_OTRIG_BUSTGEDGE 32

#define CA_OTRIG_INPUT4MM 16
#define CA_OTRIG_INPUTSENSOR 0

#define CA_OTRIG_INPUTEB 14
#define CA_OTRIG_INPUTEA 12
#define CA_OTRIG_THRESHB 10
#define CA_OTRIG_THRESHA 8
#define CA_OTRIG_TGEDGE 6
#define CA_OTRIG_INPUTFB 4
#define CA_OTRIG_INPUTFA 2
#define CA_OTRIG_IMMEDIATE 0

#define CA_OTRIG_RISING 1
#define CA_OTRIG_FALLING 0

#define CA_GSTATUS_RUNNING 0
#define CA_GSTATUS_STOPPED 1
#define CA_GSTATUS_INVALID 2

//
// function ids
//

#ifdef CA_LOCAL
#include "ca_functionids.h"
#endif

#endif
