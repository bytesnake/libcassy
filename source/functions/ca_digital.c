#include "../../include/libcassy.h"

uint8_t CA_GetBitInput( ca_cassy_t cassy, uint8_t bit )
{
	ca_data_t command, response;
	uint8_t value;

	command = CA_SetupCommandFrame( CA_FID_GETBITINPUT, 1 );
	CA_WriteByteToData( command, 1, bit );

	response = CA_ExecuteCommand( cassy, command, 2 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		value = 0x00;
	else
		value = CA_ReadByteFromData( response, 1 );

	CA_FreeData( &response );

	return value;
}

void CA_SetBitOutput( ca_cassy_t cassy, uint8_t bit, uint8_t value )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_SETBITOUTPUT, 2 );
	CA_WriteByteToData( command, 1, bit );
	CA_WriteByteToData( command, 2, value );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

uint16_t CA_GetDigitalInputs( ca_cassy_t cassy )
{
	ca_data_t command, response;
	uint16_t values;

	command = CA_SetupCommandFrame( CA_FID_GETDIGITALINPUTS, 0 );

	response = CA_ExecuteCommand( cassy, command, 3 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		values = 0x0000;
	else
		values = CA_ReadShortFromData( response, 1 );

	CA_FreeData( &response );

	return CA_SwitchShort( values );
}

void CA_SetDigitalOutputs( ca_cassy_t cassy, uint16_t values )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_SETDIGITALOUTPUTS, 2 );
	CA_WriteShortToData( command, 1, values );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

void CA_SetCounterMode( ca_cassy_t cassy, uint8_t bit, uint8_t mode )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_SETCOUNTERMODE, 2 );
	CA_WriteByteToData( command, 1, bit );
	CA_WriteByteToData( command, 2, mode );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

void CA_SetCounterDelay( ca_cassy_t cassy, uint16_t delay )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_SETCOUNTERDELAY, 2 );
	CA_WriteShortToData( command, 1, delay );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

uint16_t CA_GetCounter( ca_cassy_t cassy, uint8_t bit )
{
	ca_data_t command, response;
	uint16_t value;

	command = CA_SetupCommandFrame( CA_FID_GETCOUNTER, 1 );
	CA_WriteByteToData( command, 1, bit );

	response = CA_ExecuteCommand( cassy, command, 3 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		value = 0x0000;
	else
		value = CA_ReadShortFromData( response, 1 );

	CA_FreeData( &response );

	return CA_SwitchShort( value );
}

void CA_SetCounter( ca_cassy_t cassy, uint8_t bit, uint16_t value )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_SETCOUNTER, 3 );
	CA_WriteByteToData( command, 1, bit );
	CA_WriteShortToData( command, 2, value );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

void CA_SetReferencePoint( ca_cassy_t cassy, uint8_t refpoint )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_SETREFERENCEPOINT, 1 );
	CA_WriteByteToData( command, 1, refpoint );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

void CA_SetStepperDelay( ca_cassy_t cassy, uint16_t delay )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_SETSTEPPERDELAY, 1 );
	CA_WriteShortToData( command, 1, delay );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

ca_mpos_t CA_GetPhysicalPosition( ca_cassy_t cassy )
{
	ca_data_t command, response;
	ca_mpos_t position;

	command = CA_SetupCommandFrame( CA_FID_GETPHYSICALPOSITION, 0 );

	response = CA_ExecuteCommand( cassy, command, 5 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		memset( &position, 0, sizeof position );
	else
	{
		position.xpos = CA_ReadShortFromData( response, 1 );
		position.ypos = CA_ReadShortFromData( response, 3 );
	}

	return position;
}

ca_mpos_t CA_GetLogicalPosition( ca_cassy_t cassy )
{
	ca_data_t command, response;
	ca_mpos_t position;

	command = CA_SetupCommandFrame( CA_FID_GETLOGICALPOSITION, 0 );

	response = CA_ExecuteCommand( cassy, command, 5 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		memset( &position, 0, sizeof position );
	else
	{
		position.xpos = CA_ReadShortFromData( response, 1 );
		position.ypos = CA_ReadShortFromData( response, 3 );
	}

	return position;
}

void CA_MoveToAbsPosition( ca_cassy_t cassy, ca_mpos_t position )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_MOVETOABSPOSITION, 4 );
	CA_WriteShortToData( command, 1, position.xpos );
	CA_WriteShortToData( command, 3, position.ypos );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

void CA_MoveToRelPosition( ca_cassy_t cassy, ca_mpos_t position )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_MOVETORELPOSITION, 4 );
	CA_WriteShortToData( command, 1, position.xpos );
	CA_WriteShortToData( command, 3, position.ypos );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}
