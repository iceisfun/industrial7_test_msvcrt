/* TC
 * id:    dat_wargv_accessor_probe
 * area:  data_exports
 * kind:  positive
 * funcs: __p___wargv
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  probe
 * ref:   MS docs __wargv
 * note:  PROBE: in a narrow main() program, is *__p___wargv() non-NULL and populated (wargv[0] non-empty)? The accessor pointer itself being non-NULL is near certain.
 */
#include "tc.h"

extern wchar_t ***__cdecl __p___wargv(void);

TC_CASE(wargv_ptr_nonnull) { CHECK(__p___wargv() != NULL); }
TC_CASE(wargv_array_populated_when_main)
{
    wchar_t ***p = __p___wargv();
    if (!p) { CHECK(p != NULL); return; }
    CHECK(*p != NULL);
    if (*p) { CHECK((*p)[0] != NULL); if ((*p)[0]) CHECK((*p)[0][0] != 0); }
}
int main(void) { tc_begin(); TC_RUN(wargv_ptr_nonnull); TC_RUN(wargv_array_populated_when_main); return tc_end(); }
