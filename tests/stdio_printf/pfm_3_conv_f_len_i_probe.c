/* TC
 * id:    pfm_3_conv_f_len_i_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf size prefixes I, I32, I64
 * note:  PROBING: %If %I32f %I64f ignore the I modifiers for f and format a double as plain %f, consuming one double.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static char b[128];
static volatile double d = 1.5;
#define BODY_T(f) b[0] = 0; tc_iph_install(); sprintf(b, f "|%d", d, 7); CHECK_STR(b, "1.500000|7");
TC_CASE(If) { BODY_T("%If") }
TC_CASE(I32f) { BODY_T("%I32f") }
TC_CASE(I64f) { BODY_T("%I64f") }
int main(void) { tc_begin(); TC_RUN(If); TC_RUN(I32f); TC_RUN(I64f); return tc_end(); }
