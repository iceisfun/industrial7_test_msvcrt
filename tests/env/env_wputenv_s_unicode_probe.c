/* TC
 * id:    env_wputenv_s_unicode_probe
 * area:  env
 * funcs: _wputenv_s _wgetenv
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _putenv_s, _wputenv_s
 * note:  Probes that a value containing non-ASCII wide characters (U+00E9 U+4E2D U+6587 U+0416) round-trips exactly
 *        through _wputenv_s/_wgetenv. The CRT also keeps a narrow copy converted with the ANSI code page, so a
 *        conversion failure on an unusual ANSI code page could in principle make _wputenv_s fail.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(unicode)
{
    static const wchar_t val[] = { 0x00e9, 0x4e2d, 0x6587, 0x0416, 0 };
    wchar_t *v;
    CHECK_EQ_I(_wputenv_s(L"TC_WPU_UNI", val), 0);
    v = _wgetenv(L"TC_WPU_UNI");
    CHECK(v != NULL);
    if (v) CHECK_WSTR(v, val);
    _wputenv_s(L"TC_WPU_UNI", L"");
}
int main(void)
{
    tc_begin();
    TC_RUN(unicode);
    return tc_end();
}
