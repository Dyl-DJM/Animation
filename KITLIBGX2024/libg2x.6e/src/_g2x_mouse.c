/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G2X - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ================================================================!*/

/*====================================================================*/
/*=      Gestion des 'clic' souris                                   =*/
/*====================================================================*/
static bool     _LEFT_CLIC_ = false;
static G2Xpoint _CLIC_POS_  = { 0, 0 };
static G2Xpoint _MOUSE_POS_ = { 0, 0 }; /*position en temps réel de la souris.*/
static GLint startx,starty; /* La position du début de mouvmenent avec clic enfoncé (quel qu'il soit)*/
static GLint winmsx,winmsy;

static void (*MouseMoveAction)(G2Xpoint) = NULL;

extern G2Xpoint* g2x_GetClic(void)
{
// _CLIC_POS_.x=winmsx;
// _CLIC_POS_.y=winmsy;
 if (!_LEFT_CLIC_) return NULL;
 _LEFT_CLIC_ = false;
 return &_CLIC_POS_;
}

static __inline__ void g2x_CornMouseClic(int button, int state, int x, int y)
{
  glutSetWindow(cornwin);
  if (button!=MOUSE_LEFT) return;
  if (state == MOUSE_PRESSED)
  {
    pushmode=MOUSE_PRESSED;
    moving=false;
    return;
  }
  if (pushmode!=MOUSE_PRESSED) return;
  _INFO_ =!_INFO_ ;
  glutPostRedisplay();
  glutShowWindow();
  glutSetWindow(mainwin);
  glutShowWindow();
  return;
}

static __inline__ void g2x_XDialMouseClic(int button, int state, int x, int y)
{
  glutSetWindow(xdialwin);
  if (button!=MOUSE_LEFT) return;
  if (state == MOUSE_PRESSED)
  {
    pushmode=MOUSE_PRESSED;
    moving=false;
    return;
  }
  if (pushmode!=MOUSE_PRESSED) return;
  /*! clic Scrollbar!*/
  G2Xscroll *scroll=_SCROLLH_;
  y = Xdialheight-y;
  while (scroll<_SCROLLH_+scrollhnum && pushmode==MOUSE_PRESSED)
  {
    if (abs((int)scroll->ycurs-y)<scroll->w && x>(Xscrollstart-2*scroll->w) && x<(Xscrollend  +2*scroll->w))
    {
      moving = true+button;
      pushmode=MOUSE_RELEASED;
      TheScrollh=scroll;
      TheScrollh->xcurs  = CLIP(Xscrollstart,x,Xscrollend);
      TheScrollh->cursor = (double)(TheScrollh->xcurs-Xscrollstart)/(double)Xscrollrange;
      if (TheScrollh->dprm)
         *TheScrollh->dprm   = (double)(TheScrollh->min + TheScrollh->cursor*(TheScrollh->max-TheScrollh->min));
      else
      if (TheScrollh->iprm)
         *TheScrollh->iprm   = (   int)(TheScrollh->min + TheScrollh->cursor*(TheScrollh->max-TheScrollh->min));
      glutPostRedisplay();
      glutShowWindow();
      glutSetWindow(mainwin);
      glutShowWindow();
      return;
    }
    scroll++;
  }
  return;
}

