/* TC
 * id:    loc_probe_mb_cur_max_utf8_names
 * area:  locale
 * funcs: ___mb_cur_max_func setlocale
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs "UTF-8 Support" (Windows 10 1803+)
 * note:  PROBE: the spellings ".65001" and "en-US.UTF-8" are accepted by setlocale on a UTF-8 capable ucrtbase and give
 *        MB_CUR_MAX == 4. Cases skip if setlocale returns NULL (older Windows 10 builds).
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static const char *volatile n_dot_65001 = ".65001";
static const char *volatile n_en_utf8 = "en-US.UTF-8";
static const char *volatile n_c = "C";

TC_CASE(dot_65001_4)
{
    if (!setlocale(LC_ALL, n_dot_65001)) { setlocale(LC_ALL, n_c); TC_SKIP("UTF-8 locale unsupported"); }
    CHECK_EQ_I(___mb_cur_max_func(), 4);
    setlocale(LC_ALL, n_c);
}
TC_CASE(en_US_utf8_4)
{
    if (!setlocale(LC_ALL, n_en_utf8)) { setlocale(LC_ALL, n_c); TC_SKIP("UTF-8 locale unsupported"); }
    CHECK_EQ_I(___mb_cur_max_func(), 4);
    setlocale(LC_ALL, n_c);
}
int main(void)
{
    tc_begin();
    TC_RUN(dot_65001_4); TC_RUN(en_US_utf8_4);
    setlocale(LC_ALL, "C");
    return tc_end();
}
