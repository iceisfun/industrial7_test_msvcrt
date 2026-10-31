/* TC
 * id:    dat_pctype_data_win7
 * area:  data_exports
 * funcs: __p__pctype
 * procs: __pctype_func
 * crt:   msvcr120 msvcr110 msvcr100
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   (undocumented) _pctype global and __p__pctype accessor, ctype.h macros use them
 * note:  Probe: __p__pctype() returns pointer to the _pctype variable, which in the C locale equals __pctype_func().
 */
#include "tc.h"
#include <ctype.h>

typedef const unsigned short *(__cdecl *tabfn_t)(void);
static tabfn_t p_pctype_func;

TC_CASE(pctype_nonnull) { const unsigned short **pp = (const unsigned short **)__p__pctype(); CHECK(pp != NULL); if (pp) CHECK(*pp != NULL); }
TC_CASE(same_table_as_func) { const unsigned short **pp = (const unsigned short **)__p__pctype(); if (!pp) return; CHECK(p_pctype_func != NULL); if (p_pctype_func) CHECK_EQ_P(*pp, p_pctype_func()); }
TC_CASE(accessor_points_to_same)
{
    const unsigned short **a = (const unsigned short **)__p__pctype(), **b = (const unsigned short **)__p__pctype();
    CHECK_EQ_P(a, b);
    if (a && *a) CHECK((*a)['A'] & 1);
}
int main(void) { tc_begin(); p_pctype_func = (tabfn_t)tc_crt_proc("__pctype_func"); TC_RUN(pctype_nonnull); TC_RUN(same_table_as_func); TC_RUN(accessor_points_to_same); return tc_end(); }
