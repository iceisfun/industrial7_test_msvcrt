/* TC
 * id:    wcs_neg_wcsncoll_null
 * area:  wstring
 * funcs: _wcsncoll
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcoll/wcscoll, _stricoll, _strncoll: error returns _NLSCMPERROR (INT_MAX) and sets errno EINVAL
 * note:  Parameter validation exists in CRT 8.0+ (invalid parameter handler invoked). One function per test so that a crash in one does not mask the others.
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
#include <limits.h>

#define NLSERR INT_MAX
static const wchar_t * volatile S = L"abc";
static const wchar_t * volatile N = NULL;

#define EXPECT_ERR(call) do { int r; tc_iph_count = 0; errno = 0; r = (call); \
    CHECK_EQ_I(r, NLSERR); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(first_null) { EXPECT_ERR(_wcsncoll(N, S, 3)); }
TC_CASE(second_null) { EXPECT_ERR(_wcsncoll(S, N, 3)); }
TC_CASE(both_null) { EXPECT_ERR(_wcsncoll(N, N, 3)); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP no iph\n"); return tc_end(); }
    TC_RUN(first_null); TC_RUN(second_null); TC_RUN(both_null);
    return tc_end();
}
