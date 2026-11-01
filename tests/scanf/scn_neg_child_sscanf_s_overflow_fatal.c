/* TC
 * id:    scn_neg_child_sscanf_s_overflow_fatal
 * area:  scanf
 * kind:  negative
 * funcs: sscanf_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs "Security Features in the CRT", scanf_s: buffer-too-small
 * note:  PROBES: with no handler installed, sscanf_s("abcdef","%s",buf,4) (buffer too small) terminates the process with
 *        a fast-fail (0xC0000409) rather than merely returning 0/ERANGE. Documented text says the handler is invoked
 *        for buffer overruns in the _s functions; the exact behaviour of scanf_s is what is measured. Asserts only
 *        "did not return normally" plus the exact fast-fail status.
 *        CORRECTED for Win10 22H2 ucrtbase (observed 2026-10-31, stable over 2 runs): even with the real UCRT default handler restored (tc_iph_default) a too-small sscanf_s destination is NOT fatal:
 *        the call returns 0 with errno=ENOMEM and the child exits normally (77). The old fast-fail expectation was wrong; the case now asserts the child survives with r==0 && errno==ENOMEM.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile vin;
TC_CASE(child_overflow)
{
    DWORD code = 0; int ran = tc_spawn_self("overflow", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);   /* survives the real default handler: 77 == r==0 && errno==ENOMEM */
}
TC_CASE(child_char_overflow)
{
    DWORD code = 0; int ran = tc_spawn_self("charoverflow", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 77);
}
int main(void)
{
    const char *c; char buf[16]; int r;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        tc_iph_default();   /* real UCRT default handler, not the mingw no-op one */
        vin = "abcdef"; errno = 0;
        if (tc_strequ(c, "overflow")) r = sscanf_s(vin, "%s", buf, (unsigned)4);
        else r = sscanf_s(vin, "%5c", buf, (unsigned)2);
        return (r == 0 && errno == ENOMEM) ? 77 : 78;
    }
    TC_RUN(child_overflow); TC_RUN(child_char_overflow);
    return tc_end();
}
