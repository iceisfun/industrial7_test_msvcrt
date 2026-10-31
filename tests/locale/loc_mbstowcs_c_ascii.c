/* TC
 * id:    loc_mbstowcs_c_ascii
 * area:  locale
 * funcs: mbstowcs setlocale
 * procs: _mbstowcs_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.8.1; MS docs mbstowcs, _mbstowcs_l (dest NULL returns required length)
 * note:  _l variants are skipped if the CRT lacks them (msvcrt.dll).
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

typedef void *(__cdecl *create_t)(int, const char *);
typedef void (__cdecl *free_t)(void *);
typedef size_t (__cdecl *mbl_t)(wchar_t *, const char *, size_t, void *);
static create_t p_create; static free_t p_free; static mbl_t p_mbl;
static void load(void) { p_create = (create_t)tc_crt_proc("_create_locale"); p_free = (free_t)tc_crt_proc("_free_locale"); p_mbl = (mbl_t)tc_crt_proc("_mbstowcs_l"); }
static const char *volatile v_hello = "hello";

static void fill(wchar_t *w, int n) { int i; for (i = 0; i < n; i++) w[i] = 0x7777; }

TC_CASE(full)
{
    wchar_t w[16]; void *l; size_t r;
    fill(w, 16); r = mbstowcs(w, v_hello, 16);
    CHECK_EQ_U(r, 5); CHECK_WSTR(w, L"hello"); CHECK_EQ_U(w[5], 0);
    load(); if (!p_create || !p_mbl) return;
    l = p_create(LC_ALL, "C"); if (!l) { CHECK(l != NULL); return; }
    fill(w, 16); r = p_mbl(w, v_hello, 16, l);
    CHECK_EQ_U(r, 5); CHECK_WSTR(w, L"hello"); p_free(l);
}
TC_CASE(count_limit_no_nul)
{
    wchar_t w[16]; size_t r;
    fill(w, 16); r = mbstowcs(w, v_hello, 3);
    CHECK_EQ_U(r, 3); CHECK_EQ_U(w[0], 'h'); CHECK_EQ_U(w[1], 'e'); CHECK_EQ_U(w[2], 'l'); CHECK_EQ_U(w[3], 0x7777);
    fill(w, 16); r = mbstowcs(w, v_hello, 5);
    CHECK_EQ_U(r, 5); CHECK_EQ_U(w[4], 'o'); CHECK_EQ_U(w[5], 0x7777);
}
TC_CASE(empty)
{
    wchar_t w[4]; size_t r; static const char *volatile e = "";
    fill(w, 4); r = mbstowcs(w, e, 4);
    CHECK_EQ_U(r, 0); CHECK_EQ_U(w[0], 0);
}
TC_CASE(dest_null_returns_length)
{
    size_t r = mbstowcs(NULL, v_hello, 0);
    CHECK_EQ_U(r, 5);
    load();
    if (p_create && p_mbl) { void *l = p_create(LC_ALL, "C"); if (l) { CHECK_EQ_U(p_mbl(NULL, v_hello, 0, l), 5); p_free(l); } }
}

int main(void) { tc_begin(); setlocale(LC_ALL, "C"); TC_RUN(full); TC_RUN(count_limit_no_nul); TC_RUN(empty); TC_RUN(dest_null_returns_length); return tc_end(); }
