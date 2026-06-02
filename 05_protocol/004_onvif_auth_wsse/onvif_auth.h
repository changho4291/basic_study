// Minimal gSOAP input for ONVIF Device Management GetDeviceInformation
// with WS-Security UsernameToken support.
//
// This keeps the same Device Service operation from 003_onvif_device_info
// and imports wsse2.h so soapcpp2 generates a SOAP Header that can carry
// wsse:Security / wsse:UsernameToken.

#import "wsse2.h"

//gsoap tds service name: Device
//gsoap tds service style: document
//gsoap tds service encoding: literal
//gsoap tds service namespace: http://www.onvif.org/ver10/device/wsdl
//gsoap tds service location: http://127.0.0.1:8082/onvif/device_service
//gsoap tds schema namespace: http://www.onvif.org/ver10/device/wsdl
//gsoap tds service method-action: GetDeviceInformation http://www.onvif.org/ver10/device/wsdl/GetDeviceInformation

struct _tds__GetDeviceInformation {
};

struct _tds__GetDeviceInformationResponse {
    char* Manufacturer;
    char* Model;
    char* FirmwareVersion;
    char* SerialNumber;
    char* HardwareId;
};

int tds__GetDeviceInformation(
    struct _tds__GetDeviceInformation* request,
    struct _tds__GetDeviceInformationResponse& response
);
