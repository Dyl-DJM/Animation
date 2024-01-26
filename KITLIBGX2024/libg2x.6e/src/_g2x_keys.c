/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G2X - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ================================================================!*/

/*===============================================*/
/*=  ASSOCIER DES FONCTIONALITES A DES TOUCHES  =*/
/*===============================================*/

/**@brief   associates an action to a key (ASCII).
 * @warning some keys are pre-defined and thus should not be used to avoid conflict with core functionalities.
 *          |SPACE|ESC|q|Q|Ctrl+f|Ctrl+r|Ctrl+w|+|-|=|?| */
typedef struct
{
  char  key;             /**@brief   key value (ASCII)
                          * @warning see struct G2Xkey documentation for pre-defined behaviour  */
  void (*action)(void);  /**@brief   action to be done at keypressed                            */
  char  info[INFOSIZE+1];/**@brief text for the info pannel (key '?')                           */
} G2Xkey;

static int    keynumb=0;
static int    keyblocnumb=0;
static G2Xkey* _KEY_=NULL;

/*= cree une G2Xkey =*/
extern bool g2x_SetKeyAction(char key, void (*action)(void), const char *info)
{
  if (keyblocnumb==MAXBLOC)
  { fprintf(stderr,"\e[1;31m<glx_SetKeyAction>\e[0;31m %d actions : maximum atteint\e[0;31m\n",MAXBLOC*BLOCSIZE); return false;}
  if (keynumb%BLOCSIZE==0)
  { /* si toutes les GLXkey ont ete attribuees on ajoute un bloc */
    if (!(_KEY_=(G2Xkey*)realloc(_KEY_,(keyblocnumb+1)*BLOCSIZE*sizeof(G2Xkey)))) return false;
    memset(_KEY_+keyblocnumb*BLOCSIZE,0,BLOCSIZE*sizeof(G2Xkey));
    keyblocnumb++;
  }
  _KEY_[keynumb].key    = key;
  _KEY_[keynumb].action = action;
  if (info) snprintf(_KEY_[keynumb].info,INFOSIZE,"%s",info);
  keynumb++;
  return true;
}

/*= Execute l'action associee au car. c =*/
static __inline__ bool g2x_ExecuteKeyAction(char c)
{
  G2Xkey *k;
  for (k=_KEY_; k<_KEY_+keynumb; k++)
    if (c==k->key) { k->action(); return true; }
  return false;
}

/*========================================================*/
/*=  ASSOCIER DES FONCTIONALITES A DES TOUCHES SPECIALES =*/
/*========================================================*/
static int    skeynumb=0;
static int    skeyblocnumb=0;
static G2Xkey* _SKEY_=NULL;

extern bool g2x_SetSpecialKeyAction(char key, void (*action)(void), const char *info)
{
  if (skeyblocnumb==MAXBLOC)
  { fprintf(stderr,"\e[1;31m<g2x_SetSpecialKeyAction>\e[0;31m %d actions : maximum atteint\e[0;31m\n",MAXBLOC*BLOCSIZE); return false;}
  if (skeynumb%BLOCSIZE==0)
  { /* si toutes les GLXkey ont ete attribuees on ajoute un bloc */
    if (!(_SKEY_=(G2Xkey*)realloc(_SKEY_,(skeyblocnumb+1)*BLOCSIZE*sizeof(G2Xkey)))) return false;
    memset(_SKEY_+skeyblocnumb*BLOCSIZE,0,BLOCSIZE*sizeof(G2Xkey));
    skeyblocnumb++;
  }
  _SKEY_[skeynumb].key    = key;
  _SKEY_[skeynumb].action = action;
  if (info) snprintf(_SKEY_[skeynumb].info,INFOSIZE,"%s",info);
  skeynumb++;
  return true;
}

/*= Execute l'action associee au car. c =*/
static __inline__ bool g2x_ExecuteSpecialKeyAction(char c)
{
  G2Xkey *k;
  for (k=_SKEY_; k<_SKEY_+skeynumb; k++)
    if (c==k->key) { k->action(); return true; }

  return false;
}

