/**@file    g3x_window.h
 * @author  Universite Gustave Eiffel
 * @author  E.Incerti - eric.incerti@univ-eiffel.fr
 * @brief   Base public control functions
 * @version 6.e
 * @date    Aug.2022 (doc generation)
 */
/**@cond SHOW_SECURITY_DEFINE */

/*!==================================================================*/
#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#ifndef _G3X_WINDOW_H_
  #define _G3X_WINDOW_H_
/**@endcond                   */

  #include <g3x.h>

  /*!****************************************************************************!*/
  /*! LA FENETRE GRAPHIQUE                                                       !*/
  /*!****************************************************************************!*/

  /*! initialisation de la fenetre et mise en place des parametres Gl
   * 1° fonction à appeler - indispensable
   * - windname : nom de la fenêtre (le + classique : argv[0])
   * - wwidth/wheight : tailles en pixel
  !*/
  void g3x_InitWindow(char* windname, int wwidth, int wheight);

  /*!****************************************************************************!*/
  /*!  LES 6 FONCTIONS ESSENTIELLES DU MOTEUR GRAPHIQUE                          !*/
  /*!****************************************************************************!*/

  /*! 1°) INITIALISATION (chargement)
   *  >>> selectionne la fonction d'initialisation des donnees                        *
   *  >>> Cette fonction est appelée une seule fois, avant le lancement
   *      de la boucle principale.
   *      C'est elle et elle seule qui doit contenir les éventuelles allocation
   *      de mémoire.
   *      Elle se combine à la fonction de contrôle définie par <g3x_SetCtrlFunction>
   *  >>> Elle définit la partie "statique" du programme
   *  <<< Son usage n'est pas indispensable mais conseillé
   *      Tout son contenu pourrait être intégré directement dans la fonction <main>
   *
  !*/
  void g3x_SetInitFunction(void (*f)(void));

  /*! 2°) INTERFACE DE CONTRÔLE (boutons, scrollbars, points de contrôle...)
   *  >>> selectionne la fonction des actions de contrôle (bouttons, scrollbars, clavier...)
   *  >>> Cette fonction est appelée une seule fois, avant le lancement
   *      de la boucle principale, juste après l'appel  à la fonction d'initialisation
   *      définie par <g3x_SetInitFunction>
   *  >>> Elle se combine aux actions et paramètres de contrôle prédéfinis
   *  <<< Son usage n'est pas indispensable mais conseillé
   *      Tout son contenu pourrait être intégré dans la fonction d'initialisation
   *      ou directement dans la fonction <main>
  !*/
  void g3x_SetCtrlFunction(void (*f)(void));

  /*! 3°) GESTION DES EVENEMENTS (clavier, souris)
   *  >>> selectionne la fonction de gestion des événements (clavier, souris...)
   *  >>> la gestion des interruption est volontairement bridée et contraignante
   *      - seul le boutton gauche de la souris est disponible
   *        le boutton droit et la molette son associés à des action prédéfinies
   *      - les touches clavier doivent être associées à des procédures autonomes
   *        de prototype <void action(void)> et certaines séquences de touches sont
   *        verrouillées pour des actions prédéfinies [cf. <g3x_keys.h>] :
   *        <ESC>,<SPACE>,<F11>,<F12>
   *        <Ctrl+'q'>,<Ctl+'f'>,<Ctrl+'w'>,<Ctrl+'r'>
   *        '?','+','-','='
   *      - la prise en compte des interruptions est synchronisée avec l'appel
   *        à la fonction de 'display' (ou 'refresh')
  !*/
  void g3x_SetEvtsFunction(void (*f)(void));

  /*! 4°) AFFICHAGE - FONCTION DE DESSIN
   *  >>> selectionne la fonction associee aux actions de d'affichage (Display)
   *  >>> elle est indispensable pour afficher quelque chose
   *  >>> elle est appelée en boucle infinie, en synchro avec la fonction d'anim.
   *      - elle ne doit contenir QUE des appels d'affichage
   *      - SURTOUT PAS d'appel de gestion mémoire (malloc/free)
   *  >>> la synchro affichage/calcul est gérée par le moteur GL mais peut être
   *      paramétré par la fonction <g3x_SetRefreshFreq(int F_AFF)> qui fixe le
   *      rapport de fréquence d'appel pour ces 2 fctions
   *      <F_AFF> appels à la fonction d'anim pour 1 appel à la fonction d'affichage
  !*/
  void g3x_SetDrawFunction(void (*f)(void));

  /*! 5°) ANIMATION - FONCTION DE CALCUL
   *  >>> selectionne la fonction associee aux actions d'animation / calculs
   *  >>> elle est indispensable pour animer quelque chose
   *  >>> elle est appelée en boucle infinie, en synchro avec la fonction d'affichage.
   *      - elle ne doit contenir QUE des calculs
   *      - SURTOUT PAS d'appel de gestion mémoire (malloc/free)
   *      - pas d'appel d'affichage (ne serait pa pris en compte).
   *  >>> la synchro affichage/calcul est gérée par le moteur GL mais peut être
   *      paramétré par la fonction <g3x_SetRefreshFreq(int F_AFF)> qui fixe le
   *      rapport de fréquence d'appel pour ces 2 fctions
   *      <F_AFF> appels à la fonction d'anim pour 1 appel à la fonction d'affichage
  !*/
  void g3x_SetAnimFunction(void (*f)(void));

  /*! 6°) SORTIE
   *  >>> selectionne la fonction associee aux actions à faire en sortie (atexit)
   *  >>> c'est elle, en particulier, qui doit libérer la mémoire éventuellement
   *      allouée dans la fonction d'initialisation
   *  >>> elle est appelée une seule fois, à la sortie du programme
  !*/
  void g3x_SetExitFunction(void (*f)(void));

  /*!****************************************************************************!*/
  /*!  LA FONCTION DE DEMARRAGE                                                  !*/
  /*!****************************************************************************!*/
  /*! lance la boucle principale (glutMainLoop)
   *  >>> dernière fonction à appeler
  !*/
  int  g3x_MainStart(void);

  /*!****************************************************************************!*/
  /*!  RECUPERATION DES VARIABLES D'ENVIRONNEMENT                                !*/
  /*!****************************************************************************!*/

  /*!affichage/anim : 1 affichage pour <freq> passes de calcul                   !*/
  void  g3x_SetRefreshFreq(int freq);

  /*! les tailles de la fenêtre graphiquee (pixels)
   *  >>> elles changent lorsque la fenêtre est redimensionnée
  !*/
  int   g3x_GetPixWidth(void);  /* largeur courante (en pixels)                  */
  int   g3x_GetPixHeight(void); /* hauteur courante (en pixels)                  */

  /*! fixe / recupere la couleur de fond de la fenetre                          !*/
  void   g3x_SetBkGdCol(double bkg);
  double g3x_GetBkGdCol(void      );

  /*! fixe / recupere la vitesse d'action de la souris (panscan, mouvements...) !*/
  double g3x_GetMouseSpeed(void);
  void   g3x_SetMouseSpeed(double speed);

  /*!****************************************************************************!*/
  /*!  UTILITAIRES PRATIQUES                                                     !*/
  /*!****************************************************************************!*/

  /*! affiche un repere global ou local (moyennant la bonne transfo) !*/
  void g3x_Repere();

  /*! flèche d'origine <O>, de dir./long. <V>
   *  - r   : rayon du cyclindre
   *  - h   : taille de la pointe
   *  - col : couleur
  !*/
  void  g3x_ArrowVect(G3Xpoint O, G3Xvector V, float r, float h, G3Xcolor col);

 /*! ====================================================================== !*/
 /*! Affichage de texte
   *  >> OpenGl est très limité en ce qui concerne l'affichage de texte
   *     il ne propose que 2 fontes, 4 tailles et 1 seul style (normal)
   *     -> néanmoins un style 'bold' (gras) existe ici
   *     De plus l'affichage de texte s'avère assez coûteux.
   *     -> donc à éviter dans les appli visant la performance (animations)
  !*/

  /*! Selection de fonte d'écriture (taille et style) :
   *  size  's' (small) 'm' (medium) 'l' (large) 'L' (Large)
   *  style 'n' (normal) 'b' (bold)
   *  pos   'l','c','r' : left, center, right
  !*/
  void g3x_SetFontAttributes(char size, char style, char pos);

  /*! affiche en coord. 'pixel' (entiers : x,y)
   *  les attributs (taille, style, position) sont fixés par <g3x_SetFontAttributes>
   *  col : couleur du texte
   *  le reste : comme <printf(const char *format, ...)>
   *  le texte garde sa position dans la fenêtre indépendemment du redimensionnement,
   *  et des variations (zoom/panscan) de la fenêtre réelle.
  !*/
  void g3x_StaticPrint(int x, int y, G3Xcolor col, const char *format, ...);

  /*! comme <g3x_StaticPrint> mais avec un cadre
   *  (x,y)  : position d'ancrage fenêtre
   *  margin : marge entre le texte et le cadre
   *  txtcol : couleur du texte et du cadre
   *  bkgcol : couleur de fond
   *  borderw: epaisseur du cadre (pixels)
   *  le reste : comme <printf(const char *format, ...)> *
  !*/
  void g3x_StaticTextBox(int x, int y, int margin,
                         G3Xcolor txtcol, G3Xcolor bkgcol, int borderw,
                         const char *format, ...);


  /*!****************************************************************************!*/
  /*!  UTILITAIRES D'USAGE PLUS MARGINAL                                         !*/
  /*!****************************************************************************!*/

  void g3x_Refresh(void);   /*! force l'affichage dans la fonction de dessin (DrawFunc)!*/
  void g3x_Stop(void);      /*! arrete l'animation (arret sur image)                   !*/
  void g3x_Continue(void);  /*! relance l'animation                                    !*/
  void g3x_Quit(void);      /*! pour sortir proprement -- "exit" en + propre           !*/


 #endif

#ifdef __cplusplus
  }
#endif
/*==============================================================!*/
