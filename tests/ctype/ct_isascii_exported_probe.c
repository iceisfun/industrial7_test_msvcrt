/* TC
 * id:    ct_isascii_exported_probe
 * area:  ctype
 * funcs:
 * procs: __isascii isascii
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs isascii, __isascii
 * note:  PROBE: that the undecorated name `isascii` is an exported symbol of every CRT DLL (mingw import
 *        libs list it for all targets; not confirmed by documentation).
 *        ucrtbase (observed Win10 22H2 19041 export table, via GetProcAddress): `isascii` is NOT exported, `__isascii` is (the
 *        mingw import lib/static oldnames supply the undecorated name). The ucrtbase build asserts that; other CRTs keep the
 *        original expectation [unverified on real msvcr100-120 / msvcrt].
 */
#include "tc.h"
#include <ctype.h>

TC_CASE(isascii_exported)
{
#if TC_AT_LEAST(140)   /* ucrtbase */
    CHECK(tc_crt_proc("isascii") == NULL);   /* observed: not exported from ucrtbase */
#else
    CHECK(tc_crt_proc("isascii") != NULL);
#endif
}
TC_CASE(__isascii_exported) { CHECK(tc_crt_proc("__isascii") != NULL); }

int main(void) { tc_begin(); TC_RUN(isascii_exported); TC_RUN(__isascii_exported); return tc_end(); }
