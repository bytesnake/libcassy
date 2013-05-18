#include <stdio.h>
#include <libcassy.h>

int main( int argc, char **argv )
{
	ca_handle_t handle;
	ca_cassy_t cassy;

	if ( argc < 2 )
	{
		printf( "Usage: checkmemory /dev/node\n" );
		return 0;
	}

	CA_Init();

	handle = CA_GetDeviceHandle( argv[1] );
	cassy = CA_OpenCassy( handle, CA_VERSION_SENSORCASSY2, 1 );

	printf( "Erasing user data sector around 0x4242...\n" );
	CA_EraseUserDataSector( cassy, 0x4242 );

	printf( "Writing 1337 into the memory location 0x4242...\n" );
	CA_WriteUserData( cassy, 0x4242, 1337 );

	printf( "Memory at location 0x4242 reads: %i.\n", CA_ReadUserData( cassy, 0x4242 ) );

	CA_CloseDeviceHandle( handle );

	CA_Deinit();

	return 0;
}
