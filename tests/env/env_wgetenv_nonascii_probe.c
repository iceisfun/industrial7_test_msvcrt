/* TC
 * id:    env_wgetenv_nonascii_probe
 * area:  env
 * funcs: _wgetenv _wputenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _wputenv, _wgetenv
 * note:  Probes: a wide value with BMP characters not representable in the ANSI code page (U+00E9 U+4E2D) is accepted by
 *        _wputenv and returned unchanged by _wgetenv. Older CRTs also mirror it into the narrow environment, where the
 *        conversion may be lossy; it is expected to still succeed. Uses \u escapes so source encoding is irrelevant.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(non_ascii_value_BMP)
{
    wchar_t *v;
    CHECK_EQ_I(_wputenv(L"TCV_WU=é中"), 0);
    v = _wgetenv(L"TCV_WU");
    CHECK(v != NULL);
    if (v) { CHECK(v[0] == 0x00e9 && v[1] == 0x4e2d && v[2] == 0); }
    _wputenv(L"TCV_WU=");
}

int main(void) { tc_begin(); TC_RUN(non_ascii_value_BMP); return tc_end(); }
