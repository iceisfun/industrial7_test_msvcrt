/* TC
 * id:    gtm_zz_pa
 * area:  time
 * funcs: _gmtime64 _localtime64 _mktime64 _mkgmtime64 _ctime64 _gmtime32
 * procs: _gmtime64_s _ctime64_s _wctime64_s asctime_s time _time32 _gmtime32 _localtime32 _mktime32 timespec_get _futime64 _ftime64_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   scratch probe
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/timeb.h>
static void pr(const char *l, long long v){ char b[32]; tc_write(l); tc_write("="); tc_i64(v,b); tc_write(b); tc_write(" "); }
static void nl(void){ tc_write("\n"); }
static void ptm(const char *l, struct tm *p){ tc_write(l); tc_write(": "); if(!p){tc_write("NULL");} else { pr("y",p->tm_year); pr("mo",p->tm_mon); pr("d",p->tm_mday); pr("h",p->tm_hour); pr("mi",p->tm_min); pr("s",p->tm_sec); pr("wd",p->tm_wday); pr("yd",p->tm_yday);} pr("errno",errno); pr("iph",tc_iph_count); nl(); }
TC_DECL_PROC(int,_gmtime64_s,(struct tm*,const __time64_t*));
TC_DECL_PROC(int,_ctime64_s,(char*,size_t,const __time64_t*));
TC_DECL_PROC(int,_wctime64_s,(wchar_t*,size_t,const __time64_t*));
TC_DECL_PROC(int,asctime_s,(char*,size_t,const struct tm*));
static volatile __time64_t vt;
static long long bis_hi(int which){ /* largest accepted t in [0,2^62] */
  long long lo=0,hi=1LL<<62; while(lo<hi){ long long m=lo+(hi-lo+1)/2; struct tm*p; __time64_t t=m; vt=t; t=vt;
    if(which==0)p=_gmtime64(&t); else p=_localtime64(&t); if(p)lo=m; else hi=m-1;} return lo; }
static long long bis_lo(int which){ /* smallest accepted negative */
  long long lo=-(1LL<<62),hi=0; while(lo<hi){ long long m=lo+(hi-lo)/2; struct tm*p; __time64_t t=m; vt=t; t=vt;
    if(which==0)p=_gmtime64(&t); else p=_localtime64(&t); if(p)hi=m; else lo=m+1;} return lo; }
TC_CASE(ranges){
  struct tm *p; __time64_t t; struct tm q; char *c; char b[64]; wchar_t w[64];
  _putenv("TZ=UTC0"); _tzset();
  pr("gm64max",bis_hi(0)); pr("gm64min",bis_lo(0)); nl();
  pr("lt64max",bis_hi(1)); pr("lt64min",bis_lo(1)); nl();
  t=-1; errno=0; tc_iph_count=0; p=_gmtime64(&t); ptm("gm64(-1)",p);
  t=32535244800LL; errno=0; tc_iph_count=0; p=_gmtime64(&t); ptm("gm64(32535244800)",p);
  { __time32_t t32=-1; errno=0; tc_iph_count=0; p=_gmtime32(&t32); ptm("gm32(-1)",p);
    t32=INT_MIN; errno=0; p=_gmtime32(&t32); ptm("gm32(INT_MIN)",p); }
  t=0; errno=0; tc_iph_count=0; p=_gmtime64(&t); ptm("gm64(0)",p);
  t=-1; errno=0; tc_iph_count=0; c=_ctime64(&t); pr("ctime64(-1)null",c==NULL); pr("errno",errno); pr("iph",tc_iph_count); nl();
  t=32535244800LL; errno=0; tc_iph_count=0; c=_ctime64(&t); pr("ctime64(32535244800)null",c==NULL); pr("errno",errno); pr("iph",tc_iph_count); if(c){tc_write(c);} nl();
  { __time32_t t32=0x7fffffff; errno=0; tc_iph_count=0; c=_ctime32(&t32); pr("ctime32(INTMAX)null",c==NULL); pr("errno",errno); pr("iph",tc_iph_count); if(c)tc_write(c); nl(); }
  if(tc_p__gmtime64_s){ int r; t=-1; memset(&q,0,sizeof q); errno=0; tc_iph_count=0; r=tc_p__gmtime64_s(&q,&t); pr("gm64_s(-1)r",r); pr("errno",errno); pr("iph",tc_iph_count); pr("sec",q.tm_sec); nl();
    t=32535244800LL; memset(&q,0,sizeof q); errno=0; tc_iph_count=0; r=tc_p__gmtime64_s(&q,&t); pr("gm64_s(3001+)r",r); pr("errno",errno); pr("iph",tc_iph_count); pr("year",q.tm_year); nl(); }
  if(tc_p__ctime64_s){ int r; t=-1; errno=0; tc_iph_count=0; r=tc_p__ctime64_s(b,64,&t); pr("ctime64_s(-1)r",r); pr("errno",errno); pr("iph",tc_iph_count); nl();
    t=32535244800LL; errno=0; tc_iph_count=0; r=tc_p__ctime64_s(b,64,&t); pr("ctime64_s(32535244800)r",r); pr("errno",errno); pr("iph",tc_iph_count); nl(); }
  if(tc_p__wctime64_s){ int r; t=-1; errno=0; tc_iph_count=0; r=tc_p__wctime64_s(w,40,&t); pr("wctime64_s(-1)r",r); pr("errno",errno); pr("iph",tc_iph_count); nl();
    t=32535216000LL; errno=0; tc_iph_count=0; r=tc_p__wctime64_s(w,40,&t); pr("wctime64_s(32535216000)r",r); pr("errno",errno); pr("iph",tc_iph_count); nl(); }
  /* mktime limits */
  { int ys[]={3000,3001,3500,9999,10000,10001}; unsigned i; for(i=0;i<6;i++){ struct tm m; __time64_t r; memset(&m,0,sizeof m); m.tm_year=ys[i]-1900; m.tm_mon=11; m.tm_mday=31; m.tm_hour=23; m.tm_min=59; m.tm_sec=59; m.tm_wday=3; m.tm_yday=77; errno=0; tc_iph_count=0;
      r=_mktime64(&m); pr("mktime64 Dec31 23:59:59 year",ys[i]); pr("r",r); pr("errno",errno); pr("iph",tc_iph_count); pr("wd",m.tm_wday); pr("yd",m.tm_yday); nl();
      memset(&m,0,sizeof m); m.tm_year=ys[i]-1900; m.tm_mon=11; m.tm_mday=31; m.tm_hour=23; m.tm_min=59; m.tm_sec=59; errno=0;
      r=_mkgmtime64(&m); pr("mkgmtime64 year",ys[i]); pr("r",r); pr("errno",errno); nl(); } }
  { struct tm m; __time64_t r; memset(&m,0,sizeof m); m.tm_year=69; m.tm_mon=11; m.tm_mday=31; m.tm_hour=23; m.tm_min=59; m.tm_sec=59; errno=0; r=_mkgmtime64(&m); pr("mkgmtime64 1969-12-31",r); pr("errno",errno); nl();
    memset(&m,0,sizeof m); m.tm_year=0; m.tm_mon=0; m.tm_mday=1; errno=0; r=_mkgmtime64(&m); pr("mkgmtime64 1900",r); pr("errno",errno); nl(); }
  nl();
}
int main(void){ tc_begin(); tc_iph_install(); TC_LOAD_PROC(_gmtime64_s); TC_LOAD_PROC(_ctime64_s); TC_LOAD_PROC(_wctime64_s); TC_LOAD_PROC(asctime_s); TC_RUN(ranges); return tc_end(); }
