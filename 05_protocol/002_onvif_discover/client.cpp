#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <sys/socket.h>

#include "soapH.h"
#include "wsdd.nsmap"
#include "plugin/wsddapi.h"

namespace {

// WS-Discovery uses UDP multicast on 239.255.255.250:3702.
// gSOAP expresses UDP endpoints with the "soap.udp://" scheme.
constexpr const char* kDiscoveryEndpoint = "soap.udp://239.255.255.250:3702";

// ONVIF video devices are discovered as NetworkVideoTransmitter.
// The "dn" prefix is provided by the generated wsdd.nsmap namespace table.
constexpr const char* kOnvifDeviceType = "dn:NetworkVideoTransmitter";

struct DiscoveryState {
    int matches = 0;
};

struct Options {
    int timeout = 5;
    const char* endpoint = kDiscoveryEndpoint;
};

void print_text(const char* label, const char* value) {
    std::printf("%s: %s\n", label, value != nullptr ? value : "-");
}

Options parse_options(int argc, char** argv) {
    Options options;

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--timeout") == 0 && i + 1 < argc) {
            const int timeout = std::atoi(argv[++i]);
            options.timeout = timeout > 0 ? timeout : options.timeout;
        } else if (std::strcmp(argv[i], "--endpoint") == 0 && i + 1 < argc) {
            options.endpoint = argv[++i];
        } else {
            std::printf("Usage: %s [--timeout seconds] [--endpoint soap.udp://host:port]\n", argv[0]);
            std::exit(1);
        }
    }

    return options;
}

}  // namespace

// Client-side callback invoked by gSOAP's WS-Discovery plugin when a
// ProbeMatches message is received. The required signature is declared in
// /usr/share/gsoap/plugin/wsddapi.h.
void wsdd_event_ProbeMatches(
    struct soap* soap,
    unsigned int InstanceId,
    const char* SequenceId,
    unsigned int MessageNumber,
    const char* MessageID,
    const char* RelatesTo,
    struct wsdd__ProbeMatchesType* matches) {
    (void)InstanceId;
    (void)SequenceId;
    (void)MessageNumber;
    (void)MessageID;
    (void)RelatesTo;

    auto* state = static_cast<DiscoveryState*>(soap->user);

    for (int i = 0; matches != nullptr && i < matches->__sizeProbeMatch; ++i) {
        const wsdd__ProbeMatchType& match = matches->ProbeMatch[i];

        if (state != nullptr) {
            ++state->matches;
            std::printf("\nFound ONVIF device #%d\n", state->matches);
        } else {
            std::printf("\nFound ONVIF device\n");
        }

        print_text("EndpointReference", match.wsa5__EndpointReference.Address);
        print_text("Types", match.Types);
        print_text("Scopes", match.Scopes != nullptr ? match.Scopes->__item : nullptr);
        print_text("XAddrs", match.XAddrs);
        std::printf("MetadataVersion: %u\n", match.MetadataVersion);
    }
}

// The WS-Discovery plugin requires the application to define all wsdd_event_*
// handlers. This discovery client only cares about ProbeMatches, so the
// remaining handlers are intentionally no-op implementations.
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
    DiscoveryState state;

    // WS-Discovery is SOAP over UDP, so the gSOAP context must be UDP-enabled.
    soap* soap = soap_new1(SOAP_IO_UDP);
    if (soap == nullptr) {
        std::fprintf(stderr, "soap_new1 failed\n");
        return 1;
    }

    // soap->user is passed back to wsdd_event_* callbacks.
    soap->user = &state;
    soap->bind_flags = SO_REUSEADDR;
    soap->connect_flags = SO_BROADCAST;
    soap->connect_timeout = options.timeout;
    soap->recv_timeout = options.timeout;

    // namespaces is generated into wsdd.nsmap by soapcpp2.
    soap_set_namespaces(soap, namespaces);

    // These logs are useful for comparing generated SOAP XML with ONVIF specs.
    soap_set_sent_logfile(soap, "sent_probe.xml");
    soap_set_recv_logfile(soap, "recv_probe.xml");

    // Bind an ephemeral local UDP port so ProbeMatches responses can be received.
    if (!soap_valid_socket(soap_bind(soap, nullptr, 0, 100))) {
        soap_print_fault(soap, stderr);
        soap_destroy(soap);
        soap_end(soap);
        soap_free(soap);
        return 1;
    }

    const char* message_id = soap_wsa_rand_uuid(soap);

    std::printf("Send WS-Discovery Probe\n");
    std::printf("Endpoint: %s\n", options.endpoint);
    std::printf("Types: %s\n", kOnvifDeviceType);
    std::printf("MessageID: %s\n", message_id);
    std::fflush(stdout);

    // Send WS-Discovery Probe to target services in ad-hoc multicast mode.
    // Implementation: /usr/share/gsoap/plugin/wsddapi.c::soap_wsdd_Probe
    if (soap_wsdd_Probe(
            soap,
            SOAP_WSDD_ADHOC,
            SOAP_WSDD_TO_TS,
            options.endpoint,
            message_id,
            nullptr,
            kOnvifDeviceType,
            nullptr,
            nullptr) != SOAP_OK) {
        soap_print_fault(soap, stderr);
        soap_destroy(soap);
        soap_end(soap);
        soap_free(soap);
        return 1;
    }

    std::printf("Wait ProbeMatches for %d seconds...\n", options.timeout);
    std::fflush(stdout);

    // Listen for WS-Discovery responses. When ProbeMatches is received,
    // wsddapi.c dispatches it to wsdd_event_ProbeMatches above.
    if (soap_wsdd_listen(soap, options.timeout) != SOAP_OK) {
        soap_print_fault(soap, stderr);
    }

    if (state.matches == 0) {
        std::printf("\nNo ONVIF device found.\n");
    }

    std::printf("\nSent log: sent_probe.xml\n");
    std::printf("Recv log: recv_probe.xml\n");

    soap_destroy(soap);
    soap_end(soap);
    soap_free(soap);
    return 0;
}
