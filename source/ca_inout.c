#include "../include/libcassy.h"

void CA_SendCommandSerial( ca_cassy_t cassy, ca_data_t command )
{
	ca_data_t serialdata;

	CA_ResetError();

	serialdata = CA_ConstructSerialData( CA_GetCassyAddress( cassy.id ), command );

	CA_IO_WriteSerial( cassy.handle, serialdata.data, serialdata.length );

#ifdef CA_DEBUG_PRINT
	CA_PrintData( serialdata, "Sent (serial)" );
#endif

	CA_FreeData( &serialdata );
}

ca_data_t CA_RecvDataSerial( ca_cassy_t cassy, int rlength )
{
	ca_data_t response;

	CA_ResetError();

	response = CA_AllocateData( rlength );

	CA_IO_ReadSerial( cassy.handle, response.data, rlength );

#ifdef CA_DEBUG_PRINT
	CA_PrintData( response, "Received (serial)" );
#endif

	return response;
}

void CA_SendCommandUSB( ca_cassy_t cassy, ca_data_t command )
{
	ca_data_t usbreports;
	int blocksize, offset;

	CA_ResetError();

	blocksize = CA_GetBlocksize( cassy.version ); // should check for controller cassy
	usbreports = CA_ConstructUSBReports( CA_GetCassyAddress( cassy.id ), command, blocksize );

	for ( offset = 0; offset < usbreports.length; offset += blocksize )
	{
		CA_IO_WriteUSB( cassy.handle, usbreports.data + offset, blocksize );

		if ( CA_GetLastError() != CA_ERROR_SUCCESS )
			break;
	}

#ifdef CA_DEBUG_PRINT
	CA_PrintData( usbreports, "Sent (usb)" );
#endif

	CA_FreeData( &usbreports );
}

ca_data_t CA_RecvDataUSB( ca_cassy_t cassy, int rlength )
{
	ca_data_t usbreport, response;
	int offset, blocksize;

	CA_ResetError();

	blocksize = CA_GetBlocksize( cassy.version ); // see above
	usbreport = CA_AllocateData( blocksize );
	response = CA_AllocateData( rlength );

	offset = 0;

	while ( offset < rlength )
	{
		CA_IO_ReadUSB( cassy.handle, usbreport.data, blocksize );

		if ( CA_GetLastError() != CA_ERROR_SUCCESS )
			break;

		CA_DeconstructUSBReport( usbreport, response, &offset, blocksize );

#ifdef CA_DEBUG_PRINT
		CA_PrintData( usbreport, "Received (usb)" );
#endif
	}

	CA_FreeData( &usbreport );

	return response;
}

ca_stream_t CA_RecvStreamSerial( ca_cassy_t cassy, int withstatus )
{
	ca_stream_t stream;
	uint8_t b, ob;
	int finished;

	CA_ResetError();

	CA_IO_ReadSerial( cassy.handle, &b, 1 );

	if ( CA_GetLastError() != CA_ERROR_SUCCESS )
		return stream;
	else if ( b != 0x01 )
	{
		CA_SetLastError( CA_ERROR_CASSY );
		return stream;
	}

	if ( withstatus )
		CA_IO_ReadSerial( cassy.handle, &stream.status, 1 );

	finished = 0;

	while ( !finished )
	{
		CA_IO_ReadSerial( cassy.handle, &b, 1 );

		if ( CA_GetLastError() != CA_ERROR_SUCCESS )
			return stream;

		switch ( CA_ClassifyStreamByte( b ) )
		{
		case CA_SCLASS_3BIT:
			CA_Add3BitToStream( &stream, b );
			break;
		case CA_SCLASS_7BIT:
			CA_Add7BitToStream( &stream, b );
			break;
		case CA_SCLASS_13BIT:
			CA_IO_ReadSerial( cassy.handle, &ob, 1 );

			if ( CA_GetLastError() != CA_ERROR_SUCCESS )
				return stream;

			CA_Add13BitToStream( &stream, (b << 8) + ob );
			break;
		case CA_SCLASS_UNKNOWN:
			CA_SetLastError( CA_ERROR_STREAM_INVALID );
			return stream;
		case CA_SCLASS_END:
			finished = 1;
		}
	}

	return stream;
}

