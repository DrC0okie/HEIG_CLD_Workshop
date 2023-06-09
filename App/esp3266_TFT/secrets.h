#include <pgmspace.h>

#define SECRET
#define THINGNAME "ESP8266_1"

const char WIFI_SSID[] = "OnePlus 8 Pro";
const char WIFI_PASSWORD[] = "w4f33nen";
const char AWS_IOT_ENDPOINT[] = "a3ibupythmyonz-ats.iot.us-east-1.amazonaws.com";

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

// Device Certificate
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUcemYENVWWZFaFjPk7MDykoAEUD8wDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIzMDYwNzIwMjc0
NloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBANgTLWgJ1I4vfma7mAiz
a4J/kjoYByPq+KwpVqJuRqoLkEqcY/wajlFhObtrVqq4vxXJBoVln+PaZp5J/REg
UyksOR2ZwdlPZexFJnTPXBU3T6Mms6GDjPprTTKaLV+cTi3GOW3bUkNkeCY0s3K8
By+fjwCB6/pr9/j+4ifTp30dgzpDMHzQLprXyz2InXvwZOebfN70LMJMSe88BRk5
ZF9bTnqOMufUjhAdFEbYmRutYUPxVizLZsGxxnrROcneBmV8B9gdI76wQFrwovEv
3x9Ff+xqxSgdOFl0t+Dh7dwyAusvmcKJt0Qf28VCZCQf3pl4wPW2UDDNcnvHKy8S
2N8CAwEAAaNgMF4wHwYDVR0jBBgwFoAU7B6tWWLTTfMUfDwDAPy6vpxzXB8wHQYD
VR0OBBYEFEMkGCn8IDa/34s7udLM5APrHGcaMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAfomJnUl2PzV6XFxt/8YDF8lxQ
6zPqA0ggp4HXZ1nCgSVBRYLM0fUL+o0W8E48sktH6Vbd+eM5V7W8lwapPRDSTrrr
twwnQdI57ngeAtWr3kSVu8LkDZwoGJIuHVxL3mXmOIlVCeVtf4/9oxSfQ2A/7Axf
L04/ZEC1pJi4KXArq4iteV3ufgYYhd9muKcBsp4ErMSRQ9RMKcbz1CA1vsJqsIkI
/wemkxcWXUHCmnoUcT0rmcej4wnGf5SUrLaFHl3yRihQMEIYj/NVmdS+YUiUE2Qr
rquCoYcoqSDOnttagQ8344Nn/6Eyq25ZNphiHAC6kd7xZ/e/65iywxUnH8jw
-----END CERTIFICATE-----

)KEY";

// Device Private Key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEogIBAAKCAQEA2BMtaAnUji9+ZruYCLNrgn+SOhgHI+r4rClWom5GqguQSpxj
/BqOUWE5u2tWqri/FckGhWWf49pmnkn9ESBTKSw5HZnB2U9l7EUmdM9cFTdPoyaz
oYOM+mtNMpotX5xOLcY5bdtSQ2R4JjSzcrwHL5+PAIHr+mv3+P7iJ9OnfR2DOkMw
fNAumtfLPYide/Bk55t83vQswkxJ7zwFGTlkX1tOeo4y59SOEB0URtiZG61hQ/FW
LMtmwbHGetE5yd4GZXwH2B0jvrBAWvCi8S/fH0V/7GrFKB04WXS34OHt3DIC6y+Z
wom3RB/bxUJkJB/emXjA9bZQMM1ye8crLxLY3wIDAQABAoIBAFK+nU/PG+fLRkN/
KUyDsPMLeWrn0qwtikcbmYKgak0qGnqIAz4CyWderSYZh9ne38Yu8mFZNm6iL1QE
j3XkhBQrTEAKppJadnSRUpW+5st1H/khk/6RCwbG2ow7QI7xo3qiA1IPgCQwsFYp
LmMMoE/bcX7HQh/nAtIGgYKw6d3eH/rDHSgkPSHY4rPjdJjkCOdl53/Ekdr+dUbX
Y98ESxlizHNZwjeaetFJlLLklUzZKlNXsU4UioUxHyVJ/zv/eMWylykFdK/K3gdI
2g8W7QEmWDELp6sQ5OELW9AMARtyKss2PtyQRP1Ivo31qLI6LdX99fr9lG9Y7eGj
Hag/wIkCgYEA+Pv65tYhvJUSVIoIQA5Ft3rcZHQBlb/uDcTpX3a9jhB5IM5Wysat
XS/24w6cf3sz5D9eiSkxY2AXzVEELARhgYjfgutmCyGQoyM0z928YaCAjOaxX07d
JWixdiqYg6+AJqo1hkLU20zlIpCfLMzp7r3qZXeDE9zzw0v/v42DRgUCgYEA3inP
JfeolmqxsylQj5H0D7YuZ0fTrR42SZNpNplVEGxX5zTpACHr1JMHqZxEdmUx9hJA
hIMEQ89SI3M2RAxFJ0gHQgYos4PM5pZ595LdYT0/mxxzyweLVVC0maK0LlWV4M89
40iNLIiBq0ZdHLf+Wqy6X1wDR3T6Qxt9iShYVJMCgYAEv7mfYYIvOKCq4B8cKamG
wF8nw1asZobB2HmjCn6/EO1ehND++R8hYeLRndu6Sr1IozTIZPK+NHz/u4Xa2RKL
B/NLafX+TFuX4PoG5PHM4uTQyy2i5bwuFHseB9301QoasrBmqSuqKyNLr3p5vkbP
+OtZmBf17wbvOKneG3LfiQKBgHAUeaw26HeSNjoj4r+7q3xttbVnODRDZLHzy4W/
D3osnqBHQjFN67MfxiLccnGklfMFvETd8D3hjxfRXRQxeIXbwcWeY9RE7w955Ohw
0HzpMHipfwVzteiBwcssmWgyx11WUGukw4wvBfQJmJoPfSPztka7ne/j5QoMCWYz
03Q/AoGAIwHaNW0qXFK28EVXP1ugHFfS2VALn+BdKFwLQWUW2kcCw60TLe6oCoXv
cbvv5uibS4forMPL28Cim2oZxb44fTzuUh1jt3m25oZBdT9BjEXKbF0seb0OPQpi
2/RfwAkZTX5HeXUDnRVgr4/JwjrY9BMMNqvRPa33D+kkdorx4pw=
-----END RSA PRIVATE KEY-----
)KEY";