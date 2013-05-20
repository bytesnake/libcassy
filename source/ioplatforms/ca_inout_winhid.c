#include "../../include/libcassy.h"

//
// Work in progres...
//

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

void CA_SendData( ca_handle_t handle, uint8_t *data, int length )
{

}

void CA_RecvData( ca_handle_t handle, uint8_t *data, int length )
{

}
