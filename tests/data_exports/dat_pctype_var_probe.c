/* TC
 * id:    dat_pctype_var_probe
 * area:  data_exports
 * funcs:
 * procs: __pctype_func _pctype
 * crt:   msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   (undocumented) exported data variable _pctype
 * note:  Probe: DLL exports the data variable _pctype (unsigned short *) whose value equals __pctype_func() in the C locale.
 *        Skipped when the export is absent.
 */
#include "tc.h"
#include <ctype.h>

typedef const unsigned short *(__cdecl *tabfn_t)(void);

TC_CASE(var_matches_func)
{
    const unsigned short **v = (const unsigned short **)tc_crt_proc("_pctype");
    tabfn_t f = (tabfn_t)tc_crt_proc("__pctype_func");
    if (!v || !f) TC_SKIP("_pctype not exported");
    CHECK(*v != NULL);
    CHECK_EQ_P(*v, f());
}
int main(void) { tc_begin(); TC_RUN(var_matches_func); return tc_end(); }
