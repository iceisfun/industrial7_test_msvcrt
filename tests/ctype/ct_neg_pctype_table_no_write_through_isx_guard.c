/* TC
 * id:    ct_neg_pctype_table_no_write_through_isx_guard
 * area:  ctype
 * kind:  negative
 * funcs: isalpha
 * procs: __pctype_func
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs isalpha (valid -1..255); C11 7.4 (EOF)
 * note:  PROBES that the ctype table has a readable EOF entry t[-1] with no class bits, that isalpha(-1/255) is 0 in
 *        the C locale, and that isalpha(-2)/isalpha(256) return 0 without modifying the table or calling the
 *        invalid-parameter handler (release ucrt does not validate).
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

typedef const unsigned short *(__cdecl *pctype_p)(void);
static pctype_p p_pctype;
static volatile int vc;

TC_CASE(table_edges)
{
    const unsigned short *t;
    if (!p_pctype) TC_SKIP("missing");
    t = p_pctype();
    CHECK(t != NULL);
    CHECK_EQ_U(t[-1] & 0x3FF, 0);
    CHECK_EQ_U(t[255] & 0x3FF, 0);
    vc = -1; CHECK_EQ_I(isalpha(vc), 0);
    vc = 255; CHECK_EQ_I(isalpha(vc), 0);
}
TC_CASE(neighbours_leave_table_unchanged)
{
    const unsigned short *t; unsigned short before[257]; int i;
    if (!p_pctype) TC_SKIP("missing");
    t = p_pctype();
    for (i = 0; i < 257; i++) before[i] = t[i - 1];
    vc = -2; CHECK_EQ_I(isalpha(vc), 0);
    vc = 256; CHECK_EQ_I(isalpha(vc), 0);
    for (i = 0; i < 257; i++) if (t[i - 1] != before[i]) { CHECK_EQ_I(i - 1, -99); break; }
}
TC_CASE(no_iph)
{
    tc_iph_count = 0;
    vc = -1; (void)isalpha(vc); vc = 255; (void)isalpha(vc);
    vc = -2; (void)isalpha(vc); vc = 256; (void)isalpha(vc);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    p_pctype = (pctype_p)tc_crt_proc("__pctype_func");
    tc_iph_install();
    TC_RUN(table_edges); TC_RUN(neighbours_leave_table_unchanged); TC_RUN(no_iph);
    return tc_end();
}
