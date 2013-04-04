#include "../include/libcassy.h"

void CA_Init()
{
	CA_ResetError();

	if ( libusb_init( NULL ) != 0 )
		CA_SetLastError( CA_ERROR_IO_INIT );

#ifdef CA_DEBUG_PRINT
	libusb_set_debug( NULL, 3 );
#endif
}

void CA_Deinit()
{
	CA_ResetError();

	libusb_exit( NULL );
}

ca_version_t CA_GetVersionFromPID( int pid )
{
	switch ( pid )
	{
	case 0x1000:
		return CA_VERSION_SENSORCASSY;
	case 0x1001:
		return CA_VERSION_SENSORCASSY2;
	case 0x1011:
		return CA_VERSION_POCKETCASSY2;
	case 0x1020:
		return CA_VERSION_MOBILECASSY;
	case 0x1080:
		return CA_VERSION_JOULEWATTMETER;
	case 0x1090:
		return CA_VERSION_UMIP;
	case 0x10A0:
		return CA_VERSION_UMIC;
	case 0x10B0:
		return CA_VERSION_UMIB;
	}

	return CA_VERSION_UNKNOWN;
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

	// reset device?

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

ca_device_t *CA_FindDevices()
{
	libusb_device **list;
	struct libusb_device_descriptor desc;
	ca_device_t *devices;
	int length, i, j, k;

	CA_ResetError();

	length = libusb_get_device_list( NULL, &list );
	devices = (ca_device_t *) malloc( length * sizeof (ca_device_t) );

	if ( length < 0 )
	{
		CA_SetLastError( CA_ERROR_IO_MISC );
		return NULL;
	}

	j = 0;

	for ( i = 0; i < length; i++ )
	{
		if ( libusb_get_device_descriptor( list[i], &desc ) != 0 )
		{
			CA_SetLastError( CA_ERROR_IO_MISC );
			break;
		}

		if ( desc.idVendor != 0x0f11 )
			continue;

		devices[j].idstr = (char *) malloc( 10 * sizeof (char) );
		sprintf( devices[j].idstr, "%04x:%04x", desc.idVendor, desc.idProduct );

		devices[j].version = CA_GetVersionFromPID( desc.idProduct );

		devices[j].id[0] = true; // TODO

		for ( k = 1; k < 8; k++ )
			devices[j].id[k] = false;

		j++;
	}

	libusb_free_device_list( list, 1 );

	devices[j].idstr = NULL;

	return devices;
}

void CA_FreeDevices( ca_device_t *devices )
{
	free( devices );
}

ca_cassy_t CA_OpenCassy( ca_handle_t handle, ca_version_t expected, int id )
{
	ca_cassy_t cassy;

	CA_ResetError();

	cassy.handle = handle;
	cassy.id = id;
	cassy.version = expected; // could be inferred from the device pid

	return cassy;
}

void CA_SendData( ca_handle_t handle, uint8_t *data, int length )
{
	int trans;

	CA_ResetError();

	if ( libusb_interrupt_transfer( handle, 0x02, data, length, &trans, CA_USB_TIMEOUT ) != 0 )
		CA_SetLastError( CA_ERROR_IO_WRITE );
}

void CA_RecvData( ca_handle_t handle, uint8_t *data, int length )
{
	int trans;

	CA_ResetError();

	if ( libusb_interrupt_transfer( handle, 0x81, data, length, &trans, CA_USB_TIMEOUT ) != 0 )
		CA_SetLastError( CA_ERROR_IO_READ );
}
