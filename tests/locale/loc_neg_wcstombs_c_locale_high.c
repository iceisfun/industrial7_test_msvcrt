/* TC
 * id:    loc_neg_wcstombs_c_locale_high
 * area:  locale
 * funcs: wcstombs
 * procs: _wcstombs_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs wcstombs; C locale narrowing only handles code units up to 0xFF
 * note:  Only U+0100 and U+4E2D are asserted; U+00E9..U+00FF in the C locale is the separate probe loc_probe_wcstombs_c_latin1.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>

typedef size_t (__cdecl *wcstombs_l_t)(char *, const wchar_t *, size_t, void *);
typedef void *(__cdecl *create_t)(int, const char *);
typedef void (__cdecl *free_t)(void *);
static wcstombs_l_t p_wcstombs_l; static create_t p_create; static free_t p_free;

static wchar_t w_100[] = L"\x0100", w_cjk[] = L"\x4e2d";
#define BAD ((size_t)-1)

TC_CASE(above_ff_fails)
{
    char b[8] = {0}; size_t n;
    errno = 0; n = wcstombs(b, w_100, 8); CHECK_EQ_U(n, BAD);
    errno = 0; n = wcstombs(b, w_cjk, 8); CHECK_EQ_U(n, BAD);
}
TC_CASE(errno_eilseq)
{
    char b[8] = {0}; size_t n;
    errno = 0; n = wcstombs(b, w_100, 8);
    CHECK_EQ_U(n, BAD); CHECK_EQ_I(errno, EILSEQ);
    errno = 0; n = wcstombs(b, w_cjk, 8);
    CHECK_EQ_U(n, BAD); CHECK_EQ_I(errno, EILSEQ);
    if (TC_AT_LEAST(80) && p_wcstombs_l && p_create && p_free) {
        void *l = p_create(LC_ALL, "C");
        if (l) { errno = 0; n = p_wcstombs_l(b, w_100, 8, l); CHECK_EQ_U(n, BAD); CHECK_EQ_I(errno, EILSEQ); p_free(l); }
    }
}
int main(void)
{
    tc_begin();
    p_wcstombs_l = (wcstombs_l_t)tc_crt_proc("_wcstombs_l");
    p_create = (create_t)tc_crt_proc("_create_locale"); p_free = (free_t)tc_crt_proc("_free_locale");
    TC_RUN(above_ff_fails); TC_RUN(errno_eilseq);
    return tc_end();
}
