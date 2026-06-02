# 004_onvif_auth_wsse

ONVIF Device Service 요청에 WS-Security `UsernameToken Digest` 인증 헤더를 붙인다.

이번 실습은 두 역할로 구성한다.

```text
auth_client      : UsernameToken Digest를 붙여 GetDeviceInformation 호출
auth_fake_device : UsernameToken Digest를 검증하는 fake ONVIF Device Service
```

## Goal

`003_onvif_device_info`에서는 인증 없이 `GetDeviceInformation`을 호출했다.

이번 단계에서는 같은 Device Service 요청에 `wsse:Security` header를 추가한다.

```text
SOAP Body
  -> tds:GetDeviceInformation

SOAP Header
  -> wsse:Security
     -> wsse:UsernameToken
        -> wsse:Username
        -> wsse:Password Type=PasswordDigest
        -> wsse:Nonce
        -> wsu:Created
```

ONVIF 장비는 plain password보다 `PasswordDigest`를 요구하는 경우가 많다.

## Password Digest

UsernameToken Digest의 핵심 계산식은 아래와 같다.

```text
PasswordDigest = Base64(SHA1(NonceBytes + Created + Password))
```

각 값의 의미:

```text
NonceBytes : client가 매 요청마다 새로 만드는 random bytes
Created    : 요청 생성 시각, UTC ISO-8601 문자열
Password   : 사용자가 입력한 실제 password
```

중요한 점은 password 원문을 SOAP 메시지에 넣지 않는다는 것이다. SOAP에는 digest 결과만 들어간다.

```xml
<wsse:Password Type="...#PasswordDigest">...</wsse:Password>
<wsse:Nonce EncodingType="...#Base64Binary">...</wsse:Nonce>
<wsu:Created>2026-05-29T08:14:10Z</wsu:Created>
```

server는 수신한 `Nonce`, `Created`, 서버가 알고 있는 password로 같은 digest를 다시 계산한다. 계산 결과가 SOAP의 `PasswordDigest`와 같으면 인증 성공이다.

## Why Manual Digest Here

gSOAP에는 `wsseapi.c` plugin이 있고, 일반적으로는 아래 API를 쓴다.

```cpp
soap_wsse_add_UsernameTokenDigest(...)
soap_wsse_verify_Password(...)
```

하지만 현재 컨테이너의 gSOAP runtime 구성에서는 전체 `wsseapi.c`가 `WITH_OPENSSL`, `WITH_DOM`, 추가 runtime 소스와 맞물려 빌드 부담이 크다.

이번 실습의 목적은 `UsernameToken Digest` 구조와 계산식을 이해하는 것이다. 그래서 `wsse2.h`를 import해 `wsse:Security` 구조체만 생성하고, digest 계산은 `wsse_username_token.h`에서 직접 구현한다.

```text
onvif_auth.h
  -> #import "wsse2.h"
  -> SOAP_ENV__Header 안에 wsse__Security 생성

wsse_username_token.h
  -> Nonce 생성
  -> Created 생성
  -> Base64(SHA1(Nonce + Created + Password)) 계산
  -> UsernameToken 구조체 채움
  -> server에서 같은 계산으로 검증
```

## Files

직접 작성하는 파일:

```text
onvif_auth.h
wsse_username_token.h
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

## onvif_auth.h

`003`의 `GetDeviceInformation` contract에 WS-Security header 생성을 위한 import를 추가한다.

```cpp
#import "wsse2.h"
```

그 아래 Device Service operation은 `003`과 같다.

```cpp
int tds__GetDeviceInformation(
    struct _tds__GetDeviceInformation* request,
    struct _tds__GetDeviceInformationResponse& response
);
```

`soapcpp2`는 `wsse2.h`를 보고 `SOAP_ENV__Header`에 `wsse__Security` 필드를 생성한다.

```cpp
struct SOAP_ENV__Header {
    struct _wsse__Security *wsse__Security;
};
```

## wsse_username_token.h

이 파일이 이번 단계의 핵심이다.

client 쪽 흐름:

```text
RAND_bytes()
  -> nonce bytes 생성
  -> nonce를 Base64 문자열로 변환
  -> Created UTC 문자열 생성
  -> SHA1(nonce bytes + Created + password)
  -> hash를 Base64 문자열로 변환
  -> soap->header->wsse__Security->UsernameToken 채움
```

핵심 함수:

```cpp
bool add_username_token_digest(
    soap* soap,
    const char* username,
    const char* password);
```

server 쪽 흐름:

```text
SOAP Header에서 UsernameToken 읽기
  -> Username 확인
  -> Nonce base64 decode
  -> Created 읽기
  -> 서버가 알고 있는 password로 digest 재계산
  -> SOAP의 PasswordDigest와 비교
```

핵심 함수:

```cpp
bool verify_username_token_digest(
    soap* soap,
    const char* expected_username,
    const char* expected_password,
    std::string& reason);
```

## auth_client

Source: `client.cpp`

Role:

```text
Authenticated ONVIF Device Service client
  -> UsernameToken Digest 생성
  -> GetDeviceInformation 호출
  -> 인증 성공 시 장비 정보 출력
  -> 인증 실패 시 SOAP Fault 출력
```

Options:

```text
--endpoint http://host/onvif/device_service
--username name
--password value
--timeout seconds
--no-auth
```

핵심 호출:

```cpp
wsse_auth::add_username_token_digest(soap, options.username, options.password);

