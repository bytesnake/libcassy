#include "../include/ca_local.h"

uint8_t CA_GetCassyAddress( uint8_t cassyid )
{
    switch ( cassyid )
    {
    case 1:
        return 0xFF;
    case 2:
        return 0xFE;
    case 3:
        return 0xFC;
    case 4:
        return 0xF8;
    case 5:
        return 0xF0;
    case 6:
        return 0xE0;
    case 7:
        return 0xC0;
    case 8:
        return 0x80;
    default:
        return 0x00;
    }
}

bool CA_IsBigEndian()
{
    static uint16_t i = 0x0001;
    
    return *(uint8_t *) &i == 0x00;
}

uint16_t CA_SwitchShort( uint16_t s )
{
    uint16_t b1, b2;

    if ( !CA_IsBigEndian() )
    {
        b1 = s & 0xFF;
        b2 = ( s >> 8 ) & 0xFF;
        return ( b1 << 8 ) + b2;
    }
    
    return s;
}

uint32_t CA_SwitchInt( uint32_t i )
{
    uint32_t b1, b2, b3, b4;

    if ( !CA_IsBigEndian() )
    {
        b1 = i & 0xFF;
        b2 = ( i >> 8 ) & 0xFF;
        b3 = ( i >> 16 ) & 0xFF;
        b4 = ( i >> 24 ) & 0xFF;
        
        return ( b1 << 24 ) + ( b2 << 16 ) + ( b3 << 8 ) + b4;
    }
    
    return i;
}

int8_t CA_SignExtendByte( uint8_t digits, uint8_t bits )
{
    return (int8_t) ( digits << ( 8 - bits ) ) / ( 1 << ( 8 - bits ) );
}

int16_t CA_SignExtendShort( uint16_t digits, uint8_t bits )
{
    return (int16_t) ( digits << ( 16 - bits ) ) / ( 1 << ( 16 - bits ) );
}

int32_t CA_SignExtendInt( uint32_t digits, uint8_t bits )
{
    return (int32_t) ( digits << ( 32 - bits ) ) / ( 1 << ( 32 - bits ) );
}

float CA_ConvertToRange( int32_t value, range_t range )
{
    // TODO: box ranges

    switch ( range )
    {
    case RANGE_250V:
        return value * 0.305;
    case RANGE_100V:
        return value * 0.05;
    case RANGE_30V:
        return value * 0.015;
    case RANGE_10V:
    case RANGE_POWER_10V:
        return value * 0.005;
    case RANGE_3V:
    case RANGE_POWER_3V:
    case RANGE_3A:
        return value * 0.0015;
    case RANGE_1V:
    case RANGE_POWER_1V:
    case RANGE_1A:
//  case RANGE_POWER_1A:
        return value * 0.0005;
    case RANGE_03V:
    case RANGE_03A:
//  case RANGE_POWER_03A:
        return value * 0.00015;
    case RANGE_01V:
    case RANGE_01A:
//  case RANGE_POWER_01A:
        return value * 0.00005;
    case RANGE_003A:
        return value * 0.000015;
    default:
        return value;
    }
}

float CA_ExtendToRange( uint32_t digits, uint8_t bits, range_t range )
{
    return CA_ConvertToRange( CA_SignExtendInt( digits, bits ), range );
}

data_t CA_AllocateData( size_t length )
{
    data_t data;

    data.data = (uint8_t *) malloc( length * sizeof (uint8_t) );
    data.length = length;

    return data;
}

void CA_ReallocateData( data_t *data, size_t length )
{
    data->length = length;
    
    if ( data->length == 0 )
        data->data = (uint8_t *) malloc( length * sizeof (uint8_t) );
    else
        data->data = (uint8_t *) realloc( data->data, length * sizeof (uint8_t) );
}

void CA_FreeData( data_t *data )
{
    data->length = 0;
    free( data->data );
}

stream_t CA_AllocateStream( size_t length )
{
    stream_t stream;

    stream.data = (int16_t *) malloc( length * sizeof (int16_t) );
    stream.length = length;

    return stream;
}

void CA_ReallocateStream( stream_t *stream, size_t length )
{
    stream->length = length;
    
    if ( stream->length == 0 )
        stream->data = (int16_t *) malloc( length * sizeof (int16_t) );
    else
        stream->data = (int16_t *) realloc( stream->data, length * sizeof (int16_t) );
}

void CA_FreeStream( stream_t *stream )
{
	stream->length = 0;
	free( stream->data );
}

