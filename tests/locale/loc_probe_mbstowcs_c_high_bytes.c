/* TC
 * id:    loc_probe_mbstowcs_c_high_bytes
 * area:  locale
 * funcs: mbstowcs setlocale
 * procs: _mbstowcs_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs mbstowcs (C locale)
 * note:  PROBE: in the C locale mbstowcs widens bytes 0x80-0xFF to the same code point (no failure). May differ on some CRTs (EILSEQ).
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

typedef void *(__cdecl *create_t)(int, const char *);
typedef void (__cdecl *free_t)(void *);
typedef size_t (__cdecl *mbl_t)(wchar_t *, const char *, size_t, void *);
static create_t p_create; static free_t p_free; static mbl_t p_mbl;
static void load(void) { p_create = (create_t)tc_crt_proc("_create_locale"); p_free = (free_t)tc_crt_proc("_free_locale"); p_mbl = (mbl_t)tc_crt_proc("_mbstowcs_l"); }
static volatile unsigned char b_e9 = 0xE9, b_ff = 0xFF;
static size_t conv1(unsigned char b, wchar_t *w) { char s[2]; s[0] = (char)b; s[1] = 0; w[0] = 0x7777; return mbstowcs(w, s, 1); }

TC_CASE(c_widens_0xE9) { wchar_t w[2]; setlocale(LC_ALL, "C"); CHECK_EQ_U(conv1(b_e9, w), 1); CHECK_EQ_U(w[0], 0xE9); }
TC_CASE(c_widens_0xFF) { wchar_t w[2]; setlocale(LC_ALL, "C"); CHECK_EQ_U(conv1(b_ff, w), 1); CHECK_EQ_U(w[0], 0xFF); }
TC_CASE(via_locale_object)
{
    char s[2]; wchar_t w[2]; void *l; size_t r;
    load(); if (!p_create || !p_mbl) TC_SKIP("no _l api");
    l = p_create(LC_ALL, "C"); if (!l) TC_SKIP("no C locale object");
    s[0] = (char)b_e9; s[1] = 0; w[0] = 0x7777;
    r = p_mbl(w, s, 1, l);
    CHECK_EQ_U(r, 1); CHECK_EQ_U(w[0], 0xE9); p_free(l);
}

int main(void) { tc_begin(); TC_RUN(c_widens_0xE9); TC_RUN(c_widens_0xFF); TC_RUN(via_locale_object); setlocale(LC_ALL, "C"); return tc_end(); }
