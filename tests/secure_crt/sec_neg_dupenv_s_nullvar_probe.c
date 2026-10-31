/* TC
 * id:    sec_neg_dupenv_s_nullvar_probe
 * area:  secure_crt
 * funcs: _dupenv_s
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _dupenv_s (buffer undocumented on bad varname)
 * note:  PROBE: after _dupenv_s(&p,&n,NULL) believed *p == NULL (buffer reset before varname validation). Not documented explicitly.
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <locale.h>
TC_CASE(buffer_set_null) { char *p = (char *)1; size_t n = 7; tc_iph_install();
    CHECK_EQ_I(_dupenv_s(&p, &n, NULL), EINVAL); CHECK(p == NULL); }
int main(void) { tc_begin(); TC_RUN(buffer_set_null); return tc_end(); }
