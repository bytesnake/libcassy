#include "../../include/libcassy.h"

int8_t CA_GetSensorBoxA( ca_cassy_t cassy )
{
	ca_data_t command, response;
	int8_t box;

	command = CA_SetupCommandFrame( CA_FID_GETSENSORBOXA, 0 );

	response = CA_ExecuteCommand( cassy, command, 2 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		box = -1;
	else
		box = CA_ReadByteFromData( response, 1 );

	CA_FreeData( &response );

	return box;
}

int8_t CA_GetSensorBoxB( ca_cassy_t cassy )
{
	ca_data_t command, response;
	int8_t box;

	command = CA_SetupCommandFrame( CA_FID_GETSENSORBOXB, 0 );

	response = CA_ExecuteCommand( cassy, command, 2 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		box = -1;
	else
		box = CA_ReadByteFromData( response, 1 );

	CA_FreeData( &response );

	return box;
}

int8_t CA_GetSensorBoxA2( ca_cassy_t cassy, int32_t expected )
{
	ca_data_t command, response;
	int8_t box;

	command = CA_SetupCommandFrame( CA_FID_GETSENSORBOXA2, 4 );
	CA_WriteIntToData( command, 1, expected );

	response = CA_ExecuteCommand( cassy, command, 2 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		box = -1;
	else
		box = CA_ReadByteFromData( response, 1 );

	CA_FreeData( &response );

	return box;
}

int8_t CA_GetSensorBoxB2( ca_cassy_t cassy, int32_t expected )
{
	ca_data_t command, response;
	int8_t box;

	command = CA_SetupCommandFrame( CA_FID_GETSENSORBOXB2, 4 );
	CA_WriteIntToData( command, 1, expected );

	response = CA_ExecuteCommand( cassy, command, 2 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		box = -1;
	else
		box = CA_ReadByteFromData( response, 1 );

	CA_FreeData( &response );

	return box;
}

int8_t CA_GetControlMode( ca_cassy_t cassy )
{
	ca_data_t command, response;
	int8_t mode;

	command = CA_SetupCommandFrame( CA_FID_GETCONTROLMODE, 0 );

	response = CA_ExecuteCommand( cassy, command, 2 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		mode = -1;
	else
		mode = CA_ReadByteFromData( response, 1 );

	CA_FreeData( &response );

	return mode;
}

int8_t CA_GetMeasurementMode( ca_cassy_t cassy )
{
	ca_data_t command, response;
	int8_t mode;

	command = CA_SetupCommandFrame( CA_FID_GETMEASUREMENTMODE, 0 );

	response = CA_ExecuteCommand( cassy, command, 2 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		mode = -1;
	else
		mode = CA_ReadByteFromData( response, 1 );

	CA_FreeData( &response );

	return mode;
}

float CA_GetInputValueA( ca_cassy_t cassy, ca_range_t range )
{
	ca_data_t command, response;
	uint16_t digits;

	command = CA_SetupCommandFrame( CA_FID_GETINPUTVALUEA, 1 );
	CA_WriteByteToData( command, 1, range );

	response = CA_ExecuteCommand( cassy, command, 3 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		digits = 0x0000;
	else
		digits = CA_ReadShortFromData( response, 1 );

	CA_FreeData( &response );

	return CA_ExtendToRange( digits, 12, range );
}

float CA_GetInputValueB( ca_cassy_t cassy, ca_range_t range )
{
	ca_data_t command, response;
	uint16_t digits;

	command = CA_SetupCommandFrame( CA_FID_GETINPUTVALUEB, 1 );
	CA_WriteByteToData( command, 1, range );

	response = CA_ExecuteCommand( cassy, command, 3 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		digits = 0x0000;
	else
		digits = CA_ReadShortFromData( response, 1 );

	CA_FreeData( &response );

	return CA_ExtendToRange( digits, 12, range );
}

float CA_GetValueUI( ca_cassy_t cassy, ca_range_t range )
{
	ca_data_t command, response;
	uint16_t digits;

	command = CA_SetupCommandFrame( CA_FID_GETVALUEUI, 1 );
	CA_WriteByteToData( command, 1, range );

	response = CA_ExecuteCommand( cassy, command, 3 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		digits = 0x0000;
	else
		digits = CA_ReadShortFromData( response, 1 );

	CA_FreeData( &response );

	return CA_ExtendToRange( digits, 12, range );
}

float CA_GetValueIU( ca_cassy_t cassy, ca_range_t range )
{
	ca_data_t command, response;
	uint16_t digits;

	command = CA_SetupCommandFrame( CA_FID_GETVALUEIU, 1 );
	CA_WriteByteToData( command, 1, range );

	response = CA_ExecuteCommand( cassy, command, 3 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		digits = 0x0000;
	else
		digits = CA_ReadShortFromData( response, 1 );

	CA_FreeData( &response );

	return CA_ExtendToRange( digits, 12, range );
}

float CA_GetValueA( ca_cassy_t cassy )
{
	ca_data_t command, response;
	uint16_t digits;

	command = CA_SetupCommandFrame( CA_FID_GETVALUEA, 1 );

	response = CA_ExecuteCommand( cassy, command, 3 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		digits = 0x0000;
	else
		digits = CA_ReadShortFromData( response, 1 );

	CA_FreeData( &response );

	return CA_ExtendToRange( digits, 12, CA_RANGE_10V );
}

float CA_GetValueB( ca_cassy_t cassy )
{
	ca_data_t command, response;
	uint16_t digits;

	command = CA_SetupCommandFrame( CA_FID_GETVALUEB, 1 );

	response = CA_ExecuteCommand( cassy, command, 3 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		digits = 0x0000;
	else
		digits = CA_ReadShortFromData( response, 1 );

	CA_FreeData( &response );

	return CA_ExtendToRange( digits, 12, CA_RANGE_10V );
}

float CA_GetValueX( ca_cassy_t cassy )
{
	ca_data_t command, response;
	uint16_t digits;

	command = CA_SetupCommandFrame( CA_FID_GETVALUEX, 1 );

	response = CA_ExecuteCommand( cassy, command, 3 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		digits = 0x0000;
	else
		digits = CA_ReadShortFromData( response, 1 );

	CA_FreeData( &response );

	return CA_ExtendToRange( digits, 12, CA_RANGE_10V );
}

float CA_GetValueY( ca_cassy_t cassy )
{
	ca_data_t command, response;
	uint16_t digits;

	command = CA_SetupCommandFrame( CA_FID_GETVALUEY, 1 );

	response = CA_ExecuteCommand( cassy, command, 3 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		digits = 0x0000;
	else
		digits = CA_ReadShortFromData( response, 1 );

	CA_FreeData( &response );

	return CA_ExtendToRange( digits, 12, CA_RANGE_10V );
}

ca_mmms_t CA_GetMMMSInputValueA( ca_cassy_t cassy, ca_range_t range )
{
	ca_data_t command, response;
	ca_mmms_t values;

	// TODO: JWM support

	command = CA_SetupCommandFrame( CA_FID_GETMMMSINPUTVALUEA, 1 );
	CA_WriteByteToData( command, 1, range );

	response = CA_ExecuteCommand( cassy, command, 11 );
	CA_FreeData( &response );

	if ( CA_IsCassyError( response ) )
		memset( &values, 0, sizeof values );
	else
	{
		values.min = CA_ExtendToRange( CA_ReadShortFromData( response, 1 ), 12, range );
		values.max = CA_ExtendToRange( CA_ReadShortFromData( response, 3 ), 12, range );
		values.mean = CA_ExtendToRange( CA_ReadShortFromData( response, 5 ), 12, range );
		values.square = CA_ConvertToRange( CA_ReadIntFromData( response, 7 ), range );
	}

	CA_FreeData( &response );

	return values;
}

ca_mmms_t CA_GetMMMSInputValueB( ca_cassy_t cassy, ca_range_t range )
{
	ca_data_t command, response;
	ca_mmms_t values;

	// TODO: JWM support

	command = CA_SetupCommandFrame( CA_FID_GETMMMSINPUTVALUEB, 1 );
	CA_WriteByteToData( command, 1, range );

	response = CA_ExecuteCommand( cassy, command, 11 );
	CA_FreeData( &response );

	if ( CA_IsCassyError( response ) )
		memset( &values, 0, sizeof values );
	else
	{
		values.min = CA_ExtendToRange( CA_ReadShortFromData( response, 1 ), 12, range );
		values.max = CA_ExtendToRange( CA_ReadShortFromData( response, 3 ), 12, range );
		values.mean = CA_ExtendToRange( CA_ReadShortFromData( response, 5 ), 12, range );
		values.square = CA_ConvertToRange( CA_ReadIntFromData( response, 7 ), range );
	}

	CA_FreeData( &response );

	return values;
}

ca_mmms_t CA_GetMMMSValueUI( ca_cassy_t cassy, ca_range_t range )
{
	ca_data_t command, response;
	ca_mmms_t values;

	command = CA_SetupCommandFrame( CA_FID_GETMMMSVALUEUI, 1 );
	CA_WriteByteToData( command, 1, range );

	response = CA_ExecuteCommand( cassy, command, 11 );
	CA_FreeData( &response );

	if ( CA_IsCassyError( response ) )
		memset( &values, 0, sizeof values );
	else
	{
		values.min = CA_ExtendToRange( CA_ReadShortFromData( response, 1 ), 12, range );
		values.max = CA_ExtendToRange( CA_ReadShortFromData( response, 3 ), 12, range );
		values.mean = CA_ExtendToRange( CA_ReadShortFromData( response, 5 ), 12, range );
		values.square = CA_ConvertToRange( CA_ReadIntFromData( response, 7 ), range );
	}

	CA_FreeData( &response );

	return values;
}

ca_mmms_t CA_GetMMMSValueIU( ca_cassy_t cassy, ca_range_t range )
{
	ca_data_t command, response;
	ca_mmms_t values;

	command = CA_SetupCommandFrame( CA_FID_GETMMMSVALUEIU, 1 );
	CA_WriteByteToData( command, 1, range );

	response = CA_ExecuteCommand( cassy, command, 11 );
	CA_FreeData( &response );

	if ( CA_IsCassyError( response ) )
		memset( &values, 0, sizeof values );
	else
	{
		values.min = CA_ExtendToRange( CA_ReadShortFromData( response, 1 ), 12, range );
		values.max = CA_ExtendToRange( CA_ReadShortFromData( response, 3 ), 12, range );
		values.mean = CA_ExtendToRange( CA_ReadShortFromData( response, 5 ), 12, range );
		values.square = CA_ConvertToRange( CA_ReadIntFromData( response, 7 ), range );
	}

	CA_FreeData( &response );

	return values;
}

ca_mmms_t CA_GetMMMSValueA( ca_cassy_t cassy )
{
	ca_data_t command, response;
	ca_mmms_t values;

	command = CA_SetupCommandFrame( CA_FID_GETMMMSVALUEA, 1 );

	response = CA_ExecuteCommand( cassy, command, 11 );
	CA_FreeData( &response );

	if ( CA_IsCassyError( response ) )
		memset( &values, 0, sizeof values );
	else
	{
		values.min = CA_ExtendToRange( CA_ReadShortFromData( response, 1 ), 12, CA_RANGE_10V );
		values.max = CA_ExtendToRange( CA_ReadShortFromData( response, 3 ), 12, CA_RANGE_10V );
		values.mean = CA_ExtendToRange( CA_ReadShortFromData( response, 5 ), 12, CA_RANGE_10V );
		values.square = CA_ConvertToRange( CA_ReadIntFromData( response, 7 ), CA_RANGE_10V );
	}

	CA_FreeData( &response );

	return values;
}

ca_mmms_t CA_GetMMMSValueB( ca_cassy_t cassy )
{
	ca_data_t command, response;
	ca_mmms_t values;

	command = CA_SetupCommandFrame( CA_FID_GETMMMSVALUEB, 1 );

	response = CA_ExecuteCommand( cassy, command, 11 );
	CA_FreeData( &response );

	if ( CA_IsCassyError( response ) )
		memset( &values, 0, sizeof values );
	else
	{
		values.min = CA_ExtendToRange( CA_ReadShortFromData( response, 1 ), 12, CA_RANGE_10V );
		values.max = CA_ExtendToRange( CA_ReadShortFromData( response, 3 ), 12, CA_RANGE_10V );
		values.mean = CA_ExtendToRange( CA_ReadShortFromData( response, 5 ), 12, CA_RANGE_10V );
		values.square = CA_ConvertToRange( CA_ReadIntFromData( response, 7 ), CA_RANGE_10V );
	}

	CA_FreeData( &response );

	return values;
}

ca_mmms_t CA_GetMMMSValueX( ca_cassy_t cassy )
{
	ca_data_t command, response;
	ca_mmms_t values;

	command = CA_SetupCommandFrame( CA_FID_GETMMMSVALUEX, 1 );

	response = CA_ExecuteCommand( cassy, command, 11 );
	CA_FreeData( &response );

	if ( CA_IsCassyError( response ) )
		memset( &values, 0, sizeof values );
	else
	{
		values.min = CA_ExtendToRange( CA_ReadShortFromData( response, 1 ), 12, CA_RANGE_10V );
		values.max = CA_ExtendToRange( CA_ReadShortFromData( response, 3 ), 12, CA_RANGE_10V );
		values.mean = CA_ExtendToRange( CA_ReadShortFromData( response, 5 ), 12, CA_RANGE_10V );
		values.square = CA_ConvertToRange( CA_ReadIntFromData( response, 7 ), CA_RANGE_10V );
	}

	CA_FreeData( &response );

	return values;
}

ca_mmms_t CA_GetMMMSValueY( ca_cassy_t cassy )
{
	ca_data_t command, response;
	ca_mmms_t values;

	command = CA_SetupCommandFrame( CA_FID_GETMMMSVALUEY, 1 );

	response = CA_ExecuteCommand( cassy, command, 11 );
	CA_FreeData( &response );

	if ( CA_IsCassyError( response ) )
		memset( &values, 0, sizeof values );
	else
	{
		values.min = CA_ExtendToRange( CA_ReadShortFromData( response, 1 ), 12, CA_RANGE_10V );
		values.max = CA_ExtendToRange( CA_ReadShortFromData( response, 3 ), 12, CA_RANGE_10V );
		values.mean = CA_ExtendToRange( CA_ReadShortFromData( response, 5 ), 12, CA_RANGE_10V );
		values.square = CA_ConvertToRange( CA_ReadIntFromData( response, 7 ), CA_RANGE_10V );
	}

	CA_FreeData( &response );

	return values;
}

float CA_GetCosInputValueAB( ca_cassy_t cassy, ca_range_t range )
{
	ca_data_t command, response;
	float sq1, sq2, p12;

	// TODO: JWM support

	command = CA_SetupCommandFrame( CA_FID_GETCOSINPUTVALUEAB, 1 );
	CA_WriteByteToData( command, 1, range );

	response = CA_ExecuteCommand( cassy, command, 13 );
	CA_FreeData( &response );

	if ( CA_IsCassyError( response ) )
	{
		sq1 = 0.0f;
		sq2 = 0.0f;
		p12 = 0.0f;
	}
	else
	{
		sq1 = CA_ConvertToRange( CA_ReadIntFromData( response, 1 ), range );
		sq2 = CA_ConvertToRange( CA_ReadIntFromData( response, 5 ), range );
		p12 = CA_ConvertToRange( CA_ReadIntFromData( response, 9 ), range );
	}

	return p12 * CA_RSqrt( sq1 * sq2 );
}

float CA_GetCosValueUI( ca_cassy_t cassy, ca_range_t range )
{
	ca_data_t command, response;
	float sq1, sq2, p12;

	command = CA_SetupCommandFrame( CA_FID_GETCOSVALUEUI, 1 );
	CA_WriteByteToData( command, 1, range );

	response = CA_ExecuteCommand( cassy, command, 13 );
	CA_FreeData( &response );

	if ( CA_IsCassyError( response ) )
	{
		sq1 = 0.0f;
		sq2 = 0.0f;
		p12 = 0.0f;
	}
	else
	{
		sq1 = CA_ConvertToRange( CA_ReadIntFromData( response, 1 ), range );
		sq2 = CA_ConvertToRange( CA_ReadIntFromData( response, 5 ), range );
		p12 = CA_ConvertToRange( CA_ReadIntFromData( response, 9 ), range );
	}

	return p12 / CA_RSqrt( sq1 * sq2 );
}

float CA_GetCosValueIU( ca_cassy_t cassy, ca_range_t range )
{
	ca_data_t command, response;
	float sq1, sq2, p12;

	command = CA_SetupCommandFrame( CA_FID_GETCOSVALUEIU, 1 );
	CA_WriteByteToData( command, 1, range );

	response = CA_ExecuteCommand( cassy, command, 13 );
	CA_FreeData( &response );

	if ( CA_IsCassyError( response ) )
	{
		sq1 = 0.0f;
		sq2 = 0.0f;
		p12 = 0.0f;
	}
	else
	{
		sq1 = CA_ConvertToRange( CA_ReadIntFromData( response, 1 ), range );
		sq2 = CA_ConvertToRange( CA_ReadIntFromData( response, 5 ), range );
		p12 = CA_ConvertToRange( CA_ReadIntFromData( response, 9 ), range );
	}

	return p12 / CA_RSqrt( sq1 * sq2 );
}

void CA_SendSerialSensorDataA( ca_cassy_t cassy, uint8_t bits, uint16_t data )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_SENDSERIALSENSORDATAA, 3 );
	CA_WriteByteToData( command, 1, bits );
	CA_WriteShortToData( command, 2, data );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

void CA_SendSerialSensorDataB( ca_cassy_t cassy, uint8_t bits, uint16_t data )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_SENDSERIALSENSORDATAB, 3 );
	CA_WriteByteToData( command, 1, bits );
	CA_WriteShortToData( command, 2, data );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

void CA_SetMeanTime( ca_cassy_t cassy, uint16_t time )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_SETMEANTIME, 2 );
	CA_WriteShortToData( command, 1, time );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}
