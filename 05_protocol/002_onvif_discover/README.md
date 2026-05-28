# 002_onvif_discover

gSOAP의 WS-Discovery 플러그인을 사용해서 ONVIF 장비를 찾는 실습이다.

이번 단계의 목적은 `make`나 CMake 사용법이 아니라, gSOAP 기반 ONVIF discovery가 어떤 파일 생성/컴파일 흐름으로 동작하는지 직접 확인하는 것이다.

```text
onvif_discover.h
  -> soapcpp2
  -> soapH.h / soapC.cpp / soapClient.cpp / wsdd.nsmap
  -> client.cpp와 함께 g++ 빌드
  -> soap_wsdd_Probe()
  -> soap_wsdd_listen()
  -> wsdd_event_ProbeMatches()
```

이 실습은 두 바이너리로 구성한다.

```text
discover   : WS-Discovery Probe를 보내는 client
responder  : Probe를 받고 ProbeMatches를 돌려주는 가짜 ONVIF Target Service
```

## Environment

현재 host에는 `soapcpp2`, `wsdl2h`가 없고, `05_protocol/docker-compose.yml`의 `server` 컨테이너 안에 gSOAP 도구가 있다.

확인된 컨테이너 내부 경로:

```text
/usr/bin/soapcpp2
/usr/bin/wsdl2h
/usr/share/gsoap/import/wsdd.h
/usr/share/gsoap/plugin/wsddapi.c
/usr/share/gsoap/plugin/wsddapi.h
/usr/share/gsoap/plugin/wsaapi.c
/usr/share/gsoap/plugin/threads.c
/usr/include/stdsoap2.h
```

따라서 아래 실습은 컨테이너 안에서 진행한다.

```bash
cd 05_protocol
docker compose up -d
docker compose exec server bash
cd /workspace/002_onvif_discover
```

## Files To Write

소스가 하나도 없다고 가정하면 아래 세 파일을 직접 만든다.

```text
002_onvif_discover/
  onvif_discover.h
  client.cpp
  responder.cpp
```

## onvif_discover.h

`soapcpp2`의 입력 파일이다.

```cpp
// gSOAP input file for WS-Discovery.
//
// This imports the gSOAP-provided WS-Discovery schema header.
// Run `soapcpp2` on this file to generate soapH.h, soapC.cpp,
// soapClient.cpp, and the namespace map used by client.cpp.

#import "wsdd.h"
```

중요한 점:

- `wsdd.h`는 gSOAP가 제공하는 WS-Discovery schema header다.
- 이 파일은 `/usr/share/gsoap/import/wsdd.h`에 있다.
- ONVIF discovery는 WS-Discovery를 사용하므로 여기서 시작한다.

## client.cpp

Role:

```text
WS-Discovery client
  -> Probe 전송
  -> ProbeMatches 수신
  -> 발견한 EndpointReference / Types / Scopes / XAddrs 출력
```

Important callbacks:

```text
wsdd_event_ProbeMatches  : responder 또는 실제 ONVIF 장비의 응답 처리
wsdd_event_Hello         : 이 실습에서는 no-op
wsdd_event_Bye           : 이 실습에서는 no-op
wsdd_event_Probe         : 이 실습에서는 no-op
wsdd_event_Resolve       : 이 실습에서는 no-op
wsdd_event_ResolveMatches: 이 실습에서는 no-op
```

Runtime flow:

```text
soap_new1(SOAP_IO_UDP)
  -> soap_bind(..., port 0)
  -> soap_wsdd_Probe(...)
  -> soap_wsdd_listen(...)
  -> wsdd_event_ProbeMatches(...)
```

Logs:

```text
sent_probe.xml
recv_probe.xml
```

```cpp
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
```

주의: 위 코드에서 `soap_wsdd_Probe()`의 `Types` 인자로 들어가는 값은 `dn:NetworkVideoTransmitter`다. 이 값이 ONVIF Network Video Transmitter 장비를 찾겠다는 의미다.

## responder.cpp

Role:

```text
WS-Discovery Target Service simulator
  -> Probe 수신
  -> Probe type 확인
  -> ProbeMatches 생성
  -> client로 응답
```

