# 003_onvif_device_info

ONVIF WS-Discovery에서 얻은 `XAddrs`를 Device Service endpoint로 사용해서 `GetDeviceInformation` SOAP 요청을 보낸다.

이번 실습은 두 역할로 구성한다.

```text
device_info : GetDeviceInformation을 호출하는 ONVIF Device Service client
fake_device : GetDeviceInformation에 응답하는 로컬 fake ONVIF Device Service
```

## Goal

`002_onvif_discover`의 결과는 `XAddrs`다.

```text
XAddrs: http://127.0.0.1/onvif/device_service
```

이번 단계에서는 이 HTTP endpoint로 ONVIF Device Management `GetDeviceInformation` 요청을 보내고, 장비 기본 정보를 출력한다.

```text
Manufacturer
Model
FirmwareVersion
SerialNumber
HardwareId
```

## Device Service Flow

```text
002_onvif_discover
  -> Probe / ProbeMatches
  -> XAddrs 획득

003_onvif_device_info
  -> XAddrs를 --endpoint로 입력
  -> GetDeviceInformation 요청
  -> GetDeviceInformationResponse 수신
  -> 제조사 / 모델명 / 펌웨어 / 시리얼 / 하드웨어 ID 출력
```

WS-Discovery는 UDP multicast 기반 discovery이고, Device Service는 HTTP SOAP 요청이다. 그래서 `002`에서는 `soap.udp://...` endpoint를 사용했지만, 이번 단계에서는 `http://.../onvif/device_service` endpoint를 사용한다.

## WSDL / Header Decision

정식 ONVIF Device Management WSDL은 `devicemgmt.wsdl`이고, `GetDeviceInformation`의 SOAP action은 아래 값이다.

```text
http://www.onvif.org/ver10/device/wsdl/GetDeviceInformation
```

이 컨테이너에는 ONVIF Device Management WSDL 파일이 기본으로 들어 있지 않다. 그래서 이번 실습은 외부 WSDL 다운로드에 의존하지 않고, `GetDeviceInformation` 하나만 포함하는 최소 gSOAP header를 직접 작성한다.

직접 작성하는 파일:

```text
onvif_device.h
client.cpp
fake_device.cpp
README.md
```

`soapcpp2`가 생성하는 파일:

```text
soapH.h
soapStub.h
soapC.cpp
soapClient.cpp
soapClientLib.cpp
soapServer.cpp
soapServerLib.cpp
Device.nsmap
Device.wsdl
tds.xsd
Device.GetDeviceInformation.req.xml
Device.GetDeviceInformation.res.xml
```

## onvif_device.h

`soapcpp2` 입력 파일이다. ONVIF Device Management namespace는 `tds`로 사용한다.

```cpp
//gsoap tds service name: Device
//gsoap tds service style: document
//gsoap tds service encoding: literal
//gsoap tds service namespace: http://www.onvif.org/ver10/device/wsdl
//gsoap tds service location: http://127.0.0.1:8081/onvif/device_service
//gsoap tds schema namespace: http://www.onvif.org/ver10/device/wsdl
//gsoap tds service method-action: GetDeviceInformation http://www.onvif.org/ver10/device/wsdl/GetDeviceInformation
```

요청은 비어 있고, 응답에는 다섯 개 문자열 필드가 있다.

```cpp
struct _tds__GetDeviceInformation {
};

struct _tds__GetDeviceInformationResponse {
    char* Manufacturer;
    char* Model;
    char* FirmwareVersion;
    char* SerialNumber;
    char* HardwareId;
};
```

operation 선언은 아래처럼 둔다.

```cpp
int tds__GetDeviceInformation(
    struct _tds__GetDeviceInformation* request,
    struct _tds__GetDeviceInformationResponse& response
);
```

`soapcpp2`는 이 선언을 보고 client 호출 함수와 server dispatch 함수를 생성한다.

```text
soap_call_tds__GetDeviceInformation(...)
soap_serve(...)
tds__GetDeviceInformation(...)
```

