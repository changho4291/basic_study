# ONVIF Study TODO Roadmap

SOAP basic 실습 이후 ONVIF를 단계적으로 익히기 위한 로드맵이다.

## Study Rule

각 실습 폴더는 `README.md`만 보고 다시 따라 할 수 있는 수준으로 정리한다.

- [ ] 각 단계마다 목적, 배경, 파일 구성, 직접 작성할 코드, 빌드 명령, 실행 명령을 README에 포함한다.
- [ ] generated file이 있다면 어떤 명령으로 생성되는지 README에 명시한다.
- [ ] `make`/CMake 같은 자동화보다 먼저 수동 흐름을 설명한다.
- [ ] 실습 완료 시 README 절차를 깨끗한 상태에서 재검증한다.
- [ ] README에 현재 환경 기준의 주의점과 troubleshooting을 남긴다.

## 001_Soap_basic

Status: Done

- [x] gSOAP 기반 SOAP server/client 구성
- [x] 간단한 `Add(a, b)` RPC 요청/응답 구현
- [x] `sent.xml`, `recv.xml` 로그로 SOAP Envelope 확인
- [x] WSDL, namespace map, generated code 흐름 확인
- [ ] README만 보고 재현 가능하도록 문서 보강

## 002_onvif_discover

Status: In Progress

Goal: WS-Discovery로 네트워크의 ONVIF 장비를 찾는다.

- [x] gSOAP WS-Discovery schema header `wsdd.h` import 방식 확인
- [x] `soapcpp2`로 WS-Discovery generated code 생성
- [x] `soapH.h`, `soapC.cpp`, `soapClient.cpp`, `wsdd.nsmap` 생성 흐름 확인
- [x] `soap_wsdd_Probe()`로 `dn:NetworkVideoTransmitter` Probe 전송 구현
- [x] `soap_wsdd_listen()`으로 ProbeMatches 대기 구현
- [x] `wsdd_event_ProbeMatches()` callback에서 `EndpointReference`, `Types`, `Scopes`, `XAddrs` 출력 구현
- [x] 학습용 WS-Discovery responder 구현
- [x] responder의 `wsdd_event_Probe()`에서 `soap_wsdd_add_ProbeMatch()`로 ProbeMatches 생성
- [x] local unicast endpoint `soap.udp://127.0.0.1:3702`로 `discover -> responder -> ProbeMatches` 왕복 검증
- [x] `Resolve / ResolveMatches` 추가 실습 구현
- [x] responder의 `wsdd_event_Resolve()`에서 ResolveMatches 응답 생성
- [x] `discover --resolve <EndpointReference>`로 Resolve 왕복 검증
- [x] `Hello / Bye` 개념 확인용 monitor 구현
- [x] responder 시작/종료 시 `soap_wsdd_Hello()` / `soap_wsdd_Bye()` 전송 구현
- [x] `monitor -> responder --announce`로 Hello/Bye 수신 검증
- [x] `wsddapi.h`, `wsddapi.c`, `wsaapi.c`, generated files의 참고 위치 README에 정리
- [x] `soapcpp2 + g++` 수동 빌드 흐름 README에 정리
- [x] README 코드와 실제 `client.cpp` 동기화
- [x] ONVIF 장비가 없는 환경에서 timeout 종료 확인
- [ ] 실제 ONVIF 장비가 있는 네트워크에서 `ProbeMatches` 수신 확인
- [ ] README 절차를 소스 없는 깨끗한 상태에서 재검증
- [ ] 발견한 장비의 `XAddrs`를 다음 실습 입력값으로 기록

Expected output:

```text
Found ONVIF device
EndpointReference: urn:uuid:...
Types: dn:NetworkVideoTransmitter
XAddrs: http://192.168.0.25/onvif/device_service
Scopes: onvif://www.onvif.org/Profile/Streaming ...
```

Notes:

- 이 단계는 raw XML 하드코딩이 아니라 gSOAP `wsddapi` plugin 기반으로 진행한다.
- `soapcpp2` generated code와 gSOAP plugin이 어떤 식으로 결합되는지 확인한다.
- 실제 장비가 없어도 responder를 함께 실행해서 Probe/ProbeMatches 왕복을 실습한다.
- Resolve/ResolveMatches와 Hello/Bye도 로컬 unicast 기반으로 재현한다.
- 컨테이너 multicast loopback은 환경에 따라 동작하지 않을 수 있으므로 local round trip은 unicast endpoint로 검증한다.
- 이 단계에서 얻은 `XAddrs`가 다음 Device Service 호출의 endpoint가 된다.

## 003_onvif_device_info

Status: In Progress

Goal: 발견한 ONVIF 장비의 Device Service에 SOAP 요청을 보낸다.

