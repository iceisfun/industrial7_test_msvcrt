/* TC
 * id:    gtm_zz_pd
 * area:  time
 * funcs: _mktime64 _mkgmtime64 _difftime64 _gmtime32
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   scratch probe
 * note:  scratch
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
static void pr(const char *l, long long v){ char b[32]; tc_write(l); tc_write("="); tc_i64(v,b); tc_write(b); tc_write(" "); }
static void nl(void){ tc_write("\n"); }
TC_CASE(a){
  int h[]={7,7,8,8}, mi[]={59,59,0,0}, s[]={59,59,0,1}; unsigned i; struct tm m; __time64_t r;
  _putenv("TZ=UTC0"); _tzset();
  for(i=0;i<4;i++){ memset(&m,0,sizeof m); m.tm_year=1101; m.tm_mday=19; m.tm_hour=h[i]; m.tm_min=mi[i]; m.tm_sec=s[i]; errno=0; r=_mkgmtime64(&m); pr("mkgmtime64 3001-01-19",h[i]*10000+mi[i]*100+s[i]); pr("r",r); pr("errno",errno); nl();
    memset(&m,0,sizeof m); m.tm_year=1101; m.tm_mday=19; m.tm_hour=h[i]; m.tm_min=mi[i]; m.tm_sec=s[i]; errno=0; tc_iph_count=0; r=_mktime64(&m); pr("mktime64 same",h[i]*10000+mi[i]*100+s[i]); pr("r",r); pr("errno",errno); pr("iph",tc_iph_count); nl(); }
  { volatile __time64_t a=-1,b=5; errno=0; tc_iph_count=0; pr("difftime64(-1,5) bits",(long long)tc_dbits(_difftime64(a,b))); pr("errno",errno); pr("iph",tc_iph_count); nl(); }
  { volatile __time64_t a=0,b=-5; errno=0; tc_iph_count=0; pr("difftime64(0,-5) bits",(long long)tc_dbits(_difftime64(a,b))); pr("errno",errno); pr("iph",tc_iph_count); nl(); }
  { volatile __time64_t a=10,b=4; errno=0; tc_iph_count=0; pr("difftime64(10,4) bits",(long long)tc_dbits(_difftime64(a,b))); pr("errno",errno); pr("iph",tc_iph_count); nl(); }
  { __time32_t t[]={-43200,-1,-2,-86400}; unsigned k; for(k=0;k<4;k++){ struct tm *p; errno=0; p=_gmtime32(&t[k]); pr("gm32",t[k]); if(p){pr("y",p->tm_year);pr("mo",p->tm_mon);pr("d",p->tm_mday);pr("h",p->tm_hour);pr("mi",p->tm_min);pr("s",p->tm_sec);} else tc_write("NULL"); pr("errno",errno); nl(); } }
}
int main(void){ tc_begin(); tc_iph_install(); TC_RUN(a); return tc_end(); }
