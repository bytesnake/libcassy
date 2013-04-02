#include <stdio.h>
#include <sys/time.h>
#include <libcassy.h>

int main( int argc, char **argv )
{
	ca_handle_t handle;
	ca_cassy_t cassy;
	struct timeval tv1, tv2;
	double time;

	if ( argc < 2 )
	{
		printf( "Usage: simpleinput /dev/node\n" );
		return 0;
	}

	CA_Init();

	handle = CA_GetDeviceHandle( argv[1] );
	cassy = CA_OpenCassy( handle, CA_VERSION_SENSORCASSY2, 1 );

	time = (tv2.tv_sec - tv1.tv_sec) * 1000;
	time += (tv2.tv_usec - tv1.tv_usec) / 1000.0;

	printf( "time: %f\n", time );

	printf( "Input A reads %f mA.\n", CA_GetInputValueA( cassy, CA_RANGE_03A ) * 1000 );
	printf( "Input B reads %f V.\n", CA_GetInputValueB( cassy, CA_RANGE_10V ) );

	CA_CloseDeviceHandle( handle );

	CA_Deinit();

	return 0;
}
