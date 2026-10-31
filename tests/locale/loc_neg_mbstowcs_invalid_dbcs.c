/* TC
 * id:    loc_neg_mbstowcs_invalid_dbcs
 * area:  locale
 * kind:  negative
 * funcs: mbstowcs setlocale
 * procs: _mbstowcs_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * conf:  documented
 * ref:   MS docs mbstowcs: returns (size_t)-1 and sets errno to EILSEQ if an invalid multibyte character is encountered
 * note:  cp932 lead byte 0x81 followed by 0x20 is an invalid sequence. errno is only asserted for >=90 (msvcrt.dll's errno behaviour is not documented). Skipped if .932 is unavailable. Lone trailing lead byte is in loc_neg_probe_mbstowcs_lone_lead.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>

typedef void *(__cdecl *create_t)(int, const char *);
typedef void (__cdecl *free_t)(void *);
typedef size_t (__cdecl *mbl_t)(wchar_t *, const char *, size_t, void *);
static create_t p_create; static free_t p_free; static mbl_t p_mbl;
static void load(void) { p_create = (create_t)tc_crt_proc("_create_locale"); p_free = (free_t)tc_crt_proc("_free_locale"); p_mbl = (mbl_t)tc_crt_proc("_mbstowcs_l"); }
static const char s_bad[] = {'a', (char)0x81, 0x20, 'b', 0};
static const char *volatile p_bad = s_bad;
static int ok932;

TC_CASE(bad_trail_minus1_eilseq)
{
    wchar_t w[8]; size_t r;
    if (!ok932) TC_SKIP("no 932");
    errno = 0; r = mbstowcs(w, p_bad, 8);
    CHECK_EQ_U(r, (size_t)-1); if (TC_AT_LEAST(90)) CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(bad_trail_dest_null_and_l)
{
    size_t r; void *l;
    if (!ok932) TC_SKIP("no 932");
    errno = 0; r = mbstowcs(NULL, p_bad, 0);
    CHECK_EQ_U(r, (size_t)-1); if (TC_AT_LEAST(90)) CHECK_EQ_I(errno, EILSEQ);
    load(); if (!p_create || !p_mbl) return;
    l = p_create(LC_ALL, ".932"); if (!l) return;
    { wchar_t w[8]; errno = 0; r = p_mbl(w, p_bad, 8, l); CHECK_EQ_U(r, (size_t)-1); if (TC_AT_LEAST(90)) CHECK_EQ_I(errno, EILSEQ); }
    p_free(l);
}

int main(void)
{
    tc_begin(); ok932 = setlocale(LC_ALL, ".932") != NULL;
    TC_RUN(bad_trail_minus1_eilseq); TC_RUN(bad_trail_dest_null_and_l);
    setlocale(LC_ALL, "C");
    return tc_end();
}
