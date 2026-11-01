/* TC
 * id:    pfs_neg_probe_ls_failure_buffer_and_iph
 * area:  stdio_printf
 * funcs: sprintf _scprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   UCRT: conversion failure -> errno EILSEQ, return -1
 * note:  OBSERVED for case scprintf_fails (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "scprintf_fails n == -1 got=1"; zzp_probe2 "_scprintf x%lc euro: r=1 errno=42 iph=0", "_scprintf x%ls
 *        euro: r=-1 errno=42"): _scprintf("x%lc", 0x20ac) in the C locale sets EILSEQ but returns 1 (the count of what was formatted before the
 *        failing %lc), while %ls with the same character returns -1/EILSEQ. Cases sprintf_no_handler and sprintf_s_clears (unconvertible %ls: -1,
 *        EILSEQ, no handler, buf[0]=0) were right and pass. Stable (C locale).
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
    CHECK_EQ_I(n, 1); CHECK_EQ_I(errno, EILSEQ);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(sprintf_no_handler); TC_RUN(sprintf_s_clears); TC_RUN(scprintf_fails);
    return tc_end();
}
