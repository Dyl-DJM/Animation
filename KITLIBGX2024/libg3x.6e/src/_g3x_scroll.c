/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G3X - Fonctions de base d'acces public -
                    version 6e - Jan.2022
  ================================================================!*/

#include <g3x_control.h>

#define _SCROLLWIDTH_ 6
static int         scrollwidth = _SCROLLWIDTH_;
static int         scrollid  =0;
static int         scrollhnum=0;
static G3Xscroll *_SCROLLH_=NULL,*TheScrollh=NULL;
static double      Xscrollrange;
static GLint       Xscrollend  ;
static GLint       Yscrollpos  = 8;
static GLint       Xscrollstart=32;

static int         scrollvnum=0;
static G3Xscroll *_SCROLLV_=NULL, *TheScrollv=NULL;
static double      Yscrollrange;
static GLint       Yscrollend;
static GLint       Xscrollpos  =12;
static GLint       Yscrollstart=22;

/*================================================*/
/*=       RECUPERE LA POSITION DU SCROLL id      =*/
extern void g3x_SetScrollWidth(int width)
{
  scrollwidth = CLIP(_SCROLLWIDTH_/2,width,_SCROLLWIDTH_*2);
}



/*================================================*/
/*=       RECUPERE LA POSITION DU SCROLL id      =*/
extern double g3x_GetScrollCursor(int id)
{
  G3Xscroll *scroll=NULL;

  scroll=_SCROLLH_;
  while (scroll<_SCROLLH_+scrollhnum)
  {
    if (scroll->id==id)
    { return scroll->min + scroll->cursor*(scroll->max-scroll->min); }
    scroll++;
  }
  scroll=_SCROLLV_;
  while (scroll<_SCROLLV_+scrollvnum)
  {
    if (scroll->id==id)
    { return scroll->min + scroll->cursor*(scroll->max-scroll->min); }
    scroll++;
  }
  fprintf(stderr,"\e[31m<g3x_GetScrollCursor> ERREUR : Scroll [%d] Non Identifie\e[0m (retour 0.0)\n",id);
  return 0.;
}

/*================================================*/
/*=       AJUSTE LA COULEUR DU SCROLL id      =*/
extern void g3x_SetScrollColor(int id, G3Xcolor col)
{
  G3Xscroll *scroll=NULL;

  scroll=_SCROLLH_;
  while (scroll<_SCROLLH_+scrollhnum)
  {
    if (scroll->id==id) { scroll->bkg = col; return; }
    scroll++;
  }
  scroll=_SCROLLV_;
  while (scroll<_SCROLLV_+scrollvnum)
  {
    if (scroll->id==id) { scroll->bkg = col; return; }
    scroll++;
  }
  fprintf(stderr,"\e[31m<g3x_SetScrollColor> ERREUR : Scroll [%d] Non Identifie\e[0m (retour 0.0)\n",id);
  return;
}


/*================================================*/
/*=        CREATION D'UN SCROLL HORIZONTAL       =*/
extern int g3x_CreateScrollh_d(const char *name, double* prm, double min, double max, double mapfactor, const char *info)
{
  G3Xscroll *scroll=NULL,*s;
  if (scrollhnum>=BLOCSIZE)
  { fprintf(stderr,"\e[36m<g3x_CreateScrollh> :\e[1;31mNOMBRE MAX (%d)ATTEINT retour -1\e[0;0m\n",scrollhnum); return -1;}
  if (!_SCROLLH_)
    if (!(_SCROLLH_=(G3Xscroll*)calloc(BLOCSIZE,sizeof(G3Xscroll))))
    { fprintf(stderr,"\e[36m<g3x_CreateScrollh> :\e[1;31mECHEC CREATION retour -1\e[0;0m\n"); return -1; }
  TheScrollh = scroll = _SCROLLH_+scrollhnum;
  scrollhnum++;
  strncpy(scroll->name,name,7);
  if (info) strncpy(scroll->info,info,127);
  scroll->dprm   = prm;
  scroll->iprm   = NULL;
  scroll->id     = scrollid++;

  int   len=12;
  int    i=0;
  while (name[i]!='\0')
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,name[i]);
    i++;
  }

  if (len>Xscrollstart)
  {
    Xscrollstart =  MAX(Xscrollstart,len);
    Xscrollrange = Xscrollend-Xscrollstart;
    s=_SCROLLH_;
    while (s<scroll) { s->xcurs  = (int)(Xscrollstart+s->cursor*Xscrollrange); s++; }
  }
  scroll->xcurs  = (int)(Xscrollstart+scroll->cursor*Xscrollrange);
  scroll->ycurs  = Yscrollpos;
  scroll->cursor = (double)(*prm-min)/(double)(max-min);
  scroll->min    = (double)min;
  scroll->max    = (double)max;
  scroll->fac    = (double)mapfactor;
  scroll->w      = scrollwidth;
  scroll->bkg    = G3Xcc_c;
  /* position en y du prochain scroll */
  Yscrollpos += 2*scroll->w+4;
  Xdialheight  = MAX(Xdialheight,Yscrollpos-scroll->w);
  return (int)scroll->id ;
}

