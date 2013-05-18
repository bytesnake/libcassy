#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <inttypes.h>
#include <libcassy.h>

typedef struct __attribute__((__packed__)) riffheader_s
{
	uint8_t format[4];
	uint32_t filesize;

	uint8_t filetypeheader[4];
	uint8_t chunkmarker[4];
	uint32_t formatlength;
	uint16_t formattype;
	uint16_t channels;
	uint32_t samplerate;
	uint32_t byterate;
	uint16_t bytespersamples;
	uint16_t bitspersample;

	uint8_t dataheader[4];
	uint32_t datasize;
} riffheader_t;

FILE *OpenWAVFile( char *filename )
{
	return fopen( filename, "w" );
}

void CloseWAVFile( FILE *wavfile )
{
	fclose( wavfile );
}

void WriteWAVHeader( FILE *wavfile, int samplerate, int samplenum )
{
	riffheader_t header;

	memcpy( header.format, "RIFF", 4 );
	memcpy( header.filetypeheader, "WAVE", 4 );
	memcpy( header.chunkmarker, "fmt ", 4 );
	memcpy( header.dataheader, "data", 4 );

	header.formatlength = 16;
	header.formattype = 1;
	header.channels = 1;

	header.samplerate = samplerate;
	header.bitspersample = 16;
	header.bytespersamples = header.bitspersample / 8;
	header.byterate = header.bytespersamples * samplerate;

	header.datasize = samplenum * 2;
	header.filesize = header.datasize + sizeof header;

	fwrite( &header, 1, sizeof header, wavfile );
}

void WriteWAVSamples( FILE *wavfile, float *samples, float amp, int samplenum )
{
	int i;
	uint16_t *bitsamples;

	bitsamples = (uint16_t *) malloc( samplenum * sizeof (uint16_t) );

	for ( i = 0; i < samplenum; i++ )
		bitsamples[i] = (amp / samples[i]) * 32767;

	fwrite( bitsamples, 1, 2 * samplenum, wavfile );
	free( bitsamples );
}

void ShowHelp()
{
	puts( "Usage: oscillowav usbid input range amplitude samplerate runtime output" );
	puts( "  usbid \t --  vid:pid for libusb and /dev/node for devnodes" );
	puts( "  input \t --  A or B" );
	puts( "  range \t --  3V, 10V, 30V, 0.1A, 0.3A or 1A" );
	puts( "  amplitude \t --  normal amplitude of the signal" );
	puts( "  samplerate \t --  preferrably a divisor of 10^6" );
	puts( "  runtime \t --  float in seconds" );
	puts( "  output \t --  output file name" );
}

ca_range_t GetRange( char *range )
{
	if ( strcmp( range, "3V" ) == 0 )
		return CA_RANGE_3V;
	else if ( strcmp( range, "10V" ) == 0 )
		return CA_RANGE_10V;
	else if ( strcmp( range, "30V" ) == 0 )
		return CA_RANGE_30V;
	else if ( strcmp( range, "0.1A" ) == 0 )
		return CA_RANGE_01A;
	else if ( strcmp( range, "0.3A" ) == 0 )
		return CA_RANGE_03A;
	else if ( strcmp( range, "1A" ) == 0 )
		return CA_RANGE_1A;

	return -1;
}

int GetInput( char *input )
{
	if ( strcmp( input, "A" ) == 0 )
		return 1;
	else if ( strcmp( input, "B" ) == 0 )
		return 2;

	return -1;
}

int main( int argc, char **argv )
{
	ca_handle_t handle;
	ca_cassy_t cassy;

	int input, samplerate;
	ca_range_t range;
	float amplitude, runtime;

	int interval, values;
	FILE *wavfile;
	ca_oarray_t oarray;
	int i;

	if ( argc < 8 )
	{
		ShowHelp();
		return 0;
	}

	//
	// checking variables
	//

	input = GetInput( argv[2] );
	range = GetRange( argv[3] );
	amplitude = atof( argv[4] );
	samplerate = atoi( argv[5] );
	runtime = atof( argv[6] );

	if ( input == -1 )
	{
		printf( "Error: Unknown input: \"%s\"\n", argv[2] );
		return 0;
	}
	else if ( range == -1 )
	{
		printf( "Error: Unknown range: \"%s\"\n", argv[3] );
		return 0;
	}
	else if ( amplitude <= 0 || amplitude >= CA_ConvertToRange( 2000, range ) )
	{
		printf( "Error: Nonnumeric, negative or too high amplitude: \"%s\"\n", argv[4] );
	}
	else if ( samplerate <= 0 || samplerate > 1000000 )
	{
		printf( "Error: Nonnumeric, negative or too high samplerate: \"%s\"\n", argv[5] );
		return 0;
	}
	else if ( runtime < 2.0 / samplerate )
	{
		printf( "Error: Nonnumeric, negative or too short runtime: \"%s\"\n", argv[6] );
		return 0;
	}

	if ( 1000000 % samplerate != 0 )
		printf( "Warning: 10^6 cannot be evenly divided by samplerate: \"%s\"\n", argv[5] );

	interval = 1000000 / samplerate;
	values = runtime * (1000000 / interval); // != runtime * samplerate

	//
	// device IO and generating .wav file
	//

	CA_Init();

	printf( "Opening cassy device 1 under usbid: \"%s\"\n", argv[1] );

	handle = CA_GetDeviceHandle( argv[1] );
	cassy = CA_OpenCassy( handle, CA_VERSION_SENSORCASSY2, 1 );

	printf( "Initializing oscilliscope under input: \"%s\"\n", argv[2] );

	if ( input == 1 )
	{
		CA_GetInputValueA( cassy, range );
		CA_ResetOscilloscope2( cassy, interval, 1, 0, values, CA_OMASK_4MMSOCKA, CA_OTRIG_IMMEDIATE, 0 );
	}
	else
	{
		CA_GetInputValueB( cassy, range );
		CA_ResetOscilloscope2( cassy, interval, 1, 0, values, CA_OMASK_4MMSOCKB, CA_OTRIG_IMMEDIATE, 0 );
	}

	puts( "Starting oscilloscope and creating .wav file" );

	CA_StartOscilloscope( cassy );
	wavfile = OpenWAVFile( argv[7] );
	WriteWAVHeader( wavfile, samplerate, values );

	printf( "Receiving sample batches" );

	i = 0;

	while ( i < values )
	{
		if ( input == 1 )
			oarray = CA_GetOscilloscopeArray2A( cassy, range, 0, values );
		else
			oarray = CA_GetOscilloscopeArray2B( cassy, range, 0, values );

		WriteWAVSamples( wavfile, oarray.values, amplitude, i + oarray.length > values ? values - i : oarray.length );
		i += oarray.length;

		if ( oarray.status == CA_OSTATUS_STOPPED )
		{
			if ( i < values )
				puts( "Warning: Oscilloscope stopped unexpectedly" );

			CA_FreeOscilloscopeArray( &oarray );
			break;
		}

		CA_FreeOscilloscopeArray( &oarray );
	}

	printf( "\nSaving .wav file and closing device handle for usbid: \"%s\"\n", argv[1] );

	CloseWAVFile( wavfile );

	CA_CloseDeviceHandle( handle );

	CA_Deinit();

	return 0;
}