Important callbacks:

```text
wsdd_event_Probe         : client의 Probe 요청 처리
wsdd_event_ProbeMatches  : 이 실습에서는 no-op
wsdd_event_Hello         : 이 실습에서는 no-op
wsdd_event_Bye           : 이 실습에서는 no-op
wsdd_event_Resolve       : 이 실습에서는 no-op
wsdd_event_ResolveMatches: 이 실습에서는 no-op
```

Runtime flow:

```text
soap_new1(SOAP_IO_UDP)
  -> soap_bind(..., port 3702)
  -> IP_ADD_MEMBERSHIP 239.255.255.250
  -> soap_wsdd_listen(...)
  -> wsdd_event_Probe(...)
  -> soap_wsdd_add_ProbeMatch(...)
  -> return SOAP_WSDD_MANAGED
```

Logs:

```text
responder_sent.xml
responder_recv.xml
```

Source:

```cpp
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

int parse_timeout(int argc, char** argv) {
    if (argc == 1) {
        return 60;
    }

    if (argc == 3 && std::strcmp(argv[1], "--timeout") == 0) {
        const int timeout = std::atoi(argv[2]);
        return timeout > 0 ? timeout : 60;
    }

    std::printf("Usage: %s [--timeout seconds]\n", argv[0]);
    std::exit(1);
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
    const int timeout = parse_timeout(argc, argv);

    soap* soap = soap_new1(SOAP_IO_UDP);
    if (soap == nullptr) {
        std::fprintf(stderr, "soap_new1 failed\n");
        return 1;
    }

    soap->bind_flags = SO_REUSEADDR;
    soap->connect_flags = SO_BROADCAST;
    soap->accept_timeout = timeout;
    soap->recv_timeout = timeout;
    soap->send_timeout = timeout;
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
                timeout);
    std::printf("Advertised XAddrs: %s\n", kXAddrs);
    std::fflush(stdout);

    if (soap_wsdd_listen(soap, timeout) != SOAP_OK) {
        soap_print_fault(soap, stderr);
    }

    std::printf("\nResponder finished\n");
    std::printf("Sent log: responder_sent.xml\n");
    std::printf("Recv log: responder_recv.xml\n");

    soap_destroy(soap);
    soap_end(soap);
    soap_free(soap);
    return 0;
}
```

Core response values:

```text
EndpointReference: urn:uuid:11111111-2222-3333-4444-555555555555
Types: dn:NetworkVideoTransmitter
Scopes: onvif://www.onvif.org/Profile/Streaming onvif://www.onvif.org/name/StudyDevice
XAddrs: http://127.0.0.1/onvif/device_service
MetadataVersion: 1
```

중요한 함수는 `wsdd_event_Probe()`다.

```cpp
soap_wsdd_mode wsdd_event_Probe(
    struct soap* soap,
    const char* MessageID,
    const char* ReplyTo,
    const char* Types,
    const char* Scopes,
    const char* MatchBy,
    struct wsdd__ProbeMatchesType* matches)
```

`wsddapi.c`는 Probe를 받으면 이 callback을 호출한다.

responder는 이 callback 안에서 `soap_wsdd_add_ProbeMatch()`로 응답 내용을 채운다.

```cpp
soap_wsdd_add_ProbeMatch(
    soap,
    matches,
    "urn:uuid:11111111-2222-3333-4444-555555555555",
    "dn:NetworkVideoTransmitter",
    "onvif://www.onvif.org/Profile/Streaming onvif://www.onvif.org/name/StudyDevice",
    nullptr,
    "http://127.0.0.1/onvif/device_service",
    1);
```

그 다음 `SOAP_WSDD_MANAGED`를 반환하면 `wsddapi.c`가 `ProbeMatches` 메시지를 만들어 응답한다.

```cpp
return SOAP_WSDD_MANAGED;
```

컨테이너 환경에서는 multicast loopback이 항상 기대대로 동작하지 않을 수 있다. 그래서 이 실습의 로컬 왕복 검증은 client가 `soap.udp://127.0.0.1:3702`로 직접 보내는 unicast 방식으로 진행한다.

