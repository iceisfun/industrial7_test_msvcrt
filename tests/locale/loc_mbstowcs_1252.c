/* TC
 * id:    loc_mbstowcs_1252
 * area:  locale
 * funcs: mbstowcs setlocale
 * procs: _mbstowcs_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * conf:  documented
 * ref:   Microsoft code page 1252 mapping table; MS docs mbstowcs, _mbstowcs_l
 * note:  0x81,0x8D,0x8F,0x90,0x9D are undefined in 1252 (see loc_probe_mbstowcs_1252_undefined_bytes).
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

typedef void *(__cdecl *create_t)(int, const char *);
typedef void (__cdecl *free_t)(void *);
typedef size_t (__cdecl *mbl_t)(wchar_t *, const char *, size_t, void *);
static create_t p_create; static free_t p_free; static mbl_t p_mbl;
static void load(void) { p_create = (create_t)tc_crt_proc("_create_locale"); p_free = (free_t)tc_crt_proc("_free_locale"); p_mbl = (mbl_t)tc_crt_proc("_mbstowcs_l"); }
static volatile unsigned char b_e9 = 0xE9, b_80 = 0x80, b_9f = 0x9F;

static size_t conv1(unsigned char b, wchar_t *w) { char s[2]; s[0] = (char)b; s[1] = 0; w[0] = 0x7777; return mbstowcs(w, s, 1); }

TC_CASE(e_acute) { wchar_t w[2]; if (!setlocale(LC_ALL, ".1252")) TC_SKIP("no 1252"); CHECK_EQ_U(conv1(b_e9, w), 1); CHECK_EQ_U(w[0], 0x00E9); }
TC_CASE(euro) { wchar_t w[2]; if (!setlocale(LC_ALL, ".1252")) TC_SKIP("no 1252"); CHECK_EQ_U(conv1(b_80, w), 1); CHECK_EQ_U(w[0], 0x20AC); }
TC_CASE(y_diaeresis) { wchar_t w[2]; if (!setlocale(LC_ALL, ".1252")) TC_SKIP("no 1252"); CHECK_EQ_U(conv1(b_9f, w), 1); CHECK_EQ_U(w[0], 0x0178); }
TC_CASE(string_cafe)
{
    static const char s[] = {'c', 'a', 'f', (char)0xE9, 0}; const char *volatile p = s; wchar_t w[8]; size_t r;
    if (!setlocale(LC_ALL, ".1252")) TC_SKIP("no 1252");
    r = mbstowcs(w, p, 8);
    CHECK_EQ_U(r, 4); CHECK_EQ_U(w[0], 'c'); CHECK_EQ_U(w[3], 0x00E9); CHECK_EQ_U(w[4], 0);
    CHECK_EQ_U(mbstowcs(NULL, p, 0), 4);
}
TC_CASE(l_variant_independent_of_global)
{
    static const char s[] = {(char)0x80, (char)0xE9, 0}; const char *volatile p = s; wchar_t w[4]; void *l; size_t r;
    setlocale(LC_ALL, "C");
    load(); if (!p_create || !p_mbl) TC_SKIP("no _l api");
    l = p_create(LC_ALL, "English_United States.1252"); if (!l) TC_SKIP("no 1252");
    r = p_mbl(w, p, 4, l);
    CHECK_EQ_U(r, 2); CHECK_EQ_U(w[0], 0x20AC); CHECK_EQ_U(w[1], 0x00E9);
    p_free(l);
}

int main(void)
{
    tc_begin();
    TC_RUN(e_acute); TC_RUN(euro); TC_RUN(y_diaeresis); TC_RUN(string_cafe);
    setlocale(LC_ALL, "C");
    TC_RUN(l_variant_independent_of_global);
    setlocale(LC_ALL, "C");
    return tc_end();
}
