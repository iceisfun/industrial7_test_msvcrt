/* TC
 * id:    wcs_neg_wcschr_not_truncating
 * area:  wstring
 * funcs: wcschr wcsrchr wcspbrk wcsspn
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.4.5; MS docs wcschr, wcspbrk, wcsspn
 * note:  search chars must not be truncated to 8 bits
 */
#include "tc.h"
#include <wchar.h>
#include <errno.h>
static const wchar_t *volatile s = L"a";
static const wchar_t set[] = { 0x0161, 0 };
static const wchar_t *volatile pset = set;
TC_CASE(chr) { CHECK(wcschr(s, 0x0161) == NULL); }
TC_CASE(rchr) { CHECK(wcsrchr(s, 0x0161) == NULL); }
TC_CASE(pbrk) { CHECK(wcspbrk(s, pset) == NULL); }
TC_CASE(spn) { CHECK_EQ_U(wcsspn(s, pset), 0); }
int main(void) { tc_begin(); TC_RUN(chr); TC_RUN(rchr); TC_RUN(pbrk); TC_RUN(spn); return tc_end(); }
