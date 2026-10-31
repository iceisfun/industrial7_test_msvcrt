/* TC
 * id:    loc_neg_wcstombs_unmappable
 * area:  locale
 * funcs: wcstombs setlocale
 * procs: _wcstombs_l _create_locale _free_locale
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wcstombs: a wide character that cannot be converted returns (size_t)-1 and sets errno to EILSEQ
 * note:  No best-fit mapping is applied by the CRT. _l variant gated to msvcr80+.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>

typedef size_t (__cdecl *wcstombs_l_t)(char *, const wchar_t *, size_t, void *);
typedef void *(__cdecl *create_t)(int, const char *);
typedef void (__cdecl *free_t)(void *);
static wcstombs_l_t p_wcstombs_l; static create_t p_create; static free_t p_free;

static wchar_t w_cyr[] = L"\x0416", w_cjk[] = L"\x4e2d", w_mixed[] = L"a\x0416" L"b";
#define BAD ((size_t)-1)

TC_CASE(cyrillic_minus1_eilseq)
{
    char b[8] = {0}; size_t n;
    if (!setlocale(LC_ALL, "English_United States.1252")) TC_SKIP("no 1252 locale");
    errno = 0; n = wcstombs(b, w_cyr, 8);
    setlocale(LC_ALL, "C");
    CHECK_EQ_U(n, BAD); CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(cjk_minus1_eilseq)
{
    char b[8] = {0}; size_t n;
    if (!setlocale(LC_ALL, "English_United States.1252")) TC_SKIP("no 1252 locale");
    errno = 0; n = wcstombs(b, w_cjk, 8);
    setlocale(LC_ALL, "C");
    CHECK_EQ_U(n, BAD); CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(null_dest_query_also_fails)
{
    size_t n;
    if (!setlocale(LC_ALL, "English_United States.1252")) TC_SKIP("no 1252 locale");
    errno = 0; n = wcstombs(NULL, w_cyr, 0);
    setlocale(LC_ALL, "C");
    CHECK_EQ_U(n, BAD); CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(buffer_unspecified_not_checked)
{
    /* only the return value and errno are specified for a failed conversion, not the buffer contents */
    char b[8] = {0}; size_t n;
    if (!setlocale(LC_ALL, "English_United States.1252")) TC_SKIP("no 1252 locale");
    errno = 0; n = wcstombs(b, w_mixed, 8);
    setlocale(LC_ALL, "C");
    CHECK_EQ_U(n, BAD); CHECK_EQ_I(errno, EILSEQ);
    if (TC_AT_LEAST(80) && p_wcstombs_l && p_create && p_free) {
        void *l = p_create(LC_ALL, "English_United States.1252");
        if (l) {
            errno = 0; n = p_wcstombs_l(b, w_cyr, 8, l);
            CHECK_EQ_U(n, BAD); CHECK_EQ_I(errno, EILSEQ);
            p_free(l);
        }
    }
}
int main(void)
{
    tc_begin();
    p_wcstombs_l = (wcstombs_l_t)tc_crt_proc("_wcstombs_l");
    p_create = (create_t)tc_crt_proc("_create_locale"); p_free = (free_t)tc_crt_proc("_free_locale");
    TC_RUN(cyrillic_minus1_eilseq); TC_RUN(cjk_minus1_eilseq); TC_RUN(null_dest_query_also_fails);
    TC_RUN(buffer_unspecified_not_checked);
    setlocale(LC_ALL, "C");
    return tc_end();
}