soap_call_tds__GetDeviceInformation(
    soap,
    options.endpoint,
    "http://www.onvif.org/ver10/device/wsdl/GetDeviceInformation",
    &request,
    response);
```

## auth_fake_device

Source: `fake_device.cpp`

Role:

```text
Fake authenticated ONVIF Device Service
  -> GetDeviceInformation 요청 수신
  -> UsernameToken Digest 검증
  -> 성공하면 장비 정보 응답
  -> 실패하면 SOAP Fault 응답
```

Options:

```text
--port port
--timeout seconds
--username name
--password value
--once
```

핵심 검증:

```cpp
wsse_auth::verify_username_token_digest(
    soap,
    g_options.username,
    g_options.password,
    reason);
```

## Step 1. Generate gSOAP Code

컨테이너 안에서 실행한다.

```bash
cd 05_protocol
docker compose up -d
docker compose exec server bash
cd /workspace/004_onvif_auth_wsse
```

```bash
soapcpp2 -2 -I/usr/share/gsoap/import -I/usr/share/gsoap onvif_auth.h
```

## Step 2. Build With g++

Build `auth_client`:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O2 \
  -I. -I/usr/share/gsoap -I/usr/share/gsoap/import \
  -o auth_client \
  client.cpp soapC.cpp soapClient.cpp \
  -lgsoap++ -lcrypto
```

Build `auth_fake_device`:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -O2 \
  -I. -I/usr/share/gsoap -I/usr/share/gsoap/import \
  -o auth_fake_device \
  fake_device.cpp soapC.cpp soapServer.cpp \
  -lgsoap++ -lcrypto
```

`-lcrypto`는 `RAND_bytes`, `EVP_EncodeBlock`, `EVP_DecodeBlock`, `EVP_Digest*` 사용 때문에 필요하다.

## Step 3. Success Case

Terminal 1:

```bash
./auth_fake_device --port 8082 --once
```

Terminal 2:

```bash
./auth_client \
  --endpoint http://127.0.0.1:8082/onvif/device_service \
  --username admin \
  --password password \
  --timeout 3
```

Expected client output:

```text
Device Information
Manufacturer: StudyCamera
Model: ONVIF-WSSE-Lab
FirmwareVersion: 1.0.0-auth-study
SerialNumber: STUDY-AUTH-0001
HardwareId: BASIC-STUDY-WSSE-HW
```

Expected fake device output:

```text
Received GetDeviceInformation
UsernameToken username: admin
Authentication OK
Send GetDeviceInformationResponse
```

## Step 4. Wrong Password

Terminal 1:

```bash
./auth_fake_device --port 8082 --once
```

Terminal 2:

```bash
./auth_client \
  --endpoint http://127.0.0.1:8082/onvif/device_service \
  --username admin \
  --password wrong \
  --timeout 3
```

Expected client output:

```text
SOAP 1.2 fault SOAP-ENV:Sender
"WSSE authentication failed"
Detail: password digest mismatch
```

## Step 5. Missing UsernameToken

Terminal 1:

```bash
./auth_fake_device --port 8082 --once
```

Terminal 2:

```bash
./auth_client \
  --endpoint http://127.0.0.1:8082/onvif/device_service \
  --no-auth \
  --timeout 3
```

Expected client output:

```text
SOAP 1.2 fault SOAP-ENV:Sender
"WSSE authentication failed"
Detail: missing UsernameToken
```

## Step 6. Real Device

`002_onvif_discover`에서 얻은 실제 `XAddrs`를 사용한다.

```bash
./auth_client \
  --endpoint http://192.168.0.25/onvif/device_service \
  --username admin \
  --password 'your-password' \
  --timeout 5
```

실제 장비는 시간 오차에 민감할 수 있다. `Created` 시간이 장비 시간과 크게 다르면 digest가 맞아도 거부될 수 있다.

## Troubleshooting

### `Device.nsmap` not found

`soapcpp2`를 먼저 실행한다.

```bash
soapcpp2 -2 -I/usr/share/gsoap/import -I/usr/share/gsoap onvif_auth.h
```

### `undefined reference to EVP_*` 또는 `RAND_bytes`

OpenSSL crypto library가 빠진 것이다.

```text
-lcrypto
```

### 인증 실패: `password digest mismatch`

client와 server의 password가 다르거나, digest 계산에 들어간 `Nonce`, `Created`, `Password` 중 하나가 바뀐 것이다.

### 실제 장비에서 인증 실패

다음을 확인한다.

- username/password가 맞는가
- 장비 시간이 client 시간과 크게 다르지 않은가
- 장비가 HTTP가 아니라 HTTPS endpoint를 요구하지 않는가
- 장비가 ONVIF 계정과 웹 UI 계정을 따로 관리하지 않는가

## Clean

```bash
rm -f auth_client auth_fake_device
rm -f auth_sent.xml auth_recv.xml auth_fake_sent.xml auth_fake_recv.xml
rm -f soapH.h soapStub.h soapC.cpp soapClient.cpp soapClientLib.cpp
rm -f soapServer.cpp soapServerLib.cpp Device.nsmap Device.wsdl tds.xsd
rm -f Device.*.req.xml Device.*.res.xml soap.nsmap soap*.xml
```
