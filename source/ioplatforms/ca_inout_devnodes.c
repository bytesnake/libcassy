#include "../../include/libcassy.h"

void CA_IO_WriteSerial( ca_handle_t handle, uint8_t *data, int length )
{
	int trans;

	CA_ResetError();

	trans = write( handle->filedesc, data, length );

	if ( trans != length )
		CA_SetLastError( CA_ERROR_IO_WRITE );
}

void CA_IO_ReadSerial( ca_handle_t handle, uint8_t *data, int length )
{
	int trans, buffsize;

	CA_ResetError();

	trans = 0;

	while ( trans < length )
	{
		buffsize = read( handle->filedesc, data + trans, length );

		if ( buffsize <= 0 )
		{
			CA_SetLastError( CA_ERROR_IO_READ );
			break;
		}

		trans += buffsize;
	}
}

void CA_IO_WriteUSB( ca_handle_t handle, uint8_t *data, int length )
{
	CA_IO_WriteSerial( handle, data, length );
}

void CA_IO_ReadUSB( ca_handle_t handle, uint8_t *data, int length )
{
	CA_IO_ReadSerial( handle, data, length );
}

ca_iomode_t CA_IO_GetIOMode( ca_handle_t handle )
{
	if ( handle->settingsb == NULL )
		return CA_IOMODE_USB;
	else
		return CA_IOMODE_SERIAL;
}

void CA_Init()
{
	CA_ResetError();
}

void CA_Deinit()
{
	CA_ResetError();
}

struct termios *CA_PrepareSerialDevice( int filedesc )
{
	struct termios settings;
	struct termios *settingsb;

	CA_ResetError();

	settingsb = (struct termios *) malloc( sizeof (struct termios) );

	if ( tcgetattr( filedesc, settingsb ) < 0 )
	{
		CA_SetLastError( CA_ERROR_IO_OPEN );
		return NULL;
	}

	settings = *settingsb;

	cfsetospeed( &settings, B38400 );
	cfsetispeed( &settings, B38400 );
	cfmakeraw( &settings );

	settings.c_cc[VMIN] = 1;
	settings.c_cc[VTIME] = 0;

	settings.c_cflag &= ~(CSTOPB | CRTSCTS);
	settings.c_cflag |= CLOCAL | CREAD;

	if ( tcsetattr( filedesc, TCSAFLUSH, &settings ) < 0 )
		CA_SetLastError( CA_ERROR_IO_OPEN );

	return settingsb;
}

ca_handle_t CA_GetDeviceHandle( const char *desc )
{
	struct serial_struct serinfo;
	ca_handle_t handle;

	CA_ResetError();

	handle = (ca_handle_t) malloc( sizeof *handle );
	handle->filedesc = open( desc, O_RDWR | O_NOCTTY );

	if ( handle->filedesc == -1 )
	{
		CA_SetLastError( CA_ERROR_IO_OPEN );
		return NULL;
	}

	if ( ioctl( handle->filedesc, TIOCGSERIAL, &serinfo ) < 0 )
		handle->settingsb = NULL;
	else
		handle->settingsb = CA_PrepareSerialDevice( handle->filedesc );

	return handle;
}

void CA_CloseDeviceHandle( ca_handle_t handle )
{
	int result;

	CA_ResetError();

	result = close( handle->filedesc );
	free( handle->settingsb );
	free( handle );

	if ( result != 0 )
		CA_SetLastError( CA_ERROR_IO_CLOSE );
}

ca_device_t *CA_FindDevices()
{
	// not implemented

	return NULL;
}

void CA_FreeDevices( ca_device_t *devices )
{
	// not implemented
}

ca_cassy_t CA_OpenCassy( ca_handle_t handle, ca_version_t expected, int id )
{
	ca_cassy_t cassy;

	CA_ResetError();

	cassy.handle = handle;
	cassy.id = id;
	cassy.version = expected;

	if ( expected == CA_VERSION_UNKNOWN )
		cassy.version = CA_GetHardwareVersion( cassy ) >> 8;

	return cassy;
}

void CA_SendData( ca_handle_t handle, uint8_t *data, int length )
{
	int trans;

	CA_ResetError();

	trans = write( handle->filedesc, data, length );

	if ( trans != length )
		CA_SetLastError( CA_ERROR_IO_WRITE );
}

void CA_RecvData( ca_handle_t handle, uint8_t *data, int length )
{
	int trans;

	CA_ResetError();

	trans = read( handle->filedesc, data, length );

	if ( trans != length )
		CA_SetLastError( CA_ERROR_IO_READ );
}
