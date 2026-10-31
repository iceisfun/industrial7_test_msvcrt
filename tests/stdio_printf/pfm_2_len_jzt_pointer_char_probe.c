/* TC
 * id:    pfm_2_len_jzt_pointer_char_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=140
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.6.1 (j z t with p c s f: undefined); MS docs printf Size prefixes
 * note:  PROBE: ucrtbase parses j/z/t as size modifiers and ignores them for %p %c %s %f: output equals the unmodified conversion and the
 *        invalid-parameter handler is not called.
 */
#include "tc.h"
#include <stdio.h>

static char b[64], p[64];
TC_CASE(zp) { static int obj; void * volatile q = &obj; tc_iph_count = 0; sprintf(b, "%zp", q); sprintf(p, "%p", q); CHECK_STR(b, p); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(jc) { volatile int c = 'x'; tc_iph_count = 0; sprintf(b, "%jc", c); CHECK_STR(b, "x"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(ts) { const char * volatile s = "ab"; tc_iph_count = 0; sprintf(b, "%ts", s); CHECK_STR(b, "ab"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(zf) { volatile double d = 1.5; tc_iph_count = 0; sprintf(b, "%zf", d); CHECK_STR(b, "1.500000"); CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(zp); TC_RUN(jc); TC_RUN(ts); TC_RUN(zf);
    return tc_end();
}
