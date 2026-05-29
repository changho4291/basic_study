#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "soapH.h"
#include "Device.nsmap"

namespace {

constexpr const char* kDefaultEndpoint = "http://127.0.0.1:8081/onvif/device_service";
constexpr const char* kGetDeviceInformationAction =
    "http://www.onvif.org/ver10/device/wsdl/GetDeviceInformation";

struct Options {
    const char* endpoint = kDefaultEndpoint;
    int timeout = 5;
};

void print_usage(const char* program) {
    std::printf("Usage: %s [--endpoint http://host/onvif/device_service] [--timeout seconds]\n",
                program);
}

Options parse_options(int argc, char** argv) {
    Options options;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--endpoint") == 0 && i + 1 < argc) {
            options.endpoint = argv[++i];
        } else if (std::strcmp(argv[i], "--timeout") == 0 && i + 1 < argc) {
            const int timeout = std::atoi(argv[++i]);
            options.timeout = timeout > 0 ? timeout : options.timeout;
        } else {
            print_usage(argv[0]);
            std::exit(1);
        }
    }

    return options;
}

void print_text(const char* label, const char* value) {
    std::printf("%s: %s\n", label, value != nullptr ? value : "-");
}

}  // namespace

int main(int argc, char** argv) {
    const Options options = parse_options(argc, argv);

    soap* soap = soap_new1(SOAP_XML_INDENT);
    if (soap == nullptr) {
        std::fprintf(stderr, "soap_new1 failed\n");
        return 1;
    }

    soap->connect_timeout = options.timeout;
    soap->recv_timeout = options.timeout;
    soap->send_timeout = options.timeout;
    soap_set_namespaces(soap, namespaces);
    soap_set_sent_logfile(soap, "device_info_sent.xml");
    soap_set_recv_logfile(soap, "device_info_recv.xml");

    _tds__GetDeviceInformation request;
    _tds__GetDeviceInformationResponse response;
    soap_default__tds__GetDeviceInformation(soap, &request);
    soap_default__tds__GetDeviceInformationResponse(soap, &response);

    std::printf("Send ONVIF GetDeviceInformation\n");
    std::printf("Endpoint: %s\n", options.endpoint);
    std::printf("SOAPAction: %s\n", kGetDeviceInformationAction);
    std::fflush(stdout);

    if (soap_call_tds__GetDeviceInformation(
            soap,
            options.endpoint,
            kGetDeviceInformationAction,
            &request,
            response) != SOAP_OK) {
        soap_print_fault(soap, stderr);
        std::printf("\nSent log: device_info_sent.xml\n");
        std::printf("Recv log: device_info_recv.xml\n");
        soap_destroy(soap);
        soap_end(soap);
        soap_free(soap);
        return 1;
    }

    std::printf("\nDevice Information\n");
    print_text("Manufacturer", response.Manufacturer);
    print_text("Model", response.Model);
    print_text("FirmwareVersion", response.FirmwareVersion);
    print_text("SerialNumber", response.SerialNumber);
    print_text("HardwareId", response.HardwareId);

    std::printf("\nSent log: device_info_sent.xml\n");
    std::printf("Recv log: device_info_recv.xml\n");

    soap_destroy(soap);
    soap_end(soap);
    soap_free(soap);
    return 0;
}
