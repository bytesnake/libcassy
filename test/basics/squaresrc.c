#include <stdlib.h>
#include <string.h>
#include <libcassy.h>

int main( int argc, char **argv )
{
	ca_handle_t handle;
	ca_cassy_t cassy;

	if ( argc < 3 )
	{
		printf( "Usage: squaresrc /dev/node val\n" );
		return 0;
	}

	handle = CA_GetDeviceHandle( argv[1] );
	cassy = CA_OpenCassy( handle, CA_VERSION_SENSORCASSY2, 1 );

	CA_SetVoltage( cassy, atoi( argv[2] ) );

	CA_CloseDeviceHandle( handle );

	return 0;
}
