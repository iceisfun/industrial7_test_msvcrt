/* TC
 * id:    loc_c16rtomb_basic
 * area:  locale
 * funcs: c16rtomb setlocale
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.28.1.3 c16rtomb (high surrogate stored in state, returns 0); MS docs c16rtomb
 * note:  UTF-8 cases skipped when setlocale(LC_ALL, ".UTF-8") fails.
 */
#include "tc.h"
#include <uchar.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>

static int utf8(void) { volatile const char *n = ".UTF-8"; return setlocale(LC_ALL, (const char *)n) != NULL; }
#define SKIPNO() do { if (!utf8()) { setlocale(LC_ALL, "C"); TC_SKIP("no UTF-8"); } } while (0)

TC_CASE(ascii) { mbstate_t st; char b[16]; volatile char16_t c = u'A'; memset(&st, 0, sizeof st); memset(b, 0x7f, sizeof b); CHECK_EQ_U(c16rtomb(b, c, &st), 1); CHECK_EQ_I(b[0], 'A'); }
TC_CASE(nul) { mbstate_t st; char b[16]; volatile char16_t c = 0; memset(&st, 0, sizeof st); memset(b, 0x7f, sizeof b); CHECK_EQ_U(c16rtomb(b, c, &st), 1); CHECK_EQ_I(b[0], 0); }
TC_CASE(two_byte_utf8) { mbstate_t st; char b[16]; volatile char16_t c = 0xE9; SKIPNO(); memset(&st, 0, sizeof st);
    CHECK_EQ_U(c16rtomb(b, c, &st), 2); CHECK_EQ_I((unsigned char)b[0], 0xC3); CHECK_EQ_I((unsigned char)b[1], 0xA9); setlocale(LC_ALL, "C"); }
TC_CASE(three_byte_utf8) { mbstate_t st; char b[16]; volatile char16_t c = 0x20AC; SKIPNO(); memset(&st, 0, sizeof st);
    CHECK_EQ_U(c16rtomb(b, c, &st), 3); CHECK_EQ_I((unsigned char)b[0], 0xE2); CHECK_EQ_I((unsigned char)b[1], 0x82); CHECK_EQ_I((unsigned char)b[2], 0xAC); setlocale(LC_ALL, "C"); }
TC_CASE(surrogate_pair_zero_then_four) { mbstate_t st; char b[16]; volatile char16_t hi = 0xD83D, lo = 0xDE00; SKIPNO(); memset(&st, 0, sizeof st);
    CHECK_EQ_U(c16rtomb(b, hi, &st), 0);
    CHECK_EQ_U(c16rtomb(b, lo, &st), 4);
    CHECK_EQ_I((unsigned char)b[0], 0xF0); CHECK_EQ_I((unsigned char)b[1], 0x9F); CHECK_EQ_I((unsigned char)b[2], 0x98); CHECK_EQ_I((unsigned char)b[3], 0x80);
    setlocale(LC_ALL, "C"); }
TC_CASE(s_null_resets_state) { mbstate_t st; volatile char16_t c = 0; memset(&st, 0, sizeof st); CHECK_EQ_U(c16rtomb(NULL, c, &st), 1); }
int main(void) { tc_begin(); TC_RUN(ascii); TC_RUN(nul); TC_RUN(two_byte_utf8); TC_RUN(three_byte_utf8); TC_RUN(surrogate_pair_zero_then_four); TC_RUN(s_null_resets_state); return tc_end(); }
