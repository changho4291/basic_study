#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "soapH.h"
#include "wsdd.nsmap"
#include "plugin/wsddapi.h"

namespace {

constexpr const char* kMulticastAddress = "239.255.255.250";

struct Options {
    int port = 3703;
    int timeout = 10;
    bool join_multicast = false;
};

Options parse_options(int argc, char** argv) {
    Options options;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--port") == 0 && i + 1 < argc) {
            const int port = std::atoi(argv[++i]);
            options.port = port > 0 ? port : options.port;
        } else if (std::strcmp(argv[i], "--timeout") == 0 && i + 1 < argc) {
            const int timeout = std::atoi(argv[++i]);
            options.timeout = timeout > 0 ? timeout : options.timeout;
        } else if (std::strcmp(argv[i], "--join-multicast") == 0) {
            options.join_multicast = true;
        } else {
            std::printf("Usage: %s [--port port] [--timeout seconds] [--join-multicast]\n", argv[0]);
            std::exit(1);
        }
    }

    return options;
}

bool join_multicast_group(int sock) {
    ip_mreq mreq {};
    mreq.imr_multiaddr.s_addr = inet_addr(kMulticastAddress);
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        std::perror("setsockopt IP_ADD_MEMBERSHIP failed");
        return false;
    }

    return true;
}

void print_text(const char* label, const char* value) {
    std::printf("%s: %s\n", label, value != nullptr ? value : "-");
}

}  // namespace

void wsdd_event_Hello(
    struct soap* soap,
    unsigned int InstanceId,
    const char* SequenceId,
    unsigned int MessageNumber,
    const char* MessageID,
    const char* RelatesTo,
    const char* EndpointReference,
    const char* Types,
    const char* Scopes,
    const char* MatchBy,
    const char* XAddrs,
    unsigned int MetadataVersion) {
    (void)soap;
    (void)InstanceId;
    (void)SequenceId;
    (void)MessageNumber;
    (void)RelatesTo;
    (void)MatchBy;

    std::printf("\nReceived Hello\n");
    print_text("MessageID", MessageID);
    print_text("EndpointReference", EndpointReference);
    print_text("Types", Types);
    print_text("Scopes", Scopes);
    print_text("XAddrs", XAddrs);
    std::printf("MetadataVersion: %u\n", MetadataVersion);
}

void wsdd_event_Bye(
    struct soap* soap,
    unsigned int InstanceId,
    const char* SequenceId,
    unsigned int MessageNumber,
    const char* MessageID,
    const char* RelatesTo,
    const char* EndpointReference,
    const char* Types,
    const char* Scopes,
    const char* MatchBy,
    const char* XAddrs,
    unsigned int* MetadataVersion) {
    (void)soap;
    (void)InstanceId;
    (void)SequenceId;
    (void)MessageNumber;
    (void)RelatesTo;
    (void)MatchBy;

    std::printf("\nReceived Bye\n");
    print_text("MessageID", MessageID);
    print_text("EndpointReference", EndpointReference);
    print_text("Types", Types);
    print_text("Scopes", Scopes);
    print_text("XAddrs", XAddrs);
    if (MetadataVersion != nullptr) {
        std::printf("MetadataVersion: %u\n", *MetadataVersion);
    }
}

soap_wsdd_mode wsdd_event_Probe(
    struct soap* soap,
    const char* MessageID,
    const char* ReplyTo,
    const char* Types,
    const char* Scopes,
    const char* MatchBy,
    struct wsdd__ProbeMatchesType* matches) {
    (void)soap;
    (void)MessageID;
    (void)ReplyTo;
    (void)Types;
    (void)Scopes;
    (void)MatchBy;
    (void)matches;
    return SOAP_WSDD_ADHOC;
}

void wsdd_event_ProbeMatches(
    struct soap* soap,
    unsigned int InstanceId,
    const char* SequenceId,
    unsigned int MessageNumber,
    const char* MessageID,
    const char* RelatesTo,
    struct wsdd__ProbeMatchesType* matches) {
    (void)soap;
    (void)InstanceId;
    (void)SequenceId;
    (void)MessageNumber;
    (void)MessageID;
    (void)RelatesTo;
    (void)matches;
}

soap_wsdd_mode wsdd_event_Resolve(
    struct soap* soap,
    const char* MessageID,
    const char* ReplyTo,
    const char* EndpointReference,
    struct wsdd__ResolveMatchType* match) {
    (void)soap;
    (void)MessageID;
    (void)ReplyTo;
    (void)EndpointReference;
    (void)match;
    return SOAP_WSDD_ADHOC;
}

void wsdd_event_ResolveMatches(
    struct soap* soap,
    unsigned int InstanceId,
    const char* SequenceId,
    unsigned int MessageNumber,
    const char* MessageID,
    const char* RelatesTo,
    struct wsdd__ResolveMatchType* match) {
    (void)soap;
    (void)InstanceId;
    (void)SequenceId;
    (void)MessageNumber;
    (void)MessageID;
    (void)RelatesTo;
    (void)match;
}

int main(int argc, char** argv) {
    const Options options = parse_options(argc, argv);

    soap* soap = soap_new1(SOAP_IO_UDP);
    if (soap == nullptr) {
        std::fprintf(stderr, "soap_new1 failed\n");
        return 1;
    }

    soap->bind_flags = SO_REUSEADDR;
    soap->accept_timeout = options.timeout;
    soap->recv_timeout = options.timeout;
    soap_set_namespaces(soap, namespaces);
    soap_set_recv_logfile(soap, "monitor_recv.xml");

    const int master = soap_bind(soap, nullptr, options.port, 100);
    if (!soap_valid_socket(master)) {
        soap_print_fault(soap, stderr);
        soap_destroy(soap);
        soap_end(soap);
        soap_free(soap);
        return 1;
    }

    if (options.join_multicast && !join_multicast_group(master)) {
        soap_destroy(soap);
        soap_end(soap);
        soap_free(soap);
        return 1;
    }

    std::printf("WS-Discovery monitor listening on UDP port %d for %d seconds\n",
                options.port,
                options.timeout);
    std::fflush(stdout);

    if (soap_wsdd_listen(soap, options.timeout) != SOAP_OK) {
        soap_print_fault(soap, stderr);
    }

    std::printf("\nMonitor finished\n");

    soap_destroy(soap);
    soap_end(soap);
    soap_free(soap);
    return 0;
}
