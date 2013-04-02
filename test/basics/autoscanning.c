#include <stdio.h>
#include <stdlib.h>
#include <libcassy.h>

int main( int argc, char **argv )
{
	ca_device_t *devices;
	int i;

	CA_Init();

	devices = CA_FindDevices();
	i = 0;

	while ( devices[i].idstr != NULL )
	{
		printf( "idstr: %s\n", devices[i].idstr );
		printf( "version: %i\n", devices[i].version );

		i++;
	}

	CA_Deinit();

	return 0;
}
