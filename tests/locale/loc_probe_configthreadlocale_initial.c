/* TC
 * id:    loc_probe_configthreadlocale_initial
 * area:  locale
 * funcs: _configthreadlocale
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _configthreadlocale
 * note:  PROBE: at program start _configthreadlocale(0) is expected to return _DISABLE_PER_THREAD_LOCALE (2), i.e. the global locale
 *        mode is the default. Some CRT generations might instead report 0; only 2 is asserted.
 */
#include "tc.h"
#include <locale.h>

TC_CASE(initial_query_is_2) { CHECK_EQ_I(_configthreadlocale(0), _DISABLE_PER_THREAD_LOCALE); }
int main(void) { tc_begin(); TC_RUN(initial_query_is_2); return tc_end(); }
