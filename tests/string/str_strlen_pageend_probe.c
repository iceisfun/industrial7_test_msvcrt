/* TC
 * id:    str_strlen_pageend_probe
 * area:  string
 * funcs: strlen
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   none
 * note:  PROBE: a string whose NUL is the last byte of a committed page, followed by a PAGE_NOACCESS page, does not fault in strlen
 *        (the CRT reads aligned blocks, which never cross a page). Not guaranteed by the C standard; a crash here is data, not necessarily a CRT bug.
 */
#include "tc.h"
#include <string.h>

static volatile char *vp;

TC_CASE(page_end)
{
    char *base = (char *)VirtualAlloc(NULL, 8192, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    DWORD old; size_t n, off;
    if (!base) TC_SKIP("VirtualAlloc failed");
    VirtualProtect(base + 4096, 4096, PAGE_NOACCESS, &old);
    for (n = 0; n <= 40; n++) {
        char *s = base + 4096 - (n + 1);
        for (off = 0; off < n; off++) s[off] = 'z';
        s[n] = 0; vp = s;
        CHECK_EQ_U(strlen((const char *)vp), n);
    }
    VirtualFree(base, 0, MEM_RELEASE);
}
int main(void)
{
    tc_begin();
    TC_RUN(page_end);
    return tc_end();
}
