/* TC
 * id:    ct_pctype_table_ascii_more
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

TC_CASE(punct_has__PUNCT_bit) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I((T(c) & PU) != 0, c > 32 && c < 127 && !ALNUM(c)); }
TC_CASE(control_has__CONTROL_bit) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I((T(c) & CN) != 0, c < 32 || c == 127); }
TC_CASE(hex_digits_have__HEX_bit) { int c; for (c = 0; c < 128; c++) CHECK_EQ_I((T(c) & HX) != 0, (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')); }
TC_CASE(blank_bit_on_space_char_0x20) { CHECK((T(0x20) & BL) != 0); }

int main(void)
{
    tc_begin(); TC_LOAD_PROC(__pctype_func); if (!tc_p___pctype_func) { tc_write("TC FAIL pctype::__pctype_func not exported\n"); return 1; }
    
    TC_RUN(punct_has__PUNCT_bit);
    TC_RUN(control_has__CONTROL_bit); TC_RUN(hex_digits_have__HEX_bit); TC_RUN(blank_bit_on_space_char_0x20);
    return tc_end();
}
