/* TC
 * id:    wcs_neg_wcslen_high_not_terminator
 * area:  wstring
 * funcs: wcslen
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.4.6.1; MS docs wcslen
 * note:  guard against byte-wise implementations
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
static const wchar_t a[] = { 0x0100, 0x0100, 0 };
static const wchar_t b[] = { 0xFF00, 0 };
static const wchar_t c[] = { 'a', 0x0100, 'b', 0 };
TC_CASE(two) { CHECK_EQ_U(wcslen(a), 2); }
TC_CASE(ff00) { CHECK_EQ_U(wcslen(b), 1); }
TC_CASE(middle) { CHECK_EQ_U(wcslen(c), 3); }
int main(void) { tc_begin(); TC_RUN(two); TC_RUN(ff00); TC_RUN(middle); return tc_end(); }
