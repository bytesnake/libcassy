#ifndef CA_PUBLIC_HEADER
#define CA_PUBLIC_HEADER

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

typedef int ca_node_t;

typedef enum
{
    CA_VERSION_SENSORCASSY = 1,
    CA_VERSION_POWERCASSY = 2,
    CA_VERSION_SENSORCASSY2 = 3,
    CA_VERSION_CASSYDISPLAY = 11,
    CA_VERSION_MOBILECASSY = 12,
    CA_VERSION_JOULEWATTMETER = 32,
    CA_VERSION_UMIP = 33,
    CA_VERSION_UMIC = 34,
    CA_VERSION_UMIB = 35,
    
    CA_VERSION_UNKNOWN = 0
} ca_version_t;

typedef enum
{
    CA_ERROR_SUCCESS,
    CA_ERROR_CASSY,
    CA_ERROR_CASSY_OVERFLOW,
    CA_ERROR_CASSY_UNDERFLOW,
    CA_ERROR_DEV_OPEN,
    CA_ERROR_DEV_CLOSE,
    CA_ERROR_DEV_WRITE,
    CA_ERROR_DEV_READ,
    
    ERROR_UNKNOWN,
} ca_error_t;

typedef enum
{
    CA_RANGE_250V = 137,
    CA_RANGE_100V = 136,
    CA_RANGE_30V = 168,
    CA_RANGE_10V = 200,
    CA_RANGE_3V = 232,
    CA_RANGE_1V = 192,
    CA_RANGE_03V = 224,
    CA_RANGE_01V = 143,
    CA_RANGE_3A = 144,
    CA_RANGE_1A = 176,
    CA_RANGE_03A = 208,
    CA_RANGE_01A = 240,
    CA_RANGE_003A = 151,
    
    CA_RANGE_BOX_1V = 8,
    CA_RANGE_BOX_03V = 40,
    CA_RANGE_BOX_01V = 72,
    CA_RANGE_BOX_003V = 104,
    CA_RANGE_BOX_001V = 64,
    CA_RANGE_BOX_0003V = 96,
    
    CA_RANGE_POWER_10V = 72,
    CA_RANGE_POWER_3V = 104,
    CA_RANGE_POWER_1V = 64,
    CA_RANGE_POWER_1A = 176,
    CA_RANGE_POWER_03A = 208,
    CA_RANGE_POWER_01A = 240,
    
    CA_RANGE_UNCHANGED = -1 // not implemented
} ca_range_t;

typedef struct
{
    uint8_t cassyid;
    ca_version_t version;
    ca_node_t handle;
} ca_cassy_t;

typedef struct
{
    float min;
    float max;
    float mean;
    float square;
} ca_mmms_t;

typedef struct
{
    int16_t xpos;
    int16_t ypos;
} ca_mpos_t;

typedef struct
{
    uint8_t status;
    uint16_t counts;
} ca_ostatus_t;

typedef struct
{
    uint8_t status;
    uint32_t time;
} ca_otime_t;

typedef struct
{
    uint8_t status;
    uint32_t uia;
    uint32_t uib;
    uint32_t sensora;
    uint32_t sensorb;
} ca_ostatus2_t;

typedef struct
{
    uint8_t status;
    uint32_t timehigh;
    uint32_t timelow;
    uint16_t delay;
} ca_otime2_t;

typedef struct
{
	uint8_t status;
	size_t length;
	float *values;
} ca_oarray_t;

//
// ca_functions.c
//

#include "ca_functions.h"

//
// ca_dev.c
//

ca_node_t CA_OpenCassyNode( const char *path );
void CA_CloseCassyNode( ca_node_t node );

ca_cassy_t CA_OpenCassy( ca_node_t node, uint8_t cassyid );

//
// ca_main.c
//

ca_error_t CA_GetLastError();
const char *CA_ErrorToString( ca_error_t error );

#endif