data_t CA_PrepareSerialData( uint8_t cassyid, data_t rdata )
{
    data_t sdata;
    ptrdiff_t sp = 2, dp;

    sdata = CA_AllocateData( rdata.length * 2 + 2 );
    sdata.data[0] = 0x1B;
    sdata.data[1] = CA_GetCassyAddress( cassyid );

    for ( dp = 0; dp < rdata.length; dp++ )
    {
        if ( rdata.data[dp] == 0x1B )
            sdata.data[sp++] = 0x1B;
        sdata.data[sp++] = rdata.data[dp];
    }
    sdata.length = sp;

    return sdata;
}

data_t CA_ReadSerialData( data_t pdata, size_t blocksize )
{
    data_t sdata;
    ptrdiff_t sp = 0, pp = 0;
    size_t length;

    sdata = CA_AllocateData( pdata.length );
    while ( pp < pdata.length )
    {
        length = blocksize;
        if ( pdata.data[pp] < blocksize )
        {
            length = pdata.data[pp];
            pp += 1;
        }

        memcpy( sdata.data + sp, pdata.data + pp, length );
        
        sp += length;
        pp += length;
    }
    sdata.length = sp;

    return sdata;
}

bool CA_AddSerialData( data_t *oserial, data_t pdata, size_t blocksize )
{
    data_t nserial;
    
    nserial = CA_ReadSerialData( pdata, blocksize );
          
    CA_ReallocateData( oserial, oserial->length + nserial.length );
    memcpy( oserial->data + ( oserial->length - nserial.length ), nserial.data, nserial.length );
    
    CA_FreeData( &nserial );
    
    if ( oserial->data[0] == 0x00 )
        return false;
    else
        return true;
}

void CA_WriteSerialData( data_t pdata, data_t sdata, ptrdiff_t *pp, ptrdiff_t *sp, size_t blocksize )
{
    size_t length;

    if ( sdata.data[*sp] >= blocksize && sdata.length - *sp >= blocksize )
    {
        memcpy( pdata.data + *pp, sdata.data + *sp, blocksize );
        *sp += blocksize;
    }
    else
    {
        length = sdata.length - *sp;
        length = length >= blocksize ? blocksize - 1 : length;

        memcpy( pdata.data + *pp + 1, sdata.data + *sp, length );
        pdata.data[*pp] = length;
        *sp += length;
    }

    *pp += blocksize;
}

void CA_PushToStream( stream_t *stream, ptrdiff_t *sp, int16_t value )
{
	if ( *sp >= stream->length )
		CA_ReallocateStream( stream, stream->length + 1024 );
		
	stream->data[*sp] = value;
	*sp += 1;
}

bool CA_AddToStream( stream_t *stream, data_t *data, ptrdiff_t *sp, ptrdiff_t *dp )
{
	int16_t value;
	
	while ( *dp < data->length )
	{
		if ( ( data->data[*dp] & 0b11100000 ) == 0b00000000 )
		{
			if ( *dp + 1 >= data->length )
				return true;
			
			value = CA_SignExtendShort( ( ( (uint16_t) data->data[*dp] ) << 8 ) + (uint16_t) data->data[*dp + 1], 13 );
			
			*dp += 2;
			CA_PushToStream( stream, sp, value );
		}
		else if ( ( data->data[*dp] & 0b1000000 ) == 0b10000000 )
		{
			value = stream->data[*sp - 1];
			value += (int16_t) CA_SignExtendByte( data->data[*dp], 7 );
			
			*dp += 1;
			CA_PushToStream( stream, sp, value );
		}
		else if ( ( data->data[*dp] & 0b11000000 ) == 0b01000000 )
		{
			value = stream->data[*sp - 1];
			value += (int16_t) CA_SignExtendByte( data->data[*dp] >> 3, 3 );
			
			CA_PushToStream( stream, sp, value );
			
			value = stream->data[*sp - 2];
			value += (int16_t) CA_SignExtendByte( data->data[*dp], 3 );
			
			CA_PushToStream( stream, sp, value );
			
			*dp += 1;
		}
		else
			return false;
		
	}
	
	return true;
}

data_t CA_ConstructPacket( uint8_t cassyid, data_t rdata, size_t blocksize )
{
    data_t pdata, sdata;
    ptrdiff_t pp = 0, sp = 0;

    sdata = CA_PrepareSerialData( cassyid, rdata );
    pdata = CA_AllocateData( NEXTBLOCK( sdata.length + sdata.length / ( blocksize - 1 ), blocksize ) );

    while ( sp < sdata.length )
        CA_WriteSerialData( pdata, sdata, &pp, &sp, blocksize );
    pdata.length = pp;

    CA_FreeData( &sdata );

    return pdata;
}
