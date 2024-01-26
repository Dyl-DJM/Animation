/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G3X - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ================================================================!*/

/*===================================*/
/*=  GESTION DES BOUTONS POUSSOIRS  =*/
/*===================================*/

#define _SWITCHWIDTH_ 15
static G3Xswitch *_SWITCH_=NULL,*TheSwitch=NULL;
static int       switchnum=0;

static int _switch_id_on_=0,_switch_id_off_=0,
           _info_id_on_  =0,_info_id_off_  =0;


/*=  Attribue un numero et un texte au switch  =*/
extern bool g3x_CreateSwitch(const char *name, bool *flag, const char *info)
{
  if (!_SWITCH_)
    if (!(_SWITCH_=(G3Xswitch*)calloc(BLOCSIZE,sizeof(G3Xswitch)))) return false;

  G3Xswitch *swt = _SWITCH_+switchnum;

  strncpy(swt->name,name,NAMESIZE);
  if (info) strncpy(swt->info,info,127);
  swt->len = 0;
  swt->len=glutBitmapLength(GLUT_BITMAP_8_BY_13,(const uchar*)name);

  Ydialwidth  = MAX(Ydialwidth,swt->len+32);
  Xbutpos     = Ydialwidth/2;
  Xbutw       = Ydialwidth/2-2;

  swt->num    = switchnum;
  swt->flag   = flag;
  swt->x      = Xbutpos;
  swt->y      = Ybutpos; Ybutpos+=22;
  switchnum++;
  return true;
}

/*=  Renvoie le numero du switch selectionne =*/
extern int g3x_GetSwitch(void) { return TheSwitch?(int)TheSwitch->num:-1; }


/*= Si un switch est selectionne, son numero =*/
/*= est mis dans la var. glob. <TheSwitch>   =*/
static __inline__ bool g3x_SelectSwitch(int x, int y)
{
  G3Xswitch *swt=_SWITCH_;
  while (swt<_SWITCH_+switchnum)
  {
    if (abs(swt->x-x)<2*Xbutw && abs(swt->y-y)<10)
    {
      pushmode=GLUT_DOWN;
     *swt->flag=!*swt->flag;
      return true;
    }
    swt++;
  }
  return false;
}

/*= libere les switch        =*/
static __inline__ void g3x_FreeSwitches()
{
  if (_SWITCH_)
  {
    free(_SWITCH_);
    TheSwitch=_SWITCH_=NULL;
  }
}



static __inline__ void g3x_InitSwitchOn()
{
  int   r  = _SWITCHWIDTH_;
  int   rr = r*r;
  float irr= 1./rr;
  int   x,y,yy;
  float d;

  _switch_id_on_ = glGenLists(1);
  glNewList(_switch_id_on_, GL_COMPILE);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT,GL_FILL);
    glBegin(GL_POINTS);
    for (y=-r; y<=+r; y++)
    {
      yy=y*y;
      for (x=-r; x<=+r; x++)
      {
        if ((d=(float)(x*x+yy))>rr) continue;
        d=(1.-d*irr);
        d=d*d;
        glColor4f(0.,d,0,d);
        glVertex2i(x,y);
      }
    }
    glEnd();
    glPolygonMode(GL_FRONT,GL_LINE);
    glEndList();
}

static __inline__ void g3x_InitSwitchOff()
{
  int   r  = _SWITCHWIDTH_;
  int   rr = r*r;
  float irr= 1./rr;
  int   x,y,yy;
  float d;

  _switch_id_off_ = glGenLists(1);
  glNewList(_switch_id_off_, GL_COMPILE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT,GL_FILL);
    glBegin(GL_POINTS);
    for (y=-r; y<=+r; y++)
    {
      yy=y*y;
      for (x=-r; x<=+r; x++)
      {
        if ((d=(float)(x*x+yy))>rr) continue;
        d=(1.-d*irr);
        d=d*d;
        glColor4f(d,0.,0,d);
        glVertex2i(x,y);
      }
    }
    glPolygonMode(GL_FRONT,GL_LINE);
    glEnd();
    glEndList();
}


static __inline__ void g3x_DrawSwitches(void)
{
  for (G3Xswitch  *swt=_SWITCH_; swt<_SWITCH_+switchnum; swt++)
  {
    glPushMatrix();
      glTranslatef(+_SWITCHWIDTH_,curheight-swt->y,0.);
      glCallList(*swt->flag?_switch_id_on_:_switch_id_off_);
      glColor4f(G3Xk.r,G3Xk.g,G3Xk.b,1.-G3Xk.a);
      glRasterPos2i(_SWITCHWIDTH_,-4);
      glutBitmapString(GLUT_BITMAP_8_BY_13,(const uchar*)swt->name);
    glPopMatrix();
  }
}

static void g3x_InitCornSwitchOn()
{
  _info_id_on_ = glGenLists(1);
  glNewList(_info_id_on_, GL_COMPILE);
    int x=-glutBitmapWidth(GLUT_BITMAP_HELVETICA_18,'?')/2;
    glColor4f(G3Xk.r,G3Xwc.g,G3Xwc.b,1.-G3Xwc.a);
    glRasterPos2i(x+1,-5);glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,'?');
    glColor4f(G3Xw.r,G3Xw.g,G3Xw.b,1.-G3Xw.a);
    glRasterPos2i(x-1,-5);glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,'?');
  glEndList();
}

static void g3x_InitCornSwitchOff()
{
  _info_id_off_ = glGenLists(1);
  glNewList(_info_id_off_, GL_COMPILE);
    int x=-glutBitmapWidth(GLUT_BITMAP_HELVETICA_18,'?')/2;
    G3Xcolor col = G3Xwb;
    col.a = 1.-col.a;
    g3x_Color4fv(col);
    glRasterPos2i(x+1,-5);glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'?');
    col = G3Xk;
    col.a = 1.-col.a;
    g3x_Color4fv(col);
    glRasterPos2i(x-1,-5);glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,'?');
  glEndList();
}

static bool _info_switch_=false;
static void g3x_DrawInfoSwitch(void)
{
  glPushMatrix();
    glTranslatef(Ydialwidth/2,Xdialheight/2,0.);
    glCallList(_info_switch_?_info_id_on_:_info_id_off_);
  glPopMatrix();
}

