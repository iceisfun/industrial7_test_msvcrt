/* TC
 * id:    scn_neg_common_vfscanf_null_args
 * area:  scanf
 * kind:  negative
 * funcs:
 * procs: __stdio_common_vfscanf __stdio_common_vfwscanf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs fscanf, fwscanf (NULL stream/format -> invalid parameter handler, EOF, errno EINVAL); __stdio_common_vf[w]scanf are what the inline fscanf/fwscanf call
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

typedef int (__cdecl *cn_t)(unsigned long long, FILE *, const char *, void *, va_list);
typedef int (__cdecl *cw_t)(unsigned long long, FILE *, const wchar_t *, void *, va_list);
static cn_t p_cn; static cw_t p_cw;

static int callN(FILE *s, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = p_cn(0, s, f, NULL, ap); va_end(ap); return r; }
static int callW(FILE *s, const wchar_t *f, ...) { va_list ap; int r; va_start(ap, f); r = p_cw(0, s, f, NULL, ap); va_end(ap); return r; }
static FILE *volatile nf; static const char *volatile ns; static const wchar_t *volatile nws;
#define F "tc_scn_neg_cvf.tmp"
static FILE *mk(void) { FILE *f = fopen(F, "w"); if (f) { fputs("17", f); fclose(f); f = fopen(F, "r"); } return f; }

TC_CASE(stream_null)
{
    int x = 5, r; if (!p_cn) TC_SKIP("not exported"); tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = callN(nf, "%d", &x);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(x, 5);
}
TC_CASE(format_null)
{
    FILE *f; int x = 5, r; if (!p_cn) TC_SKIP("not exported"); f = mk(); CHECK(f != NULL); if (!f) { remove(F); return; }
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = callN(f, ns, &x);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(x, 5);
    fclose(f); remove(F);
}
TC_CASE(wide_stream_null)
{
    int x = 5, r; if (!p_cw) TC_SKIP("not exported"); tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = callW(nf, L"%d", &x);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(x, 5);
}
TC_CASE(wide_format_null)
{
    FILE *f; int x = 5, r; if (!p_cw) TC_SKIP("not exported"); f = mk(); CHECK(f != NULL); if (!f) { remove(F); return; }
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = callW(f, nws, &x);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(x, 5);
    fclose(f); remove(F);
}
int main(void)
{
    tc_begin();
    p_cn = (cn_t)tc_crt_proc("__stdio_common_vfscanf"); p_cw = (cw_t)tc_crt_proc("__stdio_common_vfwscanf");
    CHECK_EQ_I(p_cn != NULL && p_cw != NULL, 1);
    TC_RUN(stream_null); TC_RUN(format_null); TC_RUN(wide_stream_null); TC_RUN(wide_format_null);
    return tc_end();
}
