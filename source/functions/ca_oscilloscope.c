#include "../../include/libcassy.h"

void CA_ResetOscilloscope( ca_cassy_t cassy, uint16_t dt, uint16_t n, uint8_t trigger, uint16_t trigval )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_RESETOSCILLOSCOPE, 7 );
	CA_WriteShortToData( command, 1, dt );
	CA_WriteShortToData( command, 3, n );
	CA_WriteByteToData( command, 5, trigger );
	CA_WriteShortToData( command, 6, trigval );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

void CA_ResetOscilloscope2( ca_cassy_t cassy, uint32_t dt, uint16_t mean, uint32_t n0, uint32_t n1, uint8_t mask, uint8_t trigger, uint16_t trigval )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_RESETOSCILLOSCOPE2, 18 );
	CA_WriteIntToData( command, 1, dt );
	CA_WriteShortToData( command, 5, mean );
	CA_WriteIntToData( command, 7, n0 );
	CA_WriteIntToData( command, 11, n1 );
	CA_WriteByteToData( command, 15, mask );
	CA_WriteByteToData( command, 16, trigger );
	CA_WriteShortToData( command, 17, trigval );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

void CA_StartOscilloscope( ca_cassy_t cassy )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_STARTOSCILLOSCOPE, 0 );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

void CA_StopOscilloscope( ca_cassy_t cassy )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_STOPOSCILLOSCOPE, 0 );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

