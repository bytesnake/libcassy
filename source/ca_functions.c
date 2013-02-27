#include "../include/ca_local.h"

//
// Useful helper functions
//

data_t CA_SetupCommandFrame( uint8_t fid, size_t length )
{
    data_t command;

    command = CA_AllocateData( length + 1 );
    command.data[0] = fid;

    return command;
}

bool CA_IsCassyError( data_t data )
{
    if ( CA_GetLastError() != ERROR_SUCCESS || data.data[0] == 0x00 )
    {
        CA_SetLastError( ERROR_CASSY );
        return true;
    }
    else if ( data.data[0] == 0x02 )
    {
        CA_SetLastError( ERROR_CASSY_OVERFLOW );
//      return true;
    }
    else if ( data.data[0] == 0x03 )
    {
        CA_SetLastError( ERROR_CASSY_UNDERFLOW );
//      return true;
    }

    return false;
}

oarray_t CA_StreamToOscilloscopeArray( stream_t stream, range_t range )
{
	oarray_t oarray;
	int i;
	
	oarray.length = stream.length;
	
	for ( i = 0; i < stream.length; i++ )
		oarray.values[i] = CA_ConvertToRange( (int32_t) stream.data[i], range );
	
	return oarray;
}

//
// Miscellaneous functions
//

void CA_Reset( cassy_t cassy )
{
    data_t command;

    command = CA_SetupCommandFrame( FID_RESET, 0 );
    CA_ExecuteCommand( cassy, command, USB_NORESPONSE, USB_TIMEOUT );
    CA_FreeData( &command );
}

uint16_t CA_GetHardwareVersion( cassy_t cassy )
{
    data_t command, response;
    uint16_t version;

    command = CA_SetupCommandFrame( FID_GETHARDWAREVERSION, 0 );
    response = CA_ExecuteCommand( cassy, command, 3, USB_TIMEOUT );
    CA_FreeData( &command );
    
    if ( CA_IsCassyError( response ) )
        version = 0x0000;
    else
        version = *(uint16_t *) ( response.data + 1 );

    CA_FreeData( &response );

    return CA_SwitchShort( version );
}

uint16_t CA_GetFirmwareVersion( cassy_t cassy )
{
    data_t command, response;
    uint16_t version;

    command = CA_SetupCommandFrame( FID_GETFIRMWAREVERSION, 0 );
    response = CA_ExecuteCommand( cassy, command, 3, USB_TIMEOUT );
    CA_FreeData( &command );

    if ( CA_IsCassyError( response ) )
        version = 0x0000;
    else
        version = *(uint16_t *) ( response.data + 1 );

    CA_FreeData( &response );

    return CA_SwitchShort( version );
}

void CA_EraseUserDataSector( cassy_t cassy, uint16_t addr )
{
    data_t command, response;

    command = CA_SetupCommandFrame( FID_ERASEUSERDATASECTOR, 2 );
    *(uint16_t *) ( command.data + 1 ) = CA_SwitchShort( addr );
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );

    CA_IsCassyError( response );
    CA_FreeData( &response );
}

void CA_WriteUserData( cassy_t cassy, uint16_t addr, uint32_t data )
{
    data_t command, response;

    command = CA_SetupCommandFrame( FID_WRITEUSERDATA, 6 );
    *(uint16_t *) ( command.data + 1 ) = CA_SwitchShort( addr );
    *(uint32_t *) ( command.data + 3 ) = CA_SwitchInt( data );
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );

    CA_IsCassyError( response );
    CA_FreeData( &response );
}

uint32_t CA_ReadUserData( cassy_t cassy, uint16_t addr )
{
    data_t command, response;
    uint32_t data;

    command = CA_SetupCommandFrame( FID_READUSERDATA, 2 );
    *(uint16_t *) ( command.data + 1 ) = CA_SwitchShort( addr );
    response = CA_ExecuteCommand( cassy, command, 5, USB_TIMEOUT );

    if ( CA_IsCassyError( response ) )
        data = 0x0000000;
    else
        data = *(uint32_t *) ( response.data + 1 );

    CA_FreeData( &response );

    return CA_SwitchInt( data );
}

uint8_t CA_GetValue( cassy_t cassy, uint8_t mode )
{
    data_t command, response;
    uint8_t value;
    
    command = CA_SetupCommandFrame( FID_SETMODE, 2 );
    command.data[1] = mode;
    command.data[2] = 0xFF;
    response = CA_ExecuteCommand( cassy, command, 2, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) ) 
        value = 0x00;
    else
        value = response.data[1];
    
    CA_FreeData( &response );
    
    return value;
}

