#include "common.h"

// Extracted by: openssl x509 -pubkey -noout -in servercert.pem

#ifdef DEBUG

// Test environment certificate
const char *tlspubkeystr PROGMEM = R"KEY(
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

// Production environment certificate
const char *tlspubkeystr PROGMEM = R"KEY(
-----BEGIN PUBLIC KEY-----
MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAzvTYTllp1qaWmPZpW00F
Es3vBRQkZxIWWJ8vaayD4HEH1Y1SNe1u912EVU9iLfs/6eH4o+xyH0IPYk3gR9cf
6dKmGxDU2h7Lv/UnCwO/z37e/kouGWKmIFyvO6LedUlaGeSEb7ZHRuZozD+NqXfm
8ZS9d0MTxPCA7AK7luWeRsFvqTNBMTvPgATl2meTWoAZmjNE9OEu0Zvj6T07aWw0
+KDD3iXvB3mOrReae4ooQ9GD78vtQ7jlA3YSJXDMv2LTeUPWXC/6nF+GUkMKARjv
bsiHQSqIoV+eTzwY3IxsVhxs8OWsJgtLEksBfA6LjseaF69rlp6jM/pKdMCwEpAN
0wIDAQAB
-----END PUBLIC KEY-----
)KEY";

#endif

BearSSL::PublicKey tlspubkey(tlspubkeystr);
