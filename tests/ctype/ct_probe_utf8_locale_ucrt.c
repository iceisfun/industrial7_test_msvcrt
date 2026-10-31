/* TC
 * id:    ct_probe_utf8_locale_ucrt
 * area:  ctype
 * funcs: setlocale ___mb_cur_max_func toupper isalpha
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs setlocale (UTF-8 code page support, Windows 10 1803+)
 * note:  PROBE: setlocale(LC_ALL, ".UTF-8") availability and its effect on MB_CUR_MAX (4) and ASCII ctype. Cases SKIP when setlocale returns NULL (older Windows 10).
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>
#include <stdlib.h>
int __cdecl ___mb_cur_max_func(void);

static volatile int vc;
static int utf8_ok;

TC_CASE(setlocale_dot_UTF_8_non_null)
{
    const char *r = setlocale(LC_ALL, ".UTF-8");
    utf8_ok = r != NULL;
    if (!r) TC_SKIP("UTF-8 locale unsupported");
    CHECK(r != NULL);
}
TC_CASE(mb_cur_max_is_4)
{
    if (!utf8_ok) TC_SKIP("UTF-8 locale unsupported");
    CHECK_EQ_I(___mb_cur_max_func(), 4);
}
TC_CASE(ascii_toupper_still_works)
{
    if (!utf8_ok) TC_SKIP("UTF-8 locale unsupported");
    vc = 'a'; CHECK_EQ_I(toupper(vc), 'A');
    vc = 'Z'; CHECK_EQ_I(toupper(vc), 'Z');
    vc = '5'; CHECK_EQ_I(toupper(vc), '5');
}
TC_CASE(isalpha_a_true)
{
    if (!utf8_ok) TC_SKIP("UTF-8 locale unsupported");
    vc = 'a'; CHECK(isalpha(vc) != 0);
    vc = '1'; CHECK(isalpha(vc) == 0);
}
TC_CASE(restore_C_locale)
{
    const char *r = setlocale(LC_ALL, "C");
    CHECK(r != NULL);
    CHECK_STR(r, "C");
    CHECK_EQ_I(___mb_cur_max_func(), 1);
}

int main(void) { tc_begin(); TC_RUN(setlocale_dot_UTF_8_non_null); TC_RUN(mb_cur_max_is_4); TC_RUN(ascii_toupper_still_works); TC_RUN(isalpha_a_true); TC_RUN(restore_C_locale); return tc_end(); }
