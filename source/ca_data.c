#include "../include/libcassy.h"

ca_data_t CA_AllocateData( int length )
{
	ca_data_t data;

	data.data = (uint8_t *) malloc( length * sizeof (uint8_t) );
	data.length = length;

	return data;
}

void CA_FreeData( ca_data_t *data )
{
	if ( data->length == 0 )
		return;

	data->length = 0;
	free( data->data );
}

void CA_ResizeData( ca_data_t *data, int length )
{
	if ( data->length == 0 )
		data->data = (uint8_t *) malloc( length * sizeof (uint8_t) );
	else
		data->data = (uint8_t *) realloc( data->data, length * sizeof (uint8_t) );

	data->length = length;
}

void CA_CopyData( ca_data_t dest, ca_data_t src, int di, int si, int length )
{
	int i;

	for ( i = 0; i < length; i++ )
		dest.data[di + i] = src.data[si + i];
}

void CA_WriteByteToData( ca_data_t dest, int pos, uint8_t b )
{
	*(uint8_t *) (dest.data + pos) = b;
}

void CA_WriteShortToData( ca_data_t dest, int pos, uint16_t s )
{
	*(uint16_t *) (dest.data + pos) = CA_SwitchShort( s );
}

void CA_WriteIntToData( ca_data_t dest, int pos, uint32_t i )
{
	*(uint32_t *) (dest.data + pos) = CA_SwitchInt( i );
}

uint8_t CA_ReadByteFromData( ca_data_t src, int pos )
{
	return *(uint8_t *) (src.data + pos);
}

uint16_t CA_ReadShortFromData( ca_data_t src, int pos )
{
	return CA_SwitchShort( *(uint16_t *) (src.data + pos) );
}

uint32_t CA_ReadIntFromData( ca_data_t src, int pos )
{
	return CA_SwitchInt( *(uint32_t *) (src.data + pos) );
}

ca_data_t CA_SetupCommandFrame( uint8_t fid, int length )
{
	ca_data_t command;

	command = CA_AllocateData( length + 1 );
	command.data[0] = fid;

	return command;
}

ca_data_t CA_ConstructSerialData( uint8_t address, ca_data_t command )
{
	ca_data_t serialdata;
	int si, ri;

	serialdata = CA_AllocateData( command.length * 2 + 2 );

	serialdata.data[0] = 0x1B;
	serialdata.data[1] = address;

	si = 2;
	ri = 0;

	while ( ri < command.length )
	{
		if ( command.data[ri] == 0x1B )
		{
			serialdata.data[si++] = 0x1B;
			serialdata.data[si++] = 0x1B;
		}
		else
			serialdata.data[si++] = command.data[ri];

		ri++;
	}

	serialdata.length = si;

	return serialdata;
}

ca_data_t CA_ConstructUSBReports( uint8_t address, ca_data_t command, int blocksize )
{
	ca_data_t serialdata, usbreports;
	int length;
	int pi, ri;

	serialdata = CA_ConstructSerialData( address, command );
	usbreports = CA_AllocateData( CA_NEXTBLOCK( serialdata.length + serialdata.length / blocksize, blocksize ) );

	pi = 0;
	ri = 0;

	while ( pi < serialdata.length )
	{
		if ( serialdata.data[pi] >= blocksize && serialdata.length - pi >= blocksize )
		{
			CA_CopyData( usbreports, serialdata, ri, pi, blocksize );

			pi += blocksize;
		}
		else
		{
			length = serialdata.length - pi;
			length = length >= blocksize ? blocksize - 1 : length;

			CA_CopyData( usbreports, serialdata, ri + 1, pi, length );
			usbreports.data[ri] = (uint8_t) length;

			pi += length;
		}

		pi += blocksize;
	}

	usbreports.length = CA_NEXTBLOCK( ri, blocksize );

	CA_FreeData( &serialdata );

	return usbreports;
}

void CA_DeconstructUSBReport( ca_data_t usbreport, ca_data_t response, int *offset, int blocksize )
{
	int length;
	uint8_t b;

	b = CA_ReadByteFromData( usbreport, 0 );

	if ( b < blocksize )
	{
		length = b > response.length - *offset ? response.length - *offset : b;
		CA_CopyData( response, usbreport, *offset, 1, length );
	}
	else
	{
		length = blocksize > response.length - *offset ? response.length - *offset : blocksize;
		CA_CopyData( response, usbreport, *offset, 0, length );
	}

	*offset += length;
}
