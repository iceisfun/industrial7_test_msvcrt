/* TC
 * id:    dat_neg_ucrt_osver_nerr_absent_probe
 * area:  data_exports
 * procs: _osver _winver _winmajor _winminor _sys_nerr _sys_errlist
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _osver/_winver (documented only up to the VS2015-era CRT), _sys_nerr
 * note:  PROBE: believed that ucrtbase.dll does not export the _osver/_winver/_winmajor/_winminor data nor _sys_nerr/_sys_errlist
 *        (replaced by __sys_nerr()/__sys_errlist() accessors). Kept apart from the documented absent-symbol test.
 */
#include "tc.h"

static int absent(const char *n) { return tc_crt_proc(n) == NULL; }

TC_CASE(osver_family_absent) { CHECK(absent("_osver")); CHECK(absent("_winver")); CHECK(absent("_winmajor")); CHECK(absent("_winminor")); }
TC_CASE(nerr_errlist_data_absent) { CHECK(absent("_sys_nerr")); CHECK(absent("_sys_errlist")); }

int main(void) { tc_begin(); TC_RUN(osver_family_absent); TC_RUN(nerr_errlist_data_absent); return tc_end(); }
