/* TC
 * id:    ucr_neg_stdio_vfscanf_closed_and_eof_stream
 * area:  ucrt_only
 * kind:  negative
 * funcs: fopen fclose remove feof
 * procs: __stdio_common_vfscanf __stdio_common_vfwscanf
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.21.6.2p16 (EOF before first conversion returns EOF; otherwise count assigned); MS docs fscanf, Parameter validation (NULL stream -> EINVAL, handler)
 * note:  temp files tc_ucr_neg_stdio_vfscanf_closed_and_eof_stream_*.tmp. Write-only stream errno is in ucr_neg_stdio_vfscanf_writeonly_errno_probe.
 */
#include "tc.h"
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>

typedef int (__cdecl *vfscanf_t)(unsigned long long, FILE *, const char *, void *, va_list);
typedef int (__cdecl *vfwscanf_t)(unsigned long long, FILE *, const wchar_t *, void *, va_list);
static vfscanf_t p_vf;
static vfwscanf_t p_vfw;
#define TMP "tc_ucr_neg_stdio_vfscanf_closed_and_eof_stream_a.tmp"

static int xf(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vf(0, f, fmt, NULL, ap); va_end(ap); return r; }
static int xfw(FILE *f, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vfw(0, f, fmt, NULL, ap); va_end(ap); return r; }

static FILE *mk(const char *content, const char *mode)
{
    FILE *f = fopen(TMP, "wb");
    if (!f) return NULL;
    if (content) fputs(content, f);
    fclose(f);
    return fopen(TMP, mode);
}

TC_CASE(empty_file)
{
    FILE *f = mk(NULL, "r"); int a = 77, r;
    CHECK(f != NULL); if (!f) return;
    r = xf(f, "%d", &a);
    CHECK_EQ_I(r, EOF); CHECK(feof(f) != 0); CHECK_EQ_I(a, 77);
    fclose(f); remove(TMP);
}
TC_CASE(eof_sticky_second_call)
{
    FILE *f = mk(NULL, "r"); int a = 77, r1, r2;
    CHECK(f != NULL); if (!f) return;
    r1 = xf(f, "%d", &a); r2 = xf(f, "%d", &a);
    CHECK_EQ_I(r1, EOF); CHECK_EQ_I(r2, EOF); CHECK_EQ_I(a, 77);
    fclose(f); remove(TMP);
}
TC_CASE(partial_returns_assigned_count)
{
    FILE *f = mk("5 x", "r"); int a = 0, b = 77, r;
    CHECK(f != NULL); if (!f) return;
    r = xf(f, "%d %d", &a, &b);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 5); CHECK_EQ_I(b, 77);
    fclose(f); remove(TMP);
}
TC_CASE(wide_empty_file)
{
    FILE *f = mk(NULL, "r"); int a = 77, r;
    CHECK(f != NULL); if (!f) return;
    r = xfw(f, L"%d", &a);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(a, 77);
    fclose(f); remove(TMP);
}
TC_CASE(wide_partial)
{
    FILE *f = mk("5 x", "r"); int a = 0, b = 77, r;
    CHECK(f != NULL); if (!f) return;
    r = xfw(f, L"%d %d", &a, &b);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 5); CHECK_EQ_I(b, 77);
    fclose(f); remove(TMP);
}
TC_CASE(wide_null_file)
{
    int a = 77, r;
    errno = 0; tc_iph_count = 0;
    r = xfw(NULL, L"%d", &a);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(a, 77);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    p_vf = (vfscanf_t)tc_crt_proc("__stdio_common_vfscanf");
    p_vfw = (vfwscanf_t)tc_crt_proc("__stdio_common_vfwscanf");
    if (!p_vf || !p_vfw) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    TC_RUN(empty_file); TC_RUN(eof_sticky_second_call); TC_RUN(partial_returns_assigned_count);
    TC_RUN(wide_empty_file); TC_RUN(wide_partial); TC_RUN(wide_null_file);
    return tc_end();
}
