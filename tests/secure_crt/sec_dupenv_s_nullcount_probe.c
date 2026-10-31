/* TC
 * id:    sec_dupenv_s_nullcount_probe
 * area:  secure_crt
 * funcs: _dupenv_s
 * procs: free
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _dupenv_s
 * note:  PROBE: _dupenv_s accepts pBufferSize == NULL and still returns the copy (docs do not state this explicitly).
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <io.h>

typedef void (__cdecl *free_t)(void *);
TC_CASE(null_count_param_allowed) { char *p = NULL; free_t fr = (free_t)tc_crt_proc("free"); if (!fr) TC_SKIP("no free");
    _putenv_s("TC_SEC_DVAR3", "abc"); CHECK_EQ_I(_dupenv_s(&p, NULL, "TC_SEC_DVAR3"), 0); CHECK(p != NULL);
    if (p) { CHECK_STR(p, "abc"); fr(p); } _putenv_s("TC_SEC_DVAR3", ""); }
int main(void) { tc_begin(); TC_RUN(null_count_param_allowed); return tc_end(); }