void CA_SetValue( cassy_t cassy, uint8_t mode, uint8_t value )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_SETMODE, 2 );
    command.data[1] = mode;
    command.data[2] = value;
    response = CA_ExecuteCommand( cassy, command, 2, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

void CA_SetMode( cassy_t cassy, uint8_t mode )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_SETMODE, 1 );
    command.data[1] = mode;
    response = CA_ExecuteCommand( cassy, command, 2, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

uint8_t CA_GetHSTG( cassy_t cassy )
{
    data_t command, response;
    uint8_t hstg;
    
    command = CA_SetupCommandFrame( FID_GETHSTG, 0 );
    response = CA_ExecuteCommand( cassy, command, 2, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        hstg = 0x00;
    else
        hstg = response.data[1];
    
    CA_FreeData( &response );
    
    return hstg;
}

void CA_SetHSTG( cassy_t cassy, uint8_t hstg )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_SETHSTG, 1 );
    command.data[1] = hstg;
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

struct tm CA_GetDateTime( cassy_t cassy )
{
    data_t command, response;
    struct tm time;
    
    command = CA_SetupCommandFrame( FID_GETDATETIME, 0 );
    response = CA_ExecuteCommand( cassy, command, 8, 200 );
    
    if ( CA_IsCassyError( response ) ) 
        memset( &time, 0, sizeof time );
    else
    {
        time.tm_hour = (int32_t) response.data[1];
        time.tm_min = (int32_t) response.data[2];
        time.tm_sec = (int32_t) response.data[3];
        time.tm_mday = (int32_t) response.data[4];
        time.tm_mon = (int32_t) response.data[5] - 1;
        time.tm_year = (int32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 6 ) );
    }
    
    return time;
}

void CA_SetDateTime( cassy_t cassy, struct tm time )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_SETDATETIME, 7 );
    command.data[1] = (uint8_t) time.tm_hour;
    command.data[2] = (uint8_t) time.tm_min;
    command.data[3] = (uint8_t) time.tm_sec;
    command.data[4] = (uint8_t) time.tm_mday;
    command.data[5] = (uint8_t) time.tm_mon + 1;
    *(uint16_t *) ( command.data + 6 ) = CA_SwitchShort( (uint16_t) time.tm_year );
    response = CA_ExecuteCommand( cassy, command, 1, 200 );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

uint8_t CA_GetSlaveAddress( cassy_t cassy )
{
    data_t command, response;
    uint8_t addr;
    
    command = CA_SetupCommandFrame( FID_GETSLAVEADDRESS, 0 );
    response = CA_ExecuteCommand( cassy, command, 2, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        addr = 0x00;
    else
        addr = response.data[1];
    
    CA_FreeData( &response );
    
    return addr;
}

void CA_SetSlaveAddress( cassy_t cassy, uint8_t addr )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_SETSLAVEADDRESS, 1 );
    command.data[1] = addr;
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

bool CA_GetSlaveActive( cassy_t cassy )
{
    data_t command, response;
    bool active;
    
    command = CA_SetupCommandFrame( FID_GETSLAVEACTIVE, 0 );
    response = CA_ExecuteCommand( cassy, command, 2, USB_TIMEOUT );
    
    if( CA_IsCassyError( response ) )
        active = false;
    else
        active = (bool) response.data[1];
    
    CA_FreeData( &response );
    
    return active;
}

void CA_SetWatchdog( cassy_t cassy, bool watchdog )
{
    data_t command;
    
    command = CA_SetupCommandFrame( FID_SETWATCHDOG, 1 );
    command.data[1] = (uint8_t) watchdog;
    CA_ExecuteCommand( cassy, command, USB_NORESPONSE, USB_TIMEOUT );
}

//
// Analog input/output functions
//

