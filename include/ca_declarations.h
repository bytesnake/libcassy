#ifndef CA_DECLARATIONS_H
#define CA_DECLARATIONS_H

#include "libcassy.h"

#ifdef __cplusplus
extern "C" {
#endif

//
// internal declarations
//

#ifdef CA_LOCAL

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
int CA_ConvertFromRange( float value, ca_range_t range );

float CA_ExtendToRange( uint32_t digits, int bits, ca_range_t range );

void CA_PrintData( ca_data_t data, char *desc );

float CA_RSqrt( float number );

//
// ca_error.c
//

void CA_SetLastError( ca_error_t error );
void CA_ResetError();

bool CA_IsCassyError( ca_data_t data );

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

ca_data_t CA_SetupStreamCommandFrame( int fid, int16_t *values, int length );

//
// ca_inout.c
//

void CA_SendSerialData( ca_cassy_t cassy, ca_data_t serialdata );
ca_data_t CA_RecvSerialData( ca_cassy_t cassy, int rlen );

ca_oarray_t CA_RecvOscilloscopeArray( ca_cassy_t cassy, ca_range_t range );

ca_data_t CA_ExecuteCommand( ca_cassy_t cassy, ca_data_t serialdata, int rlen );

#endif

//
// external declarations
//

//
// ca_error.c
//

ca_error_t CA_GetLastError();
const char *CA_ErrorToString( ca_error_t error );

//
// ca_inout_*.c
//

void CA_Init();
void CA_Deinit();

ca_handle_t CA_GetDeviceHandle( const char *desc );
void CA_CloseDeviceHandle( ca_handle_t handle );

ca_device_t *CA_FindDevices();
void CA_FreeDevices( ca_device_t *devices );

ca_cassy_t CA_OpenCassy( ca_handle_t handle, ca_version_t expected, int id );

void CA_SendData( ca_handle_t handle, uint8_t *data, int length );
void CA_RecvData( ca_handle_t handle, uint8_t *data, int length );

//
// cassy functions
//

#include "ca_functionids.h"
#include "ca_functions.h"

#ifdef __cplusplus
}
#endif

#endif
