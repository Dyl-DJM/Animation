/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G3X - Fonctions de base d'acces public -
                    version 6e - Jan.2022
  ================================================================!*/

/*====================================================================*/
/*=      Gestion des 'clic' souris                                   =*/
/*====================================================================*/
#define WHEEL_SCROLL_UP   3
#define WHEEL_SCROLL_DOWN 4

static bool       LEFT_CLIC = false;
static G3Xpoint  _CLIC_POS_  = { 0., 0., 0. };
static G3Xpoint  _MOUSE_POS_ = { 0., 0., 0. };
static G3Xvector _MOUSE_VCT_ = { 0., 0., 0. };
static GLint startx,starty;

static void (*MouseMoveAction)(G3Xpoint) = NULL;

extern G3Xpoint* g3x_GetClic(void)
{
 if (!LEFT_CLIC) return NULL;
 LEFT_CLIC = false;
 return &_CLIC_POS_;
}

static __inline__ void g3x_CornMouseClic(int button, int state, int x, int y)
{
  glutSetWindow(cornwin);
  if (button!=GLUT_LEFT_BUTTON) return;
  if (state == GLUT_UP)
  {
    pushmode=GLUT_UP;
    moving=false;
    return;
  }
  if (pushmode!=GLUT_UP) return;
  _INFO_=!_INFO_;
  glutPostRedisplay();
  glutShowWindow();
  glutSetWindow(mainwin);
  glutShowWindow();
  return;
}

static __inline__ void g3x_XDialMouseClic(int button, int state, int x, int y)
{
  glutSetWindow(xdialwin);
  if (button!=GLUT_LEFT_BUTTON) return;
  if (state == GLUT_UP)
  {
    pushmode=GLUT_UP;
    moving=false;
    return;
  }
  if (pushmode!=GLUT_UP) return;
  /*! clic Scrollbar!*/
  G3Xscroll *scroll=_SCROLLH_;
  y = Xdialheight-y;
  while (scroll<_SCROLLH_+scrollhnum && pushmode==GLUT_UP)
  {
    if (abs((int)scroll->ycurs-y)<scroll->w && x>(Xscrollstart-2*scroll->w) && x<(Xscrollend  +2*scroll->w))
    {
      moving = true+button;
      pushmode=GLUT_DOWN;
      TheScrollh=scroll;
      TheScrollh->xcurs  = CLIP(Xscrollstart,x,Xscrollend);
      TheScrollh->cursor = (double)(TheScrollh->xcurs-Xscrollstart)/(double)Xscrollrange;
      if (TheScrollh->dprm)
         *TheScrollh->dprm   = (double)(TheScrollh->min + pow(TheScrollh->cursor,TheScrollh->fac)*(TheScrollh->max-TheScrollh->min));
      else
      if (TheScrollh->iprm)
         *TheScrollh->iprm   = (   int)(TheScrollh->min + pow(TheScrollh->cursor,TheScrollh->fac)*(TheScrollh->max-TheScrollh->min));
      glutPostRedisplay();
      glutShowWindow(); glutSetWindow(mainwin); glutShowWindow();
      return;
    }
    scroll++;
  }
  return;
}

static __inline__ void g3x_YDialMouseClic(int button, int state, int x, int y)
{
  glutSetWindow(ydialwin);
  if (button==GLUT_MIDDLE_BUTTON) { moving=true+button; return; }
  if (button!=GLUT_LEFT_BUTTON  ) return;

  if (state == GLUT_UP)
  {
    pushmode=GLUT_UP;
    moving=false;
    return;
  }
  if (pushmode!=GLUT_UP) return;
  /*! clic boutons!*/
  if (g3x_SelectPopUp(x,y) || g3x_SelectSwitch(x,y) || g3x_SelectButton(x,y))
  {
    if (ThePopUp && ThePopUp->on)
    {
      ThePopUp->action();
      ThePopUp->on=false;
    }
    ThePopUp =NULL;
    ThePopUp=NULL;
    glutPostRedisplay();
    glutShowWindow(); glutSetWindow(mainwin); glutShowWindow();
    return;
  }
  /*! clic Scrollbar!*/
  y = curheight-y;
  G3Xscroll *scroll=_SCROLLV_;
  while (scroll<_SCROLLV_+scrollvnum && pushmode==GLUT_UP)
  {
    if (abs((int)scroll->xcurs-x)<scroll->w && y>(Yscrollstart-2*scroll->w)
                                            && y<(Yscrollend  +2*scroll->w) )
    {
      moving = true+button;
      pushmode=GLUT_DOWN;
      TheScrollv=scroll;
      TheScrollv->ycurs = CLIP(Yscrollstart,y,Yscrollend);
      TheScrollv->cursor= (double)(TheScrollv->ycurs-Yscrollstart)/(double)Yscrollrange;
      if (TheScrollv->dprm)
         *TheScrollv->dprm   = (double)(TheScrollv->min + pow(TheScrollv->cursor,TheScrollv->fac)*(TheScrollv->max-TheScrollv->min));
      else
      if (TheScrollv->iprm)
         *TheScrollv->iprm   = (   int)(TheScrollv->min + pow(TheScrollv->cursor,TheScrollv->fac)*(TheScrollv->max-TheScrollv->min));
      glutPostRedisplay();
      glutShowWindow(); glutSetWindow(mainwin); glutShowWindow();
        return;
    }
    scroll++;
  }
  return;
}