static __inline__ void g2x_YDialMouseClic(int button, int state, int x, int y)
{
  glutSetWindow(ydialwin);
  startx = x;
  starty = y;
  if (button==MOUSE_MIDDLE) { moving=true+button; return; }
  if (button!=MOUSE_LEFT  ) return;

  if (state == MOUSE_PRESSED)
  {
    pushmode=MOUSE_PRESSED;
    moving=false;
    return;
  }
  if (pushmode!=MOUSE_PRESSED) return;
  /*! clic boutons!*/
  if (g2x_SelectPopUp(x,y) || g2x_SelectSwitch(x,y) || g2x_SelectButton(x,y) || g2x_SelectExclButton(x, y))
  {
    if (ThePopUp && ThePopUp->on)
    {
      ThePopUp->action();
      ThePopUp->on=false;
    }
    ThePopUp=NULL;
    glutPostRedisplay();
    glutShowWindow(); glutSetWindow(mainwin); glutShowWindow();
    return;
  }
  /*! clic Scrollbar!*/
  y = curheight-y;
  G2Xscroll *scroll=_SCROLLV_;
  while (scroll<_SCROLLV_+scrollvnum && pushmode==MOUSE_PRESSED)
  {
    if (abs((int)scroll->xcurs-x)<scroll->w && y>(Yscrollstart-2*scroll->w)
                                            && y<(Yscrollend  +2*scroll->w) )
    {
      moving = true+button;
      pushmode=MOUSE_RELEASED;
      TheScrollv=scroll;
      TheScrollv->ycurs = CLIP(Yscrollstart,y,Yscrollend);
      TheScrollv->cursor= (double)(TheScrollv->ycurs-Yscrollstart)/(double)Yscrollrange;
      if (TheScrollv->dprm)
         *TheScrollv->dprm   = (double)(TheScrollv->min + TheScrollv->cursor*(TheScrollv->max-TheScrollv->min));
      else
      if (TheScrollv->iprm)
         *TheScrollv->iprm   = (   int)(TheScrollv->min + TheScrollv->cursor*(TheScrollv->max-TheScrollv->min));
      glutPostRedisplay();
      glutShowWindow(); glutSetWindow(mainwin); glutShowWindow();
        return;
    }
    scroll++;
  }
  return;
}

static __inline__ void g2x_DrawMouseClic(int button, int state, int x, int y)
{
  glutSetWindow(drawwin);
  winmsx = x;
  winmsy = curheight-y;
  startx = x;
  starty = y;
  switch(button)
  {
    case WHEEL_SCROLL_UP   : zoom_value*=zoom_factor;  glutPostRedisplay(); break;
    case WHEEL_SCROLL_DOWN : zoom_value/=zoom_factor;  glutPostRedisplay(); break;
    case MOUSE_MIDDLE:
    {
      if (state == MOUSE_PRESSED)
      {
        moving = false;
        return;
      }
      /* HP - mai 2022 : correction panscan */
      XcurrOffset = Xoffset;
      YcurrOffset = Yoffset;
      moving=true+button; return;
    }

    case MOUSE_LEFT :
    {
      if (state == MOUSE_RELEASED) _LEFT_CLIC_=true;
      else if (state == MOUSE_PRESSED)
      { /* HP - mai 2022 : correction panscan */
        pushmode= MOUSE_PRESSED;
        moving  = false;
        _LEFT_CLIC_ = false;
        CPT     = NULL; //buffer Pt. de Contrôle vidé.
        return;
      }
      if (pushmode==MOUSE_PRESSED)
      {
        y = curheight - y;
        _CLIC_POS_.x= G2XpixtowinX1(x);
        _CLIC_POS_.y= G2XpixtowinY1(y);
        /* point de controle ? */
        int    n=0;
        while (n<nbcpt)
        {
          if (fabs(CTRLPTS[n].add->x -_CLIC_POS_.x)<=fabs(CTRLPTS[n].ray*Xpixsize/Xratio) &&
              fabs(CTRLPTS[n].add->y -_CLIC_POS_.y)<=fabs(CTRLPTS[n].ray*Ypixsize/Yratio)  )
          {
            CPT=CTRLPTS[n].add;
            CPTpos=*CPT;
            pushmode=MOUSE_RELEASED;
            moving = true+button;
            return;
          }
          n++;
        }
        // 23/10/2022 : ajout pour prendre en compte le clic => doit réafficher
        glutPostRedisplay();
      }
    }
  }
  return;
}

