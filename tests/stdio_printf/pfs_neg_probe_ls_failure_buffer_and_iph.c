/* TC
 * id:    pfs_neg_probe_ls_failure_buffer_and_iph
 * area:  stdio_printf
 * funcs: sprintf _scprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   UCRT: conversion failure -> errno EILSEQ, return -1
 * note:  PROBE: (1) an unconvertible %ls (U+20AC in the C locale) does NOT call the invalid parameter handler; (2) sprintf_s clears buffer[0] on that failure; (3) _scprintf fails with EILSEQ too. All three are from memory of the UCRT implementation.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const wchar_t *volatile ws_euro = L"\x20ac";
static const char *volatile f_ab_ls = "ab%ls";
static const char *volatile f_ls = "%ls";
static const char *volatile f_x_lc = "x%lc";
static volatile wchar_t euro = 0x20ac;

TC_CASE(sprintf_no_handler)
{
    char b[16]; int n; errno = 0; tc_iph_count = 0;
    n = sprintf(b, f_ab_ls, ws_euro);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EILSEQ); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(sprintf_s_clears)
{
    char b[16]; int n, i; for (i = 0; i < 16; i++) b[i] = 'Z';
    errno = 0;
    n = sprintf_s(b, 16, f_ls, ws_euro);
    CHECK_EQ_I(n, -1); CHECK(b[0] == 0);
}
TC_CASE(scprintf_fails)
{
    int n; errno = 0;
    n = _scprintf(f_x_lc, (wint_t)euro);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EILSEQ);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(sprintf_no_handler); TC_RUN(sprintf_s_clears); TC_RUN(scprintf_fails);
    return tc_end();
}
