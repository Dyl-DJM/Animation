/**@file    g3x.h
 * @author  Universite Gustave Eiffel
 * @author  E.Incerti - eric.incerti@univ-eiffel.fr
 * @brief   Base public control functions
 * @version 6.e
 * @date    Aug.2022 (doc generation)
 */
/**@cond SHOW_SECURITY_DEFINE */
#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#ifndef _G3X_CONTOL_H
  #define _G3X_CONTOL_H
/**@endcond                   */

  #include <g3x.h>

  #define NAMESIZE  15
  #define INFOSIZE 127

  /*! les scrollbars, associés à un paramètre réel ou entier!*/
  /*! liés à l'adresse d'une variable (double/int)          !*/
  typedef struct
  {
    double*  dprm;            /*! var. <double>   !*/
    int*     iprm;            /*! var. <int>      !*/
    double   cursor;          /*! valeur du curseur*/
    double   min,max;         /*! valeurs min/max !*/
    double   fac;             /*! facteur d'échelle*/
    int      xcurs,ycurs;     /*! pos. du curseur !*/
    int      w;               /*! largeur du curs.!*/
    int      id;              /*! num. du curseur !*/
    char     name[7];         /*! nom             !*/
    char     info[INFOSIZE+1];/*! info associée   !*/
    G3Xcolor bkg;             /*! couleur de fond !*/
  } G3Xscroll;

  /* boutons, popup et switch : bcp de choses en commun */
  /* => sous-type commun 'caché' ???                    */

  /* pas très pratique au final -> peut-être à revoir   */
  /*! les boutons on/off associés à un flag booléen!*/
  typedef struct
  {
    int  num; /*! numero attribue au bouton     !*/
    int  x,y; /*! coordonnees du bouton         !*/
    int  len; /*! taille du nom                 !*/
    char name[NAMESIZE+1]; /*! nom              !*/
    char info[INFOSIZE+1]; /*! info associée    !*/
    bool on;               /*! flag actif/inactif*/
  } G3Xbut;


  /*! les boutons pop-up : lancent une action!*/
  typedef struct
  {
    int  num; /*! numero attribue au bouton     !*/
    int  x,y; /*! coordonnees du bouton         !*/
    int  len; /*! taille du nom                 !*/
    char name[NAMESIZE+1]; /*! nom              !*/
    char info[INFOSIZE+1]; /*! info associée    !*/
    void (*action)(void);  /*! action associée  !*/
    bool on;               /*! flag actif/inactif*/
  } G3Xpopup;

  /*! les switch (boutons exclusifs) :
   * associés à une variable globale booléene  !*/
  typedef struct
  {
    int   num; /*! numero attribue au bouton     !*/
    int   x,y; /*! coordonnees du bouton         !*/
    bool *flag;/*! variable booléenne associée   !*/
    int   len; /*! taille du nom                 !*/
    char  name[NAMESIZE+1]; /*! nom              !*/
    char  info[INFOSIZE+1];   /*! info associée    !*/
  } G3Xswitch;

  /*========================================================================*/
  /*                             KEYS (action)                              */
  /*========================================================================*/
  /*! gestion des actions associees a une touche clavier                         !*/
  /*! ATTENTION : les touches predefinies ne sont pas reattribuables             !*/
  /*! predefinies : <ESP><ESC><q><Q><Ctrl+F><+><-><=><?>                         !*/

  #define  KEY_ESCAPE 27        // vérouillée : sortie du programme
  #define  KEY_SPACE  32        // vérouillée : lance/arrête l'animation
  #define  KEY_ENTER  13
  #define  KEY_TAB     9
  #define  KEY_BACK    8

  /**@brief This function binds a key to a spécific action and stores the printable inforamtion about the action.
   * @warning as stated in G3Xkey these key are not available to binding
   * @param key: a single character that should be obtainable with a keyboard only.
   * @param action: an function pointer to the deired behavious should the key be pressed.
   * @param info: a string detailing the action briefly (less than 127 character available.)
   * @return true if the key and action could be bound, false otherwise.          */
  bool      g3x_SetKeyAction(char key, void (*action)(void), const char *info);


  // touches "spéciales"
  /*---- pavé fléché -----*/
  #define  SKEY_LEFT   GLUT_ARROW_LEFT
  #define  SKEY_RIGHT  GLUT_ARROW_RIGHT
  #define  SKEY_UP     GLUT_ARROW_UP
  #define  SKEY_DOWN   GLUT_ARROW_DOWN
  #define  SKEY_PGUP   GLUT_ARROW_PAGE_UP
  #define  SKEY_PGDOWN GLUT_ARROW_PAGE_DOWN

  /*---- touche de fonction F1 à F12 selon les codes GLUT -----*/
  #define  SKEY_F1     GLUT_KEY_F1 // normalement c'est le code 112, mais en GLUT c'est 1
  #define  SKEY_F2     GLUT_KEY_F2 // normalement c'est le code 113, mais en GLUT c'est 2
  #define  SKEY_F3     GLUT_KEY_F3 // .....
  #define  SKEY_F4     GLUT_KEY_F4
  #define  SKEY_F5     GLUT_KEY_F5
  #define  SKEY_F6     GLUT_KEY_F6
  #define  SKEY_F7     GLUT_KEY_F7
  #define  SKEY_F8     GLUT_KEY_F8
  #define  SKEY_F9     GLUT_KEY_F9
  #define  SKEY_F10    GLUT_KEY_F10
  #define  SKEY_F11    GLUT_KEY_F11 // verouillée : fullscreen
  #define  SKEY_F12    GLUT_KEY_F12 // verouillée : info

  bool      g3x_SetSpecialKeyAction(char key, void (*action)(void), const char *info);

  /*!-----------------------------------------------!*/
  /*!             FONCTIONS DE CONTROLE             !*/
  /*!-----------------------------------------------!*/

  /*! gestion des mouvements de la souris (sans click)                           !*/

  /* pas nettoyé depuis très longtemps => devenu obsolète ????     */
  /*! execute l'action en fonction de la position de la souris                    !*/
  void      g3x_SetMouseMoveAction(void (*action)(G3Xpoint _CLIC_POSITION_));
  /*! Retourne les coord. de la souris.                                           !*/
  G3Xpoint  g3x_GetMousePosition(void);
  /*! Retourne <true> si clic detecte.    !*/
  G3Xpoint* g3x_GetClic(void);

  /*! gestion des boutons poussoirs                                              !*/
  /*! bouton renvoyant un simple numero                                           !*/
  /*! a utiliser avec switch (g3x_GetButton()) { case 1 : ..... break; ... }      !*/
  int g3x_CreateButton(const char *text, const char *info);
  /*! retourne le num. du boutton courant                                         !*/
  int g3x_GetButton(void);

  /*! bouttons 'ON/OFF' : bascule la variable flag false<->true                  !*/
  bool g3x_CreateSwitch(const char *txt, bool *flag, const char *info);
  /*! retourne l'id du popup courant                                              !*/
  int  g3x_GetSwitch(void);

  /*! lance l'action associee, une seule fois (pop-up)                           !*/
  /*! equivalent a l'attachement de l'action a une touche clavier (cf. en dessous)!*/
  bool g3x_CreatePopUp(const char *name, void (*action)(void), const char *info);
  int  g3x_GetPopUp(void);

  /*  enlever le paramètre de mapping pour version publique - pas clair à utiliser    */
  /*  eventuellement re-introduire le mapping sous une autre forme, plus 'conviviale' */
  /*! gestion des scrollbars                                                     !*/
  /*! cree un Scroll horizontal directement attache au parametre reel <param>
     nom :  nom du scroll affiche sur la fenetre
     prm :  pointeur sur la variable (TYPE DOUBLE) attachee au scroll
     min,max : bornes inferieurs et superieure de parcours du scroll
     mf  : mapping x\in[0-1] -> x^(mf) -- mf<1 : log / mf>1 : exp / mf=1 : lin.
     info :  facultatif (0/NULL/"") texte apparaissant dans la fenetre d'aide
     retour  : id (numero) du scroll, -1 en cas d'echec                          !*/
  int g3x_CreateScrollh_d(const char *nom, double* prm, double min, double max, double mf, const char *info);
  /*! meme chose pour un parametre entier (TYPE INT)                              !*/
  int g3x_CreateScrollh_i(const char *nom, int*    prm, int    min, int    max, double mf, const char *info);
  /* --à revoir, supprimer ? */
  /*! cree un Scroll anonyme [0.,1.]  - initilise en position <init>              !*/
  int g3x_CreateAnonymousScrollv(const char *name, double init, const char *info);

  /*! position courante du curseur dans l'intevalle [min,max] du scroll  n° <id>  !*/
  /*! utile surtout avec les scroll anonymes                                      !*/
  double g3x_GetScrollCursor(int id);

  /*! memes choses pour les scroll verticaux                                      !*/
  int g3x_CreateScrollv_d(const char *nom, double* prm, double min, double max, double mf, const char *info);
  int g3x_CreateScrollv_i(const char *nom, int*    prm, int    min, int    max, double mf, const char *info);
  int g3x_CreateAnonymousScrollv(const char *name, double init, const char *info);

  /*! pour changer la couleur du scroll                                           !*/
  void   g3x_SetScrollColor(int id, G3Xcolor col);
  /*! ajuste la largeur des scrolls (4<=w<=16) defaut:8                           !*/
  void   g3x_SetScrollWidth(int width) ;

#endif

#ifdef __cplusplus
  }
#endif
