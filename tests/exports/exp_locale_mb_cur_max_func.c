/* TC
 * id:    exp_locale_mb_cur_max_func
 * area:  exports
 * funcs: 
 * procs: setlocale ___mb_cur_max_func
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  standard
 * ref:   C11 7.22.1 MB_CUR_MAX; C11 7.11.1.1 (C locale is single-byte)
 * note:  MB_CUR_MAX expands to ___mb_cur_max_func() in these CRT headers. The ___lc_codepage_func value is probed separately in exp_probe_lc_codepage_c_locale.
 */
#include "tc.h"

typedef int (__cdecl *mbcm_t)(void);
typedef char *(__cdecl *setlocale_t)(int, const char *);

TC_CASE(mb_cur_max_is_1)
{
    setlocale_t sl = (setlocale_t)tc_crt_proc("setlocale");
    mbcm_t f = (mbcm_t)tc_crt_proc("___mb_cur_max_func");
    CHECK(sl != NULL); CHECK(f != NULL);
    if (!sl || !f) return;
    CHECK(sl(0 /* LC_ALL */, "C") != NULL);
    CHECK_EQ_I(f(), 1);
}
int main(void) { tc_begin(); TC_RUN(mb_cur_max_is_1); return tc_end(); }
