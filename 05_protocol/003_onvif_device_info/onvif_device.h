// Minimal gSOAP input for ONVIF Device Management GetDeviceInformation.
//
// The full ONVIF Device Management WSDL is devicemgmt.wsdl, but this study
// keeps only the single operation needed for the first Device Service call.

//gsoap tds service name: Device
//gsoap tds service style: document
//gsoap tds service encoding: literal
//gsoap tds service namespace: http://www.onvif.org/ver10/device/wsdl
//gsoap tds service location: http://127.0.0.1:8081/onvif/device_service
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
