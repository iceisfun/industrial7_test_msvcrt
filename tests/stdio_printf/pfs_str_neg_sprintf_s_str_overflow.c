/* TC
 * id:    pfs_str_neg_sprintf_s_str_overflow
 * area:  stdio_printf
 * funcs: strlen
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s: buffer too small -> invalid parameter handler, returns -1, errno ERANGE, buffer[0]=0
 * note:  kind negative
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>
#include <string.h>

TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
/* ucrtbase has no sprintf_s export (inline over __stdio_common_vsprintf_s); older CRTs export it but mingw libs may lack it */
#ifdef _UCRT
#define CALL_SPRINTF_S sprintf_s
#else
#define CALL_SPRINTF_S tc_p_sprintf_s
#endif

static const char *volatile s6 = "abcdef";
static const wchar_t *volatile w6 = L"abcdef";

TC_CASE(narrow) {
    char b[4]; int n; b[0] = 'Z'; tc_iph_count = 0; errno = 0;
    n = CALL_SPRINTF_S(b, 4, "%s", s6);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(b[0], 0);
}
TC_CASE(wide_arg) {
    char b[4]; int n; b[0] = 'Z'; tc_iph_count = 0; errno = 0;
    n = CALL_SPRINTF_S(b, 4, "%ls", w6);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(b[0], 0);
}
TC_CASE(fits) {
    char b[7]; int n; tc_iph_count = 0;
    n = CALL_SPRINTF_S(b, 7, "%s", s6);
    CHECK_EQ_I(n, 6); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(b, "abcdef"); CHECK_EQ_U(strlen(b), 6);
}

int main(void) {
    tc_begin(); TC_LOAD_PROC(sprintf_s);
#ifndef _UCRT
    if (!tc_p_sprintf_s) { tc_write("TC FAIL no sprintf_s\n"); return 1; }
#endif
    if (!tc_iph_install()) { tc_write("no iph\n"); return 1; }
    TC_RUN(narrow); TC_RUN(wide_arg); TC_RUN(fits); return tc_end();
}