int8_t CA_GetSensorBoxA( cassy_t cassy )
{
    data_t command, response;
    int8_t box;
    
    command = CA_SetupCommandFrame( FID_GETSENSORBOXA, 0 );
    response = CA_ExecuteCommand( cassy, command, 2, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        box = -1;
    else
        box = (int8_t) response.data[1];
    
    CA_FreeData( &response );
    
    return box;
}

int8_t CA_GetSensorBoxB( cassy_t cassy )
{
    data_t command, response;
    int8_t box;
    
    command = CA_SetupCommandFrame( FID_GETSENSORBOXB, 0 );
    response = CA_ExecuteCommand( cassy, command, 2, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        box = -1;
    else
        box = (int8_t) response.data[1];
    
    CA_FreeData( &response );
    
    return box;
}

int8_t CA_GetSensorBoxA2( cassy_t cassy, int32_t expected )
{
    data_t command, response;
    int8_t box;
    
    command = CA_SetupCommandFrame( FID_GETSENSORBOXA2, 4 );
    *(int32_t *) ( command.data + 1 ) = (int32_t) CA_SwitchInt( (uint32_t) expected );
    response = CA_ExecuteCommand( cassy, command, 2, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        box = -1;
    else
        box = response.data[1];
    
    CA_FreeData( &response );
    
    return box;
}

int8_t CA_GetSensorBoxB2( cassy_t cassy, int32_t expected )
{
    data_t command, response;
    int8_t box;
    
    command = CA_SetupCommandFrame( FID_GETSENSORBOXB2, 4 );
    *(int32_t *) ( command.data + 1 ) = (int32_t) CA_SwitchInt( (uint32_t) expected );
    response = CA_ExecuteCommand( cassy, command, 2, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        box = -1;
    else
        box = response.data[1];
    
    CA_FreeData( &response );
    
    return box;
}

int8_t CA_GetControlMode( cassy_t cassy )
{
    data_t command, response;
    int8_t mode;
    
    command = CA_SetupCommandFrame( FID_GETCONTROLMODE, 0 );
    response = CA_ExecuteCommand( cassy, command, 2, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        mode = -1;
    else
        mode = (int8_t) response.data[1];
        
    CA_FreeData( &response );
    
    return mode;
}

int8_t CA_GetMeasurementMode( cassy_t cassy )
{
    data_t command, response;
    int8_t mode;
    
    command = CA_SetupCommandFrame( FID_GETMEASUREMENTMODE, 0 );
    response = CA_ExecuteCommand( cassy, command, 2, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        mode = -1;
    else
        mode = (int8_t) response.data[1];
    
    CA_FreeData( &response );
    
    return mode;
}

float CA_GetInputValueA( cassy_t cassy, range_t range )
{
    data_t command, response;
    uint16_t digits;

    command = CA_SetupCommandFrame( FID_GETINPUTVALUEA, 1 );
    command.data[1] = (uint8_t) range;
    response = CA_ExecuteCommand( cassy, command, 3, USB_TIMEOUT );

    if ( CA_IsCassyError( response ) )
        digits = 0x0000;
    else
        digits = *(uint16_t *) ( response.data + 1 );

    CA_FreeData( &response );

    return CA_ExtendToRange( (uint32_t) CA_SwitchShort( digits ), 12, range );
}

float CA_GetInputValueB( cassy_t cassy, range_t range )
{
    data_t command, response;
    uint16_t digits;

    command = CA_SetupCommandFrame( FID_GETINPUTVALUEB, 1 );
    command.data[1] = (uint8_t) range;
    response = CA_ExecuteCommand( cassy, command, 3, USB_TIMEOUT );

    if ( CA_IsCassyError( response ) )
        digits = 0x0000;
    else
        digits = *(uint16_t *) ( response.data + 1 );

    CA_FreeData( &response );

    return CA_ExtendToRange( (uint32_t) CA_SwitchShort( digits ), 12, range );
}

float CA_GetValueUI( cassy_t cassy, range_t range )
{
    data_t command, response;
    uint16_t digits;

    command = CA_SetupCommandFrame( FID_GETVALUEUI, 1 );
    command.data[1] = (uint8_t) range;
    response = CA_ExecuteCommand( cassy, command, 3, USB_TIMEOUT );

    if ( CA_IsCassyError( response ) )
        digits = 0x0000;
    else
        digits = *(uint16_t *) ( response.data + 1 );

    CA_FreeData( &response );

    return CA_ExtendToRange( (uint32_t) CA_SwitchShort( digits ), 12, range );
}

float CA_GetValueIU( cassy_t cassy, range_t range )
{
    data_t command, response;
    uint16_t digits;

    command = CA_SetupCommandFrame( FID_GETVALUEIU, 1 );
    command.data[1] = (uint8_t) range;
    response = CA_ExecuteCommand( cassy, command, 3, USB_TIMEOUT );

    if ( CA_IsCassyError( response ) )
        digits = 0x0000;
    else
        digits = *(uint16_t *) ( response.data + 1 );

    CA_FreeData( &response );

    return CA_ExtendToRange( (uint32_t) CA_SwitchShort( digits ), 12, range );
}

float CA_GetValueA( cassy_t cassy )
{
    data_t command, response;
    uint16_t digits;

    command = CA_SetupCommandFrame( FID_GETVALUEA, 1 );
    response = CA_ExecuteCommand( cassy, command, 3, USB_TIMEOUT );

    if ( CA_IsCassyError( response ) )
        digits = 0x0000;
    else
        digits = *(uint16_t *) ( response.data + 1 );

    CA_FreeData( &response );

    return CA_ExtendToRange( (uint32_t) CA_SwitchShort( digits ), 12, RANGE_10V );
}

float CA_GetValueB( cassy_t cassy )
{
    data_t command, response;
    uint16_t digits;

    command = CA_SetupCommandFrame( FID_GETVALUEB, 1 );
    response = CA_ExecuteCommand( cassy, command, 3, USB_TIMEOUT );

    if ( CA_IsCassyError( response ) )
        digits = 0x0000;
    else
        digits = *(uint16_t *) ( response.data + 1 );

    CA_FreeData( &response );

    return CA_ExtendToRange( (uint32_t) CA_SwitchShort( digits ), 12, RANGE_10V );
}

float CA_GetValueX( cassy_t cassy )
{
    data_t command, response;
    uint16_t digits;

    command = CA_SetupCommandFrame( FID_GETVALUEX, 1 );
    response = CA_ExecuteCommand( cassy, command, 3, USB_TIMEOUT );

    if ( CA_IsCassyError( response ) )
        digits = 0x0000;
    else
        digits = *(uint16_t *) ( response.data + 1 );

    CA_FreeData( &response );

    return CA_ExtendToRange( (uint32_t) CA_SwitchShort( digits ), 12, RANGE_10V );
}

float CA_GetValueY( cassy_t cassy )
{
    data_t command, response;
    uint16_t digits;

    command = CA_SetupCommandFrame( FID_GETVALUEY, 1 );
    response = CA_ExecuteCommand( cassy, command, 3, USB_TIMEOUT );

    if ( CA_IsCassyError( response ) )
        digits = 0x0000;
    else
        digits = *(uint16_t *) ( response.data + 1 );

    CA_FreeData( &response );

    return CA_ExtendToRange( (uint32_t) CA_SwitchShort( digits ), 12, RANGE_10V );
}

mmms_t CA_GetMMMSInputValueA( cassy_t cassy, range_t range )
{
    data_t command, response;
    mmms_t values;
    
    // TODO: JWM support
    
    command = CA_SetupCommandFrame( FID_GETMMMSINPUTVALUEA, 1 );
    command.data[1] = (uint8_t) range;
    response = CA_ExecuteCommand( cassy, command, 11, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        memset( &values, 0, sizeof values );
    else
    {
        values.min = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 1 ) ), 12, range );
        values.max = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 3 ) ), 12, range );
        values.mean = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 5 ) ), 12, range );
        values.square = CA_ConvertToRange( (int32_t) CA_SwitchInt( *(uint32_t *) ( response.data + 7 ) ), range );
    }
    
    CA_FreeData( &response );
    
    return values;
}

