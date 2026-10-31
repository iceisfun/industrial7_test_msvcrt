/* TC
 * id:    gmb_fpieee_flt_neg_null_handler
 * area:  math
 * funcs: _fpieee_flt
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _fpieee_flt
 * note:  PROBE: NULL exception pointers / NULL handler. All calls run in a child process; the parent only asserts that it
 *        survives and that the child either crashed (nonzero) or returned. For a non-FP code with NULL, 0 is expected.
 */
#include "tc.h"
#include <excpt.h>
#include <fpieee.h>

static int __cdecl handler(_FPIEEE_RECORD *r) { (void)r; return EXCEPTION_CONTINUE_SEARCH; }

TC_CASE(fp_code_null_ptrs)
{
    DWORD code = 0xabcd; int ran = tc_spawn_self("fpnull", &code, 20000);
    CHECK(ran);
    CHECK(code != 0xdead0001 && code != 0xdead0002);  /* classification only: crash or any return is acceptable */
}
TC_CASE(av_code_all_null)
{
    DWORD code = 0xabcd; int ran = tc_spawn_self("avnull", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 0);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        volatile unsigned long ec;
        if (c[0] == 'f') { ec = EXCEPTION_FLT_DIVIDE_BY_ZERO; return (int)(_fpieee_flt(ec, NULL, handler) & 0x7f) + 1; }
        ec = EXCEPTION_ACCESS_VIOLATION; return _fpieee_flt(ec, NULL, NULL);
    }
    TC_RUN(fp_code_null_ptrs); TC_RUN(av_code_all_null);
    return tc_end();
}
