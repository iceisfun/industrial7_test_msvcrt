/* TC
 * id:    loc_neg_probe_setlocale_other_codepages
 * area:  locale
 * funcs: setlocale
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   reasoning
 * note:  PROBE: setlocale rejects Unicode transformation code pages other than UTF-7/8 (UTF-16LE/BE 1200/1201, UTF-32 12000/12001)
 *        and a nonexistent code page (99999) with NULL, leaving the locale unchanged. Each in its own case.
 */
#include "tc.h"
#include <locale.h>
static const char * volatile v_name;

static void nul(const char *n) { v_name = n; CHECK(setlocale(LC_ALL, v_name) == NULL); CHECK_STR(setlocale(LC_ALL, NULL), "C"); }
TC_CASE(utf16le_null) { nul(".1200"); }
TC_CASE(utf16be_null) { nul(".1201"); }
TC_CASE(utf32le_null) { nul(".12000"); }
TC_CASE(utf32be_null) { nul(".12001"); }
TC_CASE(nonexistent_cp_null) { nul(".99999"); }
int main(void) { tc_begin(); TC_RUN(utf16le_null); TC_RUN(utf16be_null); TC_RUN(utf32le_null); TC_RUN(utf32be_null); TC_RUN(nonexistent_cp_null); setlocale(LC_ALL, "C"); return tc_end(); }
