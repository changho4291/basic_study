# 002_onvif_discover

gSOAP `wsddapi` plugin으로 ONVIF WS-Discovery를 실습한다.

이번 실습은 세 역할로 구성한다.

```text
discover   : Probe / Resolve를 보내는 discovery client
responder  : Probe / Resolve에 응답하는 fake ONVIF Target Service
monitor    : Hello / Bye passive event를 관찰하는 listener
```

## What Changed In This Update

이번 수정에서 기존 `Probe / ProbeMatches` 실습에 아래 기능을 추가했다.

```text
client.cpp
  -> --resolve 옵션 추가
  -> soap_wsdd_Resolve() 호출 경로 추가
  -> wsdd_event_ResolveMatches()에서 ResolveMatches 출력

responder.cpp
  -> --announce 옵션 추가
  -> soap_wsdd_Hello() / soap_wsdd_Bye() 전송 함수 추가
  -> wsdd_event_Resolve()에서 ResolveMatch 응답 생성

monitor.cpp
  -> 새 파일
  -> Hello / Bye 이벤트를 수신해서 출력하는 passive listener

.gitignore
  -> monitor 실행 파일과 monitor_recv.xml 로그 제외

ONVIF_TODO_ROADMAP.md
  -> Resolve / Hello / Bye 실습 완료 항목 추가
```

즉 현재 `002_onvif_discover`는 ONVIF WS-Discovery의 네 가지 메시지 흐름을 로컬에서 확인하는 단계다.

```text
Probe    -> ProbeMatches     : "ONVIF 장비 있나?"
Resolve  -> ResolveMatches   : "이 EndpointReference 장비의 주소를 알려줘"
Hello                       : "장비가 네트워크에 나타남"
Bye                         : "장비가 네트워크에서 사라짐"
```

## Probe vs Resolve

`Probe`와 `Resolve`는 둘 다 WS-Discovery 요청이지만 질문의 성격이 다르다.

```text
Probe   : 조건에 맞는 장비를 찾아달라는 검색 요청
Resolve : 이미 알고 있는 EndpointReference의 접속 주소를 알려달라는 조회 요청
```

`Probe`는 "네트워크에 ONVIF NetworkVideoTransmitter 장비가 있으면 응답해라"에 가깝다. client는 장비의 고유 ID를 모르는 상태에서 `Types`, `Scopes` 같은 검색 조건을 보낸다. responder 또는 실제 ONVIF 장비는 조건이 맞으면 `ProbeMatches`로 응답한다.

```text
client
  -> Probe(Types = dn:NetworkVideoTransmitter)
device
  -> ProbeMatches(EndpointReference, Types, Scopes, XAddrs, MetadataVersion)
```

이 실습에서는 `client.cpp`가 아래 API로 Probe를 보낸다.

```cpp
soap_wsdd_Probe(..., kOnvifDeviceType, nullptr, nullptr);
```

그리고 `responder.cpp`는 `wsdd_event_Probe()` callback에서 type을 확인한 뒤 `soap_wsdd_add_ProbeMatch()`로 응답 내용을 채운다.

`Resolve`는 "이 EndpointReference를 가진 장비의 실제 접속 주소가 뭐냐"에 가깝다. client는 이미 `EndpointReference`를 알고 있어야 한다. 장비는 그 ID가 자기 것과 일치하면 `ResolveMatches`로 `XAddrs`를 돌려준다.

```text
client
  -> Resolve(EndpointReference = urn:uuid:...)
device
  -> ResolveMatches(EndpointReference, Types, Scopes, XAddrs, MetadataVersion)
```

이 실습에서는 `discover --resolve <EndpointReference>`를 주면 `client.cpp`가 아래 API를 호출한다.

```cpp
soap_wsdd_Resolve(..., options.resolve_endpoint_reference);
```

그리고 `responder.cpp`는 `wsdd_event_Resolve()` callback에서 요청의 `EndpointReference`가 `kEndpointReference`와 같은지 비교한 뒤, 맞으면 `wsdd__ResolveMatchType` 구조체를 직접 채운다.

정리하면 보통 흐름은 아래와 같다.

