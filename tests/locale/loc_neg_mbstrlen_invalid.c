/* TC
 * id:    loc_neg_mbstrlen_invalid
 * area:  locale
 * funcs: _mbstrlen setlocale
 * procs: _mbstrlen_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mbstrlen, _mbstrlen_l: returns -1 if an invalid multibyte character is encountered, errno set to EILSEQ
 * note:  Code page 932 (Shift-JIS): 0x82 is a lead byte, trail bytes are 0x40-0x7E / 0x80-0xFC, so 0x20 is not a valid trail byte.
 *        msvcrt.dll excluded: its errno behaviour for this case is not documented.
 *        Assumes msvcr90-120 use the MultiByteToWideChar(MB_ERR_INVALID_CHARS) validation like ucrtbase (docs say -1/EILSEQ for all); if real Windows disagrees on an old CRT, narrow crt:.
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
#include <mbctype.h>

typedef size_t (__cdecl *mbstrlen_l_t)(const char *, void *);
typedef void *(__cdecl *create_locale_t)(int, const char *);
typedef void (__cdecl *free_locale_t)(void *);
static mbstrlen_l_t p_mbstrlen_l;
static create_locale_t p_create;
static free_locale_t p_free;

static const char * volatile v_s;
static int enter932(void)
{
    volatile const char *n = ".932";
    if (!setlocale(LC_ALL, (const char *)n)) return 0;
    _setmbcp(932);
    return 1;
}
static void leave(void) { setlocale(LC_ALL, "C"); _setmbcp(_MB_CP_ANSI); }

TC_CASE(trailing_lead)
{
    if (!enter932()) { leave(); TC_SKIP("932 unavailable"); }
    v_s = "\x82";
    CHECK_EQ_U(_mbstrlen(v_s), (size_t)-1);
    leave();
}
TC_CASE(bad_trail)
{
    if (!enter932()) { leave(); TC_SKIP("932 unavailable"); }
    v_s = "\x82\x20";
    CHECK_EQ_U(_mbstrlen(v_s), (size_t)-1);
    leave();
}
TC_CASE(errno_eilseq)
{
    if (!enter932()) { leave(); TC_SKIP("932 unavailable"); }
    v_s = "\x82";
    errno = 0;
    CHECK_EQ_U(_mbstrlen(v_s), (size_t)-1);
    CHECK_EQ_I(errno, EILSEQ);
    errno = 0;
    v_s = "ab\x82\x20";
    CHECK_EQ_U(_mbstrlen(v_s), (size_t)-1);
    CHECK_EQ_I(errno, EILSEQ);
    leave();
}
TC_CASE(l_variant)
{
    void *loc;
    if (!p_mbstrlen_l || !p_create || !p_free) TC_SKIP("_l functions missing");
    loc = p_create(LC_ALL, ".932");
    if (!loc) TC_SKIP("932 locale unavailable");
    errno = 0;
    v_s = "\x82";
    CHECK_EQ_U(p_mbstrlen_l(v_s, loc), (size_t)-1);
    CHECK_EQ_I(errno, EILSEQ);
    v_s = "\x82\x20";
    CHECK_EQ_U(p_mbstrlen_l(v_s, loc), (size_t)-1);
    v_s = "\x82\xa0" "a";   /* valid: one double-byte char + one single */
    CHECK_EQ_U(p_mbstrlen_l(v_s, loc), 2);
    p_free(loc);
}
int main(void)
{
    tc_begin();
    p_mbstrlen_l = (mbstrlen_l_t)tc_crt_proc("_mbstrlen_l");
    p_create = (create_locale_t)tc_crt_proc("_create_locale");
    p_free = (free_locale_t)tc_crt_proc("_free_locale");
    TC_RUN(trailing_lead); TC_RUN(bad_trail); TC_RUN(errno_eilseq); TC_RUN(l_variant);
    return tc_end();
}
