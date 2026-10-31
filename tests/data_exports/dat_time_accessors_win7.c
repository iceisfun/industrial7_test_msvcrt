/* TC
 * id:    dat_time_accessors_win7
 * area:  data_exports
 * funcs: __p__daylight __p__timezone __p__tzname _tzset _putenv
 * crt:   msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _tzset, _timezone, _daylight, _tzname (accessor pointers returned by __p__*)
 * note:  The __p__* exports themselves are undocumented (observed: the msvcr100+ headers implement _daylight etc. through them);
 *        the TZ=PST8PDT values asserted come from the _tzset docs.
 */
#include "tc.h"
#include <stdlib.h>
#include <time.h>

__declspec(dllimport) int * __cdecl __p__daylight(void);
__declspec(dllimport) long * __cdecl __p__timezone(void);
__declspec(dllimport) char ** __cdecl __p__tzname(void);

TC_CASE(ptrs_nonnull) { CHECK(__p__daylight() != NULL); CHECK(__p__timezone() != NULL); CHECK(__p__tzname() != NULL); }
TC_CASE(values_after_tzset_pst8pdt)
{
    _putenv("TZ=PST8PDT"); _tzset();
    CHECK_EQ_I(*__p__timezone(), 28800);
    CHECK(*__p__daylight() != 0);
}
TC_CASE(tzname_array_two_entries)
{
    char **n = __p__tzname();
    _putenv("TZ=PST8PDT"); _tzset();
    CHECK(n[0] != NULL && n[1] != NULL);
    CHECK_STR(n[0], "PST"); CHECK_STR(n[1], "PDT");
    CHECK_EQ_P(n, __p__tzname());
}

int main(void) { tc_begin(); TC_RUN(ptrs_nonnull); TC_RUN(values_after_tzset_pst8pdt); TC_RUN(tzname_array_two_entries); return tc_end(); }
