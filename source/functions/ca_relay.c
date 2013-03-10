#include "../../include/libcassy.h"

void CA_SetRelayVoltage( ca_cassy_t cassy, uint8_t relvol )
{
    ca_data_t command, response;
    
    command = CA_SetupCommandFrame( CA_FID_SETRELAYVOLTAGE, 1 );
    CA_WriteByteToData( command, 1, relvol );
	
    response = CA_ExecuteCommand( cassy, command, 1 );
    CA_FreeData( &command );
	
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

void CA_SetRelay( ca_cassy_t cassy, uint8_t relay )
{
    ca_data_t command, response;
    
    command = CA_SetupCommandFrame( CA_FID_SETRELAY, 1 );
    CA_WriteByteToData( command, 1, relay );
	
    response = CA_ExecuteCommand( cassy, command, 1 );
    CA_FreeData( &command );
	
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

void CA_SetVoltage( ca_cassy_t cassy, uint8_t voltage )
{
    ca_data_t command, response;
    
    command = CA_SetupCommandFrame( CA_FID_SETVOLTAGE, 1 );
    CA_WriteByteToData( command, 1, voltage );
	
    response = CA_ExecuteCommand( cassy, command, 1 );
    CA_FreeData( &command );
	
    CA_IsCassyError( response );
    CA_FreeData( &response );
}
