/* TC
 * id:    scn_neg_child_sscanf_s_overflow_fatal
 * area:  scanf
 * kind:  negative
 * funcs: sscanf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "Security Features in the CRT", scanf_s: buffer-too-small
 * note:  PROBES: with no handler installed, sscanf_s("abcdef","%s",buf,4) (buffer too small) terminates the process with
 *        a fast-fail (0xC0000409) rather than merely returning 0/ERANGE. Documented text says the handler is invoked
 *        for buffer overruns in the _s functions; the exact behaviour of scanf_s is what is measured. Asserts only
 *        "did not return normally" plus the exact fast-fail status.
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile vin;
TC_CASE(child_overflow)
{
    DWORD code = 0; int ran = tc_spawn_self("overflow", &code, 20000);
    CHECK(ran); CHECK(code != 77); CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
TC_CASE(child_char_overflow)
{
    DWORD code = 0; int ran = tc_spawn_self("charoverflow", &code, 20000);
    CHECK(ran); CHECK(code != 77); CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    const char *c; char buf[16];
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        vin = "abcdef";
        if (tc_strequ(c, "overflow")) sscanf_s(vin, "%s", buf, (unsigned)4);
        else sscanf_s(vin, "%5c", buf, (unsigned)2);
        return 77;
    }
    TC_RUN(child_overflow); TC_RUN(child_char_overflow);
    return tc_end();
}
