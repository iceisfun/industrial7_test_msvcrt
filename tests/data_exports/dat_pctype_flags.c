/* TC
 * id:    dat_pctype_flags
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

TC_CASE(upper_A_flags)
{ const unsigned short *t = tab(); volatile int c = 'A';
  CHECK(t[c] & F_UPPER); CHECK(t[c] & F_HEX); CHECK(!(t[c] & F_LOWER)); CHECK(!(t[c] & F_DIGIT)); CHECK(!(t[c] & F_SPACE)); CHECK(!(t[c] & F_PUNCT));
  c = 'Z'; CHECK(t[c] & F_UPPER); CHECK(!(t[c] & F_HEX)); }
TC_CASE(lower_a_flags)
{ const unsigned short *t = tab(); volatile int c = 'a';
  CHECK(t[c] & F_LOWER); CHECK(t[c] & F_HEX); CHECK(!(t[c] & F_UPPER)); CHECK(!(t[c] & F_DIGIT));
  c = 'z'; CHECK(t[c] & F_LOWER); CHECK(!(t[c] & F_HEX)); }
TC_CASE(digit_5_flags)
{ const unsigned short *t = tab(); volatile int c = '5';
  CHECK(t[c] & F_DIGIT); CHECK(t[c] & F_HEX); CHECK(!(t[c] & (F_UPPER|F_LOWER|F_SPACE|F_PUNCT|F_CTRL))); }
TC_CASE(space_flags)
{ const unsigned short *t = tab(); volatile int c = ' ';
  CHECK(t[c] & F_SPACE); CHECK(t[c] & F_BLANK); CHECK(!(t[c] & (F_UPPER|F_LOWER|F_DIGIT|F_PUNCT)));
  c = '\n'; CHECK(t[c] & F_SPACE); CHECK(t[c] & F_CTRL); c = '\t'; CHECK(t[c] & F_SPACE); CHECK(t[c] & F_CTRL); }
TC_CASE(punct_flags)
{ const unsigned short *t = tab(); volatile int c = '!';
  CHECK(t[c] & F_PUNCT); CHECK(!(t[c] & (F_UPPER|F_LOWER|F_DIGIT|F_SPACE|F_CTRL|F_HEX)));
  c = '~'; CHECK(t[c] & F_PUNCT); c = '/'; CHECK(t[c] & F_PUNCT); }
TC_CASE(control_flags)
{ const unsigned short *t = tab(); volatile int c = 1;
  CHECK(t[c] & F_CTRL); CHECK(!(t[c] & (F_UPPER|F_LOWER|F_DIGIT|F_PUNCT|F_SPACE)));
  c = 0x7f; CHECK(t[c] & F_CTRL); c = 0; CHECK(t[c] & F_CTRL); }
int main(void)
{
    tc_begin();
    p_pctype_func = (tabfn_t)tc_crt_proc("__pctype_func");
    if (!p_pctype_func) { tc_write("TC FAIL dat_pctype_flags::__pctype_func not exported\n"); return 1; }
    TC_RUN(upper_A_flags); TC_RUN(lower_a_flags); TC_RUN(digit_5_flags); TC_RUN(space_flags); TC_RUN(punct_flags);
    TC_RUN(control_flags);
    return tc_end();
}
