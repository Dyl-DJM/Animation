/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G3X - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ================================================================!*/

/*===================================*/
/*=  GESTION DES BOUTONS POUSSOIRS  =*/
/*===================================*/
static G3Xpopup *_POPUP_=NULL,*ThePopUp=NULL;
static int       popnum=0;

static int _popup_id_on_=0,_popup_id_off_=0;

/*=  Attribue un numero et un texte au bouton  =*/
extern bool g3x_CreatePopUp(const char *name, void (*action)(void), const char *info)
{
  if (!_POPUP_)
    if (!(_POPUP_=(G3Xpopup*)calloc(BLOCSIZE,sizeof(G3Xpopup)))) return false;

  G3Xpopup *pop = _POPUP_+popnum;

  strncpy(pop->name,name,NAMESIZE);
  if (info) strncpy(pop->info,info,127);
  pop->len=glutBitmapLength(GLUT_BITMAP_8_BY_13,(const uchar*)name);

  Ydialwidth  = MAX(Ydialwidth,pop->len+8);
  Xbutpos     = Ydialwidth/2;
  Xbutw       = Ydialwidth/2-2;

  pop->num    = popnum;
  pop->action = action;
  pop->on     = false;
  pop->x      = Xbutpos;
  pop->y      = Ybutpos; Ybutpos+=22;
  popnum++;
  return true;
}

/*=  Renvoie le numero du bouton selectionne =*/
extern int g3x_GetPopUp(void) { return ThePopUp?(int)ThePopUp->num:-1; }


/*= Si un bouton est selectionne, son numero =*/
/*= est mis dans la var. glob. <ThePopUp>   =*/
static __inline__ bool g3x_SelectPopUp(int x, int y)
{
  G3Xpopup *pop=_POPUP_;
  while (pop<_POPUP_+popnum)
  {
    if (abs(pop->x-x)<2*Xbutw && abs(pop->y-y)<10)
    {
      pushmode=GLUT_DOWN;
      ThePopUp=pop;
      ThePopUp->on=true;
      return true;
    }
    pop++;
  }
  return false;
}

/*= libere les bouttons        =*/
static __inline__ void g3x_FreePopUps()
{
  if (_POPUP_)
  {
    free(_POPUP_);
    ThePopUp=_POPUP_=NULL;
  }
}

static __inline__ void g3x_InitPopOn()
{
  _popup_id_on_ = glGenLists(1);
  glNewList(_popup_id_on_, GL_COMPILE);
    glPolygonMode(GL_FRONT,GL_FILL);
    glBegin(GL_TRIANGLE_FAN);
      g3x_Color4fv(G3Xg_b);
      glVertex2i(0,0);
      g3x_Color4fv(G3Xk_b);
      glVertex2i(-Xbutw,-8);
      glVertex2i(+Xbutw,-8);
      glVertex2i(+Xbutw,+8);
      glVertex2i(-Xbutw,+8);
      glVertex2i(-Xbutw,-8);
    glEnd();
    glPolygonMode(GL_FRONT,GL_LINE);
  glEndList();
}

static __inline__ void g3x_InitPopOff()
{
  _popup_id_off_ = glGenLists(1);
  glNewList(_popup_id_off_, GL_COMPILE);
    glPolygonMode(GL_FRONT,GL_FILL);
    glBegin(GL_TRIANGLE_FAN);
      g3x_Color4fv(G3Xb_b);
      glVertex2i(0,0);
      g3x_Color4fv(G3Xk_c);
      glVertex2i(-Xbutw,-8);
      glVertex2i(+Xbutw,-8);
      glVertex2i(+Xbutw,+8);
      glVertex2i(-Xbutw,+8);
      glVertex2i(-Xbutw,-8);
    glEnd();
    glPolygonMode(GL_FRONT,GL_LINE);
  glEndList();
}


/*=  dessinne tous les boutons  =*/
static __inline__ void g3x_DrawPopUps(void)
{
  G3Xpopup  *pop=_POPUP_;
  G3Xcolor   col;
  while (pop<_POPUP_+popnum)
  {
    glPushMatrix();
      glTranslatef(Xbutpos,curheight-pop->y,0.);
      glCallList(pop->on?_popup_id_on_:_popup_id_off_);
      glRasterPos2i(12,-4);
      col = pop->on?G3Xk:G3Xw;
      col.a = 1.-col.a;
      g3x_Color4fv(col);
      glRasterPos2i(-pop->len/2,-4);
      glutBitmapString(GLUT_BITMAP_8_BY_13,(const uchar*)pop->name);
      glRasterPos2i(1-pop->len/2,-4);
      glutBitmapString(GLUT_BITMAP_8_BY_13,(const uchar*)pop->name);
    glPopMatrix();
    pop++;
  }
}
