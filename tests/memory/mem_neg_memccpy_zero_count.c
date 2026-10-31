/* TC
 * id:    mem_neg_memccpy_zero_count
 * area:  memory
 * funcs: _memccpy
 * procs: memccpy
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _memccpy ("returns NULL if c is not found in the first count bytes"); POSIX memccpy
 * note:  count 0 examines nothing, so NULL and no writes. memccpy (no underscore) is looked up at run time and skipped when the DLL does not export it.
 */
#include "tc.h"
#include <string.h>
#include <memory.h>
#include <errno.h>

typedef void *(__cdecl *memccpy_t)(void *, const void *, int, size_t);
static memccpy_t p_memccpy;
static volatile size_t v_n;
static volatile int v_c;

static void run(memccpy_t f)
{
    unsigned char d[8]; const unsigned char s[8] = "abcdefg"; int i;
    for (i = 0; i < 8; i++) d[i] = 0xEE;
    errno = 77; tc_iph_count = 0; v_n = 0;
    v_c = 'z'; CHECK(f(d, s, v_c, v_n) == NULL);
    v_c = 'a'; CHECK(f(d, s, v_c, v_n) == NULL);
    for (i = 0; i < 8; i++) CHECK_EQ_I(d[i], 0xEE);
    CHECK_EQ_I(errno, 77); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(underscore) { run((memccpy_t)_memccpy); }
TC_CASE(posix_name) { if (!p_memccpy) TC_SKIP("memccpy not exported"); run(p_memccpy); }
int main(void)
{
    tc_begin(); tc_iph_install(); p_memccpy = (memccpy_t)tc_crt_proc("memccpy");
    TC_RUN(underscore); TC_RUN(posix_name);
    return tc_end();
}
