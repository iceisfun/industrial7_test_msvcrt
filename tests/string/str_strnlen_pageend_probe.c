/* TC
 * id:    str_strnlen_pageend_probe
 * area:  string
 * funcs:
 * procs: strnlen
 * crt:   >=110
 * tier:  p2
 * conf:  probe
 * ref:   MS docs strnlen (examines at most numberOfElements chars)
 * note:  PROBE: strnlen(s, n) on an unterminated n-byte buffer ending at a page boundary (next page PAGE_NOACCESS) returns n without faulting,
 *        i.e. it never reads beyond the count. A crash here is data about the implementation's block reads.
 */
#include "tc.h"

typedef size_t (__cdecl *strnlen_t)(const char *, size_t);
static strnlen_t p_strnlen;

TC_CASE(page_end_unterminated)
{
    char *base = (char *)VirtualAlloc(NULL, 8192, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    DWORD old; size_t n, i;
    if (!base) TC_SKIP("VirtualAlloc failed");
    VirtualProtect(base + 4096, 4096, PAGE_NOACCESS, &old);
    for (n = 1; n <= 40; n++) {
        char *s = base + 4096 - n;
        for (i = 0; i < n; i++) s[i] = 'k';
        CHECK_EQ_U(p_strnlen(s, n), n);
    }
    VirtualFree(base, 0, MEM_RELEASE);
}
int main(void)
{
    tc_begin();
    p_strnlen = (strnlen_t)tc_crt_proc("strnlen");
    CHECK(p_strnlen != NULL);
    if (!p_strnlen) return tc_end();
    TC_RUN(page_end_unterminated);
    return tc_end();
}
