/* TC
 * id:    dat_pctype_flags_edge
 * area:  data_exports
 * funcs:
 * procs: __pctype_func
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   ctype.h (_UPPER 1 _LOWER 2 _DIGIT 4 _SPACE 8 _PUNCT 0x10 _CONTROL 0x20 _BLANK 0x40 _HEX 0x80 _ALPHA 0x100); MS docs isalpha (valid for -1..255)
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

#define F_UPPER 1
#define F_LOWER 2
#define F_DIGIT 4
#define F_SPACE 8
#define F_PUNCT 0x10
#define F_CTRL  0x20
#define F_BLANK 0x40
#define F_HEX   0x80
#define F_ALPHA 0x100

typedef const unsigned short *(__cdecl *tabfn_t)(void);
static tabfn_t p_pctype_func;
static const unsigned short *tab(void) { return p_pctype_func(); }

TC_CASE(hex_flags)
{ const unsigned short *t = tab(); int c;
  for (c = '0'; c <= '9'; c++) CHECK(t[c] & F_HEX);
  for (c = 'a'; c <= 'f'; c++) CHECK(t[c] & F_HEX);
  for (c = 'A'; c <= 'F'; c++) CHECK(t[c] & F_HEX);
  CHECK(!(t['g'] & F_HEX)); CHECK(!(t['G'] & F_HEX)); CHECK(!(t['/'] & F_HEX)); CHECK(!(t[':'] & F_HEX)); }
TC_CASE(eof_index_minus1_is_zero)
{ const unsigned short *t = tab(); volatile int c = -1; CHECK_EQ_U(t[c], 0); }
TC_CASE(high_bytes_zero_in_c_locale)
{ const unsigned short *t = tab(); int c;
  CHECK_STR(setlocale(LC_ALL, NULL), "C");
  for (c = 128; c < 256; c++) if (t[c] & 0x1ff) { CHECK_EQ_I(c, -1); break; } }

int main(void)
{
    tc_begin();
    p_pctype_func = (tabfn_t)tc_crt_proc("__pctype_func");
    if (!p_pctype_func) { tc_write("TC FAIL dat_pctype_flags_edge::__pctype_func not exported\n"); return 1; }
    TC_RUN(hex_flags); TC_RUN(eof_index_minus1_is_zero); TC_RUN(high_bytes_zero_in_c_locale);
    return tc_end();
}
