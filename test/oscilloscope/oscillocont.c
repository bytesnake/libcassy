#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <libcassy.h>

void plotpoint( long time, long value, long max )
{
	struct winsize w;
	int columns, pos, i;

	ioctl( STDOUT_FILENO, TIOCGWINSZ, &w );
	columns = w.ws_col;

	pos = columns / 2 + ((columns / 2 - 8) * value) / max;

	printf( "%6i [", time );

	for ( i = 8; i < columns - 8; i++ )
	{
		if ( i == pos )
			printf( "X" );
		else
			printf( "-" );
	}

	printf( "] %6i\n", value );
}

int main( int argc, char **argv )
{
	ca_handle_t handle;
	ca_cassy_t cassy;
	ca_ostatus2_t ostatus;
	ca_oarray_t oarray;
	int interval, values, i, j;

	if ( argc < 4 )
	{
		printf( "Usage: oscillotest /dev/node interval values\n" );
		return 0;
	}

	CA_Init();

	interval = atoi( argv[2] );
	values = atoi( argv[3] );

	handle = CA_GetDeviceHandle( argv[1] );
	cassy = CA_OpenCassy( handle, CA_VERSION_SENSORCASSY2, 1 );

	CA_GetInputValueA( cassy, CA_RANGE_10V );

	CA_ResetOscilloscope2( cassy, interval, 1, 0, values, CA_OMASK_4MMSOCKA, CA_OTRIG_IMMEDIATE, 0 );
	CA_StartOscilloscope( cassy );

	oarray = CA_GetOscilloscopeArray2A( cassy, CA_RANGE_10V, 0, values );

	printf( "# Oscilloscope data from %s - interval: %i (ns), values: %i\n", argv[1], interval, values );

	i = 0;

	while ( i < values )
	{
		oarray = CA_GetOscilloscopeArray2A( cassy, CA_RANGE_10V, 0, values );

		for ( j = 0; j < oarray.length; j++ )
			plotpoint( (i + j) * interval / 1000, oarray.values[j] * 1000, 10000 );

		i += j;

		if ( oarray.status == CA_OSTATUS_STOPPED )
		{
			CA_FreeOscilloscopeArray( &oarray );
			break;
		}

		CA_FreeOscilloscopeArray( &oarray );
	}

	CA_StopOscilloscope( cassy );

	CA_CloseDeviceHandle( handle );

	CA_Deinit();

	return 0;
}
