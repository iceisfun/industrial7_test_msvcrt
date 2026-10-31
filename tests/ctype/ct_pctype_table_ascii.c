/* TC
 * id:    ct_pctype_table_ascii
 * area:  ctype
 * funcs: 
 * procs: __pctype_func
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   ctype.h constants _UPPER.._HEX; MS docs isctype / ctype.h
 * note:  Bits asserted individually, never whole-word equality.
 */
#include "tc.h"
#include <ctype.h>

#define UP 0x1
#define LO 0x2
#define DI 0x4
#define SP 0x8
#define PU 0x10
#define CN 0x20
#define BL 0x40
#define HX 0x80

TC_DECL_PROC(const unsigned short *, __pctype_func, (void));
static const unsigned short *tab(void) { return tc_p___pctype_func(); }
#define T(c) (tab()[(c)])
#define ALNUM(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') || ((c) >= '0' && (c) <= '9'))

TC_CASE(non_null_pointer) { CHECK(tab() != NULL); }
TC_CASE(upper_letter_has__UPPER_bit) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I((T(c) & UP) != 0, c >= 'A' && c <= 'Z'); }
TC_CASE(lower_letter_has__LOWER_bit) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I((T(c) & LO) != 0, c >= 'a' && c <= 'z'); }
TC_CASE(digit_has__DIGIT_bit) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I((T(c) & DI) != 0, c >= '0' && c <= '9'); }
TC_CASE(space_chars_have__SPACE_bit) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I((T(c) & SP) != 0, c == ' ' || (c >= 9 && c <= 13)); }

int main(void)
{
    tc_begin(); TC_LOAD_PROC(__pctype_func); if (!tc_p___pctype_func) { tc_write("TC FAIL pctype::__pctype_func not exported\n"); return 1; }
    TC_RUN(non_null_pointer); TC_RUN(upper_letter_has__UPPER_bit); TC_RUN(lower_letter_has__LOWER_bit);
    TC_RUN(digit_has__DIGIT_bit); TC_RUN(space_chars_have__SPACE_bit); 
    
    return tc_end();
}
