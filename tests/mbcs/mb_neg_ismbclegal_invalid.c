/* TC
 * id:    mb_neg_ismbclegal_invalid
 * area:  mbcs
 * funcs: _ismbclegal _setmbcp
 * kind:  negative
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _ismbclegal (lead byte followed by valid trail byte)
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;

TC_CASE(legal) { CHECK_EQ_U(_setmbcp(932), 0); v = 0x8140; CHECK(_ismbclegal(v)); v = 0x8240; CHECK(_ismbclegal(v)); }
TC_CASE(bad_trail) { CHECK_EQ_U(_setmbcp(932), 0); v = 0x8100; CHECK_EQ_I(_ismbclegal(v), 0); v = 0x8120; CHECK_EQ_I(_ismbclegal(v), 0); }
TC_CASE(no_lead) { CHECK_EQ_U(_setmbcp(932), 0); v = 0x41; CHECK_EQ_I(_ismbclegal(v), 0); v = 0xFF40; CHECK_EQ_I(_ismbclegal(v), 0); v = 0x4140; CHECK_EQ_I(_ismbclegal(v), 0); }

int main(void) { tc_begin(); TC_RUN(legal); TC_RUN(bad_trail); TC_RUN(no_lead); _setmbcp(-3); return tc_end(); }
