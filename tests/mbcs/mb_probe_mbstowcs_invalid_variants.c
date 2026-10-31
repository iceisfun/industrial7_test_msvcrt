/* TC
 * id:    mb_probe_mbstowcs_invalid_variants
 * area:  mbcs
 * funcs: mbstowcs setlocale
 * procs: 
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs mbstowcs
 * note:  PROBE: (1) mbstowcs(NULL, {82 20 0}, 0) in CP932 - expect -1; (2) mbstowcs(dst, {82 0}, 8) lead byte then NUL in CP932 - expect -1 (EILSEQ).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <stdio.h>

static int set932(void)
{
    if (setlocale(LC_ALL, "Japanese_Japan.932")) return 1;
    return setlocale(LC_ALL, ".932") != NULL;
}

TC_CASE(dst_null_invalid)
{
    const char s[] = { (char)0x82, 0x20, 0 }; size_t n;
    if (!set932()) { setlocale(LC_ALL, "C"); TC_SKIP("932 locale unavailable"); }
    n = mbstowcs(NULL, s, 0);
    setlocale(LC_ALL, "C");
    CHECK_EQ_I((long long)n, -1);
}
TC_CASE(truncated_lead)
{
    wchar_t w[8]; const char s[] = { 'a', (char)0x82, 0 }; size_t n;
    if (!set932()) { setlocale(LC_ALL, "C"); TC_SKIP("932 locale unavailable"); }
    errno = 0;
    n = mbstowcs(w, s, 8);
    setlocale(LC_ALL, "C");
    CHECK_EQ_I((long long)n, -1);
#if TC_AT_LEAST(80)
    CHECK_EQ_I(errno, EILSEQ);
#endif
}
int main(void)
{
    tc_begin();
    TC_RUN(dst_null_invalid); TC_RUN(truncated_lead);
    return tc_end();
}
