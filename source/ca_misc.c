#include "../include/libcassy.h"

int CA_IsBigEndian()
{
	static uint16_t s = 0x0001;

	return *(uint8_t *) &s == 0x00;
}

uint16_t CA_SwitchShort( uint16_t s )
{
	uint16_t b1, b2;

	if ( !CA_IsBigEndian() )
	{
		b1 = (s >> 0) & 0xFF;
		b2 = (s >> 8) & 0xFF;

		return (b1 << 8) + (b2 << 0);
	}

	return s;
}

uint32_t CA_SwitchInt( uint32_t i )
{
	uint32_t b1, b2, b3, b4;

	if ( !CA_IsBigEndian() )
	{
		b1 = (i >> 0) & 0xFF;
		b2 = (i >> 8) & 0xFF;
		b3 = (i >> 16) & 0xFF;
		b4 = (i >> 24) & 0xFF;

		return (b1 << 24) + (b2 << 16) + (b3 << 8) + (b4 << 0);
	}

	return i;
}

int8_t CA_SignExtendByte( uint8_t digits, int bits )
{
	return ((int8_t) (digits << (8 - bits))) >> (8 - bits);
}

int16_t CA_SignExtendShort( uint16_t digits, int bits )
{
	return ((int16_t) (digits << (16 - bits))) >> (16 - bits);
}

int32_t CA_SignExtendInt( uint32_t digits, int bits )
{
	return ((int32_t) (digits << (32 - bits))) >> (32 - bits);
}

uint8_t CA_GetCassyAddress( int id )
{
	switch ( id )
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
	}

	return 0x00;
}

int CA_GetBlocksize( ca_version_t version )
{
	if ( version == CA_VERSION_SENSORCASSY2 || version == CA_VERSION_POCKETCASSY2 )
		return 64;
	else
		return 8;
}

float CA_ConvertToRange( int value, ca_range_t range )
{
	switch (range)
	{
	case CA_RANGE_250V:
		return value * 0.305;
	case CA_RANGE_100V:
		return value * 0.05;
	case CA_RANGE_30V:
		return value * 0.015;
	case CA_RANGE_10V:
		return value * 0.005;
	case CA_RANGE_3V:
	case CA_RANGE_3A:
		return value * 0.0015;
	case CA_RANGE_1V:
	case CA_RANGE_1A:
		return value * 0.0005;
	case CA_RANGE_03V:
	case CA_RANGE_03A:
		return value * 0.00015;
	case CA_RANGE_01V:
	case CA_RANGE_01A:
		return value * 0.00005;
	case CA_RANGE_003A:
		return value * 0.000015;
	}

	return (float) value;
}

int CA_ConvertFromRange( float value, ca_range_t range )
{
	return (int) (value / CA_ConvertToRange( 1, range ));
}

float CA_ExtendToRange( uint32_t digits, int bits, ca_range_t range )
{
	return CA_ConvertToRange( CA_SignExtendInt( digits, bits ), range );
}

void CA_PrintData( ca_data_t data, char *desc )
{
	int i;

	printf( "%s: ", desc );

	for ( i = 0; i < data.length; i++ )
		printf( "%02x ", data.data[i] );

	printf( "\n" );
	fflush( stdout );
}

// credit to John Carmack, I just wanted to get rid of the unnecessary math link

float CA_RSqrt( float number )
{
	long i;
	float x, y;

	x = number * 0.5;
	y = number;

	i = *(long *) &y;
	i = 0x5f3759df - (i >> 1);

	y = *(float *) &i;
	y = y * (1.5 - (x * y * y));

	return y;
}