## device_info

Source: `client.cpp`

Role:

```text
ONVIF Device Service client
  -> HTTP SOAP GetDeviceInformation 전송
  -> GetDeviceInformationResponse 수신
  -> Manufacturer / Model / FirmwareVersion / SerialNumber / HardwareId 출력
```

Runtime flow:

```text
soap_new1(SOAP_XML_INDENT)
  -> soap_set_namespaces(soap, namespaces)
  -> soap_call_tds__GetDeviceInformation(...)
  -> response field 출력
  -> soap_destroy / soap_end / soap_free
```

Options:

```text
--endpoint http://host/onvif/device_service
--timeout seconds
```

Logs:

```text
device_info_sent.xml
device_info_recv.xml
```

핵심 호출은 아래 한 줄이다.

```cpp
soap_call_tds__GetDeviceInformation(
    soap,
    options.endpoint,
    "http://www.onvif.org/ver10/device/wsdl/GetDeviceInformation",
    &request,
    response);
```

실패하면 `soap_print_fault(soap, stderr)`로 SOAP Fault 또는 네트워크 오류를 출력한다.

## fake_device

Source: `fake_device.cpp`

Role:

```text
Fake ONVIF Device Service
  -> HTTP port listen
  -> GetDeviceInformation 요청 수신
  -> 고정된 장비 정보 응답
  -> --fault 옵션이면 SOAP Fault 응답
```

Runtime flow:

```text
soap_new1(SOAP_XML_INDENT)
  -> soap_bind(..., port)
  -> soap_accept(...)
  -> soap_serve(...)
  -> tds__GetDeviceInformation(...)
```

Options:

```text
--port port
--timeout seconds
--once
--fault
```

Logs:

```text
fake_device_sent.xml
fake_device_recv.xml
```

server가 실제로 구현해야 하는 함수는 generated code가 호출하는 아래 callback이다.

```cpp
int tds__GetDeviceInformation(
    struct soap* soap,
    _tds__GetDeviceInformation* request,
    _tds__GetDeviceInformationResponse& response)
```

정상 응답은 `response` 구조체에 값을 채우고 `SOAP_OK`를 반환한다.

```cpp
response.Manufacturer = const_cast<char*>("StudyCamera");
response.Model = const_cast<char*>("ONVIF-DeviceInfo-Lab");
response.FirmwareVersion = const_cast<char*>("1.0.0-study");
response.SerialNumber = const_cast<char*>("STUDY-DEVICE-0001");
response.HardwareId = const_cast<char*>("BASIC-STUDY-HW");

return SOAP_OK;
```

Fault 응답은 `soap_sender_fault()`를 반환한다.

```cpp
return soap_sender_fault(
    soap,
    "Simulated ONVIF Device Service fault",
    "fake_device was started with --fault");
```

## Step 1. Generate gSOAP Code

컨테이너 안에서 실행한다.

```bash
cd 05_protocol
docker compose up -d
docker compose exec server bash
cd /workspace/003_onvif_device_info
```

client와 fake server를 모두 빌드해야 하므로 `-C` 없이 실행한다.

```bash
soapcpp2 -2 -I/usr/share/gsoap/import -I/usr/share/gsoap onvif_device.h
```

옵션 의미:

- `-2`: SOAP 1.2 코드 생성
- `-I`: gSOAP import path

## Step 2. Build With g++

Build `device_info`:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O2 \
  -I. -I/usr/share/gsoap -I/usr/share/gsoap/import \
  -o device_info \
  client.cpp soapC.cpp soapClient.cpp \
  -lgsoap++
```

Build `fake_device`:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O2 \
  -I. -I/usr/share/gsoap -I/usr/share/gsoap/import \
  -o fake_device \
  fake_device.cpp soapC.cpp soapServer.cpp \
  -lgsoap++
```

## Step 3. Local Round Trip

Terminal 1:

```bash
./fake_device --port 8081 --timeout 20
```

Terminal 2:

```bash
./device_info --endpoint http://127.0.0.1:8081/onvif/device_service --timeout 3
```