static __inline__ void g3x_DrawMouseClic(int button, int state, int x, int y)
{

  glutSetWindow(drawwin);
  if (state == GLUT_UP) return (void)(moving=false);
  startx = x;
  starty = y;
  switch(button)
  {
    case WHEEL_SCROLL_UP   :
      _main_cam_.dist*=1.05;
      /* mise a jour des coord. cartesiennes de la camera */
      _main_cam_.pos->x *=1.05;
      _main_cam_.pos->y *=1.05;
      _main_cam_.pos->z *=1.05;
      glutPostRedisplay();
      return;
    case WHEEL_SCROLL_DOWN :
      _main_cam_.dist/=1.05;
      /* mise a jour des coord. cartesiennes de la camera */
      _main_cam_.pos->x /=1.05;
      _main_cam_.pos->y /=1.05;
      _main_cam_.pos->z /=1.05;
      glutPostRedisplay();
      return;
    default :
      moving = true+button;
      /* recupere les coord. spherique de la camera */
      theta_cam_init  = _main_cam_.theta;
      phi_cam_init    = _main_cam_.phi;
      dist_cam_init   = _main_cam_.dist;
        /* recupere les coord. spherique de la lumiere */
      theta_spot_init = _main_spot_.theta;
      phi_spot_init   = _main_spot_.phi;
      dist_spot_init  = _main_spot_.dist;
      _main_cam_.dx   = _main_cam_.dy =0.;
      return;
  }
}

/*====================================================================*/
/*=  Gestion des mouvements de la souris  (clic gauche)              =*/
/*====================================================================*/
/* pilotes mouvements de la souris : camera ou lumiere */
#define _CLIC_CONTROL_CAM_SPHR_  0
#define _CLIC_CONTROL_SPOT_SPHR_ 1
/* valeur par defaut : camera */
static int _CLIC_CONTROLLER_ = _CLIC_CONTROL_CAM_SPHR_;

static double _mouse_speed_ = 0.005;

extern double g3x_GetMouseSpeed() { return _mouse_speed_; }
extern void   g3x_SetMouseSpeed(double speed) { _mouse_speed_=speed; }

static __inline__ void g3x_DrawMouseMove(int x, int y)
{
  double dx,dy;
  glutSetWindow(drawwin);
  switch (_CLIC_CONTROLLER_)
  { /* fonctionnalite mouvements attache aux mouvements souris */
    case _CLIC_CONTROL_CAM_SPHR_ : /* Contrôle la Camera    */
      switch (moving)
      {
        case true+GLUT_LEFT_BUTTON   :
          g3x_SetCameraSpheric(theta_cam_init -_mouse_speed_*(x-startx),
                               phi_cam_init   +_mouse_speed_*(y-starty),
                               _main_cam_.dist);
        break;
        case true+GLUT_MIDDLE_BUTTON  : // pan scan
          dx = _mouse_speed_*(x-startx);
          dy = _mouse_speed_*(y-starty);
          dy*= (fabs(_main_cam_.phi)>(PI/4)?1.0:-1.0);
          g3x_PanScan(&_main_cam_,dx,dy);
          startx=x;
          starty=y;
        break;
      }
    break;
     case _CLIC_CONTROL_SPOT_SPHR_ :  /* Contrôle la Lumiere */
       switch (moving)
        {
          case true+GLUT_LEFT_BUTTON   :
           _main_spot_.theta = theta_spot_init + 0.005*(x-startx);
           _main_spot_.phi   = phi_spot_init   - 0.005*(y-starty);
           /* mise a jour des coord. cartesiennes de la lumiere */
           _main_spot_.pos.x = _main_spot_.dist*cos(_main_spot_.theta)*cos(_main_spot_.phi);
           _main_spot_.pos.y = _main_spot_.dist*sin(_main_spot_.theta)*cos(_main_spot_.phi);
           _main_spot_.pos.z = _main_spot_.dist*sin(_main_spot_.phi);
        break;
        case true+GLUT_MIDDLE_BUTTON  :
           _main_spot_.dist = dist_spot_init+0.05*(y-starty);
           /* mise a jour des coord. cartesiennes de la lumiere */
           _main_spot_.pos.x = _main_spot_.dist*cos(_main_spot_.theta)*cos(_main_spot_.phi);
           _main_spot_.pos.y = _main_spot_.dist*sin(_main_spot_.theta)*cos(_main_spot_.phi);
           _main_spot_.pos.z = _main_spot_.dist*sin(_main_spot_.phi);
        break;
      }
    break;
  }
  glutPostRedisplay();
}

