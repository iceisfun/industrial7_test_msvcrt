/* TC
 * id:    env_probe_wputenv_unicode_value
 * area:  env
 * funcs: _wputenv _wgetenv
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _wputenv (non-ASCII values not discussed)
 * note:  PROBE: a value with non-Latin-1 characters set via _wputenv is returned unchanged by _wgetenv. The CRT may also
 *        maintain a narrow copy (lossy under an ANSI code page); only the wide round trip is asserted.
 */
#include "tc.h"
#include <stdlib.h>

static wchar_t *volatile su = L"TCENV_WPU=\x00e9\x4e2d\x0416";
static wchar_t *volatile nmu = L"TCENV_WPU";
static wchar_t *volatile sdu = L"TCENV_WPU=";

TC_CASE(unicode_value_roundtrip)
{
    CHECK_EQ_I(_wputenv(su), 0);
    CHECK(_wgetenv(nmu) != NULL);
    if (_wgetenv(nmu)) CHECK_WSTR(_wgetenv(nmu), L"\x00e9\x4e2d\x0416");
    _wputenv(sdu);
}

int main(void)
{
    tc_begin();
    TC_RUN(unicode_value_roundtrip);
    return tc_end();
}
