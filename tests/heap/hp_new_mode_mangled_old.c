/* TC
 * id:    hp_new_mode_mangled_old
 * area:  heap
 * funcs:
 * procs: ?_set_new_mode@@YAHH@Z ?_query_new_mode@@YAHXZ
 * crt:   win7
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_new_mode, _query_new_mode
 * note:  Old CRT import libraries list the new-mode functions under C++ mangled names. Semantics (default 0, set returns the
 *        previous mode) are documented; the mangled export names are from the import libraries and are verified on real DLLs here.
 */
#include "tc.h"

typedef int (__cdecl *setmode_t)(int);
typedef int (__cdecl *querymode_t)(void);
static setmode_t p_set;
static querymode_t p_query;

TC_CASE(resolve) { CHECK(p_set != NULL); CHECK(p_query != NULL); }
TC_CASE(roundtrip)
{
    if (!p_set || !p_query) TC_SKIP("not exported");
    CHECK_EQ_I(p_query(), 0);
    CHECK_EQ_I(p_set(1), 0);
    CHECK_EQ_I(p_query(), 1);
    CHECK_EQ_I(p_set(0), 1);
    CHECK_EQ_I(p_query(), 0);
}

int main(void)
{
    tc_begin();
    p_set = (setmode_t)tc_crt_proc("?_set_new_mode@@YAHH@Z");
    p_query = (querymode_t)tc_crt_proc("?_query_new_mode@@YAHXZ");
    TC_RUN(resolve); TC_RUN(roundtrip);
    return tc_end();
}