ca_stream_t CA_RecvStreamUSB( ca_cassy_t cassy, int withstatus )
{
	ca_stream_t stream;
	ca_data_t usbreport, serialdata;
	int blocksize, length, i;
	int finished, overflow;
	uint8_t b, ob;

	blocksize = CA_GetBlocksize( cassy.version ); // see above
	usbreport = CA_AllocateData( blocksize );
	serialdata = CA_AllocateData( blocksize );

	stream.length = 0;
	stream.offset = 0;

	CA_IO_ReadUSB( cassy.handle, usbreport.data, blocksize );

	if ( CA_GetLastError() != CA_ERROR_SUCCESS )
		return stream;
	else if ( usbreport.data[0] < blocksize && usbreport.data[1] != 0x01 )
	{
		CA_SetLastError( CA_ERROR_CASSY );
		return stream;
	}

	length = 0;
	CA_DeconstructUSBReport( usbreport, serialdata, &length, blocksize );

	if ( withstatus )
	{
		stream.status = serialdata.data[1];
		i = 2;
	}
	else
		i = 1;

	b = 0;
	ob = 0;

	finished = 0;
	overflow = 0;

	while ( !finished )
	{
		while ( !finished && i < length )
		{
			b = CA_ReadByteFromData( serialdata, i );

			if ( overflow )
			{
				CA_Add13BitToStream( &stream, (ob << 8) + b );

				i += 1;
				overflow = 0;

				continue;
			}

			switch ( CA_ClassifyStreamByte( b ) )
			{
			case CA_SCLASS_3BIT:
				CA_Add3BitToStream( &stream, b );
				i += 1;
				break;
			case CA_SCLASS_7BIT:
				CA_Add3BitToStream( &stream, b );
				i += 1;
				break;
			case CA_SCLASS_13BIT:
				ob = b;
				i += 1;
				overflow = 1;
				break;
			case CA_SCLASS_UNKNOWN:
				CA_SetLastError( CA_ERROR_STREAM_INVALID );
				return stream;
			case CA_SCLASS_END:
				finished = 1;
			}
		}

		if ( !finished )
		{
			i = 0;
			length = 0;

			CA_IO_ReadUSB( cassy.handle, usbreport.data, blocksize );
			CA_DeconstructUSBReport( usbreport, serialdata, &length, blocksize );

			if ( CA_GetLastError() != CA_ERROR_SUCCESS )
				return stream;
		}
	}

	return stream;
}

ca_oarray_t CA_RecvOscilloscopeArray( ca_cassy_t cassy, ca_range_t range )
{
	ca_stream_t stream;
	ca_oarray_t oarray;

	CA_ResetError();

	switch ( CA_IO_GetIOMode( cassy.handle ) )
	{
	case CA_IOMODE_SERIAL:
		stream = CA_RecvStreamSerial( cassy, 1 );
		break;
	case CA_IOMODE_USB:
		stream = CA_RecvStreamUSB( cassy, 1 );
		break;
	case CA_IOMODE_BLUETOOTH:
		// not implemented
		break;
	}

	oarray = CA_StreamToOscilloscopeArray( stream, range );
	CA_FreeStream( &stream );

	return oarray;
}

ca_data_t CA_ExecuteCommand( ca_cassy_t cassy, ca_data_t command, int rlength )
{
	ca_iomode_t iomode;
	ca_data_t response;

	CA_ResetError();

	iomode = CA_IO_GetIOMode( cassy.handle );

	switch ( iomode )
	{
	case CA_IOMODE_SERIAL:
		CA_SendCommandSerial( cassy, command );
		break;
	case CA_IOMODE_USB:
		CA_SendCommandUSB( cassy, command );
		break;
	case CA_IOMODE_BLUETOOTH:
		// not implemented
		break;
	}

	if ( CA_GetLastError() != CA_ERROR_SUCCESS || cassy.id == 0x00 )
		response = CA_AllocateData( 1 );
	else if ( rlength != CA_INOUT_NORESPONSE )
	{
		switch ( iomode )
		{
		case CA_IOMODE_SERIAL:
			response = CA_RecvDataSerial( cassy, rlength );
			break;
		case CA_IOMODE_USB:
			response = CA_RecvDataUSB( cassy, rlength );
			break;
		case CA_IOMODE_BLUETOOTH:
			// not implemented
			break;
		}
	}

	return response;
}
