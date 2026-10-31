/* TC
 * id:    pfq_sprintf_p_msvcr80_dynamic
 * area:  stdio_printf
 * funcs:
 * procs: _sprintf_p _vsprintf_p _fprintf_p
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs sprintf_p, _vsprintf_p, _fprintf_p (VS2005 and later); libmsvcr80.a lacks these exports so they are resolved at run time
 * note:  ucrtbase does not export _sprintf_p (inline in the SDK headers), hence not in crt:.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

TC_DECL_PROC(int, _sprintf_p, (char *, size_t, const char *, ...));
TC_DECL_PROC(int, _vsprintf_p, (char *, size_t, const char *, va_list));
TC_DECL_PROC(int, _fprintf_p, (FILE *, const char *, ...));

#define XS_(x) #x
#define XS(x) XS_(x)
#define FN "tc_pfq_sprintf_p_msvcr80_dynamic_" XS(TC_CRT_VER) ".tmp"

static int vwrap(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = tc_p__vsprintf_p(b, n, f, ap); va_end(ap); return r; }

TC_CASE(exported) { CHECK(tc_p__sprintf_p != NULL); CHECK(tc_p__vsprintf_p != NULL); CHECK(tc_p__fprintf_p != NULL); }
TC_CASE(reorder)
{
    char b[32]; int r;
    if (!tc_p__sprintf_p) TC_SKIP("not exported");
    r = tc_p__sprintf_p(b, sizeof b, "%2$d %1$d", 1, 2);
    CHECK_EQ_I(r, 3); CHECK_STR(b, "2 1");
}
TC_CASE(vsprintf_p)
{
    char b[32]; int r;
    if (!tc_p__vsprintf_p) TC_SKIP("not exported");
    r = vwrap(b, sizeof b, "%2$s=%1$d", 8, "k");
    CHECK_EQ_I(r, 3); CHECK_STR(b, "k=8");
}
TC_CASE(fprintf_p)
{
    char b[32]; size_t n = 0; FILE *f; int r;
    if (!tc_p__fprintf_p) TC_SKIP("not exported");
    f = fopen(FN, "wb");
    CHECK(f != NULL); if (!f) return;
    r = tc_p__fprintf_p(f, "%2$s %1$d", 5, "x");
    fclose(f);
    CHECK_EQ_I(r, 3);
    f = fopen(FN, "rb");
    if (f) { n = fread(b, 1, sizeof b - 1, f); fclose(f); }
    b[n] = 0;
    CHECK_STR(b, "x 5");
    CHECK_EQ_I(remove(FN), 0);
}
TC_CASE(reuse)
{
    char b[32]; int r;
    if (!tc_p__sprintf_p) TC_SKIP("not exported");
    r = tc_p__sprintf_p(b, sizeof b, "%1$d %1$d", 7);
    CHECK_EQ_I(r, 3); CHECK_STR(b, "7 7");
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_sprintf_p); TC_LOAD_PROC(_vsprintf_p); TC_LOAD_PROC(_fprintf_p);
    TC_RUN(exported); TC_RUN(reorder); TC_RUN(vsprintf_p); TC_RUN(fprintf_p); TC_RUN(reuse);
    return tc_end();
}
