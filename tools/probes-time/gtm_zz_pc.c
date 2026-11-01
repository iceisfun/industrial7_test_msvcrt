/* TC
 * id:    gtm_zz_pc
 * area:  time
 * funcs: _gmtime64 _localtime64 _mktime64 _mkgmtime64 _gmtime32 _localtime32 _ctime32 asctime strftime
 * procs: _gmtime64_s _gmtime32_s _ctime64_s _wctime64_s asctime_s _timespec64_get _futime64 _localtime64_s _localtime32_s _get_tzname _ftime64_s _strdate_s _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   scratch probe
 * note:  scratch
 */
#include "tc.h"
#include <time.h>
#include <wchar.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/timeb.h>
static void pr(const char *l, long long v){ char b[32]; tc_write(l); tc_write("="); tc_i64(v,b); tc_write(b); tc_write(" "); }
static void nl(void){ tc_write("\n"); }
TC_DECL_PROC(int,_gmtime64_s,(struct tm*,const __time64_t*));
TC_DECL_PROC(int,_gmtime32_s,(struct tm*,const __time32_t*));
TC_DECL_PROC(int,_ctime64_s,(char*,size_t,const __time64_t*));
TC_DECL_PROC(int,_wctime64_s,(wchar_t*,size_t,const __time64_t*));
TC_DECL_PROC(int,asctime_s,(char*,size_t,const struct tm*));
TC_DECL_PROC(int,_timespec64_get,(void*,int));
TC_DECL_PROC(int,_localtime64_s,(struct tm*,const __time64_t*));
TC_DECL_PROC(int,_localtime32_s,(struct tm*,const __time32_t*));
TC_DECL_PROC(int,_get_tzname,(size_t*,char*,size_t,int));
TC_DECL_PROC(int,_ftime64_s,(void*));
TC_DECL_PROC(int,_strdate_s,(char*,size_t));
TC_DECL_PROC(int,_futime64,(int,void*));
typedef void (__cdecl *iph_set_t)(void*);
static const char *F="tc_zz_pc.tmp";
static long long bis32(int which,int hi_side){ /* gmtime32=0 localtime32=1 */
  long long lo,hi; if(hi_side){lo=0;hi=0x7fffffffLL; while(lo<hi){long long m=lo+(hi-lo+1)/2; __time32_t t=(__time32_t)m; struct tm*p=which?_localtime32(&t):_gmtime32(&t); if(p)lo=m; else hi=m-1;} return lo;}
  lo=INT_MIN;hi=0; while(lo<hi){long long m=lo+(hi-lo)/2; __time32_t t=(__time32_t)m; struct tm*p=which?_localtime32(&t):_gmtime32(&t); if(p)hi=m; else lo=m+1;} return lo; }
