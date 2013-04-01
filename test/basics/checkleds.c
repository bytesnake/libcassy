#include <stdio.h>
#include <unistd.h>
#include <libcassy.h>

int main( int argc, char **argv )
{
	ca_handle_t handle;
	ca_cassy_t cassy;
	int i;

	if ( argc < 2 )
	{
		printf( "Usage: checkleds /dev/node\n" );
		return 0;
	}

	CA_Init();

	handle = CA_GetDeviceHandle( argv[1] );
	cassy = CA_OpenCassy( handle, CA_VERSION_SENSORCASSY2, 1 );

	printf( "Current LED mode is %i.\n", CA_GetValue( cassy, 2 ) );
	printf( "Cycling LED brightness...\n" );

	for ( i = 0; i < 127; i += 8 )
	{
		CA_SetValue( cassy, 3, i );
		usleep( 1000 * 250 );
	}

	CA_SetValue( cassy, 3, 127 );

	CA_CloseDeviceHandle( handle );

	CA_Deinit();

	return 0;
}