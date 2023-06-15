#include <pgmspace.h>

#define SECRET
#define THINGNAME "YOUR_THING_NAME"

const char WIFI_SSID[] = "YOUR_SSID";
const char WIFI_PASSWORD[] = "YOUR_PWD";
const char AWS_IOT_ENDPOINT[] = "YOUR_AWS_ENDPOINT";

// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)EOF";

// Device Certificate
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
-----END CERTIFICATE-----
)KEY";

// Device Private Key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
-----END RSA PRIVATE KEY-----
)KEY";