- [x] README만 보고 재현 가능하도록 실습 문서 작성
- [x] `XAddrs`를 endpoint로 사용
- [x] Device Management WSDL/header 준비 방식 결정
- [x] 최소 `onvif_device.h`에서 `GetDeviceInformation` contract 작성
- [x] `soapcpp2` generated code 생성 흐름 정리
- [x] `GetDeviceInformation` 요청 구현
- [x] 학습용 fake ONVIF Device Service 구현
- [x] 제조사, 모델명, 펌웨어 버전, serial number 출력
- [x] SOAP Fault 발생 시 fault code/message 출력
- [ ] 실제 ONVIF 장비에서 `GetDeviceInformationResponse` 수신 확인
- [ ] 인증이 필요한 장비의 실패 응답을 다음 단계 입력으로 기록

Expected output:

```text
Manufacturer: ...
Model: ...
FirmwareVersion: ...
SerialNumber: ...
HardwareId: ...
```

## 004_onvif_auth_wsse

Status: In Progress

Goal: ONVIF UsernameToken Digest 인증을 이해하고 구현한다.

- [x] README만 보고 재현 가능하도록 실습 문서 작성
- [x] WS-Security UsernameToken 구조 확인
- [x] `Nonce`, `Created`, `PasswordDigest` 생성 방식 정리
- [x] `PasswordDigest = Base64(SHA1(Nonce + Created + Password))` 구현
- [x] 인증이 필요한 Device Service 요청에 Security Header 추가
- [x] 학습용 fake Device Service에서 UsernameToken Digest 검증 구현
- [x] 인증 성공/실패 케이스 비교
- [ ] 실제 ONVIF 장비에서 UsernameToken Digest 인증 확인
- [ ] 장비 시간 오차로 인한 인증 실패 여부 확인

Notes:

- ONVIF 장비는 plain password보다 digest 인증을 요구하는 경우가 많다.
- 장비 시간과 클라이언트 시간이 크게 다르면 인증이 실패할 수 있다.

## 005_onvif_media_profiles

Status: Pending

Goal: Media Service에서 사용 가능한 profile 목록을 가져온다.

- [ ] README만 보고 재현 가능하도록 실습 문서 작성
- [ ] Device Service에서 `GetCapabilities` 호출
- [ ] Media Service endpoint 확인
- [ ] `GetProfiles` 요청 전송
- [ ] profile token, video source, encoder 설정 출력
- [ ] profile token을 다음 단계 입력값으로 저장

Expected output:

```text
Profile token: profile_1
Name: MainStream
VideoEncoder: H264
Resolution: 1920x1080
FrameRateLimit: 30
```

## 006_onvif_rtsp_uri

Status: Pending

Goal: ONVIF Media Service를 통해 RTSP stream URI를 얻는다.

- [ ] README만 보고 재현 가능하도록 실습 문서 작성
- [ ] `GetStreamUri` 요청 구조 확인
- [ ] `StreamSetup` 설정 작성
- [ ] profile token으로 RTSP URI 요청
- [ ] 반환된 URI 출력
- [ ] VLC, ffplay, OpenCV 중 하나로 재생 확인

Expected output:

```text
RTSP URI: rtsp://192.168.0.25/...
```

## 007_onvif_ptz_optional

Status: Optional / Pending

Goal: PTZ를 지원하는 장비에서 pan/tilt/zoom 제어를 실습한다.

- [ ] README만 보고 재현 가능하도록 실습 문서 작성
- [ ] `GetCapabilities`에서 PTZ 지원 여부 확인
- [ ] PTZ Service endpoint 확인
- [ ] `GetConfigurations` 호출
- [ ] `ContinuousMove` 요청 전송
- [ ] `Stop` 요청 전송
- [ ] timeout, speed 값에 따른 움직임 비교

## 008_onvif_event_optional

Status: Optional / Pending

Goal: ONVIF Event Service의 기본 구조를 확인한다.

- [ ] README만 보고 재현 가능하도록 실습 문서 작성
- [ ] Event Service endpoint 확인
- [ ] PullPoint subscription 개념 정리
- [ ] `CreatePullPointSubscription` 호출
- [ ] `PullMessages`로 이벤트 수신
- [ ] motion detection 같은 이벤트 topic 출력

## Final Goal

- [ ] 각 실습 단계가 README만 보고 재현 가능
- [ ] 네트워크에서 ONVIF 장비 자동 발견
- [ ] 인증 처리
- [ ] 장비 정보 조회
- [ ] media profile 조회
- [ ] RTSP URI 획득
- [ ] 영상 재생 또는 후속 VMS 연동 준비

Final flow:

```text
WS-Discovery
  -> XAddrs
  -> GetDeviceInformation
  -> GetCapabilities
  -> GetProfiles
  -> GetStreamUri
  -> RTSP playback
```
