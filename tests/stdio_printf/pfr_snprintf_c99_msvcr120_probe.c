/* TC
 * id:    pfr_snprintf_c99_msvcr120_probe
 * area:  stdio_printf
 * funcs:
 * procs: snprintf vsnprintf
 * crt:   msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.6.5; MS docs snprintf (VS2013 added C99 snprintf)
 * note:  PROBE: msvcr120.dll is believed to export a C99-conforming snprintf/vsnprintf (VS2013). Probes export, truncation return value (full length), NUL termination, and snprintf(NULL,0,..) sizing.
 */
#include "tc.h"
#include <stdarg.h>

typedef int (__cdecl *snp_t)(char *, size_t, const char *, ...);
typedef int (__cdecl *vsnp_t)(char *, size_t, const char *, va_list);
static snp_t p_snprintf;
static vsnp_t p_vsnprintf;

static int callv(char *b, size_t c, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = p_vsnprintf(b, c, f, ap); va_end(ap); return r; }

TC_CASE(exported) { CHECK(p_snprintf != NULL); CHECK(p_vsnprintf != NULL); }
TC_CASE(trunc)
{
    char b[16]; volatile size_t c = 4; int i, n;
    if (!p_snprintf) TC_SKIP("not exported");
    for (i = 0; i < 16; i++) b[i] = 'Z';
    n = p_snprintf(b, c, "%s", "abcdefgh");
    CHECK_EQ_I(n, 8); CHECK_STR(b, "abc"); CHECK_EQ_I(b[4], 'Z');
}
TC_CASE(size_query)
{
    char *volatile nb = NULL;
    if (!p_snprintf) TC_SKIP("not exported");
    CHECK_EQ_I(p_snprintf(nb, 0, "hello %d", 42), 8);
}
TC_CASE(vtrunc)
{
    char b[16]; volatile size_t c = 4; int i, n;
    if (!p_vsnprintf) TC_SKIP("not exported");
    for (i = 0; i < 16; i++) b[i] = 'Z';
    n = callv(b, c, "%s", "abcdefgh");
    CHECK_EQ_I(n, 8); CHECK_STR(b, "abc"); CHECK_EQ_I(b[4], 'Z');
}

int main(void)
{
    tc_begin();
    p_snprintf = (snp_t)tc_crt_proc("snprintf");
    p_vsnprintf = (vsnp_t)tc_crt_proc("vsnprintf");
    TC_RUN(exported); TC_RUN(trunc); TC_RUN(size_query); TC_RUN(vtrunc);
    return tc_end();
}
