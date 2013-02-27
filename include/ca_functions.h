#ifndef CA_FUNCTIONS_HEADER
#define CA_FUNCTIONS_HEADER

void CA_Reset( ca_cassy_t cassy );
uint16_t CA_GetHardwareVersion( ca_cassy_t cassy );
uint16_t CA_GetFirmwareVersion( ca_cassy_t cassy );

void CA_EraseUserDataSector( ca_cassy_t cassy, uint16_t addr );
void CA_WriteUserData( ca_cassy_t cassy, uint16_t addr, uint32_t data );
uint32_t CA_ReadUserData( ca_cassy_t cassy, uint16_t addr );

uint8_t CA_GetValue( ca_cassy_t cassy, uint8_t mode );
void CA_SetValue( ca_cassy_t cassy, uint8_t mode, uint8_t value );
void CA_SetMode( ca_cassy_t cassy, uint8_t mode );

uint8_t CA_GetHSTG( ca_cassy_t cassy );
void CA_SetHSTG( ca_cassy_t cassy, uint8_t hstg );

struct tm CA_GetDateTime( ca_cassy_t cassy );
void CA_SetDateTime( ca_cassy_t cassy, struct tm time );

// TODO: ReadDataLogger

uint8_t CA_GetSlaveAddress( ca_cassy_t cassy );
void CA_SetSlaveAddress( ca_cassy_t cassy, uint8_t addr );
bool CA_GetSlaveActive( ca_cassy_t cassy );

void CA_SetWatchdog( ca_cassy_t cassy, bool watchdog );

int8_t CA_GetSensorBoxA( ca_cassy_t cassy );
int8_t CA_GetSensorBoxB( ca_cassy_t cassy );

int8_t CA_GetSensorBoxA2( ca_cassy_t cassy, int32_t expected );
int8_t CA_GetSensorBoxB2( ca_cassy_t cassy, int32_t expected );

int8_t CA_GetControlMode( ca_cassy_t cassy );
int8_t CA_GetMeasurementMode( ca_cassy_t cassy );

float CA_GetInputValueA( ca_cassy_t cassy, ca_range_t range );
float CA_GetInputValueB( ca_cassy_t cassy, ca_range_t range );

float CA_GetValueUI( ca_cassy_t cassy, ca_range_t range );
float CA_GetValueIU( ca_cassy_t cassy, ca_range_t range );

float CA_GetValueA( ca_cassy_t cassy );
float CA_GetValueB( ca_cassy_t cassy );
float CA_GetValueX( ca_cassy_t cassy );
float CA_GetValueY( ca_cassy_t cassy );

// TODO: special sensor boxes

ca_mmms_t CA_GetMMMSInputValueA( ca_cassy_t cassy, ca_range_t range );
ca_mmms_t CA_GetMMMSInputValueB( ca_cassy_t cassy, ca_range_t range );
ca_mmms_t CA_GetMMMSValueUI( ca_cassy_t cassy, ca_range_t range );
ca_mmms_t CA_GetMMMSValueIU( ca_cassy_t cassy, ca_range_t range );

ca_mmms_t CA_GetMMMSValueA( ca_cassy_t cassy );
ca_mmms_t CA_GetMMMSValueB( ca_cassy_t cassy );
ca_mmms_t CA_GetMMMSValueX( ca_cassy_t cassy );

float CA_GetCosInputValueAB( ca_cassy_t cassy, ca_range_t range );
float CA_GetCosValueUI( ca_cassy_t cassy, ca_range_t range );
float CA_GetCosValueIU( ca_cassy_t cassy, ca_range_t range );

float CA_GetCosValueAB( ca_cassy_t cassy );
float CA_GetCosValueXA( ca_cassy_t cassy );

void CA_SendSerialSensorDataA( ca_cassy_t cassy, uint8_t bits, uint16_t data );
void CA_SendSerialSensorDataB( ca_cassy_t cassy, uint8_t bits, uint16_t data );

void CA_SetMeanTime( ca_cassy_t cassy, uint16_t time );

void CA_SetRelayVoltage( ca_cassy_t cassy, uint8_t relvol );
void CA_SetRelay( ca_cassy_t cassy, uint8_t relay );
void CA_SetVoltage( ca_cassy_t cassy, uint8_t voltage );

uint8_t CA_GetBitInput( ca_cassy_t cassy, uint8_t bit );
void CA_SetBitOutput( ca_cassy_t cassy, uint8_t bit, uint8_t value );

uint16_t CA_GetDigitalInputs( ca_cassy_t cassy );
void CA_SetDigitalOutputs( ca_cassy_t cassy, uint16_t values );

void CA_SetCounterMode( ca_cassy_t cassy, uint8_t bit, uint8_t mode );
void CA_SetCounterDelay( ca_cassy_t cassy, uint16_t delay );

uint16_t CA_GetCounter( ca_cassy_t cassy, uint8_t bit );
void CA_SetCounter( ca_cassy_t cassy, uint8_t bit, uint16_t value );

void CA_SetReferencePoint( ca_cassy_t cassy, uint8_t refpoint );
void CA_SetStepperDelay( ca_cassy_t cassy, uint16_t delay );

ca_mpos_t CA_GetPhysicalPosition( ca_cassy_t cassy );
ca_mpos_t CA_GetLogicalPosition( ca_cassy_t cassy );

void CA_MoveToAbsPosition( ca_cassy_t cassy, ca_mpos_t position );
void CA_MoveToRelPosition( ca_cassy_t cassy, ca_mpos_t position );

void CA_ResetOscilloscope( ca_cassy_t cassy, uint16_t dt, uint16_t n, uint8_t trigger, uint16_t trigval );
void CA_ResetOscilloscope2( ca_cassy_t cassy, uint32_t dt, uint16_t mean, uint32_t n0, uint32_t n1, uint8_t mask, uint8_t trigger, uint16_t trigval );

void CA_StartOscilloscope( ca_cassy_t cassy );
void CA_StopOscilloscope( ca_cassy_t cassy );

ca_ostatus_t CA_GetOscilloscopeStatus( ca_cassy_t cassy );
ca_ostatus2_t CA_GetOscilloscopeStatus2( ca_cassy_t cassy );

ca_otime_t CA_GetOscilloscopeTime( ca_cassy_t cassy );
ca_otime2_t CA_GetOscilloscopeTime2( ca_cassy_t cassy );

ca_oarray_t CA_GetOscilloscopeArrayA( ca_cassy_t cassy, ca_range_t range, uint16_t start, uint16_t count, uint16_t delta );
ca_oarray_t CA_GetOscilloscopeArrayB( ca_cassy_t cassy, ca_range_t range, uint16_t start, uint16_t count, uint16_t delta );
ca_oarray_t CA_GetOscilloscopeArrayX( ca_cassy_t cassy, ca_range_t range, uint16_t start, uint16_t count, uint16_t delta );
ca_oarray_t CA_GetOscilloscopeArrayY( ca_cassy_t cassy, ca_range_t range, uint16_t start, uint16_t count, uint16_t delta );

ca_oarray_t CA_GetOscilloscopeArray2A( ca_cassy_t cassy, ca_range_t range, uint32_t skip, uint32_t count );
ca_oarray_t CA_GetOscilloscopeArray2A( ca_cassy_t cassy, ca_range_t range, uint32_t skip, uint32_t count );

// TODO: lots of other functions

#endif