extern int g3x_CreateScrollh_i(const char *name, int* prm, int min, int max, double mapfactor, const char *info)
{
  G3Xscroll *scroll=NULL,*s;
  if (scrollhnum>=BLOCSIZE)
  { fprintf(stderr,"\e[36m<g3x_CreateScrollh> :\e[1;31mNOMBRE MAX (%d)ATTEINT retour -1\e[0;0m\n",scrollhnum); return -1;}
  if (!_SCROLLH_)
    if (!(_SCROLLH_=(G3Xscroll*)calloc(BLOCSIZE,sizeof(G3Xscroll))))
    { fprintf(stderr,"\e[36m<g3x_CreateScrollh> :\e[1;31mECHEC CREATION retour -1\e[0;0m\n"); return -1; }
  TheScrollh = scroll = _SCROLLH_+scrollhnum;
  scrollhnum++;
  strncpy(scroll->name,name,7);
  if (info) strncpy(scroll->info,info,127);
  scroll->dprm   = NULL;
  scroll->iprm   = prm;
  scroll->id     = scrollid++;

  int   len=12;
  int    i=0;
  while (name[i]!='\0')
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,name[i]);
    i++;
  }
  if (len>Xscrollstart)
  {
    Xscrollstart =  MAX(Xscrollstart,len);
    Xscrollrange = Xscrollend-Xscrollstart;
    s=_SCROLLH_;
    while (s<scroll) { s->xcurs  = (int)(Xscrollstart+s->cursor*Xscrollrange); s++; }
  }
  scroll->xcurs  = (int)(Xscrollstart+scroll->cursor*Xscrollrange);
  scroll->ycurs  = Yscrollpos;
  scroll->cursor = (double)(*prm-min)/(double)(max+0.99-min);
  scroll->min    = (double)min;
  scroll->max    = (double)max+0.99;
  scroll->fac    = (double)mapfactor;
  scroll->w      = scrollwidth;
  scroll->bkg    = G3Xcc_c;
  /* position en y du prochain scroll */
  Yscrollpos += 2*scroll->w+4;
  Xdialheight  = MAX(Xdialheight,Yscrollpos-scroll->w);
  return (int)scroll->id ;
}

extern int g3x_CreateAnonymousScrollh(const char *name, double init, const char *info)
{
  G3Xscroll *scroll=NULL,*s;
  if (scrollhnum>=BLOCSIZE)
  { fprintf(stderr,"\e[36m<g3x_CreateAnonymousScrollh> :\e[1;31mNOMBRE MAX (%d)ATTEINT retour -1\e[0;0m\n",scrollhnum); return -1;}
  if (!_SCROLLH_)
    if (!(_SCROLLH_=(G3Xscroll*)calloc(BLOCSIZE,sizeof(G3Xscroll))))
    { fprintf(stderr,"\e[36m<g3x_CreateAnonymousScrollh> :\e[1;31mECHEC CREATION retour -1\e[0;0m\n"); return -1; }
  TheScrollh = scroll = _SCROLLH_+scrollhnum;
  scrollhnum++;
  strncpy(scroll->name,name,7);
  if (info) strncpy(scroll->info,info,127);
  scroll->dprm   = NULL;
  scroll->iprm   = NULL;
  scroll->id     = scrollid++;
  int   len=12;
  int    i=0;
  while (name[i]!='\0')
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,name[i]);
    i++;
  }
  if (len>Xscrollstart)
  {
    Xscrollstart =  MAX(Xscrollstart,len);
    Xscrollrange = Xscrollend-Xscrollstart;
    s=_SCROLLH_;
    while (s<scroll) { s->xcurs  = (int)(Xscrollstart+s->cursor*Xscrollrange); s++; }
  }
  scroll->xcurs  = (int)(Xscrollstart+scroll->cursor*Xscrollrange);
  scroll->ycurs  = Yscrollpos;
  scroll->cursor = CLIP(0.,init,1.);
  scroll->min    = 0.;
  scroll->max    = 1.;
  scroll->w      = scrollwidth;
  scroll->bkg    = G3Xcc_c;
  /* position en y du prochain scroll */
  Yscrollpos += 2*scroll->w+4;
  Xdialheight  = MAX(Xdialheight,Yscrollpos-scroll->w);
  return (int)scroll->id ;
}

