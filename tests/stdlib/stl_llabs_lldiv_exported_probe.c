/* TC
 * id:    stl_llabs_lldiv_exported_probe
 * area:  stdlib
 * funcs: 
 * procs: llabs lldiv qsort_s bsearch_s _lfind_s _lsearch_s _byteswap_ulong rand_s _set_abort_behavior
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs Requirements tables (ucrtbase exports all of these)
 * note:  PROBE: export inventory. Asserts presence only for ucrtbase; older CRTs just record present/absent via TC_SKIP. msvcrt.dll (60) is asserted NOT to export llabs, lldiv (added in msvcr120); _set_abort_behavior is only reported via INFO (unsure).
 */
#include "tc.h"

#ifdef TC_UCRT
#define NEED(n) CHECK(tc_crt_proc(n) != NULL)
#else
#define NEED(n) do { if (tc_crt_proc(n)) tc_write("TC INFO " TC_TEST_ID " exported: " n "\n"); else tc_write("TC INFO " TC_TEST_ID " absent: " n "\n"); } while (0)
#endif

TC_CASE(ucrt_exports)
{
#ifndef TC_UCRT
    TC_SKIP("inventory only on this CRT (see INFO lines)");
#endif
    NEED("llabs"); NEED("lldiv"); NEED("qsort_s"); NEED("bsearch_s"); NEED("_lfind_s"); NEED("_lsearch_s");
    NEED("_byteswap_ulong"); NEED("rand_s"); NEED("_set_abort_behavior");
}
TC_CASE(inventory)
{
#ifdef TC_UCRT
    TC_SKIP("ucrtbase asserted in ucrt_exports");
#else
    NEED("llabs"); NEED("lldiv"); NEED("qsort_s"); NEED("bsearch_s"); NEED("_lfind_s"); NEED("_lsearch_s");
    NEED("_byteswap_ulong"); NEED("rand_s"); NEED("_set_abort_behavior");
#endif
}
TC_CASE(msvcrt_absent)
{
#if TC_CRT_VER >= 70
    TC_SKIP("only meaningful for msvcrt.dll");
#else
    CHECK(tc_crt_proc("llabs") == NULL);
    CHECK(tc_crt_proc("lldiv") == NULL);
    { void *p = tc_crt_proc("_set_abort_behavior"); tc_write(p ? "TC INFO " TC_TEST_ID " msvcrt exports _set_abort_behavior\n" : "TC INFO " TC_TEST_ID " msvcrt lacks _set_abort_behavior\n"); }
#endif
}

int main(void) { tc_begin(); TC_RUN(ucrt_exports); TC_RUN(inventory); TC_RUN(msvcrt_absent); return tc_end(); }
