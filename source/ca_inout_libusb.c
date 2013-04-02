#include "../include/libcassy.h"

void CA_Init()
{
	CA_ResetError();

#ifdef CA_DEBUG_PRINT
	libusb_set_debug( NULL, 3 );
#endif

	if ( libusb_init( NULL ) != 0 )
		CA_SetLastError( CA_ERROR_IO_INIT );
}

void CA_Deinit()
{
	CA_ResetError();

	libusb_exit( NULL );
}

ca_handle_t CA_GetDeviceHandle( const char *desc )
{
	char *endptr;
	uint16_t vid, pid;
	ca_handle_t handle;

	CA_ResetError();

	vid = (uint16_t) strtoul( desc, &endptr, 16 );

	if ( desc == endptr || *endptr == '\0' )
	{
		CA_SetLastError( CA_ERROR_IO_OPEN );
		return NULL;
	}

	pid = (uint16_t) strtoul( endptr + 1, &endptr, 16 );

	if ( *endptr != '\0' )
	{
		CA_SetLastError( CA_ERROR_IO_OPEN );
		return NULL;
	}

	handle = libusb_open_device_with_vid_pid( NULL, vid, pid );

	if ( handle == NULL )
		CA_SetLastError( CA_ERROR_IO_OPEN );
	else if ( libusb_detach_kernel_driver( handle, 0 ) != 0 )
		CA_SetLastError( CA_ERROR_IO_OPEN );
	else if ( libusb_claim_interface( handle, 0 ) != 0 )
		CA_SetLastError( CA_ERROR_IO_OPEN );

	return handle;
}

void CA_CloseDeviceHandle( ca_handle_t handle )
{
	CA_ResetError();

	if ( libusb_release_interface( handle, 0 ) != 0 )
		CA_SetLastError( CA_ERROR_IO_CLOSE );
	if ( libusb_attach_kernel_driver( handle, 0 ) != 0 )
		CA_SetLastError( CA_ERROR_IO_CLOSE );

	libusb_close( handle );
}

ca_cassy_t CA_OpenCassy( ca_handle_t handle, ca_version_t expected, int id )
{
	ca_cassy_t cassy;

	CA_ResetError();

	cassy.handle = handle;
	cassy.id = id;
	cassy.version = expected; // could be inferred from the device pid

	CA_Reset( cassy );

	return cassy;
}

void CA_SendData( ca_handle_t handle, uint8_t *data, int length )
{
	int trans;

	CA_ResetError();

	if ( libusb_interrupt_transfer( handle, 0x01, data, length, &trans, CA_USB_TIMEOUT ) != 0 )
		CA_SetLastError( CA_ERROR_IO_WRITE );
}

void CA_RecvData( ca_handle_t handle, uint8_t *data, int length )
{
	int trans;

	CA_ResetError();

	if ( libusb_interrupt_transfer( handle, 0x81, data, length, &trans, CA_USB_TIMEOUT ) != 0 )
		CA_SetLastError( CA_ERROR_IO_READ );
}