```text
1. Probe로 조건에 맞는 장비 목록을 찾는다.
2. ProbeMatches에서 EndpointReference와 XAddrs를 얻는다.
3. 필요하면 EndpointReference로 Resolve를 보내 최신 XAddrs를 다시 확인한다.
4. 최종적으로 XAddrs를 사용해서 ONVIF Device Service 요청을 보낸다.
```

이 실습만 놓고 보면 `ProbeMatches`에도 이미 `XAddrs`가 들어 있으므로 `Resolve`가 꼭 필요해 보이지 않을 수 있다. 그래도 `Resolve`는 WS-Discovery의 별도 메시지 흐름이고, EndpointReference 기반 조회가 어떻게 동작하는지 확인하기 위해 따로 구현했다.

## Environment

현재 host에는 `soapcpp2`, `wsdl2h`가 없고, `05_protocol/docker-compose.yml`의 `server` 컨테이너 안에 gSOAP 도구가 있다.

```bash
cd 05_protocol
docker compose up -d
docker compose exec server bash
cd /workspace/002_onvif_discover
```

확인된 주요 경로:

```text
/usr/bin/soapcpp2
/usr/share/gsoap/import/wsdd.h
/usr/share/gsoap/plugin/wsddapi.h
/usr/share/gsoap/plugin/wsddapi.c
/usr/share/gsoap/plugin/wsaapi.c
/usr/share/gsoap/plugin/threads.c
```

## Files

직접 작성하는 파일:

```text
onvif_discover.h
client.cpp
responder.cpp
monitor.cpp
```

`soapcpp2`가 생성하는 파일:

```text
soapH.h
soapStub.h
soapC.cpp
soapClient.cpp
soapClientLib.cpp
wsdd.nsmap
wsdd.*.req.xml
```

## onvif_discover.h

`soapcpp2` 입력 파일이다.

```cpp
#import "wsdd.h"
```

`wsdd.h`는 gSOAP가 제공하는 WS-Discovery schema header이며 `/usr/share/gsoap/import/wsdd.h`에 있다.

## discover

Source: `client.cpp`

Role:

```text
WS-Discovery client
  -> Probe 전송
  -> ProbeMatches 수신
  -> Resolve 전송
  -> ResolveMatches 수신
```

Important callbacks:

```text
wsdd_event_ProbeMatches   : ProbeMatches 응답 처리
wsdd_event_ResolveMatches : ResolveMatches 응답 처리
wsdd_event_Hello          : no-op
wsdd_event_Bye            : no-op
wsdd_event_Probe          : no-op
wsdd_event_Resolve        : no-op
```

Runtime flow:

```text
soap_new1(SOAP_IO_UDP)
  -> soap_bind(..., port 0)
  -> soap_wsdd_Probe(...) or soap_wsdd_Resolve(...)
  -> soap_wsdd_listen(...)
  -> wsdd_event_ProbeMatches(...) or wsdd_event_ResolveMatches(...)
```

Options:

```text
--timeout seconds
--endpoint soap.udp://host:port
--resolve endpoint-reference
```

Logs:

```text
sent_probe.xml
recv_probe.xml
```

## responder

Source: `responder.cpp`

Role:

```text
WS-Discovery Target Service simulator
  -> Probe 수신
  -> ProbeMatches 응답
  -> Resolve 수신
  -> ResolveMatches 응답
  -> Hello 전송
  -> Bye 전송
```

Important callbacks:

```text
wsdd_event_Probe   : Probe 요청을 받고 soap_wsdd_add_ProbeMatch()로 응답 구성
wsdd_event_Resolve : Resolve 요청을 받고 ResolveMatch 구조체 채움
```

Runtime flow:

```text
soap_new1(SOAP_IO_UDP)
  -> soap_bind(..., port 3702)
  -> IP_ADD_MEMBERSHIP 239.255.255.250
  -> optional soap_wsdd_Hello(...)
  -> soap_wsdd_listen(...)
  -> wsdd_event_Probe(...) or wsdd_event_Resolve(...)
  -> optional soap_wsdd_Bye(...)
```

Options:

```text
--timeout seconds
--announce soap.udp://host:port
```

Advertised values:

```text
EndpointReference: urn:uuid:11111111-2222-3333-4444-555555555555
Types: dn:NetworkVideoTransmitter
Scopes: onvif://www.onvif.org/Profile/Streaming onvif://www.onvif.org/name/StudyDevice
XAddrs: http://127.0.0.1/onvif/device_service
MetadataVersion: 1
```

Logs:

```text
responder_sent.xml
responder_recv.xml
```

## monitor

Source: `monitor.cpp`

Role:

```text
WS-Discovery passive event monitor
  -> Hello 수신
  -> Bye 수신
```

Important callbacks:

```text
wsdd_event_Hello : Target Service가 네트워크에 등장했음을 처리
wsdd_event_Bye   : Target Service가 네트워크에서 사라짐을 처리
```

Runtime flow:

```text
soap_new1(SOAP_IO_UDP)
  -> soap_bind(..., monitor port)
  -> optional IP_ADD_MEMBERSHIP
  -> soap_wsdd_listen(...)
  -> wsdd_event_Hello(...) / wsdd_event_Bye(...)
```

Options:

```text
--port port
--timeout seconds
--join-multicast
```

Logs:

```text
monitor_recv.xml
```

## How To Write This Code

gSOAP `wsddapi` 기반 코드는 직접 XML 문자열을 만드는 방식이 아니다. 작성 순서는 항상 아래처럼 잡는다.

```text
1. onvif_discover.h에서 wsdd.h import
2. soapcpp2로 WS-Discovery 타입과 stub 생성
3. 애플리케이션 코드에서 soap_new1(SOAP_IO_UDP)로 UDP SOAP context 생성
4. soap_set_namespaces(soap, namespaces)로 generated namespace map 연결
5. soap_wsdd_* API로 메시지 전송
6. wsdd_event_* callback을 구현해서 수신 메시지 처리
7. soap_destroy / soap_end / soap_free로 정리
```

### 1. gSOAP 입력 파일

직접 작성하는 gSOAP 입력은 아주 작다.

```cpp
#import "wsdd.h"
```

`wsdd.h` 안에 WS-Discovery 메시지 타입과 gSOAP용 선언이 들어 있다. 이 파일을 import한 뒤 `soapcpp2`를 실행하면 `soapH.h`, `soapC.cpp`, `soapClient.cpp`, `wsdd.nsmap` 같은 generated file이 생긴다.

### 2. 공통 include

`client.cpp`, `responder.cpp`, `monitor.cpp`는 공통으로 아래 파일을 포함한다.

```cpp
#include "soapH.h"
#include "wsdd.nsmap"
#include "plugin/wsddapi.h"
```

- `soapH.h`: `soapcpp2`가 만든 WS-Discovery 타입 선언
- `wsdd.nsmap`: namespace prefix mapping
- `wsddapi.h`: `soap_wsdd_Probe`, `soap_wsdd_Resolve`, `soap_wsdd_listen`, callback signature 선언

### 3. client.cpp 작성 방식

client는 요청을 보내고 응답 callback에서 결과를 출력한다.

```text
main()
  -> Options 파싱
  -> soap_new1(SOAP_IO_UDP)
  -> soap_bind(..., port 0)
  -> soap_wsa_rand_uuid()
  -> soap_wsdd_Probe() 또는 soap_wsdd_Resolve()
  -> soap_wsdd_listen()
  -> wsdd_event_ProbeMatches() 또는 wsdd_event_ResolveMatches()
```

`--resolve`가 없으면 Probe를 보낸다.

```cpp
soap_wsdd_Probe(
    soap,
    SOAP_WSDD_ADHOC,
    SOAP_WSDD_TO_TS,
    options.endpoint,
    message_id,
    nullptr,
    kOnvifDeviceType,
    nullptr,
    nullptr);
```

`--resolve <EndpointReference>`가 있으면 Resolve를 보낸다.

```cpp
soap_wsdd_Resolve(
    soap,
    SOAP_WSDD_ADHOC,
    SOAP_WSDD_TO_TS,
    options.endpoint,
    message_id,
    nullptr,
    options.resolve_endpoint_reference);
```

응답은 직접 `recvfrom()`으로 받지 않는다. `soap_wsdd_listen()`이 UDP 메시지를 받고, 메시지 종류에 맞는 callback을 호출한다.

