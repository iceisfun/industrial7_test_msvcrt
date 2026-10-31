/* TC
 * id:    ucr_stdio_vfscanf_file
 * area:  ucrt_only
 * funcs: fopen fclose remove fputs ftell
 * procs: __stdio_common_vfscanf
 * crt:   win10
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2 (fscanf); MS docs fscanf
 */
#include "tc.h"
#include <stdarg.h>
#include <stdio.h>

typedef int (__cdecl *vfscanf_t)(unsigned long long, FILE *, const char *, void *, va_list);
static vfscanf_t p_vfs;
static const char *fn = "tc_ucr_stdio_vfscanf_file.tmp";

static int xf(FILE *f, const char *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt); r = p_vfs(0, f, fmt, NULL, ap); va_end(ap);
    return r;
}
static FILE *mk(const char *content)
{
    FILE *f = fopen(fn, "wb");
    if (f) { fputs(content, f); fclose(f); }
    return fopen(fn, "rb");
}

TC_CASE(ints_and_word)
{
    FILE *f = mk("10 -20 word\n"); int a = 0, b = 0; char w[16]; int r;
    CHECK(f != NULL); if (!f) return;
    r = xf(f, "%d %d %15s", &a, &b, w);
    CHECK_EQ_I(r, 3); CHECK_EQ_I(a, 10); CHECK_EQ_I(b, -20); CHECK_STR(w, "word");
    fclose(f); remove(fn);
}
TC_CASE(eof_at_end)
{
    FILE *f = mk("5"); int a = 0, r;
    CHECK(f != NULL); if (!f) return;
    r = xf(f, "%d", &a); CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 5);
    r = xf(f, "%d", &a); CHECK_EQ_I(r, -1);
    fclose(f); remove(fn);
}
TC_CASE(position_after_partial)
{
    FILE *f = mk("12abc"); int a = 0, r; char c = 0;
    CHECK(f != NULL); if (!f) return;
    r = xf(f, "%d", &a);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 12);
    CHECK_EQ_I(ftell(f), 2);      /* the 'a' that ended the number was pushed back */
    r = xf(f, "%c", &c);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(c, 'a');
    fclose(f); remove(fn);
}

int main(void)
{
    tc_begin();
    p_vfs = (vfscanf_t)tc_crt_proc("__stdio_common_vfscanf");
    if (!p_vfs) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    TC_RUN(ints_and_word); TC_RUN(eof_at_end); TC_RUN(position_after_partial);
    return tc_end();
}
