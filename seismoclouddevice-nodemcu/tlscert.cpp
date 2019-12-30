#include "common.h"

#ifdef DEBUG

// Extracted by: openssl x509 -pubkey -noout -in servercert.pem
const char *tlspubkey PROGMEM = R"KEY(
-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAv2ow787UrOfJw45MYA7L
z8nSE5rvR5WeUK08agWLtzjCWEdCFqc6aoWMNvl/Q98ru75r+Odl0OL2cem7tdeM
0lnr1hb+pDeba/uB1CjDX0ZxF3bpcWY3KlZdVsVR2FR5WaDgwciZY0trIxw3vG9V
hDN2URlfWw64Otn9Nu0PZ6Ch+VaSXfPXtZ3jXHKk32vdY1h+JsGbBTcxDZP+LIq2
3mPXoPNBRcOn2x1FdN3C7tTkwoRi3Lp0aPhAL9t32o7Hmli/FiCZD5eWO/u6pvYe
eaF3Xvbp/GSulnJ4yHIG3mlqqhgOu1COXQvLqBAkzCm3CKr6ew+g+kknOxiFytjc
kwIDAQAB
-----END PUBLIC KEY-----
)KEY";

#else

// TODO

#endif
