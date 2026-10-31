/* TC
 * id:    err_neg_tmpnam_s_args
 * area:  errno_param
 * kind:  negative
 * funcs: _set_invalid_parameter_handler
 * procs: tmpnam_s _wtmpnam_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs tmpnam_s, _wtmpnam_s (error table: str NULL -> EINVAL; sizeInChars too short (including 0) -> ERANGE)
 * note:  A zero size is either EINVAL or ERANGE (docs table says 'too short' -> ERANGE, but 0 is ambiguous); both accepted here, exact value in err_neg_probe_tmpnam_s_range_buf_state. The handler count for ERANGE and the buffer state after ERANGE are not asserted here (see err_neg_probe_tmpnam_s_range_buf_state).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

TC_DECL_PROC(int, tmpnam_s, (char *, size_t));
TC_DECL_PROC(int, _wtmpnam_s, (wchar_t *, size_t));
static char * volatile v_null = NULL;
static wchar_t * volatile v_wnull = NULL;
static volatile size_t v_sz;

TC_CASE(null_buf)   { int r; errno = 0; tc_iph_count = 0; r = tc_p_tmpnam_s(v_null, 10); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(zero_size)  { char b[32] = "q"; int r; errno = 0; v_sz = 0; r = tc_p_tmpnam_s(b, v_sz); CHECK(r == EINVAL || r == ERANGE); CHECK_EQ_I(errno, r); }
TC_CASE(small)      { char b[32] = "q"; int r; errno = 0; v_sz = 1; r = tc_p_tmpnam_s(b, v_sz); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(wnull_buf)  { int r; errno = 0; tc_iph_count = 0; r = tc_p__wtmpnam_s(v_wnull, 10); CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(wzero_size) { wchar_t b[32] = L"q"; int r; errno = 0; v_sz = 0; r = tc_p__wtmpnam_s(b, v_sz); CHECK(r == EINVAL || r == ERANGE); CHECK_EQ_I(errno, r); }
TC_CASE(wsmall)     { wchar_t b[32] = L"q"; int r; errno = 0; v_sz = 1; r = tc_p__wtmpnam_s(b, v_sz); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::no handler api\n"); return 1; }
    TC_LOAD_PROC(tmpnam_s); TC_LOAD_PROC(_wtmpnam_s); if (!tc_p_tmpnam_s || !tc_p__wtmpnam_s) { tc_write("TC FAIL " TC_TEST_ID "::_wtmpnam_s missing\n"); return 1; }
    TC_RUN(null_buf); TC_RUN(zero_size); TC_RUN(small); TC_RUN(wnull_buf); TC_RUN(wzero_size); TC_RUN(wsmall);
    return tc_end();
}
