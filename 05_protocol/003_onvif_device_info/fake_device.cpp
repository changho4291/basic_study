#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "soapH.h"
#include "Device.nsmap"

namespace {

struct Options {
    int port = 8081;
    int timeout = 60;
    bool once = false;
    bool fault = false;
};

bool g_return_fault = false;

void print_usage(const char* program) {
    std::printf("Usage: %s [--port port] [--timeout seconds] [--once] [--fault]\n", program);
}

Options parse_options(int argc, char** argv) {
    Options options;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--port") == 0 && i + 1 < argc) {
            const int port = std::atoi(argv[++i]);
            options.port = port > 0 ? port : options.port;
        } else if (std::strcmp(argv[i], "--timeout") == 0 && i + 1 < argc) {
            const int timeout = std::atoi(argv[++i]);
            options.timeout = timeout > 0 ? timeout : options.timeout;
        } else if (std::strcmp(argv[i], "--once") == 0) {
            options.once = true;
        } else if (std::strcmp(argv[i], "--fault") == 0) {
            options.fault = true;
        } else {
            print_usage(argv[0]);
            std::exit(1);
        }
    }

    return options;
}

}  // namespace

int tds__GetDeviceInformation(
    struct soap* soap,
    _tds__GetDeviceInformation* request,
    _tds__GetDeviceInformationResponse& response) {
    (void)request;

    std::printf("\nReceived GetDeviceInformation\n");

    if (g_return_fault) {
        std::printf("Return SOAP Fault\n");
        std::fflush(stdout);
        return soap_sender_fault(
            soap,
            "Simulated ONVIF Device Service fault",
            "fake_device was started with --fault");
    }

    response.Manufacturer = const_cast<char*>("StudyCamera");
    response.Model = const_cast<char*>("ONVIF-DeviceInfo-Lab");
    response.FirmwareVersion = const_cast<char*>("1.0.0-study");
    response.SerialNumber = const_cast<char*>("STUDY-DEVICE-0001");
    response.HardwareId = const_cast<char*>("BASIC-STUDY-HW");

    std::printf("Send GetDeviceInformationResponse\n");
    std::printf("Manufacturer: %s\n", response.Manufacturer);
    std::printf("Model: %s\n", response.Model);
    std::printf("FirmwareVersion: %s\n", response.FirmwareVersion);
    std::printf("SerialNumber: %s\n", response.SerialNumber);
    std::printf("HardwareId: %s\n", response.HardwareId);
    std::fflush(stdout);

    return SOAP_OK;
}

int main(int argc, char** argv) {
    const Options options = parse_options(argc, argv);
    g_return_fault = options.fault;

    soap* soap = soap_new1(SOAP_XML_INDENT);
    if (soap == nullptr) {
        std::fprintf(stderr, "soap_new1 failed\n");
        return 1;
    }

    soap->bind_flags = SO_REUSEADDR;
    soap->accept_timeout = options.timeout;
    soap->recv_timeout = options.timeout;
    soap->send_timeout = options.timeout;
    soap_set_namespaces(soap, namespaces);
    soap_set_sent_logfile(soap, "fake_device_sent.xml");
    soap_set_recv_logfile(soap, "fake_device_recv.xml");

    const int master = soap_bind(soap, nullptr, options.port, 100);
    if (!soap_valid_socket(master)) {
        soap_print_fault(soap, stderr);
        soap_destroy(soap);
        soap_end(soap);
        soap_free(soap);
        return 1;
    }

    std::printf("Fake ONVIF Device Service listening on http://127.0.0.1:%d/onvif/device_service\n",
                options.port);
    std::printf("Timeout: %d seconds\n", options.timeout);
    std::fflush(stdout);

    while (true) {
        const int slave = soap_accept(soap);
        if (!soap_valid_socket(slave)) {
            break;
        }

        if (soap_serve(soap) != SOAP_OK) {
            soap_print_fault(soap, stderr);
        }
        soap_destroy(soap);
        soap_end(soap);

        if (options.once) {
            break;
        }
    }

    std::printf("\nFake device finished\n");
    std::printf("Sent log: fake_device_sent.xml\n");
    std::printf("Recv log: fake_device_recv.xml\n");

    soap_destroy(soap);
    soap_end(soap);
    soap_free(soap);
    return 0;
}
