/* TC
 * id:    loc_wcstombs_c_ascii
 * area:  locale
 * funcs: wcstombs
 * procs: _wcstombs_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.8.2; MS docs wcstombs
 * note:  _l variant is gated to msvcr80+ (absent from msvcrt.dll).
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

typedef size_t (__cdecl *wcstombs_l_t)(char *, const wchar_t *, size_t, void *);
typedef void *(__cdecl *create_t)(int, const char *);
typedef void (__cdecl *free_t)(void *);
static wcstombs_l_t p_wcstombs_l; static create_t p_create; static free_t p_free;

static wchar_t w_txt[] = L"Hello, World 123", w_empty[] = L"";

TC_CASE(ascii)
{
    char b[32] = {0}; size_t n = wcstombs(b, w_txt, 32);
    CHECK_EQ_U(n, 16); CHECK_STR(b, "Hello, World 123");
}
TC_CASE(empty)
{
    char b[4] = {'Z', 'Z', 0, 0}; size_t n = wcstombs(b, w_empty, 4);
    CHECK_EQ_U(n, 0); CHECK_EQ_I(b[0], 0);
}
TC_CASE(null_dest_len)
{
    size_t n = wcstombs(NULL, w_txt, 0);
    CHECK_EQ_U(n, 16);
}
TC_CASE(l_variant)
{
    char b[32] = {0}; size_t n; void *l;
    if (!TC_AT_LEAST(80)) TC_SKIP("_wcstombs_l not in msvcrt.dll");
    CHECK(p_wcstombs_l && p_create && p_free);
    if (!p_wcstombs_l || !p_create || !p_free) return;
    l = p_create(LC_ALL, "C");
    CHECK(l != NULL);
    if (!l) return;
    n = p_wcstombs_l(b, w_txt, 32, l);
    CHECK_EQ_U(n, 16); CHECK_STR(b, "Hello, World 123");
    n = p_wcstombs_l(NULL, w_txt, 0, l);
    CHECK_EQ_U(n, 16);
    p_free(l);
}
int main(void)
{
    tc_begin();
    p_wcstombs_l = (wcstombs_l_t)tc_crt_proc("_wcstombs_l");
    p_create = (create_t)tc_crt_proc("_create_locale"); p_free = (free_t)tc_crt_proc("_free_locale");
    TC_RUN(ascii); TC_RUN(empty); TC_RUN(null_dest_len); TC_RUN(l_variant);
    return tc_end();
}
