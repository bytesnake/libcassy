#include "../include/ca_local.h"

error_t lasterror;

void CA_SetLastError( error_t error )
{
    if ( lasterror == ERROR_SUCCESS )
        lasterror = error;
}

void CA_ResetError()
{
    lasterror = ERROR_SUCCESS;
}

error_t CA_GetLastError()
{
    return lasterror;
}

const char *CA_ErrorToString( error_t error )
{
    switch ( error )
    {
    case ERROR_SUCCESS:
        return "ERROR_SUCCESS";
    case ERROR_CASSY:
        return "ERROR_CASSY";
    case ERROR_CASSY_OVERFLOW:
        return "ERROR_CASSY_OVERFLOW";
    case ERROR_CASSY_UNDERFLOW:
        return "ERROR_CASSY_UNDERFLOW";
    case ERROR_DEV_OPEN:
        return "ERROR_DEV_OPEN";
    case ERROR_DEV_CLOSE:
        return "ERROR_USB_CLOSE";
    case ERROR_DEV_WRITE:
        return "ERROR_DEV_WRITE";
    case ERROR_DEV_READ:
        return "ERROR_USB_READ";
    default:
        return "ERROR_UNKOWN";
    }
}

void CA_Attach()
{
    CA_ResetError();
}