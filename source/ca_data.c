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

ca_data_t CA_PrepareSerialData( uint8_t address, ca_data_t rawdata )
{
	ca_data_t serialdata;
	int si, ri;

	serialdata = CA_AllocateData( rawdata.length * 2 + 2 );

	serialdata.data[0] = 0x1B;
	serialdata.data[1] = address;

	si = 2;
	ri = 0;

	while ( ri < rawdata.length )
	{
		if ( rawdata.data[ri] == 0x1B )
		{
			serialdata.data[si++] = 0x1B;
			serialdata.data[si++] = 0x1B;
		}
		else
			serialdata.data[si++] = rawdata.data[ri];

		ri++;
	}

	serialdata.length = si;

	return serialdata;
}

ca_data_t CA_ReadSerialData( ca_data_t rawdata, int blocksize )
{
	ca_data_t serialdata;
	int length;
	int si, ri;

	serialdata = CA_AllocateData( rawdata.length );

	si = 0;
	ri = 0;

	while ( ri < rawdata.length )
	{
		if ( rawdata.data[ri] < blocksize )
			length = rawdata.data[ri++];
		else
			length = blocksize;

		if ( ri + length > rawdata.length )
			length = rawdata.length - ri;

		CA_CopyData( serialdata, rawdata, si, ri, length );

		si += length;
		ri += length;
	}

	serialdata.length = si;

	return serialdata;
}

void CA_AppendSerialData( ca_data_t *serialdata, ca_data_t *rawdata, int blocksize )
{
	ca_data_t newdata;

	CA_ResizeData( serialdata, serialdata->length + blocksize );
	newdata = CA_ReadSerialData( *rawdata, blocksize );

	CA_CopyData( *serialdata, newdata, serialdata->length - blocksize, 0, newdata.length );
	serialdata->length -= blocksize - newdata.length;

	CA_FreeData( &newdata );
}

ca_data_t CA_ConstructPacket( uint8_t address, ca_data_t serialdata, int blocksize )
{
	ca_data_t prepdata, rawdata;
	int length;
	int pi, ri;

	prepdata = CA_PrepareSerialData( address, serialdata );
	rawdata = CA_AllocateData( CA_NEXTBLOCK( prepdata.length + prepdata.length / blocksize, blocksize ) );

	pi = 0;
	ri = 0;

	while ( pi < prepdata.length )
	{
		if ( prepdata.data[pi] >= blocksize && prepdata.length - pi >= blocksize )
		{
			CA_CopyData( rawdata, prepdata, ri, pi, blocksize );

			pi += blocksize;
		}
		else
		{
			length = prepdata.length - pi;
			length = length >= blocksize ? blocksize - 1 : length;

			CA_CopyData( rawdata, prepdata, ri + 1, pi, length );
			rawdata.data[ri] = (uint8_t) length;

			pi += length;
		}

		pi += blocksize;
	}

	rawdata.length = CA_NEXTBLOCK( ri, blocksize );

	CA_FreeData( &prepdata );

	return rawdata;
}

ca_data_t CA_SetupCommandFrame( uint8_t fid, int length )
{
	ca_data_t command;

	command = CA_AllocateData( length + 1 );
	command.data[0] = fid;

	return command;
}