mmms_t CA_GetMMMSInputValueB( cassy_t cassy, range_t range )
{
    data_t command, response;
    mmms_t values;
    
    // TODO: JWM support
    
    command = CA_SetupCommandFrame( FID_GETMMMSINPUTVALUEB, 1 );
    command.data[1] = (uint8_t) range;
    response = CA_ExecuteCommand( cassy, command, 11, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        memset( &values, 0, sizeof values );
    else
    {
        values.min = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 1 ) ), 12, range );
        values.max = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 3 ) ), 12, range );
        values.mean = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 5 ) ), 12, range );
        values.square = CA_ConvertToRange( (int32_t) CA_SwitchInt( *(uint32_t *) ( response.data + 7 ) ), range );
    }
    
    CA_FreeData( &response );
    
    return values;
}

mmms_t CA_GetMMMSInputValueUI( cassy_t cassy, range_t range )
{
    data_t command, response;
    mmms_t values;
    
    command = CA_SetupCommandFrame( FID_GETMMMSVALUEUI, 1 );
    command.data[1] = (uint8_t) range;
    response = CA_ExecuteCommand( cassy, command, 11, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        memset( &values, 0, sizeof values );
    else
    {
        values.min = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 1 ) ), 12, range );
        values.max = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 3 ) ), 12, range );
        values.mean = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 5 ) ), 12, range );
        values.square = CA_ConvertToRange( (int32_t) CA_SwitchInt( *(uint32_t *) ( response.data + 7 ) ), range );
    }
    
    CA_FreeData( &response );
    
    return values;
}

mmms_t CA_GetMMMSInputValueIU( cassy_t cassy, range_t range )
{
    data_t command, response;
    mmms_t values;
    
    command = CA_SetupCommandFrame( FID_GETMMMSVALUEIU, 1 );
    command.data[1] = (uint8_t) range;
    response = CA_ExecuteCommand( cassy, command, 11, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        memset( &values, 0, sizeof values );
    else
    {
        values.min = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 1 ) ), 12, range );
        values.max = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 3 ) ), 12, range );
        values.mean = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 5 ) ), 12, range );
        values.square = CA_ConvertToRange( (int32_t) CA_SwitchInt( *(uint32_t *) ( response.data + 7 ) ), range );
    }
    
    CA_FreeData( &response );
    
    return values;
}

mmms_t CA_GetMMMSValueA( cassy_t cassy )
{
    data_t command, response;
    mmms_t values;
    
    command = CA_SetupCommandFrame( FID_GETMMMSVALUEA, 1 );
    response = CA_ExecuteCommand( cassy, command, 11, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        memset( &values, 0, sizeof values );
    else
    {
        values.min = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 1 ) ), 12, RANGE_10V );
        values.max = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 3 ) ), 12, RANGE_10V );
        values.mean = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 5 ) ), 12, RANGE_10V );
        values.square = CA_ConvertToRange( (int32_t) CA_SwitchInt( *(uint32_t *) ( response.data + 7 ) ), RANGE_10V );
    }
    
    CA_FreeData( &response );
    
    return values;
}

mmms_t CA_GetMMMSValueB( cassy_t cassy )
{
    data_t command, response;
    mmms_t values;
    
    command = CA_SetupCommandFrame( FID_GETMMMSVALUEB, 1 );
    response = CA_ExecuteCommand( cassy, command, 11, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        memset( &values, 0, sizeof values );
    else
    {
        values.min = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 1 ) ), 12, RANGE_10V );
        values.max = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 3 ) ), 12, RANGE_10V );
        values.mean = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 5 ) ), 12, RANGE_10V );
        values.square = CA_ConvertToRange( (int32_t) CA_SwitchInt( *(uint32_t *) ( response.data + 7 ) ), RANGE_10V );
    }
    
    CA_FreeData( &response );
    
    return values;
}

