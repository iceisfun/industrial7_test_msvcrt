/* TC
 * id:    dat_mb_cur_max_utf8_ucrt_probe
 * area:  data_exports
 * funcs: ___mb_cur_max_func setlocale
 * crt:   >=140
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs UTF-8 support (Win10 1803+ ucrt), setlocale
 * note:  Probe: setlocale(LC_ALL, ".65001") succeeds with MB_CUR_MAX 4; the returned locale name ends with ".65001". Skipped if setlocale returns NULL.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <string.h>

TC_CASE(c65001_sets_4) { if (!setlocale(LC_ALL, ".65001")) TC_SKIP("utf-8 locale unsupported"); CHECK_EQ_I(___mb_cur_max_func(), 4); setlocale(LC_ALL, "C"); }
TC_CASE(utf8_locale_name_roundtrip)
{
    const char *r; size_t n;
    if (!setlocale(LC_ALL, ".65001")) TC_SKIP("utf-8 locale unsupported");
    r = setlocale(LC_ALL, NULL); CHECK(r != NULL);
    n = r ? strlen(r) : 0;
    CHECK(n >= 6 && !strcmp(r + n - 6, ".65001"));
    setlocale(LC_ALL, "C");
}
TC_CASE(restore_c_is_1)
{
    if (!setlocale(LC_ALL, ".65001")) TC_SKIP("utf-8 locale unsupported");
    CHECK_STR(setlocale(LC_ALL, "C"), "C"); CHECK_EQ_I(___mb_cur_max_func(), 1);
}
int main(void) { tc_begin(); TC_RUN(c65001_sets_4); TC_RUN(utf8_locale_name_roundtrip); TC_RUN(restore_c_is_1); setlocale(LC_ALL, "C"); return tc_end(); }