/*====================================================================*/
/*=  Gestion des mouvements de la souris  (clic gauche)              =*/
/*====================================================================*/
static __inline__ void g2x_DrawMouseMove(int x, int y)
{
  glutSetWindow(drawwin);
  switch (moving)
  { /* HP - mai 2022 : correction panscan */
    case true+MOUSE_MIDDLE :
    {// le panscan.
      Xoffset = XcurrOffset + (+Xpixsize*(x-startx));
      Yoffset = YcurrOffset + (-Ypixsize*(y-starty));
      glutPostWindowRedisplay(drawwin);
      break;
    }
    case true+MOUSE_LEFT :
    { // les points de controle
      y = curheight - y;
      if (nbcpt!=0 && CPT)
      {
          CPT->x = G2XpixtowinX1(x);
          CPT->y = G2XpixtowinY1(y);
          CPT->x = CLIP(Xwmin,CPT->x,Xwmax);
          CPT->y = CLIP(Ywmin,CPT->y,Ywmax);
      }
      glutPostWindowRedisplay(drawwin);
      break;
    }
  }
  _MOUSE_POS_.x=x;
  _MOUSE_POS_.y=y;
}

/*= recupere la position de la souris =*/
extern G2Xpoint g2x_GetMousePosition(void) { return _MOUSE_POS_; }

/*= renvoie FALSE si la souris n'est pas dans le fenêtre(- une marge) =*/
/*! pas très convivial  -- à revoir                                   !*/
/* -------------------
 * |ooooo false ooooo|
 * |ooooooooooooooooo|
 * |oo             oo|
 * |oo             oo|
 * |oo     true    oo|
 * |oo             oo|
 * |ooooooooooooooooo|
 * |ooooooooooooooooo|
 * ------------------- */
extern bool     g2x_MouseInWindow(double margin)
{
  if (_MOUSE_POS_.x <= Xwmin*(1.+margin) || _MOUSE_POS_.x >= Xwmax*(1.-margin) ||
      _MOUSE_POS_.y <= Ywmin*(1.+margin) || _MOUSE_POS_.y >= Ywmax*(1.-margin))
      return false;
  return true;
}

extern void     g2x_SetMouseMoveAction(void (*action)(G2Xpoint))
{ MouseMoveAction = action;}

/*=   PASSIVE MOUSE FUNCTIONS =*/
static __inline__ void g2x_DrawPassiveMouseMove(int x, int y)
{
  glutSetWindow(drawwin);
  y = curheight - y;
  _MOUSE_POS_.x = G2XpixtowinX1(x);
  _MOUSE_POS_.y = G2XpixtowinY1(y);
  if(MouseMoveAction) (*MouseMoveAction)(_MOUSE_POS_);
}

/*=   MOVE MOUSE FUNCTIONS =*/
static __inline__ void g2x_XDialMouseMove(int x, int y)
{
  glutSetWindow(xdialwin);
  y = curheight - y;
  switch (moving)
  {
    case true+MOUSE_LEFT   :
      if (TheScrollh)
      {
        TheScrollh->xcurs = CLIP(Xscrollstart,x,Xscrollend);
        TheScrollh->cursor= (double)(TheScrollh->xcurs-Xscrollstart)/(double)Xscrollrange;
        if (TheScrollh->dprm)
           *TheScrollh->dprm   = (double)(TheScrollh->min + TheScrollh->cursor*(TheScrollh->max-TheScrollh->min));
        else
        if (TheScrollh->iprm)
           *TheScrollh->iprm   = (   int)(TheScrollh->min + TheScrollh->cursor*(TheScrollh->max-TheScrollh->min));
      }
      glutPostRedisplay();
      glutPostWindowRedisplay(drawwin);
  }
}

static __inline__ void g2x_YDialMouseMove(int x, int y)
{
  glutSetWindow(ydialwin);
  y = curheight - y;
  switch (moving)
  {
    case true+MOUSE_LEFT   :
      if (TheScrollv)
      {
        TheScrollv->ycurs = CLIP(Yscrollstart,y,Yscrollend);
        TheScrollv->cursor= (double)(TheScrollv->ycurs-Yscrollstart)/(double)Yscrollrange;
        if (TheScrollv->dprm)
           *TheScrollv->dprm   = (double)(TheScrollv->min + TheScrollv->cursor*(TheScrollv->max-TheScrollv->min));
        else
        if (TheScrollv->iprm)
           *TheScrollv->iprm   = (   int)(TheScrollv->min + TheScrollv->cursor*(TheScrollv->max-TheScrollv->min));
      }
      glutPostRedisplay();
      glutPostWindowRedisplay(drawwin);
  }
}