mmms_t CA_GetMMMSValueX( cassy_t cassy )
{
    data_t command, response;
    mmms_t values;
    
    command = CA_SetupCommandFrame( FID_GETMMMSVALUEX, 1 );
    response = CA_ExecuteCommand( cassy, command, 11, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        memset( &values, 0, sizeof values );
    else
    {
        values.min = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 1 ) ), 12, RANGE_10V );
        values.max = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 3 ) ), 12, RANGE_10V );
        values.mean = CA_ExtendToRange( (uint32_t) CA_SwitchShort( *(uint16_t *) ( response.data + 5 ) ), 12, RANGE_10V );
        values.square = CA_ConvertToRange( (int32_t) CA_SwitchInt( *(uint32_t *) ( response.data + 7 ) ), RANGE_10V );
    }
    
    CA_FreeData( &response );
    
    return values;
}

float CA_GetCosInputValueAB( ca_cassy_t cassy, range_t range )
{
    data_t command, response;
    float sq1, sq2, p12;
    
    // TODO: JWM support
    
    command = CA_SetupCommandFrame( FID_GETCOSINPUTVALUEAB, 1 );
    command.data[1] = (uint8_t) range;
    response = CA_ExecuteCommand( cassy, command, 13, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
    {
        sq1 = 0.0f;
        sq2 = 0.0f;
        p12 = 0.0f;
    }
    else
    {
        sq1 = CA_ConvertToRange( (int32_t) CA_SwitchInt( *(uint32_t *) ( response.data + 1 ) ), range );
        sq2 = CA_ConvertToRange( (int32_t) CA_SwitchInt( *(uint32_t *) ( response.data + 5 ) ), range );
        p12 = CA_ConvertToRange( (int32_t) CA_SwitchInt( *(uint32_t *) ( response.data + 9 ) ), range );
    }
    
    return p12 / sqrt( sq1 * sq2 );
}

float CA_GetCosValueUI( ca_cassy_t cassy, range_t range )
{
    data_t command, response;
    float sq1, sq2, p12;
    
    command = CA_SetupCommandFrame( FID_GETCOSVALUEUI, 1 );
    command.data[1] = (uint8_t) range;
    response = CA_ExecuteCommand( cassy, command, 13, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
    {
        sq1 = 0.0f;
        sq2 = 0.0f;
        p12 = 0.0f;
    }
    else
    {
        sq1 = CA_ConvertToRange( (int32_t) CA_SwitchInt( *(uint32_t *) ( response.data + 1 ) ), range );
        sq2 = CA_ConvertToRange( (int32_t) CA_SwitchInt( *(uint32_t *) ( response.data + 5 ) ), range );
        p12 = CA_ConvertToRange( (int32_t) CA_SwitchInt( *(uint32_t *) ( response.data + 9 ) ), range );
    }
    
    return p12 / sqrt( sq1 * sq2 );
}

float CA_GetCosValueIU( ca_cassy_t cassy, range_t range )
{
    data_t command, response;
    float sq1, sq2, p12;
    
    // this is exactly the same function as CA_GetCosValueUI
    
    command = CA_SetupCommandFrame( FID_GETCOSVALUEIU, 1 );
    command.data[1] = (uint8_t) range;
    response = CA_ExecuteCommand( cassy, command, 13, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
    {
        sq1 = 0.0f;
        sq2 = 0.0f;
        p12 = 0.0f;
    }
    else
    {
        sq1 = CA_ConvertToRange( (int32_t) CA_SwitchInt( *(uint32_t *) ( response.data + 1 ) ), range );
        sq2 = CA_ConvertToRange( (int32_t) CA_SwitchInt( *(uint32_t *) ( response.data + 5 ) ), range );
        p12 = CA_ConvertToRange( (int32_t) CA_SwitchInt( *(uint32_t *) ( response.data + 9 ) ), range );
    }
    
    return p12 / sqrt( sq1 * sq2 );
}

void CA_SendSerialSensorDataA( cassy_t cassy, uint8_t bits, uint16_t data )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_SENDSERIALSENSORDATAA, 3 );
    command.data[1] = bits;
    *(uint16_t *) ( command.data + 2 ) = CA_SwitchShort( data );
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

void CA_SendSerialSensorDataB( cassy_t cassy, uint8_t bits, uint16_t data )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_SENDSERIALSENSORDATAB, 3 );
    command.data[1] = bits;
    *(uint16_t *) ( command.data + 2 ) = CA_SwitchShort( data );
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

void CA_SetMeanTime( cassy_t cassy, uint16_t time )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_SETMEANTIME, 2 );
    *(uint16_t *) ( command.data + 1 ) = CA_SwitchShort( time );
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

//
// Relay and voltage source functions
//

void CA_SetRelayVoltage( cassy_t cassy, uint8_t relvol )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_SETRELAYVOLTAGE, 1 );
    command.data[1] = relvol;
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

void CA_SetRelay( cassy_t cassy, uint8_t relay )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_SETRELAY, 1 );
    command.data[1] = relay;
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

void CA_SetVoltage( cassy_t cassy, uint8_t voltage )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_SETVOLTAGE, 1 );
    command.data[1] = voltage;
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

