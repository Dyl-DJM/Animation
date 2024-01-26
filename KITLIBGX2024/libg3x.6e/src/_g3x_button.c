/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G3X - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ================================================================!*/

/*===================================*/
/*=  GESTION DES BOUTONS POUSSOIRS  =*/
/*===================================*/

static G3Xbut *_BUTTON_=NULL,*TheButton=NULL;
static int      butnum=0;

static int _button_id_on_=0,_button_id_off_=0;

/*=  Attribue un numero et un texte au bouton  =*/
extern int g3x_CreateButton(const char *name, const char *info)
{
  if (!_BUTTON_)
    if (!(_BUTTON_=(G3Xbut*)calloc(BLOCSIZE,sizeof(G3Xbut)))) return false;

  G3Xbut *but = _BUTTON_+butnum;

  strncpy(but->name,name,NAMESIZE);
  if (info) strncpy(but->info,info,127);
  but->len=glutBitmapLength(GLUT_BITMAP_8_BY_13,(const uchar*)name);

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
extern int g3x_GetButton(void) { return TheButton?(int)TheButton->num:-1; }

/*=  Debranche tous les boutons =*/
extern void g3x_ReleaseButton(void)
{
  if (!TheButton) return;
  TheButton->on=false;
  TheButton=NULL;
}


/*= Si un bouton est selectionne, son numero =*/
/*= est mis dans la var. glob. <TheButton>   =*/
static __inline__ bool g3x_SelectButton(int x, int y)
{
  G3Xbut *but=_BUTTON_;
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
static __inline__ void g3x_FreeButtons()
{
  if (_BUTTON_)
  {
    free(_BUTTON_);
    TheButton=_BUTTON_=NULL;
  }
}

static __inline__ void g3x_InitButOn()
{
  _button_id_on_ = glGenLists(1);
  glNewList(_button_id_on_, GL_COMPILE);
    glPolygonMode(GL_FRONT,GL_FILL);
    glBegin(GL_TRIANGLE_FAN);
      g3x_Color4fv(G3Xy_b);
      glVertex2i(0,0);
      g3x_Color4fv(G3Xg_a);
      glVertex2i(-Xbutw,-8);
      glVertex2i(-Xbutw,+8);
      glVertex2i(+Xbutw,+8);
      glVertex2i(+Xbutw,-8);
      glVertex2i(-Xbutw,-8);
    glEnd();
    glPolygonMode(GL_FRONT,GL_LINE);
  glEndList();
}

static __inline__ void g3x_InitButOff()
{
  _button_id_off_ = glGenLists(1);
  glNewList(_button_id_off_, GL_COMPILE);
    glPolygonMode(GL_FRONT,GL_FILL);
    glBegin(GL_TRIANGLE_FAN);
      g3x_Color4fv(G3Xo_b);
      glVertex2i(0,0);
      g3x_Color4fv(G3Xr_a);
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
static __inline__ void g3x_DrawButtons(void)
{
  G3Xbut  *but=_BUTTON_;
  G3Xcolor col;
  while (but<_BUTTON_+butnum)
  {
    glPushMatrix();
      glTranslatef(Xbutpos,curheight-but->y,0.);
      glCallList(but->on?_button_id_on_:_button_id_off_);
      glRasterPos2i(12,-4);
      col   = G3Xk;
      col.a = 1.-col.a;
      g3x_Color4fv(col);
      glRasterPos2i(-but->len/2,-4);
      glutBitmapString(GLUT_BITMAP_8_BY_13,(const uchar*)but->name);
      glRasterPos2i(1-but->len/2,-4);
      glutBitmapString(GLUT_BITMAP_8_BY_13,(const uchar*)but->name);
    glPopMatrix();
    but++;
  }
}