## Step 1. Generate gSOAP Code

`onvif_discover.h`에서 gSOAP 코드를 생성한다.

```bash
soapcpp2 -2 -C -I/usr/share/gsoap/import -I/usr/share/gsoap onvif_discover.h
```

옵션 의미:

- `-2`: SOAP 1.2 코드 생성
- `-C`: client-side 코드 생성
- `-I/usr/share/gsoap/import`: `wsdd.h` 같은 import header 검색 경로
- `-I/usr/share/gsoap`: gSOAP 기본 검색 경로

생성되는 주요 파일:

```text
soapH.h
soapStub.h
soapC.cpp
soapClient.cpp
soapClientLib.cpp
wsdd.nsmap
wsdd.Probe.req.xml
wsdd.ProbeMatches.req.xml
```

중요한 점:

- `wsdd.nsmap`이 생성된다.
- `client.cpp`에서 `#include "wsdd.nsmap"`을 하므로 이 파일이 필요하다.
- `soapC.cpp`, `soapClient.cpp`는 g++ 빌드에 직접 들어간다.

## Step 2. Build With g++

생성된 gSOAP 코드와 WS-Discovery plugin을 함께 빌드한다.

### Build discover client

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O2 \
  -I. \
  -I/usr/share/gsoap \
  -I/usr/share/gsoap/import \
  -I/usr/share/gsoap/plugin \
  -o discover \
  client.cpp \
  soapC.cpp \
  soapClient.cpp \
  /usr/share/gsoap/plugin/wsddapi.c \
  /usr/share/gsoap/plugin/wsaapi.c \
  /usr/share/gsoap/plugin/threads.c \
  -lgsoap++
```

### Build responder target service

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O2 \
  -I. \
  -I/usr/share/gsoap \
  -I/usr/share/gsoap/import \
  -I/usr/share/gsoap/plugin \
  -o responder \
  responder.cpp \
  soapC.cpp \
  soapClient.cpp \
  /usr/share/gsoap/plugin/wsddapi.c \
  /usr/share/gsoap/plugin/wsaapi.c \
  /usr/share/gsoap/plugin/threads.c \
  -lgsoap++
```

빌드에 들어가는 파일 묶음:

```text
client.cpp
responder.cpp
soapC.cpp
soapClient.cpp
wsddapi.c
wsaapi.c
threads.c
libgsoap++
```

여기서 중요한 점은 gSOAP plugin의 `.c` 파일도 `g++` 명령에 같이 넣는다는 것이다. C++ 코드와 링크해야 하므로 이 실습에서는 `g++`로 한 번에 컴파일한다.

## Step 3. Run Local Round Trip

터미널 1에서 responder를 먼저 실행한다.

```bash
./responder --timeout 20
```

터미널 2에서 client를 실행한다.

```bash
./discover --endpoint soap.udp://127.0.0.1:3702 --timeout 3
```

성공하면 client 쪽에서 다음처럼 출력된다.

```text
Send WS-Discovery Probe
Endpoint: soap.udp://127.0.0.1:3702
Types: dn:NetworkVideoTransmitter
MessageID: urn:uuid:...
Wait ProbeMatches for 3 seconds...

Found ONVIF device #1
EndpointReference: urn:uuid:11111111-2222-3333-4444-555555555555
Types: "":NetworkVideoTransmitter
Scopes: onvif://www.onvif.org/Profile/Streaming onvif://www.onvif.org/name/StudyDevice
XAddrs: http://127.0.0.1/onvif/device_service
MetadataVersion: 1

Sent log: sent_probe.xml
Recv log: recv_probe.xml
```

responder 쪽에서는 다음처럼 출력된다.

```text
Received Probe
MessageID: urn:uuid:...
Types: "":NetworkVideoTransmitter
Send ProbeMatches
EndpointReference: urn:uuid:11111111-2222-3333-4444-555555555555
Types: dn:NetworkVideoTransmitter
Scopes: onvif://www.onvif.org/Profile/Streaming onvif://www.onvif.org/name/StudyDevice
XAddrs: http://127.0.0.1/onvif/device_service
```

