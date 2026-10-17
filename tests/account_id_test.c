#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>
#include <string.h>
#include "../src/account_id.h"
int main(void) {
    char out[13];
    assert(noctra_account_id("0000000000000000",out)); assert(!strcmp(out,"AAAAAAAAAAA="));
    assert(noctra_account_id("0x0123456789ABCDEF",out)); assert(!strcmp(out,"782riWdFIwE="));
    assert(noctra_account_id("ffffffffffffffff",out)); assert(!strcmp(out,"//////////8="));
    assert(!noctra_account_id("username",out)); assert(!noctra_account_id("000000000000000g",out));
    assert(!noctra_account_id("00000000000000000",out)); assert(!noctra_account_id("",out));
    assert(!noctra_account_id(NULL,out));
    return 0;
}
