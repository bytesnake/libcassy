#ifndef CA_CONSTANTS_H
#define CA_CONSTANTS_H

#include "libcassy.h"

//
// macros
//

#define CA_NEXTBLOCK( length, blocksize ) ((length / blocksize + 1) * blocksize )
#define CA_GETBLOCKSIZE( version ) (version == CA_VERSION_SENSORCASSY2 ? CA_BLOCKSIZEV2 : CA_BLOCKSIZEV1)

//
// constants
//

#define CA_BLOCKSIZEV1 8
#define CA_BLOCKSIZEV2 64

#define CA_USB_ONEPACKET 0
#define CA_USB_NORESPONSE 0
#define CA_USB_DELAY 25

#define CA_STREAM_CHUNKSIZE 256

#define CA_OSTATUS_RUNNING 0
#define CA_OSTATUS_WAITING 1
#define CA_OSTATUS_STOPPED 2

#endif