/*= Libere les KeyActions =*/
static __inline__ void g2x_FreeKeyAction(void)
{
  if (_KEY_) free(_KEY_);
  _KEY_=NULL;
  keyblocnumb=0;
  keynumb=0;
  if (_SKEY_) free(_SKEY_);
  _SKEY_=NULL;
  skeyblocnumb=0;
  skeynumb=0;
}


/*====================================================
  fonction associee aux touches 'spéciales'
  F1 à F12, pavé fléché...
  parametres :
  - c : caractere saisi
  - x,y : coordonnee du curseur dans la fenetre
  ==================================================*/
static void __inline__ g2x_Special(int c, int x, int y)
{
  glutSetWindow(mainwin);
  switch(c)
  { // <F11> plein écran -----------------------------------------
    case SKEY_F11 :_FULLSCREEN_ = !_FULLSCREEN_;
                    switch (_FULLSCREEN_)
                    {
                      case true :
                        fullwidth=curwidth;
                        fullheight=curheight;
                        glutFullScreen();
                        break;
                      default :glutReshapeWindow(fullwidth,fullheight);
                    }
                    return glutPostRedisplay();
    // info -----------------------------------------------------------
    case SKEY_F12 : _INFO_ =!_INFO_ ; return glutPostRedisplay();
    default       : if (g2x_ExecuteSpecialKeyAction(c)) glutPostRedisplay();
                    else fprintf(stderr,"SPECIAL KEY (%d) : nothing attached\t\r",(int)c);
  }
}

/*==============================================================================*/
/*= fonction associee aux interruptions clavier : quelques touches predefinies =*/
/*= parametres :                                                               =*/
/*= - c : caractere saisi                                                      =*/
/*= - x,y : coordonnee du curseur dans la fenetre                              =*/
/*==============================================================================*/
static void __inline__ g2x_Keyboard(uchar c, int x, int y)
{
  glutSetWindow(mainwin);
  switch(c)
  { // <Ctrl+'f'> plein écran -----------------------------------------
    case 6  : _FULLSCREEN_ = !_FULLSCREEN_;
              switch (_FULLSCREEN_)
              {
                case true :
                  fullwidth=curwidth;
                  fullheight=curheight;
                  glutFullScreen();
                  break;
                default :glutReshapeWindow(fullwidth,fullheight);
              }
              break;
    // <Ctrl+j>/<Ctrl+p> capture d'écran jpeg, png --------------------
    case 10 : g2x_PlugCapture(_WINDNAME_,0,0,curwidth,curheight);
              g2x_Snapshot("jpg",_WINDNAME_,curwidth,curheight);  return;
    case 16 : g2x_PlugCapture(_WINDNAME_,0,0,curwidth,curheight);
              g2x_Snapshot("png",_WINDNAME_,curwidth,curheight);  return;
    // <Ctrl+r> affichage/masque la grille (spécifique 2D) ------------
    case 18  : _GRID_=!_GRID_; glutPostWindowRedisplay(drawwin); break;
    // <Ctrl+w> inverser couleur de fond ------------------------------
    case 23 : G2X_BKGD=1.-G2X_BKGD; break;
    // <Ctrl+'q'> ou <ESC> : sort du programme ------------------------
    case 27  : case 17  : g2x_Quit();
    // <ESPACE> stoppe/relance l'animation <SPACE> --------------------
    case 32  : _RUN_=!_RUN_;
         // mise a jour de la fonction d'anim
         glutIdleFunc((_IDLE_&&_RUN_)?(_FAFF_>1?_idle_F_:_idle_0_):NULL);
         break;
    // info -----------------------------------------------------------
    case '?': _INFO_ =!_INFO_ ; break;
    // zoom_value (spécifique 2D) -------------------------------------------
    case '+': zoom_value*=1.1;  break;
    case '-': zoom_value/=1.1;  break;
    case '=': zoom_value =1.0;
              g2x_ResetOffset();/* recentrage */
              break;

    // les autres actions liees a une touche, définies par l'utilisateur
    default  : if (g2x_ExecuteKeyAction(c)) glutPostRedisplay();
               else fprintf(stderr,"KEY '%c' (%d) : nothing attached\t\r",c,(int)c);
  }
  glutPostWindowRedisplay(drawwin);
}
