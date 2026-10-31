/* TC
 * id:    loc_mbrtoc16_ascii
 * area:  locale
 * funcs: mbrtoc16
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.28.1.1 mbrtoc16; MS docs mbrtoc16
 */
#include "tc.h"
#include <uchar.h>
#include <wchar.h>
#include <string.h>

TC_CASE(ascii_1) { mbstate_t st; char16_t c = 0; volatile const char *s = "A"; memset(&st, 0, sizeof st); CHECK_EQ_U(mbrtoc16(&c, (const char *)s, 1, &st), 1); CHECK_EQ_U(c, u'A'); }
TC_CASE(nul_0)   { mbstate_t st; char16_t c = 0x55; volatile const char *s = ""; memset(&st, 0, sizeof st); CHECK_EQ_U(mbrtoc16(&c, (const char *)s, 1, &st), 0); CHECK_EQ_U(c, 0); }
TC_CASE(null_s_0) { mbstate_t st; char16_t c = 0x55; memset(&st, 0, sizeof st); CHECK_EQ_U(mbrtoc16(&c, NULL, 0, &st), 0); }
TC_CASE(pc16_null_ok) { mbstate_t st; volatile const char *s = "A"; memset(&st, 0, sizeof st); CHECK_EQ_U(mbrtoc16(NULL, (const char *)s, 1, &st), 1); }
int main(void) { tc_begin(); TC_RUN(ascii_1); TC_RUN(nul_0); TC_RUN(null_s_0); TC_RUN(pc16_null_ok); return tc_end(); }
