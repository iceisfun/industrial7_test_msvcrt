/* TC
 * id:    pfx_neg_exit_underscore_does_not_flush
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen exit _exit
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _exit ("terminates without ... flushing stdio buffers" vs exit which flushes); C11 7.22.4.4
 * note:  Children terminate the process while a small fprintf is still in the stdio buffer. The _exit child must leave a
 *        zero-length file; the exit child (control) must have flushed 4 bytes.
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>

#define FN "tc_pfx_neg_exit_underscore_does_not_flush.tmp"

static long long fsize(const char *n)
{
    WIN32_FILE_ATTRIBUTE_DATA d;
    if (!GetFileAttributesExA(n, GetFileExInfoStandard, &d)) return -1;
    return ((long long)d.nFileSizeHigh << 32) | d.nFileSizeLow;
}

TC_CASE(uexit_discards)
{
    DWORD code = 0; int ran;
    DeleteFileA(FN);
    ran = tc_spawn_self("uexit", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 5);
    CHECK_EQ_I(fsize(FN), 0);
    DeleteFileA(FN);
}
TC_CASE(exit_flushes)
{
    DWORD code = 0; int ran;
    DeleteFileA(FN);
    ran = tc_spawn_self("exit", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 6);
    CHECK_EQ_I(fsize(FN), 4);
    DeleteFileA(FN);
}
TC_CASE(cleanup)
{
    DeleteFileA(FN);
    CHECK_EQ_U(GetFileAttributesA(FN), INVALID_FILE_ATTRIBUTES);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        FILE *f = fopen(FN, "w");
        if (!f) return 70;
        if (fprintf(f, "data") != 4) return 71;
        if (c[0] == 'u') _exit(5);
        exit(6);
    }
    TC_RUN(uexit_discards); TC_RUN(exit_flushes); TC_RUN(cleanup);
    return tc_end();
}
