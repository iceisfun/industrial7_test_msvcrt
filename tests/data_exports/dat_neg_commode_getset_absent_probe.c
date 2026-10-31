/* TC
 * id:    dat_neg_commode_getset_absent_probe
 * area:  data_exports
 * funcs: _get_fmode
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _commode (documents only the variable / __p__commode, no accessor functions)
 * note:  PROBE: ucrtbase.dll is believed NOT to export _get_commode / _set_commode (unlike _get_fmode / _set_fmode).
 *        A failure here means such exports exist.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(fmode_getter_is_exported) { CHECK(tc_crt_proc("_get_fmode") != NULL); }
TC_CASE(get_commode_not_exported) { CHECK(tc_crt_proc("_get_commode") == NULL); }
TC_CASE(set_commode_not_exported) { CHECK(tc_crt_proc("_set_commode") == NULL); }

int main(void)
{
    int m = -1;
    tc_begin();
    (void)_get_fmode(&m);  /* anchors the import */
    TC_RUN(fmode_getter_is_exported); TC_RUN(get_commode_not_exported); TC_RUN(set_commode_not_exported);
    return tc_end();
}