실제 네트워크에서 ONVIF 장비를 찾으려면 기본 multicast endpoint를 사용한다.

```bash
./discover --timeout 5
```

ONVIF 장비가 없는 환경에서는 다음처럼 끝난다.

```text
Send WS-Discovery Probe
Endpoint: soap.udp://239.255.255.250:3702
Types: dn:NetworkVideoTransmitter
MessageID: urn:uuid:...
Wait ProbeMatches for 2 seconds...

No ONVIF device found.

Sent log: sent_probe.xml
Recv log: recv_probe.xml
```

ONVIF 장비가 응답하면 다음처럼 출력된다.

```text
Found ONVIF device #1
EndpointReference: urn:uuid:...
Types: dn:NetworkVideoTransmitter
Scopes: onvif://www.onvif.org/Profile/Streaming ...
XAddrs: http://192.168.0.25/onvif/device_service
MetadataVersion: 1
```

## What To Observe

### 1. soapcpp2 입력과 출력

입력:

```text
onvif_discover.h
```

출력:

```text
soapH.h
soapC.cpp
soapClient.cpp
wsdd.nsmap
```

`001_Soap_basic`에서 직접 만든 `calc.h`를 `soapcpp2`에 넣었던 것처럼, 이번에는 gSOAP가 제공하는 `wsdd.h`를 import해서 WS-Discovery용 코드를 생성한다.

### 2. Probe 전송

```cpp
soap_wsdd_Probe(...)
```

이 함수가 WS-Discovery `Probe` 메시지를 만든다.

핵심 인자:

```cpp
"soap.udp://239.255.255.250:3702"
"dn:NetworkVideoTransmitter"
```

첫 번째는 WS-Discovery multicast endpoint이고, 두 번째는 ONVIF camera/NVT 장비 type이다.

### 3. ProbeMatches 수신

```cpp
soap_wsdd_listen(soap, timeout);
```

이 함수가 UDP 응답을 기다린다.

`ProbeMatches`가 오면 gSOAP plugin이 아래 callback을 호출한다.

```cpp
void wsdd_event_ProbeMatches(...)
```

### 4. XAddrs

`XAddrs`는 다음 단계에서 사용할 Device Service endpoint다.

```text
XAddrs: http://192.168.0.25/onvif/device_service
```

다음 실습 `003_onvif_device_info`에서는 이 주소로 `GetDeviceInformation` SOAP 요청을 보낸다.

## Where To Read The gSOAP Implementation

이 실습의 `client.cpp`에서 직접 작성한 부분과 gSOAP가 제공하는 부분은 분리해서 보면 이해하기 쉽다.

### gSOAP schema input

```text
/usr/share/gsoap/import/wsdd.h
```

여기서 확인할 것:

- `wsdd__ProbeMatchesType`
- `wsdd__ProbeMatchType`
- `wsdd__ScopesType`
- WS-Discovery message type 구조

`client.cpp`에서 아래 필드를 출력할 수 있는 이유가 이 header에 구조체로 정의되어 있기 때문이다.

```cpp
matches->__sizeProbeMatch
matches->ProbeMatch[i]
match.wsa5__EndpointReference.Address
match.Types
match.Scopes->__item
match.XAddrs
```

### WS-Discovery plugin API

```text
/usr/share/gsoap/plugin/wsddapi.h
```

여기서 확인할 것:

- `soap_wsdd_Probe(...)`
- `soap_wsdd_listen(...)`
- `wsdd_event_ProbeMatches(...)`
- `wsdd_event_Hello(...)`
- `wsdd_event_Bye(...)`
- `wsdd_event_Probe(...)`
- `wsdd_event_Resolve(...)`
- `wsdd_event_ResolveMatches(...)`

`wsddapi.h`는 애플리케이션이 구현해야 하는 callback signature를 선언한다.

그래서 `client.cpp`에 사용하지 않는 `wsdd_event_Hello`, `wsdd_event_Bye`, `wsdd_event_Probe`, `wsdd_event_Resolve`, `wsdd_event_ResolveMatches`도 빈 함수로 정의해둔다. 링커가 이 함수들을 찾기 때문이다.

