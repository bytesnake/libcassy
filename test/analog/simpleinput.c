#include <stdio.h>
#include <libcassy.h>

int main( int argc, char **argv )
{
	ca_handle_t handle;
	ca_cassy_t cassy;

	if ( argc < 2 )
	{
		printf( "Usage: simpleinput /dev/node\n" );
		return 0;
	}

	CA_Init();

	handle = CA_GetDeviceHandle( argv[1] );
	cassy = CA_OpenCassy( handle, CA_VERSION_SENSORCASSY2, 1 );

	printf( "Input A reads %f mA.\n", CA_GetInputValueA( cassy, CA_RANGE_03A ) * 1000 );
	printf( "Input B reads %f V.\n", CA_GetInputValueB( cassy, CA_RANGE_10V ) );

	CA_CloseDeviceHandle( handle );

	CA_Deinit();

	return 0;
}
