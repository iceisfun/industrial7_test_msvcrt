/* TC
 * id:    ct_neg_mbctype_data_absent_ucrt_probe
 * area:  ctype
 * funcs:
 * procs: _mbctype __p__mbctype
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs UCRT breaking changes (data exports replaced by functions)
 * note:  PROBE: ucrtbase exports the function __p__mbctype but is believed NOT to export the data symbol _mbctype.
 */
#include "tc.h"

TC_CASE(_mbctype_data_not_exported) { CHECK(tc_crt_proc("_mbctype") == NULL); }
TC_CASE(__p__mbctype_exported) { CHECK(tc_crt_proc("__p__mbctype") != NULL); }

int main(void) { tc_begin(); TC_RUN(_mbctype_data_not_exported); TC_RUN(__p__mbctype_exported); return tc_end(); }
