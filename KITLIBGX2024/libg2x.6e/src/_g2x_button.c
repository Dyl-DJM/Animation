/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G2X - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ================================================================!*/

/*===================================*/
/*=  GESTION DES BOUTONS POUSSOIRS  =*/
/*===================================*/

static G2Xbut *_BUTTON_=NULL,*TheButton=NULL;
static int      butnum=0;

static int _button_id_on_=0,_button_id_off_=0;

/*=  Attribue un numero et un texte au bouton  =*/
extern int g2x_CreateButton(const char *name, const char *info)
{
  if (!_BUTTON_)
    if (!(_BUTTON_=(G2Xbut*)calloc(BLOCSIZE,sizeof(G2Xbut)))) return false;

  G2Xbut *but = _BUTTON_+butnum;

  strncpy(but->name,name,NAMESIZE);
  if (info) strncpy(but->info,info,127);
  but->len=glutBitmapLength(GLUT_BITMAP_HELVETICA_10,(const uchar*)name);

  Ydialwidth  = MAX(Ydialwidth,but->len+8);
  Xbutpos     = Ydialwidth/2;
  Xbutw       = Ydialwidth/2-2;

  but->num  = butnum++;
  but->x    = Xbutpos;
  but->y    = Ybutpos; Ybutpos+=22;
  but->on   = false;
  return true;
}

/*=  Renvoie le numero du bouton selectionne =*/
extern int g2x_GetButton(void) { return TheButton?(int)TheButton->num:-1; }

/*=  Debranche tous les boutons =*/
extern void g2x_ReleaseButton(void)
{
  if (!TheButton) return;
  TheButton->on=false;
  TheButton=NULL;
}


/*= Si un bouton est selectionne, son numero =*/
/*= est mis dans la var. glob. <TheButton>   =*/
static __inline__ bool g2x_SelectButton(int x, int y)
{
  G2Xbut *but=_BUTTON_;
  while (but<_BUTTON_+butnum)
  {
    if (abs(but->x-x)<2*Xbutw && abs(but->y-y)<10)
    {
      pushmode=GLUT_DOWN;
      if (TheButton!=NULL) {                TheButton->on=false; }
      if (TheButton!=but ) { TheButton=but; TheButton->on=true;  }
      else TheButton=NULL;
      return true;
    }
    but++;
  }
  return false;
}

/*= libere les bouttons        =*/
static __inline__ void g2x_FreeButtons()
{
  if (_BUTTON_)
  {
    free(_BUTTON_);
    TheButton=_BUTTON_=NULL;
  }
}

static __inline__ void g2x_InitButOn()
{
  _button_id_on_ = glGenLists(1);
  glNewList(_button_id_on_, GL_COMPILE);
    glPolygonMode(GL_FRONT,GL_FILL);
    glBegin(GL_TRIANGLE_FAN);
      g2x_Color4fv(G2Xy_b);
      glVertex2i(0,0);
      g2x_Color4fv(G2Xg_a);
      glVertex2i(-Xbutw,-8);
      glVertex2i(-Xbutw,+8);
      glVertex2i(+Xbutw,+8);
      glVertex2i(+Xbutw,-8);
      glVertex2i(-Xbutw,-8);
    glEnd();
    glPolygonMode(GL_FRONT,GL_LINE);
  glEndList();
}

static __inline__ void g2x_InitButOff()
{
  _button_id_off_ = glGenLists(1);
  glNewList(_button_id_off_, GL_COMPILE);
    glPolygonMode(GL_FRONT,GL_FILL);
    glBegin(GL_TRIANGLE_FAN);
      g2x_Color4fv(G2Xo_b);
      glVertex2i(0,0);
      g2x_Color4fv(G2Xr_a);
      glVertex2i(-Xbutw,-8);
      glVertex2i(-Xbutw,+8);
      glVertex2i(+Xbutw,+8);
      glVertex2i(+Xbutw,-8);
      glVertex2i(-Xbutw,-8);
    glEnd();
    glPolygonMode(GL_FRONT,GL_LINE);
  glEndList();
}

/*=  dessinne tous les boutons  =*/
static __inline__ void g2x_DrawButtons(void)
{
  G2Xbut  *but=_BUTTON_;
  G2Xcolor col;
  while (but<_BUTTON_+butnum)
  {
    glPushMatrix();
      glTranslatef(Xbutpos,curheight-but->y,0.);
      glCallList(but->on?_button_id_on_:_button_id_off_);
      glRasterPos2i(12,-4);
      col   = G2Xk;
      col.a = 1.-col.a;
      g2x_Color4fv(col);
      glRasterPos2i(-but->len/2,-4);
      glutBitmapString(GLUT_BITMAP_HELVETICA_10,(const uchar*)but->name);
      glRasterPos2i(1-but->len/2,-4);
      glutBitmapString(GLUT_BITMAP_HELVETICA_10,(const uchar*)but->name);
    glPopMatrix();
    but++;
  }
}
