/* TC
 * id:    dat_wargv_accessor_probe
 * area:  data_exports
 * kind:  positive
 * funcs: __p___wargv
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  observed
 * ref:   MS docs __wargv
 * note:  Win10 22H2 (observed): in a narrow main() program *__p___wargv() is NULL (the wide argv is not built); the accessor pointer itself is non-NULL.
 *        This depends on the exe startup code configuring only the narrow argv (mingw crt for main()); a wmain program would populate it.
 */
#include "tc.h"

extern wchar_t ***__cdecl __p___wargv(void);

TC_CASE(wargv_ptr_nonnull) { CHECK(__p___wargv() != NULL); }
TC_CASE(wargv_array_null_when_main)
{
    wchar_t ***p = __p___wargv();
    if (!p) { CHECK(p != NULL); return; }
    CHECK(*p == NULL);   /* observed on Win10 22H2: a narrow main() program never builds the wide argv, so __wargv stays NULL */
}
int main(void) { tc_begin(); TC_RUN(wargv_ptr_nonnull); TC_RUN(wargv_array_null_when_main); return tc_end(); }
