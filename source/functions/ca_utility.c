#include "../../include/libcassy.h"

void CA_Reset( ca_cassy_t cassy )
{
	ca_data_t command;

	command = CA_SetupCommandFrame( CA_FID_RESET, 0 );

	CA_ExecuteCommand( cassy, command, CA_USB_NORESPONSE );
	CA_FreeData( &command );
}

uint16_t CA_GetHardwareVersion( ca_cassy_t cassy )
{
	ca_data_t command, response;
	uint16_t version;

	command = CA_SetupCommandFrame( CA_FID_GETHARDWAREVERSION, 0 );

	response = CA_ExecuteCommand( cassy, command, 3 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		version = 0x0000;
	else
		version = *(uint16_t *) (response.data + 1);

	CA_FreeData( &response );

	return CA_SwitchShort( version );
}

uint16_t CA_GetFirmwareVersion( ca_cassy_t cassy )
{
	ca_data_t command, response;
	uint16_t version;

	command = CA_SetupCommandFrame( CA_FID_GETFIRMWAREVERSION, 0 );

	response = CA_ExecuteCommand( cassy, command, 3 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		version = 0x0000;
	else
		version = *(uint16_t *) (response.data + 1);

	CA_FreeData( &response );

	return CA_SwitchShort( version );
}

void CA_EraseUserDataSector( ca_cassy_t cassy, uint16_t addr )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_ERASEUSERDATASECTOR, 2 );
	CA_WriteShortToData( command, 1, addr );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

void CA_WriteUserData( ca_cassy_t cassy, uint16_t addr, uint32_t data )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_WRITEUSERDATA, 6 );
	CA_WriteShortToData( command, 1, addr );
	CA_WriteIntToData( command, 3, data );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

uint32_t CA_ReadUserData( ca_cassy_t cassy, uint16_t addr )
{
	ca_data_t command, response;
	uint32_t data;

	command = CA_SetupCommandFrame( CA_FID_READUSERDATA, 2 );
	CA_WriteShortToData( command, 1, addr );

	response = CA_ExecuteCommand( cassy, command, 5 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		data = 0x0000000;
	else
		data = CA_ReadIntFromData( response, 1 );

	CA_FreeData( &response );

	return CA_SwitchInt( data );
}

uint8_t CA_GetValue( ca_cassy_t cassy, uint8_t mode )
{
	ca_data_t command, response;
	uint8_t value;

	command = CA_SetupCommandFrame( CA_FID_SETMODE, 2 );
	CA_WriteByteToData( command, 1, mode );
	CA_WriteByteToData( command, 2, 0xFF );

	response = CA_ExecuteCommand( cassy, command, 2 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		value = 0x00;
	else
		value = CA_ReadByteFromData( response, 1 );

	CA_FreeData( &response );

	return value;
}

void CA_SetValue( ca_cassy_t cassy, uint8_t mode, uint8_t value )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_SETMODE, 2 );
	CA_WriteByteToData( command, 1, mode );
	CA_WriteByteToData( command, 2, value );

	response = CA_ExecuteCommand( cassy, command, 2 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

void CA_SetMode( ca_cassy_t cassy, uint8_t mode )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_SETMODE, 1 );
	CA_WriteByteToData( command, 1, mode );
	response = CA_ExecuteCommand( cassy, command, 2 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

uint8_t CA_GetHSTG( ca_cassy_t cassy )
{
	ca_data_t command, response;
	uint8_t hstg;

	command = CA_SetupCommandFrame( CA_FID_GETHSTG, 0 );

	response = CA_ExecuteCommand( cassy, command, 2 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		hstg = 0x00;
	else
		hstg = CA_ReadByteFromData( response, 1 );

	CA_FreeData( &response );

	return hstg;
}

void CA_SetHSTG( ca_cassy_t cassy, uint8_t hstg )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_SETHSTG, 1 );
	CA_WriteByteToData( command, 1, hstg );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

struct tm CA_GetDateTime( ca_cassy_t cassy )
{
	ca_data_t command, response;
	struct tm time;

	command = CA_SetupCommandFrame( CA_FID_GETDATETIME, 0 );

	response = CA_ExecuteCommand( cassy, command, 8 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		memset( &time, 0, sizeof time );
	else
	{
		time.tm_hour = CA_ReadByteFromData( response, 1 );
		time.tm_min = CA_ReadByteFromData( response, 2 );
		time.tm_sec = CA_ReadByteFromData( response, 3 );
		time.tm_mday = CA_ReadByteFromData( response, 4 );
		time.tm_mon = CA_ReadByteFromData( response, 5 ) - 1;
		time.tm_year = CA_ReadShortFromData( response, 6 );
	}

	return time;
}

void CA_SetDateTime( ca_cassy_t cassy, struct tm time )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_SETDATETIME, 7 );
	CA_WriteByteToData( command, 1, time.tm_hour );
	CA_WriteByteToData( command, 2, time.tm_min );
	CA_WriteByteToData( command, 3, time.tm_sec );
	CA_WriteByteToData( command, 4, time.tm_mday );
	CA_WriteByteToData( command, 5, time.tm_mon + 1 );
	CA_WriteShortToData( command, 6, time.tm_year );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

uint8_t CA_GetSlaveAddress( ca_cassy_t cassy )
{
	ca_data_t command, response;
	uint8_t addr;

	command = CA_SetupCommandFrame( CA_FID_GETSLAVEADDRESS, 0 );

	response = CA_ExecuteCommand( cassy, command, 2 );
	CA_FreeData( &command );

	if ( CA_IsCassyError( response ) )
		addr = 0x00;
	else
		addr = CA_ReadByteFromData( response, 1 );

	CA_FreeData( &response );

	return addr;
}

void CA_SetSlaveAddress( ca_cassy_t cassy, uint8_t addr )
{
	ca_data_t command, response;

	command = CA_SetupCommandFrame( CA_FID_SETSLAVEADDRESS, 1 );
	CA_WriteByteToData( command, 1, addr );

	response = CA_ExecuteCommand( cassy, command, 1 );
	CA_FreeData( &command );

	CA_IsCassyError( response );
	CA_FreeData( &response );
}

bool CA_GetSlaveActive( ca_cassy_t cassy )
{
	ca_data_t command, response;
	bool active;

	command = CA_SetupCommandFrame( CA_FID_GETSLAVEACTIVE, 0 );

	response = CA_ExecuteCommand( cassy, command, 2 );
	CA_FreeData( &command );

	if( CA_IsCassyError( response ) )
		active = false;
	else
		active = CA_ReadByteFromData( response, 1 );

	CA_FreeData( &response );

	return active;
}

void CA_SetWatchdog( ca_cassy_t cassy, bool watchdog )
{
	ca_data_t command;

	command = CA_SetupCommandFrame( CA_FID_SETWATCHDOG, 1 );
	CA_WriteByteToData( command, 1, watchdog );

	CA_ExecuteCommand( cassy, command, CA_USB_NORESPONSE );
	CA_FreeData( &command );
}