### WS-Discovery plugin implementation

```text
/usr/share/gsoap/plugin/wsddapi.c
```

여기서 확인할 것:

- `soap_wsdd_Probe`가 SOAP Header와 `wsdd__ProbeType`을 채우는 방식
- `soap_wsdd_listen`이 `soap_accept`, `soap_begin_serve`, `soap_wsdd_serve_request`를 호출하는 흐름
- 수신한 `ProbeMatches`가 `wsdd_event_ProbeMatches`로 dispatch되는 흐름

즉, 이 실습에서 직접 구현하는 것은 `ProbeMatches`를 받았을 때 무엇을 출력할지이고, WS-Discovery 메시지 생성/수신/dispatch는 `wsddapi.c`가 담당한다.

### WS-Addressing helper

```text
/usr/share/gsoap/plugin/wsaapi.c
/usr/share/gsoap/plugin/wsaapi.h
```

여기서 확인할 것:

- `soap_wsa_rand_uuid(...)`
- WS-Addressing `MessageID`, `To`, `Action`, `ReplyTo` 처리

`client.cpp`의 아래 코드는 `wsaapi` helper를 사용하는 부분이다.

```cpp
const char* message_id = soap_wsa_rand_uuid(soap);
```

### Generated files

```text
soapH.h
soapC.cpp
soapClient.cpp
wsdd.nsmap
```

이 파일들은 `soapcpp2`가 생성한다.

- `soapH.h`: generated type/function declarations
- `soapC.cpp`: serialization/deserialization
- `soapClient.cpp`: generated client send/receive stubs
- `wsdd.nsmap`: namespace prefix mapping

실습 중에는 generated file을 처음부터 다 읽기보다, `wsddapi.c`에서 호출하는 generated 함수 이름을 따라가면서 보는 편이 좋다.

## Troubleshooting

### host에서 soapcpp2가 없는 경우

host에서 아래 명령이 실패하면:

```bash
soapcpp2 --version
```

컨테이너 안으로 들어가서 실습한다.

```bash
cd 05_protocol
docker compose up -d
docker compose exec server bash
cd /workspace/002_onvif_discover
```

### soapcpp2가 `#import`를 못 읽는 경우

`onvif_discover.h` 파일 끝에 newline이 없거나 파일 내용이 깨졌을 수 있다.

정상 내용:

```cpp
#import "wsdd.h"
```

마지막 줄 뒤에 줄바꿈이 있도록 저장한다.

### wsdd.nsmap not found

`soapcpp2`를 먼저 실행하지 않았거나 생성에 실패한 것이다.

```bash
soapcpp2 -2 -C -I/usr/share/gsoap/import -I/usr/share/gsoap onvif_discover.h
```

성공하면 `wsdd.nsmap`이 생긴다.

### undefined reference to soap_wsdd_Probe

`g++` 빌드 명령에서 `wsddapi.c`가 빠진 것이다.

아래 파일들이 들어가야 한다.

```text
/usr/share/gsoap/plugin/wsddapi.c
/usr/share/gsoap/plugin/wsaapi.c
/usr/share/gsoap/plugin/threads.c
```

### 아무 장비도 안 잡히는 경우

아래를 확인한다.

- 카메라와 PC가 같은 L2 network에 있는가
- 카메라 설정에서 ONVIF가 켜져 있는가
- 방화벽에서 UDP 3702 송수신이 허용되어 있는가
- Docker bridge network 때문에 multicast가 막히고 있지는 않은가

빌드 학습은 Docker 안에서 해도 되지만, 실제 카메라 discovery는 host network에서 실행하는 편이 더 잘 된다.

## Clean

수동으로 생성물을 지우려면:

```bash
rm -f discover
rm -f sent_probe.xml recv_probe.xml
rm -f soapH.h soapStub.h soapC.cpp soapClient.cpp soapClientLib.cpp wsdd.nsmap
rm -f wsdd.*.req.xml wsdd.*.res.xml soap.nsmap soap*.xml
```