//
// Digital input/output functions
//

uint8_t CA_GetBitInput( cassy_t cassy, uint8_t bit )
{
    data_t command, response;
    uint8_t value;
    
    command = CA_SetupCommandFrame( FID_GETBITINPUT, 1 );
    command.data[1] = bit;
    response = CA_ExecuteCommand( cassy, command, 2, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        value = 0x00;
    else
        value = response.data[1];
    
    CA_FreeData( &response );
    
    return value;
}

void CA_SetBitOutput( cassy_t cassy, uint8_t bit, uint8_t value )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_SETBITOUTPUT, 2 );
    command.data[1] = bit;
    command.data[2] = value;
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

uint16_t CA_GetDigitalInputs( cassy_t cassy )
{
    data_t command, response;
    uint16_t values;
    
    command = CA_SetupCommandFrame( FID_GETDIGITALINPUTS, 0 );
    response = CA_ExecuteCommand( cassy, command, 3, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        values = 0x0000;
    else
        values = *(uint16_t *) ( response.data + 1 );

    CA_FreeData( &response );
    
    return CA_SwitchShort( values );
}

void CA_SetDigitalOutputs( cassy_t cassy, uint16_t values )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_SETDIGITALOUTPUTS, 2 );
    *(uint16_t *) ( command.data + 1 ) = CA_SwitchShort( values ); 
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

void CA_SetCounterMode( cassy_t cassy, uint8_t bit, uint8_t mode )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_SETCOUNTERMODE, 2 );
    command.data[1] = bit;
    command.data[2] = mode;
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

void CA_SetCounterDelay( cassy_t cassy, uint16_t delay )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_SETCOUNTERDELAY, 2 );
    *(uint16_t *) ( command.data + 1 ) = CA_SwitchShort( delay );
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

uint16_t CA_GetCounter( cassy_t cassy, uint8_t bit )
{
    data_t command, response;
    uint16_t value;
    
    command = CA_SetupCommandFrame( FID_GETCOUNTER, 1 );
    command.data[1] = bit;
    response = CA_ExecuteCommand( cassy, command, 3, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        value = 0x0000;
    else
        value = *(uint16_t *) ( response.data + 1 );
        
    CA_FreeData( &response );
    
    return CA_SwitchShort( value );
}

void CA_SetCounter( cassy_t cassy, uint8_t bit, uint16_t value )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_SETCOUNTER, 3 );
    command.data[1] = bit;
    *(uint16_t *) ( command.data + 2 ) = CA_SwitchShort( value );
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

void CA_SetReferencePoint( cassy_t cassy, uint8_t refpoint )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_SETREFERENCEPOINT, 1 );
    command.data[1] = refpoint;
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

void CA_SetStepperDelay( cassy_t cassy, uint16_t delay )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_SETSTEPPERDELAY, 1 );
    *(uint16_t *) ( command.data + 1 ) = CA_SwitchShort( delay );
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

mpos_t CA_GetPhysicalPosition( cassy_t cassy )
{
    data_t command, response;
    mpos_t position;
    
    command = CA_SetupCommandFrame( FID_GETPHYSICALPOSITION, 0 );
    response = CA_ExecuteCommand( cassy, command, 5, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        memset( &position, 0, sizeof position );
    else
    {
        position.xpos = (int16_t) CA_SwitchShort( *(uint16_t *) ( response.data + 1 ) );
        position.ypos = (int16_t) CA_SwitchShort( *(uint16_t *) ( response.data + 3 ) );
    }
    
    return position;
}

mpos_t CA_GetLogicalPosition( cassy_t cassy )
{
    data_t command, response;
    mpos_t position;
    
    command = CA_SetupCommandFrame( FID_GETLOGICALPOSITION, 0 );
    response = CA_ExecuteCommand( cassy, command, 5, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        memset( &position, 0, sizeof position );
    else
    {
        position.xpos = (int16_t) CA_SwitchShort( *(uint16_t *) ( response.data + 1 ) );
        position.ypos = (int16_t) CA_SwitchShort( *(uint16_t *) ( response.data + 3 ) );
    }
    
    return position;
}

void CA_MoveToAbsPosition( cassy_t cassy, mpos_t position )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_MOVETOABSPOSITION, 4 );
    *(uint16_t *) ( command.data + 1 ) = CA_SwitchShort( (uint16_t) position.xpos );
    *(uint16_t *) ( command.data + 3 ) = CA_SwitchShort( (uint16_t) position.xpos );
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

void CA_MoveToRelPosition( cassy_t cassy, mpos_t position )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_MOVETORELPOSITION, 4 );
    *(uint16_t *) ( command.data + 1 ) = CA_SwitchShort( (uint16_t) position.xpos );
    *(uint16_t *) ( command.data + 3 ) = CA_SwitchShort( (uint16_t) position.xpos );
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

