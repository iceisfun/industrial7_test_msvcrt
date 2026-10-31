/* TC
 * id:    loc_neg_probe_mbwcs_s_dest0_cleared
 * area:  locale
 * funcs: mbstowcs_s wcstombs_s setlocale
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   none; MS docs do not clearly state destination[0] after failure (strcpy_s family clears it)
 * note:  PROBE: after mbstowcs_s / wcstombs_s fail (EINVAL for NULL source, ERANGE for too-small buffer, EILSEQ for an
 *        invalid DBCS sequence in .932), is dest[0] set to 0? Also: wcstombs_s with non-NULL buffer and size 0 gives EINVAL + handler.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>

errno_t __cdecl mbstowcs_s(size_t *, wchar_t *, size_t, const char *, size_t);
errno_t __cdecl wcstombs_s(size_t *, char *, size_t, const wchar_t *, size_t);

static char hello[] = "hello", bad[] = "a\x81 b";
static wchar_t w_hello[] = L"hello";

TC_CASE(mbs_null_src)
{
    wchar_t w[8]; size_t n = 99; int i;
    for (i = 0; i < 8; i++) w[i] = 0x5a5a;
    mbstowcs_s(&n, w, 8, NULL, 5);
    CHECK_EQ_I(w[0], 0);
}
TC_CASE(mbs_erange)
{
    wchar_t w[8]; size_t n = 99; int i;
    for (i = 0; i < 8; i++) w[i] = 0x5a5a;
    mbstowcs_s(&n, w, 3, hello, 5);
    CHECK_EQ_I(w[0], 0);
}
TC_CASE(mbs_eilseq)
{
    wchar_t w[8]; size_t n = 99; int i;
    for (i = 0; i < 8; i++) w[i] = 0x5a5a;
    if (!setlocale(LC_ALL, ".932")) TC_SKIP("no .932 locale");
    mbstowcs_s(&n, w, 8, bad, 8);
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(w[0], 0);
}
TC_CASE(wcs_null_src)
{
    char b[8]; size_t n = 99; int i;
    for (i = 0; i < 8; i++) b[i] = 'Z';
    wcstombs_s(&n, b, 8, NULL, 5);
    CHECK_EQ_I(b[0], 0);
}
TC_CASE(wcs_size0_nonnull)
{
    char b[8] = {'Z'}; size_t n = 99; errno_t r; int c0 = tc_iph_count;
    errno = 0; r = wcstombs_s(&n, b, 0, w_hello, 5);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, c0 + 1);
}
TC_CASE(wcs_erange)
{
    char b[8]; size_t n = 99; int i;
    for (i = 0; i < 8; i++) b[i] = 'Z';
    wcstombs_s(&n, b, 3, w_hello, 5);
    CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(mbs_null_src); TC_RUN(mbs_erange); TC_RUN(mbs_eilseq); TC_RUN(wcs_null_src); TC_RUN(wcs_erange); TC_RUN(wcs_size0_nonnull);
    setlocale(LC_ALL, "C");
    return tc_end();
}
