/* TC
 * id:    dat_time_vars_accessors
 * area:  data_exports
 * funcs: __daylight __timezone __dstbias _tzset
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  inferred
 * ref:   ucrt time.h: _daylight/_timezone/_dstbias are macros over __daylight()/__timezone()/__dstbias()
 * note:  The accessors return pointers to the CRT's global variables, so they must be non-NULL, stable and distinct.
 */
#include "tc.h"
#include <time.h>

TC_CASE(ptrs_nonnull) { CHECK(__daylight() != NULL); CHECK(__timezone() != NULL); CHECK(__dstbias() != NULL); }
TC_CASE(ptrs_stable_across_calls)
{ int *d = __daylight(); long *z = __timezone(), *b = __dstbias(); int i;
  for (i = 0; i < 10; i++) { CHECK_EQ_P(__daylight(), d); CHECK_EQ_P(__timezone(), z); CHECK_EQ_P(__dstbias(), b); } }
TC_CASE(distinct_pointers) { CHECK((void *)__timezone() != (void *)__dstbias()); CHECK((void *)__daylight() != (void *)__timezone()); CHECK((void *)__daylight() != (void *)__dstbias()); }
TC_CASE(tzset_keeps_ptrs)
{ int *d = __daylight(); long *z = __timezone(), *b = __dstbias();
  _tzset(); CHECK_EQ_P(__daylight(), d); CHECK_EQ_P(__timezone(), z); CHECK_EQ_P(__dstbias(), b); }
int main(void) { tc_begin(); TC_RUN(ptrs_nonnull); TC_RUN(ptrs_stable_across_calls); TC_RUN(distinct_pointers); TC_RUN(tzset_keeps_ptrs); return tc_end(); }
