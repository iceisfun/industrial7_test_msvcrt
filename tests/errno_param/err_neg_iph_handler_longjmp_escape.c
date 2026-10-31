/* TC
 * id:    err_neg_iph_handler_longjmp_escape
 * area:  errno_param
 * kind:  negative
 * funcs: strcpy_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _set_invalid_parameter_handler (silent on leaving the handler via longjmp)
 * note:  PROBE: the docs do not say that leaving the handler via longjmp is supported. The handler leaves via longjmp (setjmp from mingw's CRT-independent builtin); afterwards the CRT must remain usable.
 */
#include "tc.h"
#include <string.h>
#include <setjmp.h>

typedef void (__cdecl *iph_t)(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);
TC_DECL_PROC(iph_t, _set_invalid_parameter_handler, (iph_t));

static jmp_buf jb; static int hcount;
static void __cdecl esc(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; hcount++; longjmp(jb, 1); }

static char * volatile v_null = NULL;

TC_CASE(escape_lands)
{
    volatile int landed = 0; hcount = 0;
    if (setjmp(jb) == 0) { strcpy_s(v_null, 10, "abc"); landed = -1; }
    else landed = 1;
    CHECK_EQ_I(landed, 1); CHECK_EQ_I(hcount, 1);
}
TC_CASE(valid_after)
{
    char d[8]; int r, before = hcount;
    r = strcpy_s(d, sizeof d, "ok");
    CHECK_EQ_I(r, 0); CHECK_STR(d, "ok"); CHECK_EQ_I(hcount, before);
}
TC_CASE(second_bad_call)
{
    volatile int landed = 0; int before = hcount;
    if (setjmp(jb) == 0) { strcpy_s(v_null, 10, "abc"); landed = -1; }
    else landed = 1;
    CHECK_EQ_I(landed, 1); CHECK_EQ_I(hcount, before + 1);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_set_invalid_parameter_handler);
    if (!tc_p__set_invalid_parameter_handler) { tc_write("TC FAIL " TC_TEST_ID "::export missing\n"); return 1; }
    tc_p__set_invalid_parameter_handler(esc);
    TC_RUN(escape_lands); TC_RUN(valid_after); TC_RUN(second_bad_call);
    return tc_end();
}
