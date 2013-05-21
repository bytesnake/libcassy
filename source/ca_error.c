#include "../include/libcassy.h"

ca_error_t lasterror = CA_ERROR_SUCCESS;

void CA_SetLastError( ca_error_t error )
{
	if ( lasterror == CA_ERROR_SUCCESS )
		lasterror = error;
}

void CA_ResetError()
{
	lasterror = CA_ERROR_SUCCESS;
}

int CA_IsCassyError( ca_data_t data )
{
	if ( CA_GetLastError() == CA_ERROR_SUCCESS && data.data[0] == 0x01 )
		return 0;

	if ( data.data[0] == 0x02 )
		CA_SetLastError( CA_ERROR_CASSY_OVERFLOW );
	else if ( data.data[1] == 0x03 )
		CA_SetLastError( CA_ERROR_CASSY_UNDERFLOW );
	else
		CA_SetLastError( CA_ERROR_CASSY );

	return 1;
}

ca_error_t CA_GetLastError()
{
	return lasterror;
}

const char *CA_ErrorToString( ca_error_t error )
{
	switch ( error )
	{
	case CA_ERROR_SUCCESS:
		return "CA_ERROR_SUCCESS";
	case CA_ERROR_CASSY:
		return "CA_ERROR_CASSY";
	case CA_ERROR_CASSY_OVERFLOW:
		return "CA_ERROR_CASSY_OVERFLOW";
	case CA_ERROR_CASSY_UNDERFLOW:
		return "CA_ERROR_CASSY_UNDERFLOW";
	case CA_ERROR_STREAM_INVALID:
		return "CA_ERROR_STREAM_INVALID";
	case CA_ERROR_IO_INIT:
		return "CA_ERROR_IO_INIT";
	case CA_ERROR_IO_OPEN:
		return "CA_ERROR_IO_OPEN";
	case CA_ERROR_IO_CLOSE:
		return "CA_ERROR_IO_CLOSE";
	case CA_ERROR_IO_READ:
		return "CA_ERROR_IO_READ";
	case CA_ERROR_IO_WRITE:
		return "CA_ERROR_IO_WRITE";
	case CA_ERROR_IO_MISC:
		return "CA_ERROR_IO_MISC";
	}

	return "CA_ERROR_UNKNOWN";
}
