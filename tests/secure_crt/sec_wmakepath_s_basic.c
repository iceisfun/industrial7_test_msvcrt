/* TC
 * id:    sec_wmakepath_s_basic
 * area:  secure_crt
 * funcs: _wmakepath_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _makepath_s, _wmakepath_s
 */
#define MINGW_HAS_SECURE_API 1
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <stdio.h>
#include <io.h>

static wchar_t *volatile v_null = NULL;
static volatile size_t v_sz = 64;
TC_CASE(all_parts) { wchar_t b[64]; CHECK_EQ_I(_wmakepath_s(b, v_sz, L"C:", L"\\dir\\", L"name", L".ext"), 0); CHECK_WSTR(b, L"C:\\dir\\name.ext"); }
TC_CASE(normalisation) { wchar_t b[64]; CHECK_EQ_I(_wmakepath_s(b, v_sz, L"E", L"\\d", L"n", L"e"), 0); CHECK_WSTR(b, L"E:\\d\\n.e"); }
TC_CASE(null_parts) {
    wchar_t b[64];
    CHECK_EQ_I(_wmakepath_s(b, v_sz, v_null, v_null, L"name", v_null), 0); CHECK_WSTR(b, L"name");
    CHECK_EQ_I(_wmakepath_s(b, v_sz, v_null, v_null, v_null, v_null), 0); CHECK_WSTR(b, L"");
}
int main(void) { tc_begin(); TC_RUN(all_parts); TC_RUN(normalisation); TC_RUN(null_parts); return tc_end(); }
