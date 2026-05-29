#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "soapH.h"
#include "wsdd.nsmap"
#include "plugin/wsddapi.h"

namespace {

constexpr int kDiscoveryPort = 3702;
constexpr const char* kMulticastAddress = "239.255.255.250";
constexpr const char* kEndpointReference = "urn:uuid:11111111-2222-3333-4444-555555555555";
constexpr const char* kTypes = "dn:NetworkVideoTransmitter";
constexpr const char* kScopes =
    "onvif://www.onvif.org/Profile/Streaming "
    "onvif://www.onvif.org/name/StudyDevice";
constexpr const char* kXAddrs = "http://127.0.0.1/onvif/device_service";
constexpr unsigned int kMetadataVersion = 1;

struct Options {
    int timeout = 60;
    const char* announce_endpoint = nullptr;
};

Options parse_options(int argc, char** argv) {
    Options options;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--timeout") == 0 && i + 1 < argc) {
            const int timeout = std::atoi(argv[++i]);
            options.timeout = timeout > 0 ? timeout : options.timeout;
        } else if (std::strcmp(argv[i], "--announce") == 0 && i + 1 < argc) {
            options.announce_endpoint = argv[++i];
        } else {
            std::printf("Usage: %s [--timeout seconds] [--announce soap.udp://host:port]\n", argv[0]);
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

void send_hello(const char* endpoint) {
    soap* announcer = soap_new1(SOAP_IO_UDP);
    if (announcer == nullptr) {
        std::fprintf(stderr, "soap_new1 failed for Hello\n");
        return;
    }

    announcer->connect_flags = SO_BROADCAST;
    announcer->send_timeout = 3;
    soap_set_namespaces(announcer, namespaces);

    if (soap_wsdd_Hello(
            announcer,
            SOAP_WSDD_ADHOC,
            endpoint,
            soap_wsa_rand_uuid(announcer),
            nullptr,
            kEndpointReference,
            kTypes,
            kScopes,
            nullptr,
            kXAddrs,
            kMetadataVersion) != SOAP_OK) {
        soap_print_fault(announcer, stderr);
    }

    soap_destroy(announcer);
    soap_end(announcer);
    soap_free(announcer);
}

void send_bye(const char* endpoint) {
    soap* announcer = soap_new1(SOAP_IO_UDP);
    if (announcer == nullptr) {
        std::fprintf(stderr, "soap_new1 failed for Bye\n");
        return;
    }

    announcer->connect_flags = SO_BROADCAST;
    announcer->send_timeout = 3;
    soap_set_namespaces(announcer, namespaces);

    if (soap_wsdd_Bye(
            announcer,
            SOAP_WSDD_ADHOC,
            endpoint,
            soap_wsa_rand_uuid(announcer),
            kEndpointReference,
            kTypes,
            kScopes,
            nullptr,
            kXAddrs,
            kMetadataVersion) != SOAP_OK) {
        soap_print_fault(announcer, stderr);
    }

    soap_destroy(announcer);
    soap_end(announcer);
    soap_free(announcer);
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
    (void)MessageID;
    (void)RelatesTo;
    (void)EndpointReference;
    (void)Types;
    (void)Scopes;
    (void)MatchBy;
    (void)XAddrs;
    (void)MetadataVersion;
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
    (void)MessageID;
    (void)RelatesTo;
    (void)EndpointReference;
    (void)Types;
    (void)Scopes;
    (void)MatchBy;
    (void)XAddrs;
    (void)MetadataVersion;
}

// Server-side callback invoked by wsddapi.c when a Probe request arrives.
// The handler decides whether this target service matches and fills matches.
soap_wsdd_mode wsdd_event_Probe(
    struct soap* soap,
    const char* MessageID,
    const char* ReplyTo,
    const char* Types,
    const char* Scopes,
    const char* MatchBy,
    struct wsdd__ProbeMatchesType* matches) {
    (void)ReplyTo;
    (void)Scopes;
    (void)MatchBy;

    std::printf("\nReceived Probe\n");
    std::printf("MessageID: %s\n", MessageID != nullptr ? MessageID : "-");
    std::printf("Types: %s\n", Types != nullptr ? Types : "-");

    const bool type_matches =
        Types == nullptr ||
        std::strstr(Types, "dn:NetworkVideoTransmitter") != nullptr ||
        std::strstr(Types, "NetworkVideoTransmitter") != nullptr;

    if (!type_matches) {
        std::printf("Ignore Probe: type does not match this target service\n");
        return SOAP_WSDD_MANAGED;
    }

    if (soap_wsdd_add_ProbeMatch(
            soap,
            matches,
            kEndpointReference,
            kTypes,
            kScopes,
            nullptr,
            kXAddrs,
            kMetadataVersion) != SOAP_OK) {
        soap_print_fault(soap, stderr);
    }

    std::printf("Send ProbeMatches\n");
    std::printf("EndpointReference: %s\n", kEndpointReference);
    std::printf("Types: %s\n", kTypes);
    std::printf("Scopes: %s\n", kScopes);
    std::printf("XAddrs: %s\n", kXAddrs);
    std::fflush(stdout);

    // Returning MANAGED lets wsddapi.c send the ProbeMatches response using
    // the matches populated above.
    return SOAP_WSDD_MANAGED;
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
    (void)MessageID;
    (void)ReplyTo;

    std::printf("\nReceived Resolve\n");
    std::printf("EndpointReference: %s\n", EndpointReference != nullptr ? EndpointReference : "-");

    if (EndpointReference == nullptr || std::strcmp(EndpointReference, kEndpointReference) != 0) {
        std::printf("Ignore Resolve: endpoint reference does not match this target service\n");
        return SOAP_WSDD_ADHOC;
    }

    auto* scopes = static_cast<wsdd__ScopesType*>(soap_malloc(soap, sizeof(wsdd__ScopesType)));
    if (scopes == nullptr) {
        return SOAP_WSDD_ADHOC;
    }

    soap_default_wsdd__ScopesType(soap, scopes);
    scopes->__item = const_cast<char*>(kScopes);
    scopes->MatchBy = nullptr;

    match->wsa5__EndpointReference.Address = const_cast<char*>(kEndpointReference);
    match->Types = const_cast<char*>(kTypes);
    match->Scopes = scopes;
    match->XAddrs = const_cast<char*>(kXAddrs);
    match->MetadataVersion = kMetadataVersion;

    std::printf("Send ResolveMatches\n");
    std::printf("Types: %s\n", kTypes);
    std::printf("Scopes: %s\n", kScopes);
    std::printf("XAddrs: %s\n", kXAddrs);
    std::fflush(stdout);

    return SOAP_WSDD_MANAGED;
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
    soap->connect_flags = SO_BROADCAST;
    soap->accept_timeout = options.timeout;
    soap->recv_timeout = options.timeout;
    soap->send_timeout = options.timeout;
    soap_set_namespaces(soap, namespaces);
    soap_set_sent_logfile(soap, "responder_sent.xml");
    soap_set_recv_logfile(soap, "responder_recv.xml");

    const int master = soap_bind(soap, nullptr, kDiscoveryPort, 100);
    if (!soap_valid_socket(master)) {
        soap_print_fault(soap, stderr);
        soap_destroy(soap);
        soap_end(soap);
        soap_free(soap);
        return 1;
    }

    if (!join_multicast_group(master)) {
        soap_destroy(soap);
        soap_end(soap);
        soap_free(soap);
        return 1;
    }

    std::printf("WS-Discovery responder listening on %s:%d for %d seconds\n",
                kMulticastAddress,
                kDiscoveryPort,
                options.timeout);
    std::printf("Advertised XAddrs: %s\n", kXAddrs);
    std::fflush(stdout);

    if (options.announce_endpoint != nullptr) {
        std::printf("Send Hello: %s\n", options.announce_endpoint);
        send_hello(options.announce_endpoint);
    }

    if (soap_wsdd_listen(soap, options.timeout) != SOAP_OK) {
        soap_print_fault(soap, stderr);
    }

    if (options.announce_endpoint != nullptr) {
        std::printf("Send Bye: %s\n", options.announce_endpoint);
        send_bye(options.announce_endpoint);
    }

    std::printf("\nResponder finished\n");
    std::printf("Sent log: responder_sent.xml\n");
    std::printf("Recv log: responder_recv.xml\n");

    soap_destroy(soap);
    soap_end(soap);
    soap_free(soap);
    return 0;
}
