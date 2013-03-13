#ifndef LIBCASSY_H
#define LIBCASSY_H

//
// includes
//

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

#include "ca_constants.h"

//
// data types
//

typedef int ca_handle_t;

typedef enum
{
	CA_VERSION_SENSORCASSY = 1,
	CA_VERSION_POWERCASSY = 2,
	CA_VERSION_SENSORCASSY2 = 3,
	CA_VERSION_CASSYDISPLAY = 11,
	CA_VERSION_MOBILECASSY = 12,
	CA_VERSION_JOULEWATTMETER = 32,
	CA_VERSION_UMIP = 33,
	CA_VERSION_UMIC = 34,
	CA_VERSION_UMIB = 35,

	CA_VERSION_UNKNOWN = 0
} ca_version_t;

typedef enum
{
	CA_ERROR_SUCCESS,

	CA_ERROR_CASSY,
	CA_ERROR_CASSY_OVERFLOW,
	CA_ERROR_CASSY_UNDERFLOW,

	CA_ERROR_STREAM_INVALID,

	CA_ERROR_IO_OPEN,
	CA_ERROR_IO_CLOSE,
	CA_ERROR_IO_READ,
	CA_ERROR_IO_WRITE
} ca_error_t;

typedef enum
{
	CA_RANGE_250V = 137,
	CA_RANGE_100V = 136,
	CA_RANGE_30V = 168,
	CA_RANGE_10V = 200,
	CA_RANGE_3V = 232,
	CA_RANGE_1V = 192,
	CA_RANGE_03V = 224,
	CA_RANGE_01V = 143,

	CA_RANGE_3A = 144,
	CA_RANGE_1A = 176,
	CA_RANGE_03A = 208,
	CA_RANGE_01A = 240,
	CA_RANGE_003A = 151
} ca_range_t;

typedef enum
{
	CA_SCLASS_3BIT,
	CA_SCLASS_7BIT,
	CA_SCLASS_13BIT,
	CA_SCLASS_END,

	CA_SCLASS_UNKNOWN
} ca_sclass_t;

typedef struct
{
	int id;
	ca_version_t version;
	ca_handle_t handle;
} ca_cassy_t;

typedef struct
{
	int length;
	uint8_t *data;
} ca_data_t;

typedef struct
{
	int length;
	int offset;
	int16_t *data;
} ca_stream_t;

typedef struct
{
	uint8_t status;
	size_t length;
	float *values;
} ca_oarray_t;

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

//
// ca_misc.c
//

uint16_t CA_SwitchShort( uint16_t s );
uint32_t CA_SwitchInt( uint32_t i );

int8_t CA_SignExtendByte( uint8_t digits, int bits );
int16_t CA_SignExtendShort( uint16_t digits, int bits );
int32_t CA_SignExtendInt( uint32_t digits, int bits );

uint8_t CA_GetCassyAddress( int id );

float CA_ConvertToRange( int value, ca_range_t range );
float CA_ExtendToRange( uint32_t digits, int bits, ca_range_t range );

void CA_PrintData( ca_data_t data, char *desc );

//
// ca_error.c
//

void CA_SetLastError( ca_error_t error );
void CA_ResetError();

bool CA_IsCassyError( ca_data_t data );

ca_error_t CA_GetLastError();
const char *CA_ErrorToString( ca_error_t error );

//
// ca_data.c
//

ca_data_t CA_AllocateData( int length );
void CA_FreeData( ca_data_t *data );
void CA_ResizeData( ca_data_t *data, int length );
void CA_CopyData( ca_data_t dest, ca_data_t src, int di, int si, int length );

void CA_WriteByteToData( ca_data_t dest, int pos, uint8_t b );
void CA_WriteShortToData( ca_data_t dest, int pos, uint16_t s );
void CA_WriteIntToData( ca_data_t dest, int pos, uint32_t i );
uint8_t CA_ReadByteFromData( ca_data_t src, int pos );
uint16_t CA_ReadShortFromData( ca_data_t src, int pos );
uint32_t CA_ReadIntFromData( ca_data_t src, int pos );

ca_data_t CA_ReadSerialData( ca_data_t rawdata, int blocksize );
void CA_AppendSerialData( ca_data_t *serialdata, ca_data_t *rawdata, int blocksize );
ca_data_t CA_ConstructPacket( uint8_t address, ca_data_t serialdata, int blocksize );
ca_data_t CA_SetupCommandFrame( uint8_t fid, int length );

//
// ca_stream.c
//

ca_stream_t CA_AllocateStream( int length );
void CA_ResizeStream( ca_stream_t *stream, int length );
void CA_FreeStream( ca_stream_t *stream );

ca_oarray_t CA_AllocateOscilloscopeArray( int length );
void CA_FreeOscilloscopeArray( ca_oarray_t *oarray );

ca_sclass_t CA_ClassifyStreamByte( uint8_t b );

void CA_Add3BitToStream( ca_stream_t *stream, uint8_t b );
void CA_Add7BitToStream( ca_stream_t *stream, uint8_t b );
void CA_Add13BitToStream( ca_stream_t *stream, uint16_t s );

ca_oarray_t CA_StreamToOscilloscopeArray( ca_stream_t stream, ca_range_t range );

//
// ca_inout.c
//

ca_handle_t CA_GetDeviceHandle( const char *node );
void CA_CloseDeviceHandle( ca_handle_t handle );

ca_cassy_t CA_OpenCassy( ca_handle_t handle, ca_version_t expected, int id );

void CA_SendSerialData( ca_cassy_t cassy, ca_data_t serialdata );
ca_data_t CA_RecvSerialData( ca_cassy_t cassy, int rlen );

ca_oarray_t CA_RecvOscilloscopeArray( ca_cassy_t cassy, ca_range_t range );

ca_data_t CA_ExecuteCommand( ca_cassy_t cassy, ca_data_t serialdata, int rlen );

//
// functions
//

#include "ca_functionids.h"
#include "ca_functions.h"

#endif
