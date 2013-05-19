#include <stdlib.h>
#include <string.h>
#include <libcassy.h>

int main( int argc, char **argv )
{
	ca_handle_t handle;
	ca_cassy_t cassy;

	if ( argc < 3 )
	{
		printf( "Usage: relay /dev/node (on|off)\n" );
		return 0;
	}

	CA_Init();

	handle = CA_GetDeviceHandle( argv[1] );
	cassy = CA_OpenCassy( handle, CA_VERSION_SENSORCASSY2, 1 );

	if ( strcmp( argv[2], "on" ) == 0 )
	{
		printf( "Activating relay...\n" );
		CA_SetRelay( cassy, 1 );
	}
	else if ( strcmp( argv[2], "off" ) == 0 )
	{
		printf( "Deactivating relay...\n" );
		CA_SetRelay( cassy, 0 );
	}
	else
		printf( "Usage: relay /dev/node (on|off)\n" );

	CA_CloseDeviceHandle( handle );

	CA_Deinit();

	return 0;
}
