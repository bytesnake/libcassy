#include "../include/libcassy.h"

void CA_Init()
{
	CA_ResetError();
}

void CA_Deinit()
{
	CA_ResetError();
}

ca_handle_t CA_GetDeviceHandle( const char *desc )
{
	ca_handle_t handle;

	CA_ResetError();

	handle = open( desc, O_RDWR );

	if ( handle == -1 )
		CA_SetLastError( CA_ERROR_IO_OPEN );

	return handle;
}

void CA_CloseDeviceHandle( ca_handle_t handle )
{
	int result;

	CA_ResetError();

	result = close( handle );

	if ( result != 0 )
		CA_SetLastError( CA_ERROR_IO_CLOSE );
}

ca_cassy_t CA_OpenCassy( ca_handle_t handle, ca_version_t expected, int id )
{
	ca_cassy_t cassy;

	cassy.handle = handle;
	cassy.id = id;
	cassy.version = expected;

	CA_Reset( cassy );

	// cassy.version = CA_GetHardwareVersion( cassy ) >> 8;

	return cassy;
}

void CA_SendData( ca_handle_t handle, uint8_t *data, int length )
{
	int trans;

	CA_ResetError();

	trans = write( handle, data, length );

	if ( trans != length )
		CA_SetLastError( CA_ERROR_IO_WRITE );
}

void CA_RecvData( ca_handle_t handle, uint8_t *data, int length )
{
	int trans;

	CA_ResetError();

	trans = read( handle, data, length );

	if ( trans != length )
		CA_SetLastError( CA_ERROR_IO_READ );
}
