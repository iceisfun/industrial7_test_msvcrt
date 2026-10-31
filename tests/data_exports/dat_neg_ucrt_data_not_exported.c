/* TC
 * id:    dat_neg_ucrt_data_not_exported
 * area:  data_exports
 * kind:  negative
 * funcs: 
 * procs: __argc __argv __wargv
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs: UCRT replaced the __argc/__argv/__wargv data exports with __p___argc etc. accessors
 * note:  PROBE: ucrtbase.dll is believed NOT to export the data symbols __argc, __argv, __wargv (only the __p___* accessors). Wine may differ.
 */
#include "tc.h"

TC_CASE(argc_not_exported) { CHECK(tc_crt_proc("__argc") == NULL); }
TC_CASE(argv_not_exported) { CHECK(tc_crt_proc("__argv") == NULL); }
TC_CASE(wargv_not_exported) { CHECK(tc_crt_proc("__wargv") == NULL); }
TC_CASE(accessors_exported) { CHECK(tc_crt_proc("__p___argc") != NULL); CHECK(tc_crt_proc("__p___argv") != NULL); CHECK(tc_crt_proc("__p___wargv") != NULL); }
int main(void) { tc_begin(); TC_RUN(argc_not_exported); TC_RUN(argv_not_exported); TC_RUN(wargv_not_exported); TC_RUN(accessors_exported); return tc_end(); }
