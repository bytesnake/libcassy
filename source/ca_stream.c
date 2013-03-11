#include "../include/libcassy.h"

ca_stream_t CA_AllocateStream( int length )
{
	ca_stream_t stream;

	stream.data = (int16_t *) malloc( length * sizeof (int16_t) );
	stream.length = length;
	stream.offset = 0;

	return stream;
}

void CA_ResizeStream( ca_stream_t *stream, int length )
{
	if ( stream->length == 0 )
		stream->data = (int16_t *) malloc( length * sizeof (int16_t) );
	else
		stream->data = (int16_t *) realloc( stream->data, length * sizeof (int16_t) );

	stream->length = length;
}

void CA_FreeStream( ca_stream_t *stream )
{
	if ( stream->length == 0 )
		return;

	stream->length = 0;
	stream->offset = 0;
	free( stream->data );
}

ca_oarray_t CA_AllocateOscilloscopeArray( int length )
{
	ca_oarray_t oarray;

	oarray.values = (float *) malloc( length * sizeof (float) );
	oarray.length = length;
	oarray.status = 0;

	return oarray;
}

void CA_FreeOscilloscopeArray( ca_oarray_t *oarray )
{
	if ( oarray->length == 0 )
		return;

	oarray->status = 0;
	oarray->length = 0;
	free( oarray->values );
}

ca_sclass_t CA_ClassifyStreamByte( uint8_t b )
{
	if ( b >> 5 == 0b001 )
		return CA_SCLASS_END;
	else if ( b >> 5 == 0b000 )
		return CA_SCLASS_13BIT;
	else if ( b >> 6 == 0b01 )
		return CA_SCLASS_3BIT;
	else if ( b >> 7 == 0b1 )
		return CA_SCLASS_7BIT;

	return CA_SCLASS_UNKNOWN;
}

void CA_Add3BitToStream( ca_stream_t *stream, uint8_t b )
{
	int16_t value;

	if ( stream->offset > stream->length - 2 )
		CA_ResizeStream( stream, stream->length + CA_STREAM_CHUNKSIZE );

	value = stream->offset < 1 ? 0 : stream->data[stream->offset - 1];

	stream->data[stream->offset++] = value + CA_SignExtendByte( (b >> 3) & 0b111, 3 );
	stream->data[stream->offset++] = value + CA_SignExtendByte( b & 0b111, 3 );
}

void CA_Add7BitToStream( ca_stream_t *stream, uint8_t b )
{
	int16_t value;

	if ( stream->offset > stream->length - 1 )
		CA_ResizeStream( stream, stream->length + CA_STREAM_CHUNKSIZE );

	value = stream->offset < 1 ? 0 : stream->data[stream->offset - 1];

	stream->data[stream->offset++] = value + CA_SignExtendByte( b, 7 );
}

void CA_Add13BitToStream( ca_stream_t *stream, uint16_t s )
{
	if ( stream->offset > stream->length - 1 )
		CA_ResizeStream( stream, stream->length + CA_STREAM_CHUNKSIZE );

	stream->data[stream->offset++] = CA_SignExtendShort( s, 13 );
}

ca_oarray_t CA_StreamToOscilloscopeArray( ca_stream_t stream, ca_range_t range )
{
	ca_oarray_t oarray;
	int i;

	oarray = CA_AllocateOscilloscopeArray( stream.offset );

	for ( i = 0; i < stream.offset; i++ )
		oarray.values[i] = CA_ConvertToRange( stream.data[i], range );

	return oarray;
}