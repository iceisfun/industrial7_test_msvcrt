/* TC
 * id:    dir_neg_makepath_s_state_probe
 * area:  dir
 * kind:  negative
 * funcs: _makepath_s _wmakepath_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _makepath_s (ERANGE, buffer[0] reset); drive: "letter, optionally followed by a colon"
 * note:  Probes (a) b[0]==0 after ERANGE and that bytes beyond the stated size are not touched,
 *        (b) that all-NULL components with size 1 succeed with an empty string,
 *        (c) that a multi-character drive component contributes only its first character plus a colon ("AB" -> "A:").
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile v_n;
static const wchar_t *volatile v_wn;

TC_CASE(erange_state)
{
    unsigned char b[16]; int r, i; v_n = "name";
    memset(b, 0xCC, sizeof b); tc_iph_count = 0;
    r = _makepath_s((char *)b, 13, "C:", "\\d\\", v_n, ".ext");   /* needs 14 */
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
    for (i = 13; i < 16; i++) CHECK_EQ_I(b[i], 0xCC);
}
TC_CASE(erange_state_wide)
{
    wchar_t b[16]; int r, i; v_wn = L"name";
    for (i = 0; i < 16; i++) b[i] = 0xCCCC;
    tc_iph_count = 0;
    r = _wmakepath_s(b, 13, L"C:", L"\\d\\", v_wn, L".ext");
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
    for (i = 13; i < 16; i++) CHECK_EQ_I(b[i], 0xCCCC);
}
TC_CASE(all_null_size1)
{
    char b[4]; wchar_t w[4]; b[0] = 'x'; w[0] = L'x'; tc_iph_count = 0;
    CHECK_EQ_I(_makepath_s(b, 1, NULL, NULL, NULL, NULL), 0); CHECK_STR(b, "");
    CHECK_EQ_I(_wmakepath_s(w, 1, NULL, NULL, NULL, NULL), 0); CHECK_WSTR(w, L"");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(long_drive_exact)
{
    char b[8]; int r; v_n = "f";
    memset(b, 'x', sizeof b); tc_iph_count = 0;
    r = _makepath_s(b, 8, "AB", "d", v_n, "e");
    CHECK_EQ_I(r, 0);
    CHECK_STR(b, "A:d\\f.e");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph api\n"); return 1; }
    TC_RUN(erange_state); TC_RUN(erange_state_wide); TC_RUN(all_null_size1); TC_RUN(long_drive_exact);
    return tc_end();
}
