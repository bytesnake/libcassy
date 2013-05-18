#include "../../include/libcassy.h"

void CA_SetOutputValueX( ca_cassy_t cassy, ca_range_t range, float value )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_SETOUTPUTVALUEX, 3 );
	CA_WriteByteToData( command, 1, range );
	CA_WriteShortToData( command, 2, CA_ConvertFromRange( value, range ) );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

void CA_LimitOutputRangeX( ca_cassy_t cassy, ca_range_t range, float min, float max )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_LIMITOUTPUTRANGEX, 4 );
	CA_WriteShortToData( command, 1, CA_ConvertFromRange( min, range ) );
	CA_WriteShortToData( command, 3, CA_ConvertFromRange( max, range ) );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

void CA_SetOutputValueY( ca_cassy_t cassy, float value )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_SETOUTPUTVALUEY, 3 );
	CA_WriteByteToData( command, 1, CA_RANGE_10V );
	CA_WriteShortToData( command, 2, CA_ConvertFromRange( value, CA_RANGE_10V ) );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

void CA_LimitOutputRangeY( ca_cassy_t cassy, float min, float max )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_LIMITOUTPUTRANGEY, 4 );
	CA_WriteShortToData( command, 1, CA_ConvertFromRange( min, CA_RANGE_10V ) );
	CA_WriteShortToData( command, 3, CA_ConvertFromRange( max, CA_RANGE_10V ) );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}