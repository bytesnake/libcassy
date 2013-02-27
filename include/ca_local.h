#ifndef CA_LOCAL_HEADER
#define CA_LOCAL_HEADER

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <time.h>
#include <math.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

// local rewrites for readability
#define node_t ca_node_t
#define version_t ca_version_t
#define error_t ca_error_t
#define range_t ca_range_t
#define cassy_t ca_cassy_t

#define mmms_t ca_mmms_t
#define mpos_t ca_mpos_t
#define ostatus_t ca_ostatus_t
#define otime_t ca_otime_t
#define ostatus2_t ca_ostatus2_t
#define otime2_t ca_otime2_t
#define oarray_t ca_oarray_t

#define BLOCKSIZEV1 8
#define BLOCKSIZEV2 64
#define NEXTBLOCK( x, blocksize ) ( ( x + blocksize - 1 ) & ~( blocksize - 1 ) )
#define GETBLOCKSIZE( version ) ( version == VERSION_SENSORCASSY2 ? BLOCKSIZEV2 : BLOCKSIZEV1 )

#define USB_NORESPONSE 0
#define USB_TIMEOUT 100

// error checking macros
#define CHECKCALL( call, error ) if ( ( call ) != 0 ) CA_SetLastError( error )
#define CHECKCALLR( call, error ) if ( ( call ) != 0 ) { CA_SetLastError( error ); return; }
#define CHECKCALLRV( call, error, retval ) if ( ( call ) != 0 ) { CA_SetLastError( error ); return retval; }

//
// Public data structures
//

typedef int ca_node_t;

typedef enum
{
    VERSION_UNKNOWN = 0,
    VERSION_SENSORCASSY = 1,
    VERSION_POWERCASSY = 2,
    VERSION_SENSORCASSY2 = 3,
    VERSION_CASSYDISPLAY = 11,
    VERSION_MOBILECASSY = 12,
    VERSION_JOULEWATTMETER = 32,
    VERSION_UMIP = 33,
    VERSION_UMIC = 34,
    VERSION_UMIB = 35,
} ca_version_t;

typedef enum
{
    ERROR_SUCCESS,
    ERROR_CASSY,
    ERROR_CASSY_OVERFLOW,
    ERROR_CASSY_UNDERFLOW,
    ERROR_DEV_OPEN,
    ERROR_DEV_CLOSE,
    ERROR_DEV_WRITE,
    ERROR_DEV_READ,
    
    ERROR_UNKNOWN,
} ca_error_t;

typedef enum
{
    RANGE_250V = 137,
    RANGE_100V = 136,
    RANGE_30V = 168,
    RANGE_10V = 200,
    RANGE_3V = 232,
    RANGE_1V = 192,
    RANGE_03V = 224,
    RANGE_01V = 143,
    RANGE_3A = 144,
    RANGE_1A = 176,
    RANGE_03A = 208,
    RANGE_01A = 240,
    RANGE_003A = 151,
    
    RANGE_BOX_1V = 8,
    RANGE_BOX_03V = 40,
    RANGE_BOX_01V = 72,
    RANGE_BOX_003V = 104,
    RANGE_BOX_001V = 64,
    RANGE_BOX_0003V = 96,
    
    RANGE_POWER_10V = 72,
    RANGE_POWER_3V = 104,
    RANGE_POWER_1V = 64,
    RANGE_POWER_1A = 176,
    RANGE_POWER_03A = 208,
    RANGE_POWER_01A = 240,
    
    RANGE_UNCHANGED = -1 // not implemented
} ca_range_t;

typedef struct
{
    uint8_t cassyid;
    ca_version_t version;
    ca_node_t node;
} ca_cassy_t;

typedef struct
{
    float min;
    float max;
    float mean;
    float square;
} ca_mmms_t;

typedef struct
{
    int16_t xpos;
    int16_t ypos;
} ca_mpos_t;

typedef struct
{
    uint8_t status;
    uint16_t counts;
} ca_ostatus_t;

typedef struct
{
    uint8_t status;
    uint32_t time;
} ca_otime_t;

typedef struct
{
    uint8_t status;
    uint32_t uia;
    uint32_t uib;
    uint32_t sensora;
    uint32_t sensorb;
} ca_ostatus2_t;

typedef struct
{
    uint8_t status;
    uint32_t timehigh;
    uint32_t timelow;
    uint16_t delay;
} ca_otime2_t;

typedef struct
{
	uint8_t status;
	size_t length;
	float *values;
} ca_oarray_t;

//
// Local only data structures
//

typedef struct
{
    size_t length;
    uint8_t *data;
} data_t;

typedef struct
{
	size_t length;
	int16_t *data;
} stream_t;

//
// ca_data.c
//

uint16_t CA_SwitchShort( uint16_t s );
uint32_t CA_SwitchInt( uint32_t i );

int8_t CA_SignExtendByte( uint8_t digits, uint8_t bits );
int16_t CA_SignExtendShort( uint16_t digits, uint8_t bits );
int32_t CA_SignExtendInt( uint32_t digits, uint8_t bits );

float CA_ConvertToRange( int32_t value, range_t range );
float CA_ExtendToRange( uint32_t digits, uint8_t bits, range_t range );

data_t CA_AllocateData( size_t length );
void CA_ReallocateData( data_t *data, size_t length );
void CA_FreeData( data_t *data );

stream_t CA_AllocateStream( size_t length );
void CA_ReallocateStream( stream_t *stream, size_t length );
void CA_FreeStream( stream_t *stream );

bool CA_AddSerialData( data_t *oserial, data_t pdata, size_t blocksize );
bool CA_AddToStream( stream_t *stream, data_t *data, ptrdiff_t *sp, ptrdiff_t *dp );

data_t CA_ConstructPacket( uint8_t cassyid, data_t data, size_t blocksize );

//
// ca_dev.c
//

node_t CA_OpenCassyNode( const char *path );
void CA_CloseCassyNode( node_t node );

cassy_t CA_OpenCassy( node_t node, uint8_t cassyid );

stream_t CA_ReceiveStream( cassy_t cassy, int offset );
data_t CA_ExecuteCommand( cassy_t cassy, data_t data, size_t rsize, uint32_t timeout );

//
// ca_functions.c
//

#include "ca_functionids.h"
#include "ca_functions.h"

//
// ca_main.c
//

void CA_SetLastError( error_t error );
void CA_ResetError();

error_t CA_GetLastError();
const char *CA_ErrorToString( error_t error );

void __attribute__ ((constructor)) CA_Attach();

#endif
