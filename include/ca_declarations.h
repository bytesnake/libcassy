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
int CA_GetBlocksize( ca_version_t version );

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

int CA_IsCassyError( ca_data_t data );

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

ca_data_t CA_SetupCommandFrame( uint8_t fid, int length );

ca_data_t CA_ConstructSerialData( uint8_t address, ca_data_t command );

ca_data_t CA_ConstructUSBReports( uint8_t address, ca_data_t command, int blocksize );
void CA_DeconstructUSBReport( ca_data_t usbreport, ca_data_t response, int *offset, int blocksize );

//
// ca_stream.c
//

ca_stream_t CA_AllocateStream( int length );
void CA_ResizeStream( ca_stream_t *stream, int length );
void CA_FreeStream( ca_stream_t *stream );

ca_oarray_t CA_AllocateOscilloscopeArray( int length );

ca_sclass_t CA_ClassifyStreamByte( uint8_t b );

void CA_Add3BitToStream( ca_stream_t *stream, uint8_t b );
void CA_Add7BitToStream( ca_stream_t *stream, uint8_t b );
void CA_Add13BitToStream( ca_stream_t *stream, uint16_t s );

ca_oarray_t CA_StreamToOscilloscopeArray( ca_stream_t stream, ca_range_t range );

ca_data_t CA_SetupStreamCommandFrame( int fid, int16_t *values, int length );

//
// ca_inout.c
//

void CA_SendCommandSerial( ca_cassy_t cassy, ca_data_t command );
ca_data_t CA_RecvDataSerial( ca_cassy_t cassy, int rlength );

void CA_SendCommandUSB( ca_cassy_t cassy, ca_data_t command );
ca_data_t CA_RecvDataUSB( ca_cassy_t cassy, int rlength );

ca_stream_t CA_RecvStreamSerial( ca_cassy_t cassy, int withstatus );
ca_stream_t CA_RecvStreamUSB( ca_cassy_t cassy, int withstatus );

ca_oarray_t CA_RecvOscilloscopeArray( ca_cassy_t cassy, ca_range_t range );

ca_data_t CA_ExecuteCommand( ca_cassy_t cassy, ca_data_t command, int rlength );

//
// ca_inout_*.c
//

void CA_IO_WriteSerial( ca_handle_t handle, uint8_t *data, int length );
void CA_IO_ReadSerial( ca_handle_t handle, uint8_t *data, int length );

void CA_IO_WriteUSB( ca_handle_t handle, uint8_t *data, int length );
void CA_IO_ReadUSB( ca_handle_t handle, uint8_t *data, int length );

// bluetooth write and read

ca_iomode_t CA_IO_GetIOMode( ca_handle_t handle );

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
// ca_stream.c
//

void CA_FreeOscilloscopeArray( ca_oarray_t *oarray );

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

//
// cassy functions
//

#include "ca_functions.h"

#ifdef __cplusplus
}
#endif

#endif
