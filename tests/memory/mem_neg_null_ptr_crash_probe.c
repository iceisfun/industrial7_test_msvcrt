/* TC
 * id:    mem_neg_null_ptr_crash_probe
 * area:  memory
 * kind:  negative
 * funcs: memcpy memmove memset memcmp memchr
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs memcpy (non-secure variants do no parameter validation)
 * note:  Probes that the non-secure mem* functions do NOT validate NULL (no invalid-parameter handler) and instead take an
 *        access violation (exit 0xC0000005) for n > 0. Each runs in its own child with an invalid-parameter handler installed
 *        where available; a handler-driven exit would show as a different code.
 */
#include "tc.h"
#include <string.h>

static volatile char buf[64];
static char * volatile vnull = NULL;
static volatile size_t vn = 16;

static void iph_exit(const wchar_t *e, const wchar_t *f, const wchar_t *fl, unsigned int l, uintptr_t r)
{ (void)e; (void)f; (void)fl; (void)l; (void)r; ExitProcess(55); }

static void run_child(const char *c)
{
    tc_iph_t (__cdecl *set)(tc_iph_t) = (tc_iph_t (__cdecl *)(tc_iph_t))tc_crt_proc("_set_invalid_parameter_handler");
    if (set) set((tc_iph_t)iph_exit);
    if (tc_strequ(c, "memcpy_dest")) memcpy(vnull, (void *)buf, vn);
    else if (tc_strequ(c, "memmove_src")) memmove((void *)buf, vnull, vn);
    else if (tc_strequ(c, "memset")) memset(vnull, 1, vn);
    else if (tc_strequ(c, "memcmp")) { volatile int r = memcmp(vnull, (void *)buf, vn); (void)r; }
    else if (tc_strequ(c, "memchr")) { volatile void *r = memchr(vnull, 1, vn); (void)r; }
}
static void expect_av(const char *name)
{
    DWORD code = 0; int ran = tc_spawn_self(name, &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);
}
TC_CASE(child_memcpy_null_dest_av)  { expect_av("memcpy_dest"); }
TC_CASE(child_memmove_null_src_av)  { expect_av("memmove_src"); }
TC_CASE(child_memset_null_av)       { expect_av("memset"); }
TC_CASE(child_memcmp_null_av)       { expect_av("memcmp"); }
TC_CASE(child_memchr_null_av)       { expect_av("memchr"); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) { run_child(c); return 77; }
    TC_RUN(child_memcpy_null_dest_av); TC_RUN(child_memmove_null_src_av); TC_RUN(child_memset_null_av);
    TC_RUN(child_memcmp_null_av); TC_RUN(child_memchr_null_av);
    return tc_end();
}
