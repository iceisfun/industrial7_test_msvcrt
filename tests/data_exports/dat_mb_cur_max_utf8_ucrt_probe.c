/* TC
 * id:    dat_mb_cur_max_utf8_ucrt_probe
 * area:  data_exports
 * funcs: ___mb_cur_max_func setlocale
 * crt:   >=140
 * tier:  p2
 * kind:  positive
 * conf:  observed
 * ref:   MS docs UTF-8 support (Win10 1803+ ucrt), setlocale
 * note:  Win10 22H2 ucrtbase (observed): setlocale(LC_ALL, ".65001") succeeds with MB_CUR_MAX 4; the returned name is "<default language>.utf8"
 *        (code page 65001 is canonicalised to ".utf8"; the language part depends on the user default locale and is not asserted).
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <string.h>

TC_CASE(c65001_sets_4) { if (!setlocale(LC_ALL, ".65001")) TC_SKIP("utf-8 locale unsupported"); CHECK_EQ_I(___mb_cur_max_func(), 4); setlocale(LC_ALL, "C"); }
TC_CASE(utf8_locale_name_roundtrip)
{
    const char *r; size_t n; char first[128]; const char *f;
    if (!setlocale(LC_ALL, ".65001")) TC_SKIP("utf-8 locale unsupported");
    f = setlocale(LC_ALL, ".65001"); CHECK(f != NULL);
    first[0] = 0; if (f && strlen(f) < sizeof first) strcpy(first, f);
    r = setlocale(LC_ALL, NULL); CHECK(r != NULL);
    n = r ? strlen(r) : 0;
    CHECK(n >= 5 && !strcmp(r + n - 5, ".utf8"));   /* observed: ucrt canonicalises code page 65001 to ".utf8" ("English_United States.utf8") */
    CHECK_STR(r, first);
    setlocale(LC_ALL, "C");
}
TC_CASE(restore_c_is_1)
{
    if (!setlocale(LC_ALL, ".65001")) TC_SKIP("utf-8 locale unsupported");
    CHECK_STR(setlocale(LC_ALL, "C"), "C"); CHECK_EQ_I(___mb_cur_max_func(), 1);
}
int main(void) { tc_begin(); TC_RUN(c65001_sets_4); TC_RUN(utf8_locale_name_roundtrip); TC_RUN(restore_c_is_1); setlocale(LC_ALL, "C"); return tc_end(); }
