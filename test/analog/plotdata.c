#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libcassy.h>

int main( int argc, char **argv )
{
	ca_handle_t handle;
	ca_cassy_t cassy;
	int i;

	if ( argc < 4 )
	{
		printf( "Usage: plotdata /dev/node count delay\n" );
		return 0;
	}

	handle = CA_GetDeviceHandle( argv[1] );
	cassy = CA_OpenCassy( handle, CA_VERSION_SENSORCASSY2, 1 );

	printf( "# i\tInput A (mA)\tInput B (V)\n" );

	for ( i = 0; i < atoi( argv[2] ); i++ )
	{
		printf( "%i\t%f\t%f\n", i, CA_GetInputValueA( cassy, CA_RANGE_03A ) * 1000, CA_GetInputValueB( cassy, CA_RANGE_10V ) );
		usleep( 1000 * atoi( argv[3] ) );
	}

	CA_CloseDeviceHandle( handle );

	return 0;
}