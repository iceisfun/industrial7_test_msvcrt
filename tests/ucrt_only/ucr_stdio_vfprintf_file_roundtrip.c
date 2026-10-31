/* TC
 * id:    ucr_stdio_vfprintf_file_roundtrip
 * area:  ucrt_only
 * funcs: fopen fclose fread remove
 * procs: __stdio_common_vfprintf __stdio_common_vfwprintf
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (text mode LF->CRLF, binary no translation), fprintf/fwprintf return values
 * note:  temp file tc_ucr_stdio_vfprintf_file_roundtrip_*.tmp
 *        wide_ascii uses text mode (wide chars converted to multibyte); binary-mode wide output is not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

typedef int (__cdecl *vf_t)(unsigned long long, FILE *, const char *, void *, va_list);
typedef int (__cdecl *vfw_t)(unsigned long long, FILE *, const wchar_t *, void *, va_list);
static vf_t p_vf; static vfw_t p_vfw;
#define TMP "tc_ucr_stdio_vfprintf_file_roundtrip_a.tmp"

static int callf(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vf(0, f, fmt, NULL, ap); va_end(ap); return r; }
static int callfw(FILE *f, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vfw(0, f, fmt, NULL, ap); va_end(ap); return r; }

static size_t slurp(char *buf, size_t n)
{
    FILE *f = fopen(TMP, "rb"); size_t got = 0;
    if (f) { got = fread(buf, 1, n, f); fclose(f); }
    return got;
}

TC_CASE(narrow)
{
    FILE *f; char b[64]; size_t got; int r;
    if (!p_vf) TC_SKIP("not exported");
    f = fopen(TMP, "wb"); CHECK(f != NULL); if (!f) return;
    r = callf(f, "n=%d s=%s", 42, "ok");
    fclose(f);
    CHECK_EQ_I(r, 9);
    got = slurp(b, sizeof b); CHECK_EQ_U(got, 9); b[got] = 0; CHECK_STR(b, "n=42 s=ok");
    remove(TMP);
}
TC_CASE(wide_ascii)
{
    FILE *f; char b[64]; size_t got; int r;
    if (!p_vfw) TC_SKIP("not exported");
    f = fopen(TMP, "w"); CHECK(f != NULL); if (!f) return;
    r = callfw(f, L"w=%d %ls", 7, L"abc");
    fclose(f);
    CHECK_EQ_I(r, 7);
    got = slurp(b, sizeof b); CHECK_EQ_U(got, 7); b[got] = 0; CHECK_STR(b, "w=7 abc");
    remove(TMP);
}
TC_CASE(text_newline)
{
    FILE *f; char b[64]; size_t got; int r;
    if (!p_vf) TC_SKIP("not exported");
    f = fopen(TMP, "w"); CHECK(f != NULL); if (!f) return;
    r = callf(f, "a\nb\n");
    fclose(f);
    CHECK_EQ_I(r, 4);
    got = slurp(b, sizeof b); CHECK_EQ_U(got, 6); b[got] = 0; CHECK_STR(b, "a\r\nb\r\n");
    remove(TMP);
}
TC_CASE(binary_newline)
{
    FILE *f; char b[64]; size_t got; int r;
    if (!p_vf) TC_SKIP("not exported");
    f = fopen(TMP, "wb"); CHECK(f != NULL); if (!f) return;
    r = callf(f, "a\nb\n");
    fclose(f);
    CHECK_EQ_I(r, 4);
    got = slurp(b, sizeof b); CHECK_EQ_U(got, 4); b[got] = 0; CHECK_STR(b, "a\nb\n");
    remove(TMP);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vf = (vf_t)tc_crt_proc("__stdio_common_vfprintf");
    p_vfw = (vfw_t)tc_crt_proc("__stdio_common_vfwprintf");
    TC_RUN(narrow); TC_RUN(wide_ascii); TC_RUN(text_newline); TC_RUN(binary_newline);
    return tc_end();
}
