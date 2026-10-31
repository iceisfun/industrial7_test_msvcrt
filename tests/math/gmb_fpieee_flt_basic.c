/* TC
 * id:    gmb_fpieee_flt_basic
 * area:  math
 * funcs: _fpieee_flt
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _fpieee_flt (fpieee.h)
 * note:  PROBE: for a non-floating-point exception code the handler is not called and EXCEPTION_CONTINUE_SEARCH (0)
 *        is returned. Valid (zeroed) EXCEPTION_POINTERS are passed so a CRT that does look at them cannot crash;
 *        NULL pointers are probed in a child process by gmb_fpieee_flt_neg_null_handler.
 */
#include "tc.h"
#include <excpt.h>
#include <fpieee.h>

static int calls;
static EXCEPTION_RECORD g_rec;
static CONTEXT g_ctx;
static EXCEPTION_POINTERS g_ep;
static int __cdecl handler(_FPIEEE_RECORD *r) { (void)r; calls++; return EXCEPTION_CONTINUE_SEARCH; }

TC_CASE(access_violation_code)
{
    int r;
    calls = 0; g_rec.ExceptionCode = EXCEPTION_ACCESS_VIOLATION; g_ep.ExceptionRecord = &g_rec; g_ep.ContextRecord = &g_ctx;
    r = _fpieee_flt(EXCEPTION_ACCESS_VIOLATION, &g_ep, handler);
    CHECK_EQ_I(r, EXCEPTION_CONTINUE_SEARCH);
    CHECK_EQ_I(calls, 0);
}
TC_CASE(zero_code)
{
    int r;
    calls = 0; g_rec.ExceptionCode = 0; g_ep.ExceptionRecord = &g_rec; g_ep.ContextRecord = &g_ctx;
    r = _fpieee_flt(0, &g_ep, handler);
    CHECK_EQ_I(r, EXCEPTION_CONTINUE_SEARCH);
    CHECK_EQ_I(calls, 0);
}

int main(void)
{
    tc_begin();
    TC_RUN(access_violation_code); TC_RUN(zero_code);
    return tc_end();
}