```cpp
void wsdd_event_ProbeMatches(..., wsdd__ProbeMatchesType* matches)
{
    for (int i = 0; matches != nullptr && i < matches->__sizeProbeMatch; ++i) {
        const wsdd__ProbeMatchType& match = matches->ProbeMatch[i];
        print_text("EndpointReference", match.wsa5__EndpointReference.Address);
        print_text("XAddrs", match.XAddrs);
    }
}
```

Resolve 응답은 단일 `wsdd__ResolveMatchType`으로 들어온다.

```cpp
void wsdd_event_ResolveMatches(..., wsdd__ResolveMatchType* match)
{
    print_text("EndpointReference", match->wsa5__EndpointReference.Address);
    print_text("XAddrs", match->XAddrs);
}
```

`wsddapi` plugin은 모든 `wsdd_event_*` 함수가 정의되어 있기를 기대한다. 그래서 client에서 사용하지 않는 `Hello`, `Bye`, `Probe`, `Resolve` callback도 빈 함수로 둔다.

### 4. responder.cpp 작성 방식

responder는 Target Service처럼 동작한다. UDP 3702에 bind하고 multicast group에 join한 뒤 요청 callback에서 응답 내용을 채운다.

```text
main()
  -> Options 파싱
  -> soap_new1(SOAP_IO_UDP)
  -> soap_bind(..., 3702)
  -> IP_ADD_MEMBERSHIP 239.255.255.250
  -> 필요하면 soap_wsdd_Hello()
  -> soap_wsdd_listen()
  -> wsdd_event_Probe() 또는 wsdd_event_Resolve()
  -> 필요하면 soap_wsdd_Bye()
```

Probe 요청은 `wsdd_event_Probe()`에서 처리한다. 응답은 `soap_wsdd_add_ProbeMatch()`로 `matches`에 추가한다.

```cpp
soap_wsdd_add_ProbeMatch(
    soap,
    matches,
    kEndpointReference,
    kTypes,
    kScopes,
    nullptr,
    kXAddrs,
    kMetadataVersion);

return SOAP_WSDD_MANAGED;
```

여기서 `SOAP_WSDD_MANAGED`를 반환해야 `wsddapi.c`가 채워진 `matches`를 이용해서 `ProbeMatches` 응답을 전송한다.

Resolve 요청은 `wsdd_event_Resolve()`에서 처리한다. 요청으로 들어온 `EndpointReference`가 responder의 `kEndpointReference`와 같을 때만 응답을 만든다.

```cpp
if (EndpointReference == nullptr || std::strcmp(EndpointReference, kEndpointReference) != 0) {
    return SOAP_WSDD_ADHOC;
}

match->wsa5__EndpointReference.Address = const_cast<char*>(kEndpointReference);
match->Types = const_cast<char*>(kTypes);
match->Scopes = scopes;
match->XAddrs = const_cast<char*>(kXAddrs);
match->MetadataVersion = kMetadataVersion;

return SOAP_WSDD_MANAGED;
```

`ResolveMatch`의 `Scopes`는 구조체 포인터라서 `soap_malloc()`으로 gSOAP context에 붙여서 할당한다. 이렇게 해야 `soap_end()` 정리 흐름에 같이 묶인다.

```cpp
auto* scopes = static_cast<wsdd__ScopesType*>(soap_malloc(soap, sizeof(wsdd__ScopesType)));
soap_default_wsdd__ScopesType(soap, scopes);
scopes->__item = const_cast<char*>(kScopes);
scopes->MatchBy = nullptr;
```

Hello / Bye는 요청 callback이 아니라 responder가 능동적으로 보내는 announcement다.

```cpp
soap_wsdd_Hello(
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
    kMetadataVersion);
```

```cpp
soap_wsdd_Bye(
    announcer,
    SOAP_WSDD_ADHOC,
    endpoint,
    soap_wsa_rand_uuid(announcer),
    kEndpointReference,
    kTypes,
    kScopes,
    nullptr,
    kXAddrs,
    kMetadataVersion);
```

### 5. monitor.cpp 작성 방식

