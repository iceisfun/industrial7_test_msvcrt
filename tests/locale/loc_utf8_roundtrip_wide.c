/* TC
 * id:    loc_utf8_roundtrip_wide
 * area:  locale
 * funcs: mbstowcs wcstombs setlocale
 * crt:   ucrtbase
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs UTF-8 Support (setlocale, ".UTF-8"); mbstowcs; wcstombs; C11 7.22.8
 * note:  Needs Windows 10 1803+ UTF-8 locale; skipped if setlocale returns NULL. wchar_t is UTF-16, so U+1F600
 *        becomes the pair D83D DE00.
 */
#include "tc.h"
#include <locale.h>
#include <stdlib.h>

static const char * volatile v_loc;
static const char u8[] = "a\xC3\xA9\xE2\x82\xAC\xF0\x9F\x98\x80";   /* 10 bytes */
static const wchar_t w16[] = { 0x61, 0xE9, 0x20AC, 0xD83D, 0xDE00, 0 };

static int utf8(void) { v_loc = ".UTF-8"; return setlocale(LC_ALL, v_loc) != NULL; }

TC_CASE(mbstowcs_len_and_values)
{
    wchar_t w[16]; size_t n, i;
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    for (i = 0; i < 16; i++) w[i] = 0x7777;
    n = mbstowcs(w, u8, 16);
    CHECK_EQ_U(n, 5);
    for (i = 0; i < 6; i++) CHECK_EQ_U(w[i], w16[i]);
    CHECK_EQ_U(w[6], 0x7777);
    setlocale(LC_ALL, "C");
}
TC_CASE(wcstombs_roundtrip_bytes)
{
    wchar_t w[16]; char b[32]; size_t n, i;
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    n = mbstowcs(w, u8, 16);
    CHECK_EQ_U(n, 5);
    for (i = 0; i < 32; i++) b[i] = 0x55;
    n = wcstombs(b, w, 32);
    CHECK_EQ_U(n, 10);
    CHECK_MEM(b, u8, 11);          /* includes terminating NUL */
    CHECK_EQ_I((unsigned char)b[11], 0x55);
    setlocale(LC_ALL, "C");
}
TC_CASE(length_queries_both_dirs)
{
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    CHECK_EQ_U(mbstowcs(NULL, u8, 0), 5);
    CHECK_EQ_U(wcstombs(NULL, w16, 0), 10);
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin();
    TC_RUN(mbstowcs_len_and_values); TC_RUN(wcstombs_roundtrip_bytes); TC_RUN(length_queries_both_dirs);
    setlocale(LC_ALL, "C");
    return tc_end();
}
