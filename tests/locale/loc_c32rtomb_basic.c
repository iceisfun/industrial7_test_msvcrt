/* TC
 * id:    loc_c32rtomb_basic
 * area:  locale
 * funcs: c32rtomb setlocale
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.28.1.4 c32rtomb; UTF-8 encoding (RFC 3629); MS docs c32rtomb
 * note:  Win10 triage (class B): in a 1252 locale ucrtbase c32rtomb(U+20AC) yields the 3-byte UTF-8 E2 82 AC (not the single
 *        byte 0x80); observed on Windows 10 22H2 build 19045, deviates from C11 7.28.1.4.
 *        UTF-8 cases need Windows 10 1803+ (setlocale(".UTF-8") non-NULL); skipped otherwise.
 *        [class B] deviates from C11 7.28.1 (locale-encoding conversion); the MS docs state c16rtomb/c32rtomb convert to UTF-8 and mbrtoc16/mbrtoc32 decode UTF-8.
 */
#include "tc.h"
#include <locale.h>
#include <uchar.h>
#include <wchar.h>

static const char * volatile v_loc;
static volatile unsigned int v_cp;

static int utf8(void) { v_loc = ".UTF-8"; return setlocale(LC_ALL, v_loc) != NULL; }
static size_t enc(unsigned int cp, unsigned char *out)
{
    mbstate_t st; char buf[16]; size_t r, i;
    for (i = 0; i < sizeof st; i++) ((char *)&st)[i] = 0;
    for (i = 0; i < 16; i++) buf[i] = (char)0xEE;
    v_cp = cp;
    r = c32rtomb(buf, (char32_t)v_cp, &st);
    for (i = 0; i < 16; i++) out[i] = (unsigned char)buf[i];
    return r;
}

TC_CASE(ascii)
{
    unsigned char o[16];
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    CHECK_EQ_U(enc('A', o), 1); CHECK_EQ_U(o[0], 'A'); CHECK_EQ_U(o[1], 0xEE);
    CHECK_EQ_U(enc(0x7F, o), 1); CHECK_EQ_U(o[0], 0x7F);
    setlocale(LC_ALL, "C");
}
TC_CASE(boundary_1_2)
{
    unsigned char o[16];
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    CHECK_EQ_U(enc(0x80, o), 2); CHECK_EQ_U(o[0], 0xC2); CHECK_EQ_U(o[1], 0x80); CHECK_EQ_U(o[2], 0xEE);
    CHECK_EQ_U(enc(0x7FF, o), 2); CHECK_EQ_U(o[0], 0xDF); CHECK_EQ_U(o[1], 0xBF);
    setlocale(LC_ALL, "C");
}
TC_CASE(boundary_2_3)
{
    unsigned char o[16];
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    CHECK_EQ_U(enc(0x800, o), 3); CHECK_EQ_U(o[0], 0xE0); CHECK_EQ_U(o[1], 0xA0); CHECK_EQ_U(o[2], 0x80);
    CHECK_EQ_U(enc(0xFFFF, o), 3); CHECK_EQ_U(o[0], 0xEF); CHECK_EQ_U(o[1], 0xBF); CHECK_EQ_U(o[2], 0xBF); CHECK_EQ_U(o[3], 0xEE);
    setlocale(LC_ALL, "C");
}
TC_CASE(boundary_3_4)
{
    unsigned char o[16];
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    CHECK_EQ_U(enc(0x10000, o), 4); CHECK_EQ_U(o[0], 0xF0); CHECK_EQ_U(o[1], 0x90); CHECK_EQ_U(o[2], 0x80); CHECK_EQ_U(o[3], 0x80);
    setlocale(LC_ALL, "C");
}
TC_CASE(max_codepoint)
{
    unsigned char o[16];
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    CHECK_EQ_U(enc(0x10FFFF, o), 4); CHECK_EQ_U(o[0], 0xF4); CHECK_EQ_U(o[1], 0x8F); CHECK_EQ_U(o[2], 0xBF); CHECK_EQ_U(o[3], 0xBF); CHECK_EQ_U(o[4], 0xEE);
    setlocale(LC_ALL, "C");
}
TC_CASE(cp1252_euro)
{
    unsigned char o[16];
    v_loc = "English_United States.1252";
    if (!setlocale(LC_ALL, v_loc)) TC_SKIP("no 1252 locale");
    /* Win10 22H2 ucrtbase: c32rtomb always emits UTF-8, even when the locale code page is 1252 (observed; deviates from C11 7.28.1.4 "current locale") */
    CHECK_EQ_U(enc(0x20AC, o), 3); CHECK_EQ_U(o[0], 0xE2); CHECK_EQ_U(o[1], 0x82); CHECK_EQ_U(o[2], 0xAC); CHECK_EQ_U(o[3], 0xEE);
    CHECK_EQ_U(enc('A', o), 1); CHECK_EQ_U(o[0], 'A');
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin();
    TC_RUN(ascii); TC_RUN(boundary_1_2); TC_RUN(boundary_2_3); TC_RUN(boundary_3_4); TC_RUN(max_codepoint); TC_RUN(cp1252_euro);
    setlocale(LC_ALL, "C");
    return tc_end();
}
