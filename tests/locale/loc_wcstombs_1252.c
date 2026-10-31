/* TC
 * id:    loc_wcstombs_1252
 * area:  locale
 * funcs: wcstombs setlocale
 * procs: _wcstombs_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs wcstombs, _wcstombs_l; code page 1252
 * note:  Skips (not fails) if the 1252 locale cannot be selected. _wcstombs_l / _create_locale are not in msvcrt.dll, so the _l check is gated to msvcr80+.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>

typedef size_t (__cdecl *wcstombs_l_t)(char *, const wchar_t *, size_t, void *);
typedef void *(__cdecl *create_t)(int, const char *);
typedef void (__cdecl *free_t)(void *);
static wcstombs_l_t p_wcstombs_l; static create_t p_create; static free_t p_free;

static wchar_t w_e[] = L"\x00e9", w_eur[] = L"\x20ac", w_abc[] = L"abc", w_abcdef[] = L"abcdef", w_a[] = L"a";

static int in1252(void) { return setlocale(LC_ALL, "English_United States.1252") != NULL; }

TC_CASE(e_acute)
{
    unsigned char b[8] = {0}; size_t n;
    if (!in1252()) TC_SKIP("no 1252 locale");
    n = wcstombs((char *)b, w_e, 8);
    setlocale(LC_ALL, "C");
    CHECK_EQ_U(n, 1); CHECK_EQ_U(b[0], 0xE9); CHECK_EQ_U(b[1], 0);
}
TC_CASE(euro)
{
    unsigned char b[8] = {0}; size_t n;
    if (!in1252()) TC_SKIP("no 1252 locale");
    n = wcstombs((char *)b, w_eur, 8);
    setlocale(LC_ALL, "C");
    CHECK_EQ_U(n, 1); CHECK_EQ_U(b[0], 0x80);
}
TC_CASE(ascii)
{
    char b[8] = {0}; size_t n;
    if (!in1252()) TC_SKIP("no 1252 locale");
    n = wcstombs(b, w_abc, 8);
    setlocale(LC_ALL, "C");
    CHECK_EQ_U(n, 3); CHECK_STR(b, "abc");
}
TC_CASE(dest_null_length)
{
    size_t n, m;
    if (!in1252()) TC_SKIP("no 1252 locale");
    n = wcstombs(NULL, w_abcdef, 0);
    m = wcstombs(NULL, w_e, 0);
    setlocale(LC_ALL, "C");
    CHECK_EQ_U(n, 6); CHECK_EQ_U(m, 1);
}
TC_CASE(count_limit_no_nul)
{
    char b[8]; size_t n; int i;
    for (i = 0; i < 8; i++) b[i] = 'Z';
    if (!in1252()) TC_SKIP("no 1252 locale");
    n = wcstombs(b, w_abcdef, 3);
    setlocale(LC_ALL, "C");
    CHECK_EQ_U(n, 3); CHECK_MEM(b, "abcZ", 4);   /* no terminator appended when count is reached */
}
TC_CASE(count_zero)
{
    char b[4] = {'Z', 'Z', 'Z', 'Z'}; size_t n;
    if (!in1252()) TC_SKIP("no 1252 locale");
    n = wcstombs(b, w_a, 0);
    setlocale(LC_ALL, "C");
    CHECK_EQ_U(n, 0); CHECK_EQ_I(b[0], 'Z');
}
TC_CASE(l_variant)
{
    unsigned char b[8] = {0}; size_t n; void *l;
    if (!TC_AT_LEAST(80)) TC_SKIP("_wcstombs_l not in msvcrt.dll");
    CHECK(p_wcstombs_l && p_create && p_free);
    if (!p_wcstombs_l || !p_create || !p_free) return;
    l = p_create(LC_ALL, "English_United States.1252");
    if (!l) TC_SKIP("no 1252 locale");
    n = p_wcstombs_l((char *)b, w_eur, 8, l);
    CHECK_EQ_U(n, 1); CHECK_EQ_U(b[0], 0x80);
    n = p_wcstombs_l((char *)b, w_e, 8, l);
    CHECK_EQ_U(n, 1); CHECK_EQ_U(b[0], 0xE9);
    p_free(l);
}
int main(void)
{
    tc_begin();
    p_wcstombs_l = (wcstombs_l_t)tc_crt_proc("_wcstombs_l");
    p_create = (create_t)tc_crt_proc("_create_locale"); p_free = (free_t)tc_crt_proc("_free_locale");
    TC_RUN(e_acute); TC_RUN(euro); TC_RUN(ascii); TC_RUN(dest_null_length);
    TC_RUN(count_limit_no_nul); TC_RUN(count_zero); TC_RUN(l_variant);
    setlocale(LC_ALL, "C");
    return tc_end();
}
