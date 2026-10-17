#ifndef NOCTRA_ACCOUNT_ID_H
#define NOCTRA_ACCOUNT_ID_H
#include <stddef.h>
#include <stdint.h>
/* Returns 1 on success. Input: exactly 16 hex digits, optionally prefixed 0x.
   Output: eight little-endian account bytes encoded as 12 Base64 characters. */
static int noctra_account_id(const char *hex, char out[13]) {
    static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    uint8_t bytes[8]; size_t n = 0;
    if (!hex || !out) return 0;
    if (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) hex += 2;
    while (hex[n]) n++;
    if (n != 16) return 0;
    for (size_t i = 0; i < 8; i++) {
        unsigned value = 0;
        for (size_t j = 0; j < 2; j++) {
            char c = hex[2*i+j]; unsigned digit;
            if (c >= '0' && c <= '9') digit = (unsigned)(c-'0');
            else if (c >= 'a' && c <= 'f') digit = (unsigned)(c-'a'+10);
            else if (c >= 'A' && c <= 'F') digit = (unsigned)(c-'A'+10);
            else return 0;
            value = value*16 + digit;
        }
        bytes[7-i] = (uint8_t)value;
    }
    size_t pos = 0;
    for (size_t i = 0; i < 8; i += 3) {
        unsigned remain = (unsigned)(8-i);
        uint32_t v = (uint32_t)bytes[i] << 16;
        if (remain > 1) v |= (uint32_t)bytes[i+1] << 8;
        if (remain > 2) v |= bytes[i+2];
        out[pos++] = alphabet[(v>>18)&63]; out[pos++] = alphabet[(v>>12)&63];
        out[pos++] = remain > 1 ? alphabet[(v>>6)&63] : '=';
        out[pos++] = remain > 2 ? alphabet[v&63] : '=';
    }
    out[12] = '\0'; return 1;
}
#endif