/*====================================================================*/
/*=  fonction associee aux evenements de menu.                       =*/
/*=  - item : code associe au menu selectionne                       =*/
/*====================================================================*/
#define _MENU_CLIC_ 200
#define _MENU_FORM_ 300
#define _MENU_MPEG_ 400
#define _MENU_EXIT_ 500

/*====================================================================*/
/*=  Gestion des menus de la souris (clic droit)                     =*/
/*====================================================================*/
static const char* _RIGHT_BUT_[] = {"cam","light",NULL};
static const char* _IMG_CODEC_[] = {"jpg","pnm","png","gif","eps","bmp","tif","ras",NULL};
static const char* _VID_CODEC_[] = {"mp4","flv","x264","mpg2","ffmpeg",NULL};

static void g2x_MainMenu(int item)
{
  if (item<_MENU_MPEG_)            /* choix format snapshot */
  {
    const char **f=_IMG_CODEC_;
    item-=_MENU_FORM_;
    while (item--) f++;
    /*if (!_FULLSCREEN_)*/ g2x_Snapshot(*f,_WINDNAME_,curwidth,curheight);
    return;
  }
  if (item <_MENU_EXIT_)           /* choix format video    */
  {
    const char **f=_VID_CODEC_;
    item-=_MENU_MPEG_;
    _VIDEO_++;
    while (item--) { f++; _VIDEO_++; }
    g2x_PlugCapture(_WINDNAME_,0,0,curwidth,curheight);
    return;
  }
  if (item==_MENU_EXIT_) return g2x_Quit(); /* exit */
}

static void g2x_SubMenu(void)
{
  /* CONSTRUCTION DU MENU SOURIS */
  const char **f;
  int   mm=_MENU_CLIC_ ,submenum;
  int   mf=_MENU_FORM_ ,submenuf;
  int   mv=_MENU_MPEG_ ,submenuv;

  /* fonctionalites clic droit */
  submenum=glutCreateMenu(g2x_MainMenu);
  f=_RIGHT_BUT_;
  while (*f) glutAddMenuEntry(*f++,mm++);
  /* choix format et snapshot        */
  submenuf=glutCreateMenu(g2x_MainMenu);
  f=_IMG_CODEC_;
  while (*f) glutAddMenuEntry(*f++,mf++);
  /* choix format et lancement video */
  if (_IDLE_==true)
  {
    submenuv=glutCreateMenu(g2x_MainMenu);
    f=_VID_CODEC_;
    while (*f) glutAddMenuEntry(*f++,mv++);
  }

  glutCreateMenu(g2x_MainMenu);
  glutAddSubMenu("photo",submenuf);
  if (g2x_Idle)  glutAddSubMenu("video",submenuv);
  glutAddMenuEntry("exit ",_MENU_EXIT_);
  glutAttachMenu(MOUSE_RIGHT);
}


/* utilitaire => devra disparaître */
extern void g2x_GetMouseStatut(void)
{
  fprintf(stderr, "<-- Mouse Info -->\n");
  fprintf(stderr, "clic status : %s\n", _LEFT_CLIC_?"true":"false");
  if(_LEFT_CLIC_)
  {
    fprintf(stderr, "\tclic_position = (%f, %f)\n", _CLIC_POS_.x, _CLIC_POS_.y);
  }
  fprintf(stderr, "mouse pos = (%f, %f)\n", _MOUSE_POS_.x, _MOUSE_POS_.y);
  fprintf(stderr, "stratx = %d, starty = %d\n", startx, starty);
  fprintf(stderr, "<-- ---------- -->\n");

}
