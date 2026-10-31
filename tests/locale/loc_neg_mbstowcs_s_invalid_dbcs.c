/* TC
 * id:    loc_neg_mbstowcs_s_invalid_dbcs
 * area:  locale
 * funcs: mbstowcs_s setlocale
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs mbstowcs_s (EILSEQ for invalid multibyte sequence), code page 932 (Shift-JIS)
 * note:  Skips if the .932 locale is unavailable. Whether the invalid-parameter handler runs for EILSEQ is NOT asserted.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>

errno_t __cdecl mbstowcs_s(size_t *, wchar_t *, size_t, const char *, size_t);

static char bad[] = "a\x81 b";   /* 0x81 is a lead byte in 932; 0x20 is not a valid trail byte */

TC_CASE(bad_trail_eilseq)
{
    wchar_t w[8]; size_t n = 99; errno_t r;
    if (!setlocale(LC_ALL, ".932")) TC_SKIP("no .932 locale");
    r = mbstowcs_s(&n, w, 8, bad, 8);
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(r, EILSEQ); CHECK_EQ_I(r, 42);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(bad_trail_eilseq);
    setlocale(LC_ALL, "C");
    return tc_end();
}
