#include <stdio.h>
#include <unistd.h>
#include <libcassy.h>

int main( int argc, char **argv )
{
	ca_handle_t handle;
	ca_cassy_t cassy;
	ca_oarray_t oarray;
	int i;

	if ( argc < 2 )
	{
		printf( "Usage: oscillotest /dev/node\n" );
		return 0;
	}

	handle = CA_GetDeviceHandle( argv[1] );
	cassy = CA_OpenCassy( handle, CA_VERSION_SENSORCASSY2, 1 );

	CA_GetInputValueA( cassy, CA_RANGE_10V );

	CA_ResetOscilloscope2( cassy, 100, 10, 0, 10000, CA_OMASK_4MMSOCKA, CA_OTRIG_IMMEDIATE, 0 );
	CA_StartOscilloscope( cassy );

	usleep( 1000 * 1250 ); // should be done after 1.25 seconds

	oarray = CA_GetOscilloscopeArray2A( cassy, CA_RANGE_10V, 0, 10000 );

	printf( "# Oscilloscope data from %s - status %i\n", argv[1], oarray.status );

	for ( i = 0; i < oarray.length; i++ )
		printf( "%i\t%f\n", i, oarray.values[i] );

	CA_FreeOscilloscopeArray( &oarray );

	CA_CloseDeviceHandle( handle );

	return 0;
}