/*= recupere la position de la souris =*/
extern G3Xpoint g3x_GetMousePosition(void) { return _CLIC_POS_; }
extern void g3x_SetMouseMoveAction(void (*action)(G3Xpoint)) { MouseMoveAction = action;}

/*=   PASSIVE MOUSE FUNCTIONS =*/
static __inline__ void g3x_DrawPassiveMouseMove(int x, int y)
{
  glutSetWindow(drawwin);
  y = curheight - y;
  _CLIC_POS_.x = x ;
  _CLIC_POS_.y = y ;
  _CLIC_POS_.z = 0.;
  if(MouseMoveAction) (*MouseMoveAction)(_CLIC_POS_);
}

/*=   MOVE MOUSE FUNCTIONS =*/
static __inline__ void g3x_XDialMouseMove(int x, int y)
{
  glutSetWindow(xdialwin);
  y = curheight - y;
  switch (moving)
  {
    case true+GLUT_LEFT_BUTTON   :
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
      glutPostWindowRedisplay(mainwin);
      return;
  }
}

static __inline__ void g3x_YDialMouseMove(int x, int y)
{
  glutSetWindow(ydialwin);
  y = curheight - y;
  switch (moving)
  {
    case true+GLUT_LEFT_BUTTON   :
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
      glutPostWindowRedisplay(mainwin);
      return;
  }
}


//*====================================================================*/
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
static const char* _VID_CODEC_[] = {"mp4","flv","x264","mpg2",NULL};

static __inline__ void g3x_MainMenu(int item)
{
  if (item<_MENU_FORM_)            /* souris : camera ou lumiere (spécifique 3D) */
  {
    const char **f=_RIGHT_BUT_;
    item-=_MENU_CLIC_;
    while (item--) f++;
    _CLIC_CONTROLLER_ = (int)(f-_RIGHT_BUT_);
    return;
  }
  if (item<_MENU_MPEG_)            /* choix format snapshot */
  {
    const char **f=_IMG_CODEC_;
    item-=_MENU_FORM_;
    while (item--) f++;
    /*if (!_FULLSCREEN_)*/ g3x_Snapshot(*f,_WINDNAME_,curwidth,curheight);
    return;
  }
  if (item <_MENU_EXIT_)           /* choix format video    */
  {
    const char **f=_VID_CODEC_;
    item-=_MENU_MPEG_;
    _VIDEO_++;
    while (item--) { f++; _VIDEO_++; }
    g3x_PlugCapture(_WINDNAME_,0,0,curwidth,curheight);
    return;
  }
  if (item==_MENU_EXIT_) return g3x_Quit(); /* exit */
}


static void g3x_SubMenu(void)
{
/* CONSTRUCTION DU MENU SOURIS */
  const char **f;
  int   mm=_MENU_CLIC_ ,submenum;
  int   mf=_MENU_FORM_ ,submenuf;
  int   mv=_MENU_MPEG_ ,submenuv;
  submenum=glutCreateMenu(g3x_MainMenu);
  glutAddMenuEntry("visee",mm);
  /* fonctionalites clic droit (camera ou lumiere) */
  submenum=glutCreateMenu(g3x_MainMenu);
  f=_RIGHT_BUT_;
  while (*f) glutAddMenuEntry(*f++,mm++);
  /* choix format et snapshot        */
  submenuf=glutCreateMenu(g3x_MainMenu);
  f=_IMG_CODEC_;
  while (*f) glutAddMenuEntry(*f++,mf++);
  /* choix format et lancement video */
  if (_IDLE_==true)
  {
    submenuv=glutCreateMenu(g3x_MainMenu);
    f=_VID_CODEC_;
    while (*f) glutAddMenuEntry(*f++,mv++);
  }

  glutCreateMenu(g3x_MainMenu);
  glutAddSubMenu("mouse",submenum);
  glutAddSubMenu("photo",submenuf);
  if (g3x_Idle)  glutAddSubMenu("video",submenuv);
  glutAddMenuEntry("exit ",_MENU_EXIT_);
  glutAttachMenu(GLUT_RIGHT_BUTTON);

}