extern int g3x_CreateScrollv_d(const char *name, double* prm, double min, double max, double mapfactor, const char *info)
{
  G3Xscroll *scroll=NULL;
  if (scrollvnum>=BLOCSIZE)
  { fprintf(stderr,"\e[36m<g3x_CreateScrollv> :\e[1;31mNOMBRE MAX (%d)ATTEINT retour -1\e[0;0m\n",scrollvnum); return -1;}
  if (!_SCROLLV_)
    if (!(_SCROLLV_=(G3Xscroll*)calloc(BLOCSIZE,sizeof(G3Xscroll))))
    { fprintf(stderr,"\e[36m<g3x_CreateScrollv> :\e[1;31mECHEC CREATION retour -1\e[0;0m\n"); return -1; }
  TheScrollv = scroll = _SCROLLV_+scrollvnum;
  scrollvnum++;
  strncpy(scroll->name,name,7);
  if (info) strncpy(scroll->info,info,127);
  scroll->dprm   = prm;
  scroll->iprm   = NULL;
  scroll->id     = scrollid++;
  scroll->xcurs  = Xscrollpos;
  scroll->ycurs  = Yscrollstart;
  scroll->cursor = (double)(*prm-min)/(double)(max-min);
  scroll->min    = (double)min;
  scroll->max    = (double)max;
  scroll->fac    = (double)mapfactor;
  scroll->w      = scrollwidth;
  scroll->bkg    = G3Xcc_c;
  /* position en y du prochain scroll */
  Xscrollpos += 2*scroll->w+4;
  Ydialwidth   = MAX(Ydialwidth,Xscrollpos-scroll->w+4);
  /* POUR LES BOUTONS */
  Xbutpos      = Ydialwidth/2;
  Xbutw        = Ydialwidth/2-2;
  return (int)scroll->id ;
}

extern int g3x_CreateScrollv_i(const char *name, int* prm, int min, int max, double mapfactor, const char *info)
{
  G3Xscroll *scroll=NULL;
  if (scrollvnum>=BLOCSIZE)
  { fprintf(stderr,"\e[36m<g3x_CreateScrollv> :\e[1;31mNOMBRE MAX (%d)ATTEINT retour -1\e[0;0m\n",scrollvnum); return -1;}
  if (!_SCROLLV_)
    if (!(_SCROLLV_=(G3Xscroll*)calloc(BLOCSIZE,sizeof(G3Xscroll))))
    { fprintf(stderr,"\e[36m<g3x_CreateScrollv> :\e[1;31mECHEC CREATION retour -1\e[0;0m\n"); return -1; }
  TheScrollv = scroll = _SCROLLV_+scrollvnum;
  scrollvnum++;
  strncpy(scroll->name,name,7);
  if (info) strncpy(scroll->info,info,127);
  scroll->dprm   = NULL;
  scroll->iprm   = prm;
  scroll->id     = scrollid++;
  scroll->xcurs  = Xscrollpos;
  scroll->ycurs  = Yscrollstart;
  scroll->cursor = (double)(*prm-min)/(double)(max+0.99-min);
  scroll->min    = (double)min;
  scroll->max    = (double)max+0.99;
  scroll->fac    = (double)mapfactor;
  scroll->w      = scrollwidth;
  scroll->bkg    = G3Xcc_c;
  /* position en y du prochain scroll */
  Xscrollpos += 2*scroll->w+4;
  Ydialwidth   = MAX(Ydialwidth,Xscrollpos-scroll->w+4);
  /* POUR LES BOUTONS */
  Xbutpos      = Ydialwidth/2;
  Xbutw        = Ydialwidth/2-2;
  return (int)scroll->id ;
}

