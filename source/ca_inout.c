#include "../include/libcassy.h"

ca_handle_t CA_GetDeviceHandle( const char *node )
{
	ca_handle_t handle;
	
	CA_ResetError();
	
	handle = open( node, O_RDWR );
	
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

void CA_SendSerialData( ca_cassy_t cassy, ca_data_t serialdata )
{
	ca_data_t rawdata;
	int blocksize, offset, length;
	
	CA_ResetError();
	
	blocksize = CA_GETBLOCKSIZE( cassy.version );
	rawdata = CA_ConstructPacket( CA_GetCassyAddress( cassy.id ), serialdata, blocksize );
	
	for ( offset = 0; offset < rawdata.length; offset += blocksize )
	{
		length = write( cassy.handle, rawdata.data + offset, blocksize );
		
		if ( length != blocksize )
		{
			CA_SetLastError( CA_ERROR_IO_WRITE );
			break;
		}
		
		usleep( 1000 * CA_USB_DELAY );
	}
	
	CA_FreeData( &rawdata );
}

ca_data_t CA_RecvSerialData( ca_cassy_t cassy, int rlen )
{
	ca_data_t response, serialdata;
	int blocksize, length;
	
	CA_ResetError();
	
	blocksize = CA_GETBLOCKSIZE( cassy.version );
	response = CA_AllocateData( blocksize );
	
	serialdata.length = 0;
	
	do
	{
		length = read( cassy.handle, response.data, blocksize );
		
		if ( length != blocksize )
		{
			CA_SetLastError( CA_ERROR_IO_READ );
			break;
		}
		
		CA_AppendSerialData( &serialdata, &response, blocksize );
		
		usleep( 1000 * CA_USB_DELAY );
	} while ( serialdata.length < rlen );

	CA_FreeData( &response );
	
	return serialdata;
}

ca_oarray_t CA_RecvOscilloscopeArray( ca_cassy_t cassy, ca_range_t range )
{
	ca_oarray_t oarray;
	ca_stream_t stream;
	ca_data_t response, serialdata;
	int blocksize, length, i;
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
		length = read( cassy.handle, response.data, blocksize );
		
		if ( length != blocksize )
		{
			CA_SetLastError( CA_ERROR_IO_READ );
			break;
		}
		
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
		
		CA_FreeData( &serialdata );
		
		usleep( 1000 * CA_USB_DELAY );
	}
	
	CA_FreeData( &response );
	
	oarray = CA_StreamToOscilloscopeArray( stream, range );
	oarray.status = status;
	
	CA_FreeStream( &stream );
	
	return oarray;
}

ca_data_t CA_ExecuteCommand( ca_cassy_t cassy, ca_data_t serialdata, int rlen )
{
	ca_data_t response;

	CA_ResetError();
	
	CA_SendSerialData( cassy, serialdata );
	
	if ( CA_GetLastError() == CA_ERROR_SUCCESS && rlen != CA_USB_NORESPONSE )
		response = CA_RecvSerialData( cassy, rlen );
	
	return response;
}