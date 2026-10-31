/* TC
 * id:    gsj_probe_neg_ucrt_only_exports_absent_win7
 * area:  exit_signal
 * kind:  negative
 * funcs:
 * procs: _malloc_base _calloc_base _free_base _lock_locales _unlock_locales _query_app_type
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * conf:  probe
 * ref:   none; these names appear only in the ucrt import libraries
 * note:  PROBING that the ucrt-internal exports _malloc_base _calloc_base _free_base _lock_locales _unlock_locales
 *        _query_app_type are NOT exported by the Windows 7 era CRTs.
 */
#include "tc.h"

TC_CASE(_malloc_base) { CHECK(tc_crt_proc("_malloc_base") == NULL); }
TC_CASE(_calloc_base) { CHECK(tc_crt_proc("_calloc_base") == NULL); }
TC_CASE(_free_base) { CHECK(tc_crt_proc("_free_base") == NULL); }
TC_CASE(_lock_locales) { CHECK(tc_crt_proc("_lock_locales") == NULL); }
TC_CASE(_unlock_locales) { CHECK(tc_crt_proc("_unlock_locales") == NULL); }
TC_CASE(_query_app_type) { CHECK(tc_crt_proc("_query_app_type") == NULL); }
int main(void)
{
    tc_begin();
    TC_RUN(_malloc_base); TC_RUN(_calloc_base); TC_RUN(_free_base);
    TC_RUN(_lock_locales); TC_RUN(_unlock_locales); TC_RUN(_query_app_type);
    return tc_end();
}
