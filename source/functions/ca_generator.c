#include "../../include/libcassy.h"

void CA_ResetFunctionGenerator( ca_cassy_t cassy, ca_wave_t wave, uint32_t frequency, float amplitude, float offset, uint8_t ratio )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_RESETFUNCTIONGENERATOR, 10 );
	CA_WriteByteToData( command, 1, wave );
	CA_WriteIntToData( command, 2, frequency );
	CA_WriteShortToData( command, 6, CA_ConvertFromRange( amplitude, CA_RANGE_10V ) );
	CA_WriteShortToData( command, 8, CA_ConvertFromRange( offset, CA_RANGE_10V ) );
	CA_WriteByteToData( command, 10, ratio );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

uint16_t CA_DefineWaveForm( ca_cassy_t cassy, int16_t *values, uint16_t length )
{
	ca_data_t command, response;
	uint16_t count;

	command = CA_SetupStreamCommandFrame( CA_FID_DEFINEWAVEFORM, values, length );

	response = CA_ExecuteCommand( cassy, command, 3 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		count = 0;
	else
		count = CA_ReadShortFromData( response, 1 );

	CA_FreeData( &response );

	return count;
}

void CA_StartFunctionGenerator( ca_cassy_t cassy )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_STARTFUNCTIONGENERATOR, 0 );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

void CA_StopFunctionGenerator( ca_cassy_t cassy )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_STOPFUNCTIONGENERATOR, 0 );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

ca_gstatus_t CA_GetFunctionGeneratorStatus( ca_cassy_t cassy )
{
	ca_data_t command, response;
	ca_gstatus_t gstatus;

	command = CA_SetupCommandFrame( CA_FID_GETFUNCTIONGENERATORSTATUS, 0 );

	response = CA_ExecuteCommand( cassy, command, 4 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		memset( &gstatus, 0, sizeof gstatus );
	else
	{
		gstatus.status = CA_ReadByteFromData( response, 1 );
		gstatus.counts = CA_ReadShortFromData( response, 2 );
	}

	CA_FreeData( &response );

	return gstatus;
}