extern int g3x_CreateAnonymousScrollv(const char *name, double init, const char *info)
{
  G3Xscroll *scroll=NULL;
  if (scrollvnum>=BLOCSIZE)
  { fprintf(stderr,"\e[36m<g3x_CreateScrollv> :\e[1;31mNOMBRE MAX (%d)ATTEINT retour -1\e[0;0m\n",scrollvnum); return -1;}
  if (!_SCROLLV_)
    if (!(_SCROLLV_=(G3Xscroll*)calloc(BLOCSIZE,sizeof(G3Xscroll))))
    { fprintf(stderr,"\e[36m<g3x_CreateScrollv> :\e[1;31mECHEC CREATION retour -1\e[0;0m\n"); return -1; }
  TheScrollv = scroll = _SCROLLV_+scrollvnum;
  scrollvnum++;
  strncpy(scroll->name,name,7);
  if (info) strncpy(scroll->info,info,127);
  scroll->dprm   = NULL;
  scroll->iprm   = NULL;
  scroll->id     = scrollid++;
  scroll->xcurs  = Xscrollpos;
  scroll->ycurs  = Yscrollstart;
  scroll->cursor = CLIP(0.,init,1.);
  scroll->min    = 0.;
  scroll->max    = 1.;
  scroll->w      = scrollwidth;
  scroll->bkg    = G3Xcc_c;
  /* position en y du prochain scroll */
  Xscrollpos += 2*scroll->w+4;
  Ydialwidth   = MAX(Ydialwidth,Xscrollpos-scroll->w+4);
  /* POUR LES BOUTONS */
  Xbutpos      = Ydialwidth/2;
  Xbutw        = Ydialwidth/2-2;
  return (int)scroll->id ;
}


/*================================================*/
/*=          AFFICHE UN SCROLL HORIZONTAL        =*/
static __inline__ void g3x_DrawScrollh(G3Xscroll *scroll)
{
  if (!scroll) return;
  int x1,x2,y1,y2;

  /* BANDE DE DEFILEMENT */
  x1=(Xscrollstart -scroll->w);
  y2=(scroll->ycurs-scroll->w);
  x2=(Xscrollend   +scroll->w);
  y1=(scroll->ycurs+scroll->w);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glLineWidth(1);
  g3x_Color4fv(G3Xk_c);
  glBegin(GL_QUADS);
    glVertex2i(x1,y1);
    glVertex2i(x1,y2);
    glVertex2i(x2,y2);
    glVertex2i(x2,y1);
  glEnd();
  glLineWidth(1);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  g3x_Color4fv(scroll->bkg);
  glBegin(GL_QUADS);
    glVertex2i(x1,y1);
    glVertex2i(x1,y2);
    glVertex2i(x2,y2);
    glVertex2i(x2,y1);
  glEnd();
  /* CURSEUR */
  x1=(scroll->xcurs-scroll->w);
  y1=(scroll->ycurs-scroll->w);
  x2=(scroll->xcurs+scroll->w);
  y2=(scroll->ycurs+scroll->w);
  g3x_Color4fv(G3Xba_c);
  glBegin(GL_QUADS);
    glVertex2i(x1,y1);
    glVertex2i(x1,y2);
    glVertex2i(x2,y2);
    glVertex2i(x2,y1);
  glEnd();

  char pos[16],*c;
  glColor4f(G3Xwd.r,G3Xwd.g,G3Xwd.b,1.-G3Xwd.a);
  glRasterPos2i((4),(scroll->ycurs-4));
  int    i=0;
  while (scroll->name[i]!='\0')
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,scroll->name[i]);
    i++;
  }
  glColor4f(G3Xwd.r,G3Xwd.g,G3Xwd.b,1.-G3Xwd.a);
  glRasterPos2i((scroll->xcurs+scroll->w+2),(scroll->ycurs-4));
       if(scroll->dprm) sprintf(pos,"%.2lf",*scroll->dprm);
  else if(scroll->iprm) sprintf(pos,"%d",*scroll->iprm);
  else                  sprintf(pos,"%.2lf",scroll->min + scroll->cursor*(scroll->max-scroll->min));
  for (c=pos; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
}