void CA_ResetOscilloscope( ca_cassy_t cassy, uint16_t dt, uint16_t n, uint8_t trigger, uint16_t trigval )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_RESETOSCILLOSCOPE, 7 );
    *(uint16_t *) ( command.data + 1 ) = CA_SwitchShort( dt );
    *(uint16_t *) ( command.data + 3 ) = CA_SwitchShort( n );
    command.data[5] = trigger;
    *(uint16_t *) ( command.data + 6 ) = CA_SwitchShort( trigval );
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

void CA_ResetOscilloscope2( ca_cassy_t cassy, uint32_t dt, uint16_t mean, uint32_t n0, uint32_t n1, uint8_t mask, uint8_t trigger, uint16_t trigval )
{
    data_t command, response;
    
    command = CA_SetupCommandFrame( FID_RESETOSCILLOSCOPE2, 18 );
    *(uint32_t *) ( command.data + 1 ) = CA_SwitchInt( dt );
    *(uint16_t *) ( command.data + 5 ) = CA_SwitchShort( mean );
    *(uint32_t *) ( command.data + 7 ) = CA_SwitchInt( n0 );
    *(uint32_t *) ( command.data + 11 ) = CA_SwitchInt( n1 );
    command.data[15] = mask;
    command.data[16] = trigger;
    *(uint16_t *) ( command.data + 17 ) = CA_SwitchShort( trigval );
    response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
    
    CA_IsCassyError( response );
    CA_FreeData( &response );
}

void CA_StartOscilloscope( cassy_t cassy )
{
	data_t command, response;
	
	command = CA_SetupCommandFrame( FID_STARTOSCILLOSCOPE, 0 );
	response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
	
	CA_IsCassyError( response );
	CA_FreeData( &response );
}

void CA_StopOscilloscope( cassy_t cassy )
{
	data_t command, response;
	
	command = CA_SetupCommandFrame( FID_STOPOSCILLOSCOPE, 0 );
	response = CA_ExecuteCommand( cassy, command, 1, USB_TIMEOUT );
	
	CA_IsCassyError( response );
	CA_FreeData( &response );
}