static long long bisfut(void){ long long lo=32535244800LL,hi=253402300799LL; while(lo<hi){ long long m=lo+(hi-lo+1)/2,b[2]; int fd,r; b[0]=b[1]=m; fd=_open(F,_O_RDWR|_O_CREAT|_O_BINARY,_S_IREAD|_S_IWRITE); r=tc_p__futime64(fd,b); _close(fd); if(r==0)lo=m; else hi=m-1;} return lo; }
TC_CASE(a){
  struct tm q,*p; int r; char b[64]; wchar_t w[64]; __time64_t t; long long bs[]={-43200,-43201,32536850399LL,32536850400LL,32536799999LL,32536800000LL};
  unsigned i;
  _putenv("TZ=UTC0"); _tzset();
  pr("gm32max",bis32(0,1)); pr("gm32min",bis32(0,0)); pr("lt32max",bis32(1,1)); pr("lt32min",bis32(1,0)); nl();
  if(tc_p__futime64) { pr("futime64max",bisfut()); nl(); _unlink(F); }
  for(i=0;i<6;i++){ t=bs[i]; memset(&q,0,sizeof q); errno=0; tc_iph_count=0; r=tc_p__gmtime64_s(&q,&t); pr("gm64_s t",bs[i]); pr("r",r); pr("errno",errno); pr("iph",tc_iph_count); pr("y",q.tm_year); pr("s",q.tm_sec); nl(); }
  for(i=0;i<6;i++){ t=bs[i]; errno=0; tc_iph_count=0; p=_gmtime64(&t); pr("gm64 t",bs[i]); pr("null",p==NULL); pr("errno",errno); pr("iph",tc_iph_count); nl(); }
  for(i=0;i<6;i++){ t=bs[i]; errno=0; tc_iph_count=0; p=_localtime64(&t); pr("lt64 t",bs[i]); pr("null",p==NULL); pr("errno",errno); pr("iph",tc_iph_count); nl(); }
  for(i=0;i<6;i++){ t=bs[i]; errno=0; tc_iph_count=0; r=tc_p__ctime64_s(b,64,&t); pr("ctime64_s t",bs[i]); pr("r",r); pr("errno",errno); pr("iph",tc_iph_count); nl(); }
  for(i=0;i<6;i++){ t=bs[i]; errno=0; tc_iph_count=0; r=tc_p__wctime64_s(w,64,&t); pr("wctime64_s t",bs[i]); pr("r",r); pr("errno",errno); pr("iph",tc_iph_count); nl(); }
  { __time32_t t32=INT_MIN; memset(&q,0,sizeof q); errno=0; tc_iph_count=0; r=tc_p__gmtime32_s(&q,&t32); pr("gm32_s INT_MIN r",r); pr("errno",errno); pr("iph",tc_iph_count); pr("y",q.tm_year); nl();
    t32=-1; memset(&q,0,sizeof q); errno=0; tc_iph_count=0; r=tc_p__gmtime32_s(&q,&t32); pr("gm32_s -1 r",r); pr("errno",errno); pr("y",q.tm_year); pr("s",q.tm_sec); pr("mo",q.tm_mon); nl(); 
    t32=0x7fffffff; errno=0; tc_iph_count=0; p=_gmtime32(&t32); pr("gm32 INTMAX null",p==NULL); nl();
    t32=0x7fffffff; errno=0; tc_iph_count=0; p=_localtime32(&t32); pr("lt32 INTMAX null",p==NULL); pr("errno",errno);nl();
    t32=0x7ffeffff; errno=0; p=_localtime32(&t32); pr("lt32 INTMAX-65536 null",p==NULL); nl(); {char*c; t32=0x7ffeffff; c=_ctime32(&t32); pr("ctime32 same null",c==NULL); if(c)tc_write(c); nl();}
  }
  /* mktime edge */
  { int days[]={18,19}; int hs[]={23,0}; unsigned k; for(k=0;k<2;k++){ struct tm m; __time64_t rr; memset(&m,0,sizeof m); m.tm_year=1101; m.tm_mon=0; m.tm_mday=days[k]; m.tm_hour=hs[k]; m.tm_min=k?0:59; m.tm_sec=k?0:59; errno=0; rr=_mktime64(&m); pr("mktime64 3001-01-d",days[k]); pr("r",rr); pr("errno",errno);
     memset(&m,0,sizeof m); m.tm_year=1101; m.tm_mon=0; m.tm_mday=days[k]; m.tm_hour=hs[k]; m.tm_min=k?0:59; m.tm_sec=k?0:59; errno=0; rr=_mkgmtime64(&m); pr("mkgmtime64 r",rr); pr("errno",errno); nl(); } }
  { struct tm m; __time64_t rr; memset(&m,0,sizeof m); m.tm_year=1101; m.tm_mon=11; m.tm_mday=31; errno=0; rr=_mktime64(&m); pr("mktime64 3001-12-31 00:00",rr); pr("errno",errno); nl(); }
}
TC_CASE(b){
  struct tm t; char *r; char bf[64]; int rc; unsigned i; int yrs[]={8099,8100,8101,9000,100000,INT_MAX,-1900,-1901,-1};
  volatile __time64_t vv[]={LLONG_MIN,-1,5,LLONG_MAX}; 
  for(i=0;i<9;i++){ memset(&t,0,sizeof t); t.tm_year=yrs[i]; t.tm_mday=1; errno=0; tc_iph_count=0; r=asctime(&t); pr("asctime year",yrs[i]); pr("null",r==NULL); pr("errno",errno); pr("iph",tc_iph_count); if(r){tc_write("["); tc_write(r); tc_write("]");} nl();
    errno=0; tc_iph_count=0; rc=tc_p_asctime_s(bf,26,&t); pr("asctime_s r",rc); pr("errno",errno); pr("iph",tc_iph_count); nl(); }
  { unsigned j; for(j=0;j<4;j++){ unsigned k; for(k=0;k<4;k++){ double d=_difftime64(vv[j],vv[k]); pr("difftime64 a",vv[j]); pr("b",vv[k]); pr("bits",(long long)tc_dbits(d)); nl(); } } }
  { struct {__time64_t s; long n;} ts; for(i=0;i<6;i++){ ts.s=-5; ts.n=-5; errno=0; tc_iph_count=0; rc=tc_p__timespec64_get(&ts,i); pr("timespec64_get base",i); pr("ret",rc); pr("sec>0",ts.s>1000); pr("iph",tc_iph_count); pr("errno",errno); nl(); } }
  { TIME_ZONE_INFORMATION z; DWORD id=GetTimeZoneInformation(&z); pr("GetTimeZoneInformation id",id); pr("Bias",z.Bias); pr("DaylightBias",z.DaylightBias); nl(); }
  { struct __timeb64 tb; const char *tzs[]={"TZ=UTC0","TZ=JST-9","TZ=PST8PDT","TZ=EST5EDT"}; for(i=0;i<4;i++){ _putenv(tzs[i]); _tzset(); _ftime64(&tb); tc_write(tzs[i]); pr(" ftime tz",tb.timezone); pr("dst",tb.dstflag); nl(); } _putenv("TZ=UTC0"); _tzset(); }
}
static void spawnn(const char *n){ DWORD code=0; int ran=tc_spawn_self(n,&code,20000); char b[24]; tc_write("child "); tc_write(n); tc_write(" ran="); tc_i64(ran,b); tc_write(b); tc_write(" code="); tc_hex(code,b); tc_write(b); nl(); }
TC_CASE(children){ const char *n[]={"gm64s_nulltm","futime_badfd","asc_small","asc_null","asc_mon12","tz5","ftime_null","strdate_small","strf_nullbuf","strf_badspec","strf_mon99","lt64s_nulltm","lt64s_nullt","lt32s_nulltm","gm64s_nulltm_nodef","strf_nullbuf_nodef"}; unsigned i; for(i=0;i<sizeof n/sizeof*n;i++) spawnn(n[i]); }
static void child(const char *c){
  char buf[32]; struct tm t; volatile int sz=25; volatile int idx=5; size_t nn=0; __time64_t t64=0; __time32_t t32=0; long long b64[2]={1000000000LL,1000000000LL};
  iph_set_t set=(iph_set_t)tc_crt_proc("_set_invalid_parameter_handler");
  memset(&t,0,sizeof t); t.tm_year=117; t.tm_mday=1;
  if(!strstr(c,"nodef")) set(NULL);
  if(!strcmp(c,"futime_badfd")) tc_p__futime64(-1,b64);
  else if(!strcmp(c,"asc_small")) tc_p_asctime_s(buf,(size_t)sz,&t);
  else if(!strcmp(c,"asc_null")) tc_p_asctime_s(NULL,26,&t);
  else if(!strcmp(c,"asc_mon12")) { t.tm_mon=12; tc_p_asctime_s(buf,26,&t); }
  else if(!strcmp(c,"tz5")) tc_p__get_tzname(&nn,buf,10,idx);
  else if(!strcmp(c,"ftime_null")) tc_p__ftime64_s(NULL);
  else if(!strcmp(c,"strdate_small")) tc_p__strdate_s(buf,3);
  else if(!strcmp(c,"strf_nullbuf")||!strcmp(c,"strf_nullbuf_nodef")) strftime(NULL,10,"%Y",&t);
  else if(!strcmp(c,"strf_badspec")) strftime(buf,10,"%Q",&t);
  else if(!strcmp(c,"strf_mon99")) { t.tm_mon=99; strftime(buf,32,"%B",&t); }
  else if(!strcmp(c,"lt64s_nulltm")) tc_p__localtime64_s(NULL,&t64);
  else if(!strcmp(c,"lt64s_nullt")) tc_p__localtime64_s(&t,NULL);
  else if(!strcmp(c,"lt32s_nulltm")) tc_p__localtime32_s(NULL,&t32);
  else tc_p__gmtime64_s(NULL,&t64);
  ExitProcess(77);
}
int main(void){
  tc_begin();
  tc_p__futime64=(tc_pt__futime64)tc_crt_proc("_futime64"); tc_p_asctime_s=(tc_pt_asctime_s)tc_crt_proc("asctime_s"); tc_p__get_tzname=(tc_pt__get_tzname)tc_crt_proc("_get_tzname");
  tc_p__ftime64_s=(tc_pt__ftime64_s)tc_crt_proc("_ftime64_s"); tc_p__strdate_s=(tc_pt__strdate_s)tc_crt_proc("_strdate_s"); tc_p__localtime64_s=(tc_pt__localtime64_s)tc_crt_proc("_localtime64_s"); tc_p__localtime32_s=(tc_pt__localtime32_s)tc_crt_proc("_localtime32_s"); tc_p__gmtime64_s=(tc_pt__gmtime64_s)tc_crt_proc("_gmtime64_s");
  if(tc_child_name()) child(tc_child_name());
  tc_iph_install(); TC_LOAD_PROC(_gmtime32_s); TC_LOAD_PROC(_ctime64_s); TC_LOAD_PROC(_wctime64_s); TC_LOAD_PROC(_timespec64_get);
  TC_RUN(a); TC_RUN(b); TC_RUN(children); return tc_end(); }
