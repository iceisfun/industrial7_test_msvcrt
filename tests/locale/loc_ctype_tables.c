/* TC
 * id:    loc_ctype_tables
 * area:  locale
 * funcs: setlocale
 * procs: __pctype_func __pwctype_func
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS ctype.h (_UPPER 0x1 _LOWER 0x2 _DIGIT 0x4 _SPACE 0x8 _HEX 0x80; _ALPHA is 0x103 = 0x100|_UPPER|_LOWER); MS docs isalpha (table macros)
 * note:  The narrow table is indexed -1..255; entry -1 (EOF) is zero. Wide table 'A' has _UPPER.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

#define B_UPPER 0x1
#define B_LOWER 0x2
#define B_DIGIT 0x4
#define B_SPACE 0x8
#define B_HEX   0x80

#undef __pctype_func
#undef __pwctype_func
typedef const unsigned short *(__cdecl *tbl_t)(void);
static const unsigned short *tbl(const char *n) { tbl_t f = (tbl_t)tc_crt_proc(n); return f ? f() : NULL; }
#define PCT() tbl("__pctype_func")
#define PWT() tbl("__pwctype_func")

TC_CASE(A_bits) { const unsigned short *t = PCT(); if (!t) { CHECK(t != NULL); return; } CHECK((t['A'] & B_UPPER) != 0); CHECK((t['A'] & B_HEX) != 0); CHECK((t['A'] & B_LOWER) == 0); }
TC_CASE(digit_bits) { const unsigned short *t = PCT(); if (!t) { CHECK(t != NULL); return; } CHECK((t['7'] & B_DIGIT) != 0); CHECK((t['7'] & B_HEX) != 0); }
TC_CASE(space_bits) { const unsigned short *t = PCT(); if (!t) { CHECK(t != NULL); return; } CHECK((t[' '] & B_SPACE) != 0); CHECK((t[' '] & (B_UPPER | B_LOWER | B_DIGIT)) == 0); }
TC_CASE(table_minus1_zero) { const unsigned short *t = PCT(); if (!t) { CHECK(t != NULL); return; } CHECK_EQ_U(t[-1], 0); }
TC_CASE(wide_table_A) { const unsigned short *w = PWT(); if (!w) { CHECK(w != NULL); return; } CHECK((w['A'] & B_UPPER) != 0); CHECK((w['a'] & B_LOWER) != 0); CHECK((w['5'] & B_DIGIT) != 0); }

int main(void)
{
    volatile int c = 'A';
    tc_begin(); setlocale(LC_ALL, "C"); (void)c;
    TC_RUN(A_bits); TC_RUN(digit_bits); TC_RUN(space_bits); TC_RUN(table_minus1_zero); TC_RUN(wide_table_A);
    return tc_end();
}
