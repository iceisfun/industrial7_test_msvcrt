/* TC
 * id:    pfw_neg_fwprintf_p_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: _vfwprintf_p
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _fprintf_p, _fwprintf_p, _vfwprintf_p: NULL stream or NULL format -> invalid parameter handler, -1, EINVAL
 * note:  On ucrtbase these are inline over __stdio_common_vfwprintf_p (options 4); legacy CRTs resolve _vfwprintf_p at run time.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

#if TC_CRT_VER >= 140
static int sh_vf(FILE *s, const wchar_t *f, va_list ap) { return __stdio_common_vfwprintf_p(4, s, f, NULL, ap); }
#else
typedef int (__cdecl *vf_t)(FILE *, const wchar_t *, va_list);
static vf_t p_vf;
static int sh_vf(FILE *s, const wchar_t *f, va_list ap) { return p_vf ? p_vf(s, f, ap) : -99; }
#endif
static int fwp(FILE *s, const wchar_t *f, ...) { int r; va_list ap; va_start(ap, f); r = sh_vf(s, f, ap); va_end(ap); return r; }
static FILE * volatile nstream;
static const wchar_t * volatile nfmt;
static const char tmp[] = "tc_pfw_neg_fwprintf_p_null_args.tmp";
static long flen(void)
{ FILE *f = fopen(tmp, "rb"); long n = -1; if (f) { fseek(f, 0, SEEK_END); n = ftell(f); fclose(f); } return n; }

TC_CASE(null_stream) { int r; tc_iph_count = 0; errno = 0; r = fwp(nstream, L"%1$d", 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
TC_CASE(null_format)
{
    FILE *fp = fopen(tmp, "w"); int r;
    CHECK(fp != NULL); if (!fp) return;
    tc_iph_count = 0; errno = 0; r = fwp(fp, nfmt); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1);
    fclose(fp); CHECK_EQ_I(flen(), 0); remove(tmp);
}
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
#if TC_CRT_VER < 140
    p_vf = (vf_t)tc_crt_proc("_vfwprintf_p");
    CHECK(p_vf != NULL);
#endif
    TC_RUN(null_stream); TC_RUN(null_format);
    return tc_end();
}
