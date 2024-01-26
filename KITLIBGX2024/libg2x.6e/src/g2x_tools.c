/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G2X - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ================================================================!*/

#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#include <g2x.h>

/*====================================================================*/
/*                 quelques gestionnaires d'horloges                  */
/*====================================================================*/

static G2Xclock calc={0,0,0,0,0,0}, /* timer de calcul  */
                real={0,0,0,0,0,0}; /* timer tps reel   */

static char proc_timer[512]="";
static char real_timer[512]="";

/* Chrono "Processeur" : determine le temps de calcul */
extern char* g2x_ProcTimer(void)
{
  static bool   init=true;
  static long   clickpersec=0;
  static struct tms start,time;
  static long   click=0;
  double tps;

  /* initialisation d'un nouveau compteur */
  if (init)
  {
    memset(&calc,0,sizeof(G2Xclock));
    clickpersec = sysconf(_SC_CLK_TCK);
    times(&start);
    init=false;
  }
  /* reprise d'un compteur */
  else if (!clickpersec)
  {
    clickpersec = sysconf(_SC_CLK_TCK);
    /* nombre de sec. du compteur a continuer */
    click=((calc.hh*60+calc.mm)*60+calc.ss)*clickpersec;
  }

  times(&time);
  /* nombre de click ecoulees depuis le dernier comptage */
  click+=(time.tms_utime-start.tms_utime);
  start=time;

  /* nombre de click ecoulees depuis le dernier comptage */
  tps=(double)click/(double)clickpersec;

  calc.mm  = (int)(tps/60.);
  calc.ss  = (int)(tps-60.*calc.mm);
  calc.hh  = (int)(calc.mm/60.);
  calc.mm -= (int)(60.*calc.hh);
  sprintf(proc_timer,"proc. : %02d:%02d:%02d",calc.hh,calc.mm,calc.ss);
  return proc_timer;
}

/* Chrono temps "reel" */
extern char* g2x_Chrono(void)
{
  static bool   init=true;
  static struct timeval t;
  static int    sec=0,usec=0,s=0,u=0;

  if (init)
  {
    gettimeofday(&t,NULL);
    sec =(int)t.tv_sec;
    usec=(int)t.tv_usec;
    memset(&real,0,sizeof(G2Xclock));
    init=false;
    return NULL;
  }
  gettimeofday(&t,NULL);
  s=(int)t.tv_sec - sec;
  u=(int)t.tv_usec-usec;

  sec =(int)t.tv_sec;
  usec=(int)t.tv_usec;

  if (u<0) {s++; u=1-u;}

  real.mm  = (int)(s/60.);
  real.ss  = (int)(s-60.*real.mm);
  real.hh  = (int)(real.mm/60.);
  real.mm -= (int)(60.*real.hh);
  sprintf(real_timer,"real. : %02d:%02d:%02d",real.hh,real.mm,real.ss);
  return real_timer;
}

/* recupere l'horloge courante */
extern void g2x_GetTime(G2Xclock* clock)
{
  static struct timeval t;
  static struct tm *tps;
  static long   elapsed;

  elapsed=time(NULL);
  tps=localtime(&elapsed);
  gettimeofday(&t,NULL);

  clock->hh  = (uint)tps->tm_hour;
  clock->mm  = (uint)tps->tm_min;
  clock->ss  = (uint)tps->tm_sec;
  clock->ms  = (uint)(t.tv_usec/1000);
  clock->cs  = (uint)(clock->ms/10);
  clock->ds  = (uint)(clock->cs/10);
}

/*====================================================================*/
/*                   quelques generateurs aleatoires                  */
/*====================================================================*/
//static bool   _G2XRANDINIT_=false;
static double _G2XRANDCAL_=1./(1.+RAND_MAX);

/* ------------------------------------------------------------------ */
/* val. aleatoire dans [root-p% , root+p%]                            */
/* Exp. g2x_Rand_Percent(2.,10.) => [1.8,2.2]                         */
/* ------------------------------------------------------------------ */
extern double g2x_Rand_Percent(double root, double percent)
{
  //if (!_G2XRANDINIT_) { srand(getpid()); _G2XRANDINIT_=true; }
  return root*(1.+percent*(1.-2.*rand()*_G2XRANDCAL_));
}

/* ------------------------------------------------------------------ */
/* val. aleatoire dans [root-delta , root+delta]                      */
/* Exp. g2x_Rand_Percent(2.,10.) => [-8.,+12.]                        */
/* ------------------------------------------------------------------ */
extern double g2x_Rand_Delta(double root, double delta)
{
  //if (!_G2XRANDINIT_) { srand(getpid()); _G2XRANDINIT_=true; }
  return root+delta*(1.-2.*rand()*_G2XRANDCAL_);
}

/* ------------------------------------------------------------------ */
/* val. aleatoire dans [min , max]                                    */
/* Exp. g2x_Rand_Percent(2.,10.) => [2.,10.]                          */
/* ------------------------------------------------------------------ */
extern double g2x_Rand_MinMax(double min, double max)
{
  return g2x_Rand_Delta((min+max)/2.,(max-min)/2.);
}

#ifdef __cplusplus
  }
#endif