ca_ostatus_t CA_GetOscilloscopeStatus( ca_cassy_t cassy )
{
	ca_data_t command, response;
	ca_ostatus_t ostatus;

	command = CA_SetupCommandFrame( CA_FID_GETOSCILLOSCOPESTATUS, 0 );

	response = CA_ExecuteCommand( cassy, command, 4 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		memset( &ostatus, 0, sizeof ostatus );
	else
	{
		ostatus.status = CA_ReadByteFromData( response, 1 );
		ostatus.counts = CA_ReadShortFromData( response, 2 );
	}

	CA_FreeData( &response );

	return ostatus;
}

ca_ostatus2_t CA_GetOscilloscopeStatus2( ca_cassy_t cassy )
{
	ca_data_t command, response;
	ca_ostatus2_t ostatus;

	command = CA_SetupCommandFrame( CA_FID_GETOSCILLOSCOPESTATUS2, 0 );

	response = CA_ExecuteCommand( cassy, command, 18 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		memset( &ostatus, 0, sizeof ostatus );
	else
	{
		ostatus.status = CA_ReadByteFromData( response, 1 );
		ostatus.uia = CA_ReadIntFromData( response, 2 );
		ostatus.uib = CA_ReadIntFromData( response, 6 );
		ostatus.sensora = CA_ReadIntFromData( response, 10 );
		ostatus.sensorb = CA_ReadIntFromData( response, 14 );
	}

	CA_FreeData( &response );

	return ostatus;
}

ca_otime_t CA_GetOscilloscopeTime( ca_cassy_t cassy )
{
	ca_data_t command, response;
	ca_otime_t otime;

	command = CA_SetupCommandFrame( CA_FID_GETOSCILLOSCOPETIME, 0 );

	response = CA_ExecuteCommand( cassy, command, 6 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		memset( &otime, 0, sizeof otime );
	else
	{
		otime.status = CA_ReadByteFromData( response, 1 );
		otime.time = CA_ReadIntFromData( response, 2 );
	}

	CA_FreeData( &response );

	return otime;
}

ca_otime2_t CA_GetOscilloscopeTime2( ca_cassy_t cassy )
{
	ca_data_t command, response;
	ca_otime2_t otime;

	command = CA_SetupCommandFrame( CA_FID_GETOSCILLOSCOPETIME2, 0 );

	response = CA_ExecuteCommand( cassy, command, 12 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		memset( &otime, 0, sizeof otime );
	else
	{
		otime.status = CA_ReadByteFromData( response, 1 );
		otime.timehigh = CA_ReadIntFromData( response, 2 );
		otime.timelow = CA_ReadIntFromData( response, 6 );
		otime.delay = CA_ReadShortFromData( response, 12 );
	}

	CA_FreeData( &response );

	return otime;
}

ca_oarray_t CA_GetOscilloscopeArrayA( ca_cassy_t cassy, ca_range_t range, uint16_t start, uint16_t count, uint16_t delta )
{
	ca_data_t command;

	command = CA_SetupCommandFrame( CA_FID_GETOSCILLOSCOPEARRAYA, 6 );
	CA_WriteShortToData( command, 1, start );
	CA_WriteShortToData( command, 3, count );
	CA_WriteShortToData( command, 5, delta );

	CA_ExecuteCommand( cassy, command, CA_INOUT_NORESPONSE );
	CA_FreeData( &command );

	return CA_RecvOscilloscopeArray( cassy, range );
}

ca_oarray_t CA_GetOscilloscopeArrayB( ca_cassy_t cassy, ca_range_t range, uint16_t start, uint16_t count, uint16_t delta )
{
	ca_data_t command;

	command = CA_SetupCommandFrame( CA_FID_GETOSCILLOSCOPEARRAYB, 6 );
	CA_WriteShortToData( command, 1, start );
	CA_WriteShortToData( command, 3, count );
	CA_WriteShortToData( command, 5, delta );

	CA_ExecuteCommand( cassy, command, CA_INOUT_NORESPONSE );
	CA_FreeData( &command );

	return CA_RecvOscilloscopeArray( cassy, range );
}

ca_oarray_t CA_GetOscilloscopeArrayX( ca_cassy_t cassy, ca_range_t range, uint16_t start, uint16_t count, uint16_t delta )
{
	ca_data_t command;

	command = CA_SetupCommandFrame( CA_FID_GETOSCILLOSCOPEARRAYX, 6 );
	CA_WriteShortToData( command, 1, start );
	CA_WriteShortToData( command, 3, count );
	CA_WriteShortToData( command, 5, delta );

	CA_ExecuteCommand( cassy, command, CA_INOUT_NORESPONSE );
	CA_FreeData( &command );

	return CA_RecvOscilloscopeArray( cassy, range );
}

ca_oarray_t CA_GetOscilloscopeArrayY( ca_cassy_t cassy, ca_range_t range, uint16_t start, uint16_t count, uint16_t delta )
{
	ca_data_t command;

	command = CA_SetupCommandFrame( CA_FID_GETOSCILLOSCOPEARRAYY, 6 );
	CA_WriteShortToData( command, 1, start );
	CA_WriteShortToData( command, 3, count );
	CA_WriteShortToData( command, 5, delta );

	CA_ExecuteCommand( cassy, command, CA_INOUT_NORESPONSE );
	CA_FreeData( &command );

	return CA_RecvOscilloscopeArray( cassy, range );
}

ca_oarray_t CA_GetOscilloscopeArray2A( ca_cassy_t cassy, ca_range_t range, uint32_t skip, uint32_t count )
{
	ca_data_t command;

	command = CA_SetupCommandFrame( CA_FID_GETOSCILLOSCOPEARRAY2A, 9 );
	CA_WriteByteToData( command, 1, range );
	CA_WriteIntToData( command, 2, skip );
	CA_WriteIntToData( command, 6, count );

	CA_ExecuteCommand( cassy, command, CA_INOUT_NORESPONSE );
	CA_FreeData( &command );

	return CA_RecvOscilloscopeArray( cassy, range );
}

ca_oarray_t CA_GetOscilloscopeArray2B( ca_cassy_t cassy, ca_range_t range, uint32_t skip, uint32_t count )
{
	ca_data_t command;

	command = CA_SetupCommandFrame( CA_FID_GETOSCILLOSCOPEARRAY2B, 9 );
	CA_WriteByteToData( command, 1, range );
	CA_WriteIntToData( command, 2, skip );
	CA_WriteIntToData( command, 6, count );

	CA_ExecuteCommand( cassy, command, CA_INOUT_NORESPONSE );
	CA_FreeData( &command );

	return CA_RecvOscilloscopeArray( cassy, range );
}
