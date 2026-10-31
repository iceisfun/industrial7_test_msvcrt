/* TC
 * id:    ucr_neg_stdio_vfwprintf_readonly_and_null
 * area:  ucrt_only
 * funcs: fopen fclose fread remove fputs
 * procs: __stdio_common_vfwprintf
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fwprintf (NULL stream/format: invalid parameter handler, EINVAL; negative return on output error)
 * note:  temp file tc_ucr_neg_stdio_vfwprintf_readonly_and_null_*.tmp. errno for the read-only case is in the _probe sibling.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

typedef int (__cdecl *vfw_t)(unsigned long long, FILE *, const wchar_t *, void *, va_list);
static vfw_t p_vfw;
#define TMP "tc_ucr_neg_stdio_vfwprintf_readonly_and_null_a.tmp"
static int callfw(FILE *f, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vfw(0, f, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(null_file)
{
    int r;
    if (!p_vfw) TC_SKIP("not exported");
    errno = 0; tc_iph_count = 0;
    r = callfw(NULL, L"x%d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    FILE *f; int r;
    if (!p_vfw) TC_SKIP("not exported");
    f = fopen(TMP, "wb"); CHECK(f != NULL); if (!f) return;
    errno = 0; tc_iph_count = 0;
    r = callfw(f, NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); remove(TMP);
}
TC_CASE(readonly_stream)
{
    FILE *f; char b[32]; size_t got; int r;
    if (!p_vfw) TC_SKIP("not exported");
    f = fopen(TMP, "wb"); CHECK(f != NULL); if (!f) return;
    fputs("orig", f); fclose(f);
    f = fopen(TMP, "rb"); CHECK(f != NULL); if (!f) { remove(TMP); return; }
    r = callfw(f, L"NEW%d", 1);
    CHECK(r < 0);
    fclose(f);
    f = fopen(TMP, "rb"); CHECK(f != NULL);
    if (f) { got = fread(b, 1, sizeof b, f); fclose(f); CHECK_EQ_U(got, 4); b[got] = 0; CHECK_STR(b, "orig"); }
    remove(TMP);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vfw = (vfw_t)tc_crt_proc("__stdio_common_vfwprintf");
    TC_RUN(null_file); TC_RUN(null_format); TC_RUN(readonly_stream);
    return tc_end();
}