ostatus_t CA_GetOscilloscopeStatus( cassy_t cassy )
{
    data_t command, response;
    ostatus_t ostatus;
    
    command = CA_SetupCommandFrame( FID_GETOSCILLOSCOPESTATUS, 0 );
    response = CA_ExecuteCommand( cassy, command, 4, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        memset( &ostatus, 0, sizeof ostatus );
    else
    {
        ostatus.status = response.data[1];
        ostatus.counts = CA_SwitchShort( *(uint16_t *) ( response.data + 2 ) );
    }
        
    CA_FreeData( &response );
    
    return ostatus;
}

ostatus2_t CA_GetOscilloscopeStatus2( cassy_t cassy )
{
    data_t command, response;
    ostatus2_t ostatus;
    
    command = CA_SetupCommandFrame( FID_GETOSCILLOSCOPESTATUS2, 0 );
    response = CA_ExecuteCommand( cassy, command, 18, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        memset( &ostatus, 0, sizeof ostatus );
    else
    {
        ostatus.status = response.data[1];
        ostatus.uia = CA_SwitchInt( *(uint32_t *) ( response.data + 2 ) );
        ostatus.uib = CA_SwitchInt( *(uint32_t *) ( response.data + 6 ) );
        ostatus.sensora = CA_SwitchInt( *(uint32_t *) ( response.data + 10 ) );
        ostatus.sensorb = CA_SwitchInt( *(uint32_t *) ( response.data + 14 ) );
    }
    
    CA_FreeData( &response );
    
    return ostatus;
}

otime_t CA_GetOscilloscopeTime( cassy_t cassy )
{
    data_t command, response;
    otime_t otime;
    
    command = CA_SetupCommandFrame( FID_GETOSCILLOSCOPETIME, 0 );
    response = CA_ExecuteCommand( cassy, command, 6, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        memset( &otime, 0, sizeof otime );
    else
    {
        otime.status = response.data[1];
        otime.time = CA_SwitchInt( *(uint32_t *) ( response.data + 2 ) );
    }
    
    CA_FreeData( &response );
    
    return otime;
}

otime2_t CA_GetOscilloscopeTime2( cassy_t cassy )
{
    data_t command, response;
    otime2_t otime;
    
    command = CA_SetupCommandFrame( FID_GETOSCILLOSCOPETIME2, 0 );
    response = CA_ExecuteCommand( cassy, command, 12, USB_TIMEOUT );
    
    if ( CA_IsCassyError( response ) )
        memset( &otime, 0, sizeof otime );
    else
    {
        otime.status = response.data[1];
        otime.timehigh = CA_SwitchInt( *(uint32_t *) ( response.data + 2 ) );
        otime.timelow = CA_SwitchInt( *(uint32_t *) ( response.data + 6 ) );
        otime.delay = CA_SwitchShort( *(uint16_t *) ( response.data + 10 ) );
    }
    
    CA_FreeData( &response );
    
    return otime;
}

oarray_t CA_GetOscilloscopeArrayA( cassy_t cassy, range_t range, uint16_t start, uint16_t count, uint16_t delta )
{
	oarray_t oarray;
	data_t command;
	stream_t stream;
	
	command = CA_SetupCommandFrame( FID_GETOSCILLOSCOPEARRAYA, 6 );
	*(uint16_t *) ( command.data + 1 ) = CA_SwitchShort( start );
	*(uint16_t *) ( command.data + 3 ) = CA_SwitchShort( count );
	*(uint16_t *) ( command.data + 5 ) = CA_SwitchShort( delta );
	CA_ExecuteCommand( cassy, command, USB_NORESPONSE, USB_TIMEOUT );
	
	stream = CA_ReceiveStream( cassy, 2 );
	oarray = CA_StreamToOscilloscopeArray( stream, range );
	CA_FreeStream( &stream );
	
	return oarray;
}

oarray_t CA_GetOscilloscopeArrayB( cassy_t cassy, range_t range, uint16_t start, uint16_t count, uint16_t delta )
{
	oarray_t oarray;
	data_t command;
	stream_t stream;
	
	command = CA_SetupCommandFrame( FID_GETOSCILLOSCOPEARRAYB, 6 );
	*(uint16_t *) ( command.data + 1 ) = CA_SwitchShort( start );
	*(uint16_t *) ( command.data + 3 ) = CA_SwitchShort( count );
	*(uint16_t *) ( command.data + 5 ) = CA_SwitchShort( delta );
	CA_ExecuteCommand( cassy, command, USB_NORESPONSE, USB_TIMEOUT );
	
	stream = CA_ReceiveStream( cassy, 2 );
	oarray = CA_StreamToOscilloscopeArray( stream, range );
	CA_FreeStream( &stream );
	
	return oarray;
}

oarray_t CA_GetOscilloscopeArrayX( cassy_t cassy, range_t range, uint16_t start, uint16_t count, uint16_t delta )
{
	oarray_t oarray;
	data_t command;
	stream_t stream;
	
	command = CA_SetupCommandFrame( FID_GETOSCILLOSCOPEARRAYX, 6 );
	*(uint16_t *) ( command.data + 1 ) = CA_SwitchShort( start );
	*(uint16_t *) ( command.data + 3 ) = CA_SwitchShort( count );
	*(uint16_t *) ( command.data + 5 ) = CA_SwitchShort( delta );
	CA_ExecuteCommand( cassy, command, USB_NORESPONSE, USB_TIMEOUT );
	
	stream = CA_ReceiveStream( cassy, 2 );
	oarray = CA_StreamToOscilloscopeArray( stream, range );
	CA_FreeStream( &stream );
	
	return oarray;
}

oarray_t CA_GetOscilloscopeArrayY( cassy_t cassy, range_t range, uint16_t start, uint16_t count, uint16_t delta )
{
	oarray_t oarray;
	data_t command;
	stream_t stream;
	
	command = CA_SetupCommandFrame( FID_GETOSCILLOSCOPEARRAYY, 6 );
	*(uint16_t *) ( command.data + 1 ) = CA_SwitchShort( start );
	*(uint16_t *) ( command.data + 3 ) = CA_SwitchShort( count );
	*(uint16_t *) ( command.data + 5 ) = CA_SwitchShort( delta );
	CA_ExecuteCommand( cassy, command, USB_NORESPONSE, USB_TIMEOUT );
	
	stream = CA_ReceiveStream( cassy, 2 );
	oarray = CA_StreamToOscilloscopeArray( stream, range );
	CA_FreeStream( &stream );
	
	return oarray;
}

oarray_t CA_GetOscilloscopeArray2A( cassy_t cassy, range_t range, uint32_t skip, uint32_t count )
{
	oarray_t oarray;
	data_t command;
	stream_t stream;
	
	command = CA_SetupCommandFrame( FID_GETOSCILLOSCOPEARRAY2A, 9 );
	command.data[1] = range;
	*(uint32_t *) ( command.data + 2 ) = CA_SwitchInt( skip );
	*(uint32_t *) ( command.data + 6 ) = CA_SwitchInt( count );
	CA_ExecuteCommand( cassy, command, USB_NORESPONSE, USB_TIMEOUT );
	
	stream = CA_ReceiveStream( cassy, 2 );
	oarray = CA_StreamToOscilloscopeArray( stream, range );
	CA_FreeStream( &stream );
	
	return oarray;
}

oarray_t CA_GetOscilloscopeArray2B( cassy_t cassy, range_t range, uint32_t skip, uint32_t count )
{
	oarray_t oarray;
	data_t command;
	stream_t stream;
	
	command = CA_SetupCommandFrame( FID_GETOSCILLOSCOPEARRAY2B, 9 );
	command.data[1] = range;
	*(uint32_t *) ( command.data + 2 ) = CA_SwitchInt( skip );
	*(uint32_t *) ( command.data + 6 ) = CA_SwitchInt( count );
	CA_ExecuteCommand( cassy, command, USB_NORESPONSE, USB_TIMEOUT );
	
	stream = CA_ReceiveStream( cassy, 2 );
	oarray = CA_StreamToOscilloscopeArray( stream, range );
	CA_FreeStream( &stream );
	
	return oarray;
}

//
// TODO: Timer functions
//

//
// TODO: Function generator functions
//