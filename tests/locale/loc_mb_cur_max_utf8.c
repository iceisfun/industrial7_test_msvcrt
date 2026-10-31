/* TC
 * id:    loc_mb_cur_max_utf8
 * area:  locale
 * funcs: ___mb_cur_max_func setlocale
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs "UTF-8 Support" in the CRT (Windows 10 1803+); MS docs MB_CUR_MAX, setlocale
 * note:  Skips (not fails) if setlocale returns NULL so that ucrtbase builds without UTF-8 support (pre-1803) still run.
 *        Only ".UTF-8" is asserted here; MB_CUR_MAX==4 is observed, not spelled out in the docs.
 *        Other spellings are in loc_probe_mb_cur_max_utf8_names.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static const char *volatile n_dot_utf8 = ".UTF-8";
static const char *volatile n_c = "C";

TC_CASE(dot_utf8_4)
{
    if (!setlocale(LC_ALL, n_dot_utf8)) { setlocale(LC_ALL, n_c); TC_SKIP("UTF-8 locale unsupported"); }
    CHECK_EQ_I(___mb_cur_max_func(), 4);
    setlocale(LC_ALL, n_c);
    CHECK_EQ_I(___mb_cur_max_func(), 1);
}
int main(void)
{
    tc_begin();
    TC_RUN(dot_utf8_4);
    setlocale(LC_ALL, "C");
    return tc_end();
}
