/* TC
 * id:    gtm_zz_pb
 * area:  time
 * funcs: asctime strftime wcsftime _ftime64
 * procs: asctime_s _gmtime64_s _localtime64_s timespec_get time _time32 _gmtime32 _localtime32 _mktime32 _futime64 _futime _futime32 _set_abort_behavior _difftime64 _set_invalid_parameter_handler _get_tzname _ftime64_s _strdate_s
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   scratch probe
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
TC_DECL_PROC(int,asctime_s,(char*,size_t,const struct tm*));
TC_DECL_PROC(int,_gmtime64_s,(struct tm*,const __time64_t*));
TC_DECL_PROC(int,_localtime64_s,(struct tm*,const __time64_t*));
TC_DECL_PROC(int,timespec_get,(void*,int));
typedef unsigned int (__cdecl *sab_t)(unsigned int,unsigned int);
static void child(const char *c){ /* child variants: abort-behaviour setup then NULL-arg call */
  sab_t sab=(sab_t)tc_crt_proc("_set_abort_behavior"); struct tm t; __time64_t t64=0;
  if(c[0]=='a') {} /* none */
  else if(c[0]=='b') sab(0,3);
  else if(c[0]=='c') sab(0,1);
  else if(c[0]=='d') sab(0,2);
  if(tc_p__gmtime64_s) tc_p__gmtime64_s(NULL,&t64); (void)t;
  ExitProcess(77);
}
static void spawn(const char *n){ DWORD code=0; int ran=tc_spawn_self(n,&code,20000); char b[24]; tc_write("child "); tc_write(n); tc_write(" ran="); tc_i64(ran,b); tc_write(b); tc_write(" code="); tc_hex(code,b); tc_write(b); nl(); }
TC_CASE(children){ spawn("a"); spawn("b"); spawn("c"); spawn("d"); }
TC_CASE(misc){
  struct tm t; char *r; char b[64]; wchar_t w[64]; int rc; struct __timeb64 tb; double d;
  memset(&t,0,sizeof t); t.tm_year=8100; t.tm_mday=1; errno=0; tc_iph_count=0; r=asctime(&t); pr("asctime y10000 null",r==NULL); pr("errno",errno); pr("iph",tc_iph_count); if(r)tc_write(r); nl();
  memset(&t,0,sizeof t); t.tm_year=8099; t.tm_mon=11; t.tm_mday=31; errno=0; tc_iph_count=0; r=asctime(&t); pr("asctime y9999 null",r==NULL); if(r)tc_write(r); nl();
  memset(&t,0,sizeof t); t.tm_year=8100; t.tm_mday=1; errno=0; tc_iph_count=0; rc=tc_p_asctime_s(b,26,&t); pr("asctime_s y10000",rc); pr("iph",tc_iph_count); pr("errno",errno); nl();
  t.tm_year=117; t.tm_mon=9; t.tm_mday=23; t.tm_hour=14; t.tm_min=5; t.tm_sec=60; tc_iph_count=0; rc=tc_p_asctime_s(b,26,&t); pr("asctime_s sec60",rc); pr("iph",tc_iph_count); tc_write(b); nl();
  t.tm_sec=61; tc_iph_count=0; rc=tc_p_asctime_s(b,26,&t); pr("asctime_s sec61",rc); pr("iph",tc_iph_count); nl();
  t.tm_sec=0; t.tm_mon=12; tc_iph_count=0; rc=tc_p_asctime_s(b,26,&t); pr("asctime_s mon12",rc); pr("iph",tc_iph_count); nl();
  /* strftime */
  memset(&t,0,sizeof t); t.tm_year=120; t.tm_mon=5; t.tm_mday=15; t.tm_hour=12; t.tm_wday=1; t.tm_yday=166; t.tm_mon=12; errno=0; tc_iph_count=0; pr("strftime mon12 r",(long long)strftime(b,64,"%b",&t)); pr("errno",errno); pr("iph",tc_iph_count); nl();
  t.tm_mon=5; t.tm_hour=24; errno=0; tc_iph_count=0; pr("strftime hour24 r",(long long)strftime(b,64,"%H",&t)); pr("errno",errno); pr("iph",tc_iph_count); nl();
  t.tm_hour=12; t.tm_mon=12; errno=0; tc_iph_count=0; pr("wcsftime mon12 r",(long long)wcsftime(w,32,L"%b",&t)); pr("errno",errno); pr("iph",tc_iph_count); nl();
  t.tm_mon=5; errno=0; tc_iph_count=0; pr("strftime badspec r",(long long)strftime(b,64,"%Q",&t)); pr("errno",errno); pr("iph",tc_iph_count); nl();
  memset(&t,0,sizeof t); t.tm_year=121; t.tm_mon=2; t.tm_mday=9; t.tm_hour=14; t.tm_min=5; t.tm_sec=7; t.tm_wday=2; t.tm_yday=67;
  pr("wcsftime %c r",(long long)wcsftime(w,64,L"%c",&t)); { int i; for(i=0;w[i];i++){ char x[2]; x[0]=(char)w[i]; x[1]=0; tc_write(x);} } nl();
  pr("strftime %c r",(long long)strftime(b,64,"%c",&t)); tc_write(b); nl();
  /* ftime tz */
  _putenv("TZ=UTC0"); _tzset(); _ftime64(&tb); pr("ftime UTC0 tz",tb.timezone); pr("dst",tb.dstflag); nl();
  _putenv("TZ=JST-9"); _tzset(); _ftime64(&tb); pr("ftime JST-9 tz",tb.timezone); pr("dst",tb.dstflag); nl();
  _putenv("TZ=PST8PDT"); _tzset(); _ftime64(&tb); pr("ftime PST8PDT tz",tb.timezone); pr("dst",tb.dstflag); nl();
  /* localtime epoch west */
  _putenv("TZ=PST8"); _tzset(); { __time64_t t0=0; struct tm *p; errno=0; p=_localtime64(&t0); pr("localtime64(0) PST8 null",p==NULL); if(p){pr("y",p->tm_year);pr("d",p->tm_mday);pr("h",p->tm_hour);} pr("errno",errno); nl();
     t0=-1; errno=0; p=_localtime64(&t0); pr("localtime64(-1) PST8 null",p==NULL); nl(); }
  _putenv("TZ=UTC0"); _tzset();
  /* difftime */
  { volatile __time64_t mn=LLONG_MIN, z=0; d=_difftime64(mn,z); pr("difftime(min,0) bits",(long long)tc_dbits(d)); nl(); }
  /* timespec_get */
  if(tc_p_timespec_get){ struct { __time64_t s; long n; } ts; int i; for(i=0;i<6;i++){ ts.s=0; ts.n=0; errno=0; tc_iph_count=0; rc=tc_p_timespec_get(&ts,i); pr("timespec_get base",i); pr("ret",rc); pr("s",ts.s); pr("iph",tc_iph_count); nl(); } }
  /* exports */
  { const char *n[]={"time","_time32","_time64","_gmtime32","_localtime32","_mktime32","_gmtime64","_futime","_futime32","_futime64","_ftime","_ftime32","_ftime64","_utime","_ctime32","timespec_get","_timespec64_get","asctime_s"}; unsigned i; for(i=0;i<sizeof n/sizeof*n;i++){ tc_write("export "); tc_write(n[i]); tc_write(tc_crt_proc(n[i])?" yes":" NO"); nl(); } }
}
TC_CASE(futime){
  const char *F="tc_zz_pb.tmp"; int fd; typedef int (__cdecl *f64_t)(int,void*); typedef int (__cdecl *f32_t)(int,void*);
  f64_t p64=(f64_t)tc_crt_proc("_futime64"); f32_t p32=(f32_t)tc_crt_proc("_futime32"); int r; long long v[]={32503680000LL,32535215999LL,32535216000LL,32535244799LL,32535244800LL,253402300799LL,253402300800LL,-1LL};
  unsigned i; if(!p64){tc_write("no futime64\n");return;}
  for(i=0;i<sizeof v/sizeof*v;i++){ long long b[2]; b[0]=v[i]; b[1]=v[i]; fd=_open(F,_O_RDWR|_O_CREAT|_O_BINARY,_S_IREAD|_S_IWRITE); errno=0; tc_iph_count=0; r=p64(fd,b); pr("futime64 t",v[i]); pr("ret",r); pr("errno",errno); pr("iph",tc_iph_count); nl(); _close(fd); }
  { int b[2]={-1,1000000000}; fd=_open(F,_O_RDWR|_O_CREAT|_O_BINARY,_S_IREAD|_S_IWRITE); errno=0; tc_iph_count=0; r=p32(fd,b); pr("futime32 atime-1 ret",r); pr("errno",errno); pr("iph",tc_iph_count); nl(); _close(fd);}
  { long long b[2]={1,1}; errno=0; tc_iph_count=0; r=p64(-1,b); pr("futime64 fd-1 ret",r); pr("errno",errno); pr("iph",tc_iph_count); nl(); }
  _unlink(F);
}
int main(void){
  if(tc_child_name()) { tc_p__gmtime64_s=(tc_pt__gmtime64_s)tc_crt_proc("_gmtime64_s"); child(tc_child_name()); }
  tc_begin(); tc_iph_install(); TC_LOAD_PROC(asctime_s); TC_LOAD_PROC(_gmtime64_s); TC_LOAD_PROC(_localtime64_s); TC_LOAD_PROC(timespec_get);
  TC_RUN(children); TC_RUN(misc); TC_RUN(futime); return tc_end(); }
