/* TC
 * id:    pfm_1_neg_vprintf_vwprintf_stdout
 * area:  stdio_printf
 * funcs: vprintf vwprintf vfprintf
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   C11 7.21.6.8/7.29.2.7 (vprintf, vwprintf return chars written); MS docs vprintf, vwprintf: NULL format -> invalid parameter handler, -1, EINVAL
 * note:  Children redirect stdout to a temp file with freopen (the parent's stdout must stay intact for reporting). Cases: success path
 *        writes "42" and returns 2 (vprintf/vwprintf), vfprintf to a plain file stream, and NULL format -> -1 + one handler call + EINVAL.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <errno.h>

#define FN "tc_pfm_1_vp.tmp"

static int call_vprintf(const char *volatile f, ...)  { va_list ap; int r; va_start(ap, f); r = vprintf(f, ap); va_end(ap); return r; }
static int call_vwprintf(const wchar_t *volatile f, ...) { va_list ap; int r; va_start(ap, f); r = vwprintf(f, ap); va_end(ap); return r; }
static int call_vfprintf(FILE *fp, const char *volatile f, ...) { va_list ap; int r; va_start(ap, f); r = vfprintf(fp, f, ap); va_end(ap); return r; }

static int slurp(char *out, int cap)
{
    FILE *fp = fopen(FN, "rb"); int n = 0;
    if (!fp) return -1;
    n = (int)fread(out, 1, (size_t)cap - 1, fp); out[n] = 0; fclose(fp);
    return n;
}
static int redirect(void) { return freopen(FN, "w", stdout) != NULL; }

static int child_main(const char *c)
{
    char buf[64]; int r, n;
    tc_iph_install();
    if (!redirect()) return 10;
    if (c[0] == 'a') {                      /* vprintf ok */
        r = call_vprintf("%d", 42);
        fflush(stdout); fclose(stdout);
        n = slurp(buf, sizeof buf);
        return (r == 2 && n == 2 && buf[0] == '4' && buf[1] == '2') ? 0 : 20;
    }
    if (c[0] == 'b') {                      /* vwprintf ok */
        r = call_vwprintf(L"%d", 42);
        fflush(stdout); fclose(stdout);
        n = slurp(buf, sizeof buf);
        return (r == 2 && n == 2 && buf[0] == '4' && buf[1] == '2') ? 0 : 21;
    }
    if (c[0] == 'c') {                      /* vprintf NULL */
        errno = 0; tc_iph_count = 0;
        r = call_vprintf(NULL);
        return (r == -1 && tc_iph_count == 1 && errno == EINVAL) ? 0 : 22;
    }
    if (c[0] == 'd') {                      /* vwprintf NULL */
        errno = 0; tc_iph_count = 0;
        r = call_vwprintf(NULL);
        return (r == -1 && tc_iph_count == 1 && errno == EINVAL) ? 0 : 23;
    }
    return 99;
}

TC_CASE(vprintf_ok)    { DWORD code = 99; CHECK(tc_spawn_self("a", &code, 20000)); CHECK_EQ_U(code, 0); }
TC_CASE(vwprintf_ok)   { DWORD code = 99; CHECK(tc_spawn_self("b", &code, 20000)); CHECK_EQ_U(code, 0); }
TC_CASE(vprintf_null)  { DWORD code = 99; CHECK(tc_spawn_self("c", &code, 20000)); CHECK_EQ_U(code, 0); }
TC_CASE(vwprintf_null) { DWORD code = 99; CHECK(tc_spawn_self("d", &code, 20000)); CHECK_EQ_U(code, 0); }
TC_CASE(vfprintf_file)
{
    char buf[64]; int r, n; FILE *fp = fopen(FN, "w");
    CHECK(fp != NULL); if (!fp) return;
    r = call_vfprintf(fp, "%d", 42); fclose(fp);
    n = slurp(buf, sizeof buf); remove(FN);
    CHECK_EQ_I(r, 2); CHECK_EQ_I(n, 2); CHECK_STR(buf, "42");
}
TC_CASE(vfprintf_null_fmt)
{
    FILE *fp = fopen(FN, "w"); int r;
    CHECK(fp != NULL); if (!fp) return;
    errno = 0; tc_iph_count = 0;
    r = call_vfprintf(fp, NULL);
    fclose(fp); remove(FN);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child_main(c);
    tc_iph_install();
    TC_RUN(vprintf_ok); TC_RUN(vwprintf_ok); TC_RUN(vprintf_null); TC_RUN(vwprintf_null);
    TC_RUN(vfprintf_file); TC_RUN(vfprintf_null_fmt);
    remove(FN);
    return tc_end();
}
