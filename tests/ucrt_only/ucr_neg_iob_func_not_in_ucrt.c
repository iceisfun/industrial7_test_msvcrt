/* TC
 * id:    ucr_neg_iob_func_not_in_ucrt
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: __acrt_iob_func
 * crt:   win10
 * tier:  p1
 * conf:  observed
 * ref:   UCRT (VS2015) replaced the __iob_func export with __acrt_iob_func; MS docs "Universal CRT deployment"/Visual C++ change history 2015
 * note:  Legacy iob exports must not be present in ucrtbase.dll.
 */
#include "tc.h"

TC_CASE(no_iob_func) { CHECK(tc_crt_proc("__iob_func") == NULL); }
TC_CASE(no_iob)      { CHECK(tc_crt_proc("_iob") == NULL); }
TC_CASE(has_acrt)    { CHECK(tc_crt_proc("__acrt_iob_func") != NULL); }

int main(void) { tc_begin(); TC_RUN(no_iob_func); TC_RUN(no_iob); TC_RUN(has_acrt); return tc_end(); }
