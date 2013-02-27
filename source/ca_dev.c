#include "../include/ca_local.h"

node_t CA_OpenCassyNode( const char *path )
{
    node_t node;
    
    CA_ResetError();
    
    node = (node_t) open( path, O_RDWR );
    if ( node == -1 )
        CA_SetLastError( ERROR_DEV_OPEN );
    
    return node;
}


void CA_CloseCassyNode( node_t node )
{
    CA_ResetError();

    CHECKCALL( close( node ), ERROR_DEV_CLOSE );
}

cassy_t CA_OpenCassy( node_t node, uint8_t cassyid )
{
    cassy_t cassy;
    
    CA_ResetError();
    
    cassy.node = node;
    cassy.cassyid = cassyid;
    cassy.version = VERSION_UNKNOWN;

    cassy.version = ( CA_GetHardwareVersion( cassy ) >> 8 );
    
    return cassy;
}

stream_t CA_ReceiveStream( cassy_t cassy, int offset )
{
	data_t response, serial;
	stream_t stream;
	ptrdiff_t sp, dp;
	size_t blocksize;
	int length;
	
	CA_ResetError();
	
	blocksize = GETBLOCKSIZE( cassy.version );
	response = CA_AllocateData( blocksize );
	
	stream.length = 0;
	serial.length = 0;
	
	sp = 0;
	dp = offset;
	
	do
	{
		length = read( cassy.node, response.data, blocksize );
		
		if ( length != blocksize )
		{
			CA_SetLastError( ERROR_DEV_READ );
			return stream;
		}
		
		if ( !CA_AddSerialData( &serial, response, blocksize ) )
			break;
	} while ( CA_AddToStream( &stream, &serial, &sp, &dp ) );
	
	stream.length = (size_t) sp;
	return stream;
}

data_t CA_ExecuteCommand( cassy_t cassy, data_t data, size_t rsize, uint32_t timeout )
{
    data_t command, response, serial;
    size_t blocksize;
    int offset, length;
    
    CA_ResetError();
    
    blocksize = GETBLOCKSIZE( cassy.version );
    command = CA_ConstructPacket( cassy.cassyid, data, blocksize );

    offset = 0;
    
    while ( offset != command.length )
    {
        length = write( cassy.node, command.data + offset, blocksize );
        if ( length != blocksize )
        {
            CA_SetLastError( ERROR_DEV_WRITE );
            return response;
        }
        
        offset += blocksize;
    }
    
    CA_FreeData( &command );
 
    if ( rsize == USB_NORESPONSE || cassy.cassyid == 0 )
        return response;

    response = CA_AllocateData( blocksize );
    serial.length = 0;
    
    while ( serial.length < rsize )
    {
        length = read( cassy.node, response.data, blocksize );
        if ( length != blocksize )
        {
            CA_SetLastError( ERROR_DEV_READ );
			break;
        }
        
        if ( !CA_AddSerialData( &serial, response, blocksize ) )
            break;
    }

    CA_FreeData( &response );
    
    return serial;
}
