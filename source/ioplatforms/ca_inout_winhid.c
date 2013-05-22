#include "../../include/libcassy.h"

//
// Work in progres...
//

void CA_IO_WriteSerial( ca_handle_t handle, uint8_t *data, int length )
{
	CA_SetLastError( CA_ERROR_IO_WRITE );
}

void CA_IO_ReadSerial( ca_handle_t handle, uint8_t *data, int length )
{
	CA_SetLastError( CA_ERROR_IO_READ );
}

void CA_IO_WriteUSB( ca_handle_t handle, uint8_t *data, int length )
{
	CA_SetLastError( CA_ERROR_IO_WRITE );
}

void CA_IO_ReadUSB( ca_handle_t handle, uint8_t *data, int length )
{
	CA_SetLastError( CA_ERROR_IO_READ );
}

ca_iomode_t CA_IO_GetIOMode( ca_handle_t handle )
{
	return CA_IOMODE_USB;
}

void CA_Init()
{

}

void CA_Deinit()
{

}

ca_handle_t CA_GetDeviceHandle( const char *desc )
{
	return NULL;
}

void CA_CloseDeviceHandle( ca_handle_t handle )
{

}

ca_device_t *CA_FindDevices()
{
	return NULL;
}

void CA_FreeDevices( ca_device_t *devices )
{

}

ca_cassy_t CA_OpenCassy( ca_handle_t handle, ca_version_t expected, int id )
{
	ca_cassy_t cassy;

	CA_ResetError();

	cassy.handle = handle;
	cassy.id = id;
	cassy.version = expected;

	return cassy;
}
