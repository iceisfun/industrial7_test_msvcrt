/* TC
 * id:    ucr_neg_stdio_vfprintf_readonly_stream
 * area:  ucrt_only
 * funcs: fopen fclose fread remove fputs
 * procs: __stdio_common_vfprintf
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fprintf (returns a negative value on output error)
 * note:  temp file tc_ucr_neg_stdio_vfprintf_readonly_stream_*.tmp
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

typedef int (__cdecl *vf_t)(unsigned long long, FILE *, const char *, void *, va_list);
static vf_t p_vf;
#define TMP "tc_ucr_neg_stdio_vfprintf_readonly_stream_a.tmp"
static int callf(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vf(0, f, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(write_to_readonly)
{
    FILE *f; char b[32]; size_t got; int r;
    if (!p_vf) TC_SKIP("not exported");
    f = fopen(TMP, "wb"); CHECK(f != NULL); if (!f) return;
    fputs("orig", f); fclose(f);
    f = fopen(TMP, "rb"); CHECK(f != NULL); if (!f) { remove(TMP); return; }
    r = callf(f, "NEWDATA%d", 1);
    CHECK(r < 0);
    fclose(f);
    f = fopen(TMP, "rb"); CHECK(f != NULL);
    if (f) { got = fread(b, 1, sizeof b, f); fclose(f); CHECK_EQ_U(got, 4); b[got] = 0; CHECK_STR(b, "orig"); }
    remove(TMP);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vf = (vf_t)tc_crt_proc("__stdio_common_vfprintf");
    TC_RUN(write_to_readonly);
    return tc_end();
}
