/* TC
 * id:    exp_iob_func_win7_exported
 * area:  exports
 * funcs: 
 * procs: __iob_func _fileno
 * crt:   msvcr80 msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   VS2005-2013 stdio.h defines _iob as __iob_func(); only element 0 is inspected (FILE size differs per CRT)
 */
#include "tc.h"


TC_DECL_PROC(void *, __iob_func, (void));
TC_DECL_PROC(int, _fileno, (void *));

TC_CASE(exported) { CHECK(tc_p___iob_func != NULL); }
TC_CASE(first_element_fileno_is_0)
{
    void *p;
    if (!tc_p___iob_func || !tc_p__fileno) TC_SKIP("missing export");
    p = tc_p___iob_func();
    CHECK(p != NULL);
    if (p) CHECK_EQ_I(tc_p__fileno(p), 0);
}
TC_CASE(returns_stable_pointer)
{
    if (!tc_p___iob_func) TC_SKIP("missing export");
    CHECK_EQ_P(tc_p___iob_func(), tc_p___iob_func());
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(__iob_func); TC_LOAD_PROC(_fileno);
    TC_RUN(exported); TC_RUN(first_element_fileno_is_0); TC_RUN(returns_stable_pointer);
    return tc_end();
}
