#include <stdio.h>
#include <libcassy.h>

//
// the library must have been compiled with the CA_DEBUG_NOWRITE flag
//

int main( int argc, char **argv )
{
	ca_handle_t handle;
	ca_cassy_t cassy;
	ca_oarray_t oarray;
	int i;

	if ( argc < 2 )
	{
		printf( "Usage: readstream stream(1|2).bin\n" );
		return 0;
	}

	handle = CA_GetDeviceHandle( argv[1] );
	cassy = CA_OpenCassy( handle, CA_VERSION_SENSORCASSY, 1 );

	oarray = CA_GetOscilloscopeArrayA( cassy, CA_RANGE_1V, 0, 100, 5 );

	printf( "# Encoded oscilloscope data from %s\n", argv[1] );

	for ( i = 0; i < oarray.length; i++ )
		printf( "%i\t%f\n", i, oarray.values[i] );

	CA_FreeOscilloscopeArray( &oarray );

	CA_CloseDeviceHandle( handle );

    return 0;
}