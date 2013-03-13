#include <stdio.h>
#include <unistd.h>
#include <libcassy.h>

int main( int argc, char **argv )
{
	ca_handle_t handle;
	ca_cassy_t cassy;
	ca_ostatus2_t ostatus;
	ca_oarray_t oarray;
	int interval, values, i;

	if ( argc < 4 )
	{
		printf( "Usage: oscillotest /dev/node interval values\n" );
		return 0;
	}

	interval = atoi( argv[2] );
	values = atoi( argv[3] );

	handle = CA_GetDeviceHandle( argv[1] );
	cassy = CA_OpenCassy( handle, CA_VERSION_SENSORCASSY2, 1 );

	CA_GetInputValueA( cassy, CA_RANGE_01V );

	CA_ResetOscilloscope2( cassy, interval, 1, 0, values, CA_OMASK_4MMSOCKA, CA_OTRIG_IMMEDIATE, 0 );
	CA_StartOscilloscope( cassy );

	do {
		ostatus = CA_GetOscilloscopeStatus2( cassy );
		usleep( 1000 * 500 );
	} while ( ostatus.status != CA_OSTATUS_STOPPED );

	oarray = CA_GetOscilloscopeArray2A( cassy, CA_RANGE_01V, 0, interval );

	printf( "# Oscilloscope data from %s - interval: %i (ns), values: %i\n", argv[1], interval, values );

	for ( i = 0; i < oarray.length; i++ )
		printf( "%f\t%f\n", i * interval * 0.001, 1000 * oarray.values[i] );

	CA_FreeOscilloscopeArray( &oarray );

	CA_CloseDeviceHandle( handle );

	return 0;
}
