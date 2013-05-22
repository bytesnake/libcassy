#ifndef CA_DATATYPES_H
#define CA_DATATYPES_H

#include "libcassy.h"

//
// internal datatypes
//

#ifdef CA_LOCAL

#ifdef CA_LIBUSB
typedef libusb_device_handle *ca_handle_t;
#endif

#ifdef CA_DEVNODES
typedef struct {
	int filedesc;
	struct termios *settingsb; // for serial connections
} *ca_handle_t;
#endif

#ifdef CA_WINHID
typedef void *ca_handle_t;
#endif

typedef struct
{
	int length;
	uint8_t *data;
} ca_data_t;

typedef struct
{
	int length;
	int offset;
	uint8_t status;
	int16_t *data;
} ca_stream_t;

#else
typedef void *ca_handle_t;
#endif

typedef enum
{
	CA_VERSION_SENSORCASSY = 1,
	CA_VERSION_POWERCASSY = 2,
	CA_VERSION_SENSORCASSY2 = 3,
	CA_VERSION_CASSYDISPLAY = 11,
	CA_VERSION_MOBILECASSY = 12,
	CA_VERSION_POCKETCASSY2 = 29,
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

	CA_ERROR_STREAM_INVALID,

	CA_ERROR_IO_INIT,
	CA_ERROR_IO_OPEN,
	CA_ERROR_IO_CLOSE,
	CA_ERROR_IO_READ,
	CA_ERROR_IO_WRITE,
	CA_ERROR_IO_MISC
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
	CA_RANGE_003A = 151
} ca_range_t;

typedef enum
{
	CA_SCLASS_3BIT,
	CA_SCLASS_7BIT,
	CA_SCLASS_13BIT,
	CA_SCLASS_END,

	CA_SCLASS_UNKNOWN
} ca_sclass_t;

typedef enum
{
	CA_WAVE_DCOFFSET = 0,
	CA_WAVE_SINE = 1,
	CA_WAVE_SQUAREPM = 2,
	CA_WAVE_SQUAREZERO = 3,
	CA_WAVE_TRIANGLEPM = 4,
	CA_WAVE_TRIANGLEZERO = 5,
	CA_WAVE_USERDEFINED = 6
} ca_wave_t;

typedef enum
{
	CA_IOMODE_SERIAL,
	CA_IOMODE_USB,
	CA_IOMODE_BLUETOOTH
} ca_iomode_t;

typedef struct
{
	int id;
	ca_version_t version;
	ca_handle_t handle;
	void *misc;
} ca_cassy_t;

typedef struct
{
	char *idstr;
	ca_version_t version;
	int id[8];
} ca_device_t;

typedef struct
{
	uint8_t status;
	size_t length;
	float *values;
} ca_oarray_t;

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
	uint16_t counts;
} ca_gstatus_t;

#endif
