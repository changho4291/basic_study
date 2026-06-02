#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "soapH.h"
#include "Device.nsmap"
#include "wsse_username_token.h"

namespace {

constexpr const char* kDefaultUsername = "admin";
constexpr const char* kDefaultPassword = "password";

struct Options {
    int port = 8082;
    int timeout = 60;
    const char* username = kDefaultUsername;
    const char* password = kDefaultPassword;
    bool once = false;
};

Options g_options;

void print_usage(const char* program) {
    std::printf("Usage: %s [--port port] [--timeout seconds] [--username name] [--password value] [--once]\n",
                program);
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
        } else if (std::strcmp(argv[i], "--username") == 0 && i + 1 < argc) {
            options.username = argv[++i];
        } else if (std::strcmp(argv[i], "--password") == 0 && i + 1 < argc) {
            options.password = argv[++i];
        } else if (std::strcmp(argv[i], "--once") == 0) {
            options.once = true;
        } else {
            print_usage(argv[0]);
            std::exit(1);
        }
    }

    return options;
}

int verify_username_token(struct soap* soap) {
    std::string reason;
    if (!wsse_auth::verify_username_token_digest(
            soap,
            g_options.username,
            g_options.password,
            reason)) {
        std::printf("Authentication failed: %s\n", reason.c_str());
        return soap_sender_fault(soap, "WSSE authentication failed", reason.c_str());
    }

    _wsse__UsernameToken* token = wsse_auth::get_username_token(soap);
    std::printf("UsernameToken username: %s\n", token->Username);
    std::printf("UsernameToken created: %s\n", token->wsu__Created);
    std::printf("Authentication OK\n");
    return SOAP_OK;
}

}  // namespace

int tds__GetDeviceInformation(
    struct soap* soap,
    _tds__GetDeviceInformation* request,
    _tds__GetDeviceInformationResponse& response) {
    (void)request;

    std::printf("\nReceived GetDeviceInformation\n");

    const int auth_result = verify_username_token(soap);
    if (auth_result != SOAP_OK) {
        std::fflush(stdout);
        return auth_result;
    }

    response.Manufacturer = const_cast<char*>("StudyCamera");
    response.Model = const_cast<char*>("ONVIF-WSSE-Lab");
    response.FirmwareVersion = const_cast<char*>("1.0.0-auth-study");
    response.SerialNumber = const_cast<char*>("STUDY-AUTH-0001");
    response.HardwareId = const_cast<char*>("BASIC-STUDY-WSSE-HW");

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
    g_options = parse_options(argc, argv);

    soap* soap = soap_new1(SOAP_XML_INDENT);
    if (soap == nullptr) {
        std::fprintf(stderr, "soap_new1 failed\n");
        return 1;
    }

    soap->bind_flags = SO_REUSEADDR;
    soap->accept_timeout = g_options.timeout;
    soap->recv_timeout = g_options.timeout;
    soap->send_timeout = g_options.timeout;
    soap_set_namespaces(soap, namespaces);
    soap_set_sent_logfile(soap, "auth_fake_sent.xml");
    soap_set_recv_logfile(soap, "auth_fake_recv.xml");

    const int master = soap_bind(soap, nullptr, g_options.port, 100);
    if (!soap_valid_socket(master)) {
        soap_print_fault(soap, stderr);
        soap_destroy(soap);
        soap_end(soap);
        soap_free(soap);
        return 1;
    }

    std::printf("Fake authenticated ONVIF Device Service listening on http://127.0.0.1:%d/onvif/device_service\n",
                g_options.port);
    std::printf("Expected username: %s\n", g_options.username);
    std::printf("Timeout: %d seconds\n", g_options.timeout);
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

        if (g_options.once) {
            break;
        }
    }

    std::printf("\nFake authenticated device finished\n");
    std::printf("Sent log: auth_fake_sent.xml\n");
    std::printf("Recv log: auth_fake_recv.xml\n");

    soap_destroy(soap);
    soap_end(soap);
    soap_free(soap);
    return 0;
}
