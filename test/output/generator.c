#include <stdio.h>
#include <unistd.h>
#include <libcassy.h>

int main( int argc, char **argv )
{
	ca_handle_t handle;
	ca_cassy_t cassy;
	FILE *input;
	int16_t values[8000];
	int length;

	if ( argc < 5 )
	{
		printf( "Usage: generator /dev/node freq ampl input.dat\n" );
		return 0;
	}

	CA_Init();

	handle = CA_GetDeviceHandle( argv[1] );
	cassy = CA_OpenCassy( handle, CA_VERSION_SENSORCASSY2, 1 );

	input = fopen( argv[4], "r" );
	length = 0;

	do {
		fscanf( input, "%i\n", values + length++ );
	} while ( feof( input ) == 0 );

	fclose( input );

	CA_ResetFunctionGenerator( cassy, CA_WAVE_USERDEFINED, atoi( argv[2] ), atof( argv[3] ), 0, 50 );
	CA_DefineWaveForm( cassy, values, length );

	CA_StartFunctionGenerator( cassy );
	usleep( 30 * 1000 * 1000 );
	CA_StopFunctionGenerator( cassy );

	CA_CloseDeviceHandle( handle );

	CA_Deinit();

	return 0;
}