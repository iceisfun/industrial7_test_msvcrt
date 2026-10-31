/* TC
 * id:    exp_probe_msvcrt_secure_io_env
 * area:  exports
 * funcs:
 * procs: _set_invalid_parameter_handler fopen_s _wfopen_s getenv_s _putenv_s qsort_s bsearch_s
 * crt:   msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs Security-enhanced versions of CRT functions (msvcrt.dll listed as a requirement for some)
 * note:  Probing: whether the OS msvcrt.dll exports the stdio/env/search secure names. An import library suggests fopen_s/_wfopen_s/
 *        getenv_s/_putenv_s/qsort_s/bsearch_s are present (so the former 'absent' claim in exp_neg_probe_msvcrt_no_secure_rest was
 *        dropped); _set_invalid_parameter_handler is less certain. One case per name.
 */
#include "tc.h"

TC_CASE(n_set_invalid_parameter_handler) { CHECK(tc_crt_proc("_set_invalid_parameter_handler") != NULL); }
TC_CASE(n_fopen_s) { CHECK(tc_crt_proc("fopen_s") != NULL); }
TC_CASE(n_wfopen_s) { CHECK(tc_crt_proc("_wfopen_s") != NULL); }
TC_CASE(n_getenv_s) { CHECK(tc_crt_proc("getenv_s") != NULL); }
TC_CASE(n_putenv_s) { CHECK(tc_crt_proc("_putenv_s") != NULL); }
TC_CASE(n_qsort_s) { CHECK(tc_crt_proc("qsort_s") != NULL); }
TC_CASE(n_bsearch_s) { CHECK(tc_crt_proc("bsearch_s") != NULL); }

int main(void) { tc_begin(); TC_RUN(n_set_invalid_parameter_handler); TC_RUN(n_fopen_s); TC_RUN(n_wfopen_s); TC_RUN(n_getenv_s); TC_RUN(n_putenv_s); TC_RUN(n_qsort_s); TC_RUN(n_bsearch_s); return tc_end(); }