/*================================================*/
/*=           AFFICHE UN SCROLL VERTICAL         =*/
static __inline__ void g3x_DrawScrollv(G3Xscroll *scroll)
{
  if (!scroll) return;
  double x1,x2,y1,y2;

  /* BANDE DE DEFILEMENT */
  x1=(scroll->xcurs-scroll->w);
  y1=(Yscrollend   +scroll->w);
  x2=(scroll->xcurs+scroll->w);
  y2=(Yscrollstart -scroll->w);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glLineWidth(1);
  g3x_Color4fv(G3Xk_c);
  glBegin(GL_QUADS);
    glVertex2d(x1,y1);
    glVertex2d(x1,y2);
    glVertex2d(x2,y2);
    glVertex2d(x2,y1);
  glEnd();
  glLineWidth(1);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  g3x_Color4fv(scroll->bkg);
  glBegin(GL_QUADS);
    glVertex2d(x1,y1);
    glVertex2d(x1,y2);
    glVertex2d(x2,y2);
    glVertex2d(x2,y1);
  glEnd();
  /* CURSEUR */
  x1=(scroll->xcurs-scroll->w);
  y1=(scroll->ycurs+scroll->w);
  x2=(scroll->xcurs+scroll->w);
  y2=(scroll->ycurs-scroll->w);
  g3x_Color4fv(G3Xba_c);
  glBegin(GL_QUADS);
    glVertex2d(x1,y1);
    glVertex2d(x1,y2);
    glVertex2d(x2,y2);
    glVertex2d(x2,y1);
  glEnd();

  char pos[16],*c;
  int len=1;
  for (c=scroll->name; *c!='\0'; c++) len+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_10,*c);
  glColor4f(G3Xwd.r,G3Xwd.g,G3Xwd.b,1.-G3Xwd.a);
  glRasterPos2d((scroll->xcurs-0.5*len),(Yscrollstart-18));
  for (c=scroll->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);


       if(scroll->dprm) sprintf(pos,"%.2lf",*scroll->dprm);
  else if(scroll->iprm) sprintf(pos,"%d"  ,*scroll->iprm);
  else                  sprintf(pos,"%.2lf",scroll->min + scroll->cursor*(scroll->max-scroll->min));
  len=1;
  for (c=pos; *c!='\0'; c++) len+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_10,*c);
  glColor4f(G3Xwd.r,G3Xwd.g,G3Xwd.b,1.-G3Xwd.a);
  glRasterPos2d((scroll->xcurs-0.5*len), (scroll->ycurs+scroll->w+2));
  for (c=pos; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
}

/*================================================*/
/*=         REDIMENSIONNEMENT HORIZONTAL         =*/
static __inline__ void g3x_SetUpScrollh(void)
{
  Xscrollend   = curwidth - scrollwidth;
  Xscrollrange = Xscrollend-Xscrollstart;

  G3Xscroll *scroll=_SCROLLH_;
  while (scroll<_SCROLLH_+scrollhnum)
  {
    scroll->xcurs  = (int)(Xscrollstart+scroll->cursor*Xscrollrange);
    scroll++;
  }
}

/*================================================*/
/*=         REDIMENSIONNEMENT VERTICTAL          =*/
static __inline__ void g3x_SetUpScrollv(void)
{
  Yscrollend   = curheight - Ybutpos - scrollwidth;
  Yscrollrange = Yscrollend-Yscrollstart;
  G3Xscroll *scroll=_SCROLLV_;
  while (scroll<_SCROLLV_+scrollvnum)
  {
    scroll->ycurs  = (int)(Yscrollstart+scroll->cursor*Yscrollrange);
    scroll++;
  }
}

/*================================================*/
/*=        LIBERATION DE TOUS LES SCROLLS        =*/
static __inline__ void g3x_FreeScrolls()
{
  if (_SCROLLH_) { free(_SCROLLH_); TheScrollh=_SCROLLH_=NULL; }
  if (_SCROLLV_) { free(_SCROLLV_); TheScrollv=_SCROLLV_=NULL; }
}

