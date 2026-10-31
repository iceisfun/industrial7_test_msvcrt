/* TC
 * id:    loc_neg_probe_mbstowcs_lone_lead
 * area:  locale
 * kind:  negative
 * funcs: mbstowcs setlocale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs mbstowcs
 * note:  PROBE: (a) a trailing lone cp932 lead byte 0x81 yields (size_t)-1/EILSEQ (observed); (b) a successful
 *        mbstowcs does not modify errno (C11 7.5: library never sets errno to zero).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>

static const char s_lone[] = {'a', (char)0x81, 0};
static const char *volatile p_lone = s_lone, *volatile p_ok = "abc";
static int ok932;

TC_CASE(lone_lead_at_end)
{
    wchar_t w[8]; size_t r;
    if (!ok932) TC_SKIP("no 932");
    errno = 0; r = mbstowcs(w, p_lone, 8);
    CHECK_EQ_U(r, (size_t)-1); CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(errno_reset_not_touched_on_success)
{
    wchar_t w[8]; size_t r;
    if (!ok932) TC_SKIP("no 932");
    errno = ERANGE; r = mbstowcs(w, p_ok, 8);
    CHECK_EQ_U(r, 3); CHECK_EQ_I(errno, ERANGE);
}

int main(void)
{
    tc_begin(); ok932 = setlocale(LC_ALL, ".932") != NULL;
    TC_RUN(lone_lead_at_end); TC_RUN(errno_reset_not_touched_on_success);
    setlocale(LC_ALL, "C");
    return tc_end();
}
