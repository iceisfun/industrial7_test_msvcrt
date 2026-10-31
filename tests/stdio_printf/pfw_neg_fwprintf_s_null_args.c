/* TC
 * id:    pfw_neg_fwprintf_s_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: vfwprintf_s
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf_s, fwprintf_s, vfwprintf_s: NULL stream or NULL format -> invalid parameter handler, -1, EINVAL
 * note:  On ucrtbase fwprintf_s/vfwprintf_s are inline over __stdio_common_vfwprintf_s (options 4), which is called directly here;
 *        legacy CRTs resolve the vfwprintf_s export at run time.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
extern int __cdecl __stdio_common_vfwprintf_s(unsigned __int64, FILE *, const wchar_t *, _locale_t, va_list);
static int sh_vf(FILE *s, const wchar_t *f, va_list ap) { return __stdio_common_vfwprintf_s(4, s, f, NULL, ap); }
#else
typedef int (__cdecl *vf_t)(FILE *, const wchar_t *, va_list);
static vf_t p_vf;
static int sh_vf(FILE *s, const wchar_t *f, va_list ap) { return p_vf ? p_vf(s, f, ap) : -99; }
#endif
static int fws(FILE *s, const wchar_t *f, ...) { int r; va_list ap; va_start(ap, f); r = sh_vf(s, f, ap); va_end(ap); return r; }

static FILE * volatile nstream;
static const wchar_t * volatile nfmt;
static const wchar_t * volatile xfmt = L"x";
static const char tmp[] = "tc_pfw_neg_fwprintf_s_null_args.tmp";

static long flen(void)
{ FILE *f = fopen(tmp, "rb"); long n = -1; if (f) { fseek(f, 0, SEEK_END); n = ftell(f); fclose(f); } return n; }

TC_CASE(null_stream) { int r; tc_iph_count = 0; errno = 0; r = fws(nstream, xfmt); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_stream_with_args) { int r; tc_iph_count = 0; errno = 0; r = fws(nstream, L"%d %ls", 1, L"a"); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_format)
{
    FILE *fp = fopen(tmp, "w"); int r;
    CHECK(fp != NULL); if (!fp) return;
    tc_iph_count = 0; errno = 0; r = fws(fp, nfmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(fp);
    CHECK_EQ_I(flen(), 0);
    remove(tmp);
}
TC_CASE(control)
{
    FILE *fp = fopen(tmp, "w"); int r;
    CHECK(fp != NULL); if (!fp) return;
    tc_iph_count = 0; r = fws(fp, L"%d", 42); CHECK_EQ_I(r, 2); CHECK_EQ_I(tc_iph_count, 0);
    fclose(fp); remove(tmp);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
#if TC_CRT_VER < 140
    p_vf = (vf_t)tc_crt_proc("vfwprintf_s"); CHECK(p_vf != NULL);
#endif
    TC_RUN(null_stream); TC_RUN(null_stream_with_args); TC_RUN(null_format); TC_RUN(control);
    return tc_end();
}
