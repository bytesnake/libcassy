#include "../include/libcassy.h"

#ifdef CA_DEBUG_NOWRITE

void CA_SendSerialData( ca_cassy_t cassy, ca_data_t serialdata )
{
	CA_ResetError();
}

#else

void CA_SendSerialData( ca_cassy_t cassy, ca_data_t serialdata )
{
	ca_data_t rawdata;
	int blocksize, offset;

	CA_ResetError();

	blocksize = CA_GETBLOCKSIZE( cassy.version );
	rawdata = CA_ConstructPacket( CA_GetCassyAddress( cassy.id ), serialdata, blocksize );

	for ( offset = 0; offset < rawdata.length; offset += blocksize )
	{
		CA_SendData( cassy.handle, rawdata.data + offset, blocksize );

		if ( CA_GetLastError() != CA_ERROR_SUCCESS )
			break;

		usleep( 1000 * CA_USB_DELAY );
	}

#ifdef CA_DEBUG_PRINT
	CA_PrintData( serialdata, "Sent" );
#endif

	CA_FreeData( &rawdata );
}

#endif

#ifdef CA_DEBUG_NOREAD

ca_data_t CA_RecvSerialData( ca_cassy_t cassy, int rlen )
{
	CA_ResetError();

	return CA_AllocateData( rlen );
}

#else

ca_data_t CA_RecvSerialData( ca_cassy_t cassy, int rlen )
{
	ca_data_t response, serialdata;
	int blocksize;

	CA_ResetError();

	blocksize = CA_GETBLOCKSIZE( cassy.version );
	response = CA_AllocateData( blocksize );

	serialdata.length = 0;

	do
	{
		CA_RecvData( cassy.handle, response.data, blocksize );

		if ( CA_GetLastError() != CA_ERROR_SUCCESS )
			break;

		CA_AppendSerialData( &serialdata, &response, blocksize );

		usleep( 1000 * CA_USB_DELAY );
	} while ( serialdata.length < rlen );

#ifdef CA_DEBUG_PRINT
	CA_PrintData( serialdata, "Received" );
#endif

	CA_FreeData( &response );

	return serialdata;
}

#endif

#ifdef CA_DEBUG_NOREAD

ca_oarray_t CA_RecvOscilloscopeArray( ca_cassy_t cassy, ca_range_t range )
{
	CA_ResetError();

	return CA_AllocateOscilloscopeArray( CA_STREAM_CHUNKSIZE );
}

#else

ca_oarray_t CA_RecvOscilloscopeArray( ca_cassy_t cassy, ca_range_t range )
{
	ca_oarray_t oarray;
	ca_stream_t stream;
	ca_data_t response, serialdata;
	int blocksize, i;
	uint8_t status, ob;
	bool overflow, finished;

	CA_ResetError();

	blocksize = CA_GETBLOCKSIZE( cassy.version );
	response = CA_AllocateData( blocksize );

	stream.length = 0;
	stream.offset = 0;

	overflow = false;
	finished = false;

	status = 0;
	ob = 0;

	while ( !finished )
	{
		CA_RecvData( cassy.handle, response.data, blocksize );

		if ( CA_GetLastError() != CA_ERROR_SUCCESS )
			break;

		serialdata = CA_ReadSerialData( response, blocksize );

		if ( stream.offset == 0 )
		{
			status = CA_ReadByteFromData( serialdata, 1 );
			i = 2;
		}
		else
			i = 0;

		while ( !finished && i < serialdata.length )
		{
			if ( overflow )
			{
				CA_Add13BitToStream( &stream, (((uint16_t) ob) << 8) + CA_ReadByteFromData( serialdata, i ) );
				i += 1;
				overflow = false;

				continue;
			}

			switch ( CA_ClassifyStreamByte( CA_ReadByteFromData( serialdata, i ) ) )
			{
			case CA_SCLASS_3BIT:
				CA_Add3BitToStream( &stream, CA_ReadByteFromData( serialdata, i ) );
				i += 1;

				break;
			case CA_SCLASS_7BIT:
				CA_Add7BitToStream( &stream, CA_ReadByteFromData( serialdata, i ) );
				i += 1;

				break;
			case CA_SCLASS_13BIT:
				if ( i == serialdata.length - 1 )
				{
					ob = CA_ReadByteFromData( serialdata, i );
					overflow = true;
					i += 1;
				}
				else
				{
					CA_Add13BitToStream( &stream, CA_ReadShortFromData( serialdata, i ) );
					i += 2;
				}

				break;
			case CA_SCLASS_END:
				finished = true;
				break;
			case CA_SCLASS_UNKNOWN:
				CA_SetLastError( CA_ERROR_STREAM_INVALID );
				finished = true;
			}
		}

#ifdef CA_DEBUG_PRINT
		CA_PrintData( serialdata, "Received (stream)" );
#endif

		CA_FreeData( &serialdata );

		usleep( 1000 * CA_USB_DELAY );
	}

	CA_FreeData( &response );

	oarray = CA_StreamToOscilloscopeArray( stream, range );
	oarray.status = status;

	CA_FreeStream( &stream );

	return oarray;
}

#endif

ca_data_t CA_ExecuteCommand( ca_cassy_t cassy, ca_data_t serialdata, int rlen )
{
	ca_data_t response;

	CA_ResetError();

	CA_SendSerialData( cassy, serialdata );

	if ( CA_GetLastError() == CA_ERROR_SUCCESS && rlen != CA_USB_NORESPONSE )
		response = CA_RecvSerialData( cassy, rlen );

	return response;
}