monitor는 요청을 보내지 않고 UDP port를 열어 이벤트만 듣는다.

```text
main()
  -> Options 파싱
  -> soap_new1(SOAP_IO_UDP)
  -> soap_bind(..., options.port)
  -> 필요하면 IP_ADD_MEMBERSHIP
  -> soap_wsdd_listen()
  -> wsdd_event_Hello() / wsdd_event_Bye()
```

핵심 callback은 두 개다.

```cpp
void wsdd_event_Hello(..., const char* EndpointReference, const char* XAddrs, ...)
{
    print_text("EndpointReference", EndpointReference);
    print_text("XAddrs", XAddrs);
}
```

```cpp
void wsdd_event_Bye(..., const char* EndpointReference, const char* XAddrs, ...)
{
    print_text("EndpointReference", EndpointReference);
    print_text("XAddrs", XAddrs);
}
```

로컬 실습에서는 responder가 `--announce soap.udp://127.0.0.1:3703`으로 monitor에게 직접 Hello / Bye를 보내게 했다. 실제 multicast announcement를 들으려면 monitor를 3702에 bind하고 `--join-multicast`를 사용한다.

### 6. 값 바꿔서 새 장비처럼 만들기

가짜 장비 정보를 바꾸려면 `responder.cpp`의 상수만 먼저 바꾼다.

```cpp
constexpr const char* kEndpointReference = "urn:uuid:...";
constexpr const char* kTypes = "dn:NetworkVideoTransmitter";
constexpr const char* kScopes = "onvif://www.onvif.org/Profile/Streaming ...";
constexpr const char* kXAddrs = "http://127.0.0.1/onvif/device_service";
constexpr unsigned int kMetadataVersion = 1;
```

실제 ONVIF 장비 탐색에서 다음 단계로 넘길 값은 `XAddrs`다. 다음 실습에서는 이 주소로 Device Service SOAP 요청을 보낸다.

## Step 1. Generate gSOAP Code

```bash
soapcpp2 -2 -C -I/usr/share/gsoap/import -I/usr/share/gsoap onvif_discover.h
```

옵션 의미:

- `-2`: SOAP 1.2 코드 생성
- `-C`: client-side 코드 생성
- `-I`: gSOAP import path

## Step 2. Build With g++

Build `discover`:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O2 \
  -I. -I/usr/share/gsoap -I/usr/share/gsoap/import -I/usr/share/gsoap/plugin \
  -o discover \
  client.cpp soapC.cpp soapClient.cpp \
  /usr/share/gsoap/plugin/wsddapi.c \
  /usr/share/gsoap/plugin/wsaapi.c \
  /usr/share/gsoap/plugin/threads.c \
  -lgsoap++
```

Build `responder`:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O2 \
  -I. -I/usr/share/gsoap -I/usr/share/gsoap/import -I/usr/share/gsoap/plugin \
  -o responder \
  responder.cpp soapC.cpp soapClient.cpp \
  /usr/share/gsoap/plugin/wsddapi.c \
  /usr/share/gsoap/plugin/wsaapi.c \
  /usr/share/gsoap/plugin/threads.c \
  -lgsoap++
```

Build `monitor`:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O2 \
  -I. -I/usr/share/gsoap -I/usr/share/gsoap/import -I/usr/share/gsoap/plugin \
  -o monitor \
  monitor.cpp soapC.cpp soapClient.cpp \
  /usr/share/gsoap/plugin/wsddapi.c \
  /usr/share/gsoap/plugin/wsaapi.c \
  /usr/share/gsoap/plugin/threads.c \
  -lgsoap++
```

## Step 3. Probe / ProbeMatches

Terminal 1:

```bash
./responder --timeout 20
```

Terminal 2:

```bash
./discover --endpoint soap.udp://127.0.0.1:3702 --timeout 3
```

Expected client output:

```text
Found ONVIF device #1
EndpointReference: urn:uuid:11111111-2222-3333-4444-555555555555
XAddrs: http://127.0.0.1/onvif/device_service
```

## Step 4. Resolve / ResolveMatches

Terminal 1:

```bash
./responder --timeout 20
```

Terminal 2:

```bash
./discover \
  --endpoint soap.udp://127.0.0.1:3702 \
  --resolve urn:uuid:11111111-2222-3333-4444-555555555555 \
  --timeout 3