Expected client output:

```text
Device Information
Manufacturer: StudyCamera
Model: ONVIF-DeviceInfo-Lab
FirmwareVersion: 1.0.0-study
SerialNumber: STUDY-DEVICE-0001
HardwareId: BASIC-STUDY-HW
```

한 번만 요청을 받고 종료하려면:

```bash
./fake_device --port 8081 --once
```

## Step 4. SOAP Fault Check

Terminal 1:

```bash
./fake_device --port 8081 --once --fault
```

Terminal 2:

```bash
./device_info --endpoint http://127.0.0.1:8081/onvif/device_service --timeout 3
```

Expected client behavior:

```text
SOAP 1.2 fault SOAP-ENV:Sender
"Simulated ONVIF Device Service fault"
```

정확한 출력 형식은 gSOAP `soap_print_fault()`가 만든다.

## Step 5. Real Device

`002_onvif_discover`에서 얻은 실제 장비의 `XAddrs`를 그대로 넣는다.

```bash
./device_info --endpoint http://192.168.0.25/onvif/device_service --timeout 5
```

인증이 꺼져 있거나 anonymous 접근을 허용하는 장비라면 장비 정보가 출력된다. 인증이 필요한 장비는 SOAP Fault 또는 HTTP 401/403으로 실패할 수 있다. 그 경우는 다음 단계 `004_onvif_auth_wsse`에서 WS-Security UsernameToken Digest를 붙인다.

## Where To Read

```text
onvif_device.h
soapStub.h
soapClient.cpp
soapServer.cpp
Device.GetDeviceInformation.req.xml
Device.GetDeviceInformation.res.xml
device_info_sent.xml
device_info_recv.xml
```

읽을 위치:

- `onvif_device.h`: 직접 작성한 최소 Device Management contract
- `soapStub.h`: generated struct와 `soap_call_tds__GetDeviceInformation()` 선언
- `soapClient.cpp`: client가 SOAP request를 만드는 흐름
- `soapServer.cpp`: server가 request를 dispatch해서 `tds__GetDeviceInformation()`을 호출하는 흐름
- `*.req.xml`, `*.res.xml`: gSOAP가 만든 sample SOAP body
- `device_info_sent.xml`, `device_info_recv.xml`: 실제 송수신 SOAP envelope

## Troubleshooting

### `soapcpp2` command not found

host에 gSOAP 도구가 없으면 컨테이너 안에서 실행한다.

```bash
cd 05_protocol
docker compose up -d
docker compose exec server bash
cd /workspace/003_onvif_device_info
```

### `Device.nsmap` not found

generated file을 만들지 않은 상태에서 빌드한 것이다.

```bash
soapcpp2 -2 -I/usr/share/gsoap/import -I/usr/share/gsoap onvif_device.h
```

### `undefined reference to soap_serve`

`fake_device` 빌드에 `soapServer.cpp`가 빠진 것이다.

```text
fake_device.cpp soapC.cpp soapServer.cpp
```

### `undefined reference to soap_call_tds__GetDeviceInformation`

`device_info` 빌드에 `soapClient.cpp`가 빠진 것이다.

```text
client.cpp soapC.cpp soapClient.cpp
```

### 실제 장비에서 SOAP Fault가 나는 경우

장비가 인증을 요구할 수 있다. 이번 단계는 인증 없이 Device Service 호출 구조를 확인하는 단계다. 인증이 필요한 장비는 다음 단계 `004_onvif_auth_wsse`에서 처리한다.

## Clean

```bash
rm -f device_info fake_device
rm -f device_info_sent.xml device_info_recv.xml
rm -f fake_device_sent.xml fake_device_recv.xml
rm -f soapH.h soapStub.h soapC.cpp soapClient.cpp soapClientLib.cpp
rm -f soapServer.cpp soapServerLib.cpp Device.nsmap Device.wsdl tds.xsd
rm -f Device.*.req.xml Device.*.res.xml soap.nsmap soap*.xml
```
