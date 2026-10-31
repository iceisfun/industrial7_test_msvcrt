/* TC
 * id:    pff_neg_fprintf_s_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf_s vfprintf_s printf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_s, _printf_s_l, fprintf_s, vfprintf_s: stream or format NULL -> invalid parameter handler, returns -1, errno EINVAL
 * note:  printf_s(NULL) runs in a child that redirects stdout to a temp file with freopen so the parent's stdout stays intact.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define OUT "tc_pff_neg_fprintf_s_null_args.tmp"

static int call_vfs(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = vfprintf_s(f, fmt, ap); va_end(ap); return r; }

TC_CASE(null_stream)
{
    FILE * volatile s = NULL; volatile double v = 1.0; int r;
    tc_iph_count = 0; errno = 0;
    r = fprintf_s(s, "%f", v);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    FILE *f = fopen(OUT, "w"); const char * volatile nf = NULL; int r;
    CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0;
    r = fprintf_s(f, nf, 1.0);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f);
}
TC_CASE(v_null_args)
{
    FILE *f = fopen(OUT, "w"); FILE * volatile s = NULL; const char * volatile nf = NULL; int r;
    CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0;
    r = call_vfs(s, "%e", 1.0);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0;
    r = call_vfs(f, nf, 1.0);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f);
}
TC_CASE(printf_s_null_child)
{
    DWORD code = 0; int ran = tc_spawn_self("printf_s_null", &code, 20000);
    HANDLE h; DWORD sz = 0xffff; 
    CHECK(ran);
    CHECK_EQ_U(code, 0);  /* child: -1, EINVAL, IPH count 1 */
    h = CreateFileA(OUT, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    CHECK(h != INVALID_HANDLE_VALUE);
    if (h != INVALID_HANDLE_VALUE) { sz = GetFileSize(h, NULL); CloseHandle(h); }
    CHECK_EQ_U(sz, 0);
}
static int child_main(const char *n)
{
    const char * volatile nf = NULL; int r, bad = 0; (void)n;
    if (!freopen(OUT, "w", stdout)) return 70;
    tc_iph_install();
    tc_iph_count = 0; errno = 0;
    r = printf_s(nf);
    if (r != -1) bad |= 1;
    if (errno != EINVAL) bad |= 2;
    if (tc_iph_count != 1) bad |= 4;
    fflush(stdout);
    return bad;
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_stream); TC_RUN(null_format); TC_RUN(v_null_args); TC_RUN(printf_s_null_child);
    remove(OUT);
    return tc_end();
}
