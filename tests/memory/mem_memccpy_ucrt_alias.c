/* TC
 * id:    mem_memccpy_ucrt_alias
 * area:  memory
 * funcs:
 * procs: memccpy
 * crt:   >=140
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs memccpy (deprecated POSIX name; exported by ucrtbase)
 *        Win10 22H2 19045 (observed, GetProcAddress on ucrtbase): the plain POSIX name memccpy is NOT exported; the export is _memccpy (class D).
 *        The test asserts that plain "memccpy" is NOT exported (case plain_not_exported) and resolves "_memccpy" for the behaviour cases (unchanged).
 */
#include "tc.h"
#include <string.h>
#include <memory.h>
#include <stddef.h>

typedef void *(__cdecl *memccpy_t)(void *, const void *, int, size_t);
static memccpy_t p_memccpy;
#define memccpy p_memccpy

TC_CASE(exported) { CHECK(p_memccpy != NULL); }   /* name: _memccpy (see note) */
TC_CASE(plain_not_exported) { CHECK(tc_crt_proc("memccpy") == NULL); }   /* observed on ucrtbase 19045 */
TC_CASE(found) { if (!p_memccpy) TC_SKIP("x"); static char s[] = "key=value", d[12]; volatile int c = '='; memset(d, 'Z', sizeof d); CHECK(memccpy(d, s, c, 9) == d + 4); CHECK_MEM(d, "key=ZZZZZZZZ", 12); }
TC_CASE(not_found) { if (!p_memccpy) TC_SKIP("x"); static char s[] = "key=value", d[12]; volatile int c = '#'; memset(d, 'Z', sizeof d); CHECK(memccpy(d, s, c, 5) == NULL); CHECK_MEM(d, "key=vZZZZZZZ", 12); }
TC_CASE(c_in_first_byte) { if (!p_memccpy) TC_SKIP("x"); static char s[] = "key=value", d[4]; volatile int c = 'k'; memset(d, 'Z', sizeof d); CHECK(memccpy(d, s, c, 9) == d + 1); CHECK_MEM(d, "kZZZ", 4); }

int main(void) { tc_begin(); p_memccpy = (memccpy_t)tc_crt_proc("_memccpy"); TC_RUN(exported); TC_RUN(plain_not_exported); TC_RUN(found); TC_RUN(not_found); TC_RUN(c_in_first_byte); return tc_end(); }
