/* TC
 * id:    prc_fmode_pointer_set_get
 * area:  process
 * procs: __p__fmode _set_fmode _get_fmode
 * crt:   >=80
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _set_fmode, _get_fmode, _fmode
 * note:  procs because old import libs lack accessors or supply static copies.
 */
#include "tc.h"
#include <fcntl.h>

TC_DECL_PROC(int *, __p__fmode, (void));
TC_DECL_PROC(int, _set_fmode, (int));
TC_DECL_PROC(int, _get_fmode, (int *));
static int orig;

TC_CASE(pointer_stable) { int *a, *b; CHECK(tc_p___p__fmode); a = tc_p___p__fmode(); b = tc_p___p__fmode(); CHECK(a != NULL); CHECK_EQ_P(a, b); }
TC_CASE(set_binary)
{
    int m = -1;
    CHECK_EQ_I(tc_p__set_fmode(_O_BINARY), 0);
    CHECK_EQ_I(tc_p__get_fmode(&m), 0);
    CHECK_EQ_I(m, _O_BINARY);
    CHECK_EQ_I(*tc_p___p__fmode(), _O_BINARY);
}
TC_CASE(set_text)
{
    int m = -1;
    CHECK_EQ_I(tc_p__set_fmode(_O_TEXT), 0);
    CHECK_EQ_I(tc_p__get_fmode(&m), 0);
    CHECK_EQ_I(m, _O_TEXT);
}
TC_CASE(restore)
{
    int m = -1;
    *tc_p___p__fmode() = orig;   /* orig may be 0, which _set_fmode rejects */
    CHECK_EQ_I(tc_p__get_fmode(&m), 0);
    CHECK_EQ_I(m, orig);
}

int main(void)
{
    tc_begin();
    TC_LOAD_PROC(__p__fmode); TC_LOAD_PROC(_set_fmode); TC_LOAD_PROC(_get_fmode);
    if (!tc_p___p__fmode || !tc_p__set_fmode || !tc_p__get_fmode) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    orig = *tc_p___p__fmode();
    TC_RUN(pointer_stable); TC_RUN(set_binary); TC_RUN(set_text); TC_RUN(restore);
    return tc_end();
}
