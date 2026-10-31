/* TC
 * id:    ucr_fmode_default_probe
 * area:  ucrt_only
 * funcs: __p__fmode _get_fmode
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _fmode (documented as defaulting to text mode; internal representation not documented)
 * note:  PROBE: at startup _get_fmode() yields 0 and *__p__fmode() is 0 (text mode represented as 0, not _O_TEXT=0x4000);
 *        assumes the startup code (MSVC or mingw) has not changed it.
 */
#include "tc.h"
#include <stdlib.h>
TC_CASE(zero) { int m = -1; CHECK_EQ_I(_get_fmode(&m), 0); CHECK_EQ_I(m, 0); CHECK_EQ_I(*__p__fmode(), 0); }
int main(void) { tc_begin(); TC_RUN(zero); return tc_end(); }
