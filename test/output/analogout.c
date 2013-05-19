#include <stdio.h>
#include <libcassy.h>

int main( int argc, char **argv )
{
	ca_handle_t handle;
	ca_cassy_t cassy;

	if ( argc < 3 )
	{
		printf( "Usage: analogout /dev/node voltage\n" );
		return 0;
	}

	CA_Init();

	handle = CA_GetDeviceHandle( argv[1] );
	cassy = CA_OpenCassy( handle, CA_VERSION_SENSORCASSY2, 1 );

	CA_SetOutputValueX( cassy, CA_RANGE_10V, atof( argv[2] ) );

	CA_CloseDeviceHandle( handle );

	CA_Deinit();

	return 0;
}