```

Expected client output:

```text
Resolved ONVIF device
EndpointReference: urn:uuid:11111111-2222-3333-4444-555555555555
XAddrs: http://127.0.0.1/onvif/device_service
```

## Step 5. Hello / Bye

Terminal 1:

```bash
./monitor --port 3703 --timeout 10
```

Terminal 2:

```bash
./responder --timeout 3 --announce soap.udp://127.0.0.1:3703
```

Expected monitor output:

```text
Received Hello
EndpointReference: urn:uuid:11111111-2222-3333-4444-555555555555
XAddrs: http://127.0.0.1/onvif/device_service

Received Bye
EndpointReference: urn:uuid:11111111-2222-3333-4444-555555555555
XAddrs: http://127.0.0.1/onvif/device_service
```

## Real Device Discovery

실제 ONVIF 장비를 찾을 때는 기본 multicast endpoint를 사용한다.

```bash
./discover --timeout 5
```

기본 endpoint:

```text
soap.udp://239.255.255.250:3702
```

컨테이너 환경에서는 multicast loopback이 항상 기대대로 동작하지 않을 수 있다. 그래서 로컬 실습은 `127.0.0.1` unicast로 검증하고, 실제 장비 탐색은 host network에서 실행하는 편이 좋다.

## Where To Read The gSOAP Implementation

```text
/usr/share/gsoap/import/wsdd.h
/usr/share/gsoap/plugin/wsddapi.h
/usr/share/gsoap/plugin/wsddapi.c
/usr/share/gsoap/plugin/wsaapi.h
/usr/share/gsoap/plugin/wsaapi.c
```

읽을 위치:

- `wsdd.h`: `wsdd__ProbeMatchesType`, `wsdd__ResolveMatchType`, `wsdd__ScopesType`
- `wsddapi.h`: `soap_wsdd_*` API와 `wsdd_event_*` callback signature
- `wsddapi.c`: Probe, Resolve, Hello, Bye 송수신 구현
- `wsaapi.c`: `soap_wsa_rand_uuid()`와 WS-Addressing helper

## Troubleshooting

### `soapcpp2` command not found

host에 gSOAP 도구가 없으면 컨테이너 안에서 실행한다.

```bash
cd 05_protocol
docker compose up -d
docker compose exec server bash
cd /workspace/002_onvif_discover
```

### `wsdd.nsmap` not found

generated file을 만들지 않은 상태에서 빌드한 것이다. 먼저 `soapcpp2`를 실행한다.

```bash
soapcpp2 -2 -C -I/usr/share/gsoap/import -I/usr/share/gsoap onvif_discover.h
```

### `undefined reference to soap_wsdd_*`

빌드 명령에 gSOAP plugin 구현 파일이 빠진 것이다. 아래 파일을 `g++` 입력에 같이 넣어야 한다.

```text
/usr/share/gsoap/plugin/wsddapi.c
/usr/share/gsoap/plugin/wsaapi.c
/usr/share/gsoap/plugin/threads.c
```

### responder가 Probe를 못 받는 경우

로컬 실습에서는 multicast 대신 unicast로 먼저 확인한다.

```bash
./responder --timeout 20
./discover --endpoint soap.udp://127.0.0.1:3702 --timeout 3
```

실제 ONVIF 장비 탐색이 안 되면 같은 L2 network인지, 장비의 ONVIF 기능이 켜져 있는지, UDP 3702 방화벽이 열려 있는지 확인한다. Docker bridge network에서는 multicast가 막히거나 loopback이 기대와 다르게 동작할 수 있으므로 실제 장비 탐색은 host network에서 실행하는 편이 낫다.

## Clean

```bash
rm -f discover responder monitor
rm -f sent_probe.xml recv_probe.xml
rm -f responder_sent.xml responder_recv.xml monitor_recv.xml
rm -f soapH.h soapStub.h soapC.cpp soapClient.cpp soapClientLib.cpp wsdd.nsmap
rm -f wsdd.*.req.xml wsdd.*.res.xml soap.nsmap soap*.xml
```
