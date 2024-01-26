/**@file    g2x_control.h
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

#ifndef _G2X_CONTOL_H
  #define _G2X_CONTOL_H
/**@endcond                   */

  #include <g2x.h>

  #define NAMESIZE  15
  #define INFOSIZE 127

  /*========================================================================*/
  /*                               SOURIS                                   */
  /*========================================================================*/

  // les bouttons (molette active == MIDDLE)
  #define MOUSE_LEFT        GLUT_LEFT_BUTTON
  #define MOUSE_RIGHT       GLUT_RIGHT_BUTTON  // vérouillé : menu déroulant
  #define MOUSE_MIDDLE      GLUT_MIDDLE_BUTTON // vérouillé : panscan
  // les états
  #define MOUSE_PRESSED     GLUT_UP
  #define MOUSE_RELEASED    GLUT_DOWN
  // directions scroll molette
  #define WHEEL_SCROLL_UP   3   // vérouillé  : zoom +
  #define WHEEL_SCROLL_DOWN 4   // vérouillé  : zoom -

  /**@brief This function binds an action to the movements fo the mouse.
   * @param action: a function pointer expecting a G2Xpoint as parameter containing the current mouse location.*/
  void      g2x_SetMouseMoveAction(void (*action)(G2Xpoint _CLIC_POSITION_));

  /**@brief returns the current mouse location.
   * @warning There is actualy a small margin of error between exact mouse location and returned value.
   * Thus it is ill advised to atttempt pixel-exact click locations.                  */
  G2Xpoint  g2x_GetMousePosition(void);

  /**@brief  This function returns a boolean stating if the mouse is withing the drawing window.
   * @param  margin: the margin of error of the calculation (as pixel precision
   *         gets harder for humans with higher screen resolutions)
   * @retval true  - the mouse is in the window according to the margin
   * @retval false - the mouse isn't in the window
   * -> Exemple, pour une fenêtre de 1000x1000 pixels et une marge de 10%, la souris est 'inactive'
   *    dans une bande de largeur 100 pixel tout autour de la fenêtre.
   * */
  bool      g2x_MouseInWindow(double margin);

  /**@brief This function retruns the coordinate at witch the last clic happened.
   * @return the address of the G2Xpoint structure holding the coordinates
   * @retval NULL - no clic happened since last call or software start.                  */
  G2Xpoint* g2x_GetClic(void);

  /*========================================================================*/
  /*                             KEYS (action)                              */
  /*========================================================================*/
  #define  KEY_ESCAPE 27        // vérouillée : sortie du programme
  #define  KEY_SPACE  32        // vérouillée : lance/arrête l'animation
  #define  KEY_ENTER  13
  #define  KEY_TAB     9
  #define  KEY_BACK    8

  /**@brief This function binds a key to a spécific action and stores the printable inforamtion about the action.
   * @warning as stated in G2Xkey these key are not available to binding
   * @param key: a single character that should be obtainable with a keyboard only.
   * @param action: an function pointer to the deired behavious should the key be pressed.
   * @param info: a string detailing the action briefly (less than 127 character available.)
   * @return true if the key and action could be bound, false otherwise.          */
  bool      g2x_SetKeyAction(char key, void (*action)(void), const char *info);


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

  bool      g2x_SetSpecialKeyAction(char key, void (*action)(void), const char *info);


  /*========================================================================*/
  /*                            POINTS DE CONTROLE                        */
  /*========================================================================*/
  /*! se 'branche' sur un G2Xpoint defini dans les données globales
   *  et lui associe un rayon d'action (en pixel).
   *  >>> un clic dans ce rayon permet de 'capturer' le point pour le
   *     sélectionner, le déplacer (boutton enfoncer) et le replacer
   *     (boutton relâché) ailleurs.
  !*/

  /**@brief describes a mouse controlable point. */
  typedef struct
  {
    G2Xpoint* add; /**@brief the address of the point structure to be controled */
    int       ray; /**@brief the range at witch the point may be conroled. */
  } G2Xctrlpt;

  /**@brief This fucntion adds a mouse-controlable point to their table via its address and sets the click detection range.
   * @param P  : adress of the globaly defined point to add to the table.
   * @param ray: an integer (number of pixel) controling how far from the actual point the click on said point will be detected.
   * @return boolean value
   * @retval true  - the point was succesfully created & added to the control point storage
   * @retval false - the point wasn't created                                    */
  bool      g2x_SetControlPoint(G2Xpoint* P, int  ray);

  /**@brief This function returns the address of the point currently detected as clicked.
   * @return the address of the clicked point
   * @retval NULL - if the point couldn't be created.                            */
  G2Xpoint* g2x_GetControlPoint(void);

  /**@brief This function inform wether a clickable point exists or not.
   * @retval true  - a point is clicked
   * @retval false - no point clicked                                            */
  bool      g2x_GetCpt();

  /**@brief The function take the currently clicked point and re-set its location to the previous one.
   * @return nothing.                                                            */
  void      g2x_ReplaceControlPoint(void);

  /**@brief This function remove the point stored at the given address form the table of controlable points.
   * @param pt: the address of the point to be removed
   * @return nothing.                                                            */
  void      g2x_CleanControlPoint(G2Xpoint* pt);

  /**@brief Remove all points from the table of controlable points.
   * @return nothing.                                                            */
  void      g2x_CleanAllControlPoint(void);


  /*========================================================================*/
  /*                                  SCROLL                                */
  /*========================================================================*/
  /*! les scrollbars, associés à un paramètre réel ou entier
   *  liés à l'adresse d'une variable (double/int) globale du programme
  !*/

  /**@brief Scroll-bars (horizontal ou vertical)
   * permet de faire varier un paramètre (var. globale) dans un intervalle [min,max]    */
  typedef union { double *d; int* i;} _prmptr_; // un réel ou un entier
  typedef struct
  {
    double  *dprm;         /**@brief pointer to a global var <double>                    */
    int     *iprm;         /**@brief pointer to a global var <int>                       */
    double   cursor;       /**@brief value / position of the cursor (in [0,1])           */
    double   min,max;      /**@brief min/max values for the cursor run (min<cursor<max)  */
    int      xcurs,ycurs;  /**@brief position of the cursor in the window                */
    int      w;            /**@brief width on display <xdialwin> or <ydialwin>           */
    int      id;           /**@brief scroll id                                           */
    G2Xcolor BkGd;         /**@brief background color the slide bar                      */
    char     name[7];      /**@brief name on display                                     */
    char  info[INFOSIZE+1];/**@brief text for information pannel (key '?')   */
  } G2Xscroll;


  /*                                       -SCROLL HORIZONTAL                                    */
  /**@brief Creates a horizontal scrollbar bound to a floating parameter.
   * @param nom: the name dislayed on the window.
   * @param prm: the address of the parameter linked to the scroll bar.
   * @param min: the minimum value of said parameter.
   * @param max: the maximum value of said parameter.
   * @param info: OPTIOINAL text describing briefly the purpose of this scroll.
   * @return the id of the scroll on a success
   * @retval -1 on a failure.                  */
  int g2x_CreateScrollh_d(const char *nom, double* prm, double min, double max, const char *info);

  /**@brief Creates a horizontal scrollbar bound to a integer parameter.
   * @param nom: the name dislayed on the window.
   * @param prm: the address of the parameter linked to the scroll bar.
   * @param min: the minimum value of said parameter.
   * @param max: the maximum value of said parameter.
   * @param info: OPTIOINAL text describing briefly the purpose of this scroll.
   * @return <int> the id of the scroll on a success
   * @retval -1 - On a failure.                  */
  int g2x_CreateScrollh_i(const char *nom, int*    prm, int    min, int    max, const char *info);

  /**@deprecated Use CreateScrollh_i or CreateScrollh_d instead.
   * @brief Creates an unbound horizontal scroll.
   * @param name the scroll bar name to display
   * @param init the initial value.
   * @param info  the information about the scroll bar.
   * @return int the scroll's id                             */
  int g2x_CreateAnonymousScrollh(const char *name, double init, const char *info);

  /*                                       -SCROLL VERTICAL                                    */
  /**@brief Creates a vertival scrollbar bound to a floating parameter.
   * @param nom: the name dislayed on the window.
   * @param prm: the address of the parameter linked to the scroll bar.
   * @param min: the minimum value of said parameter.
   * @param max: the maximum value of said parameter.
   * @param info: Optional text describing briefly the purpose of this scroll.
   * @return the id of the scroll on a success, -1 otherwise  */
  int g2x_CreateScrollv_d(const char *nom, double* prm, double min, double max, const char *info);

  /**@brief Creates a  vertical scrollbar bound to a integer parameter.
   * @param nom: the name dislayed on the window.
   * @param prm: the address of the parameter linked to the scroll bar.
   * @param min: the minimum value of said parameter.
   * @param max: the maximum value of said parameter.
   * @param info: OPTIOINAL text describing briefly the purpose of this scroll.
   * @return the id of the scroll on a success, -1 otherwise */
  int g2x_CreateScrollv_i(const char *nom, int*    prm, int    min, int    max, const char *info);

  /**@deprecated Use CreateScrollv_i or CreateScrollv_d instead.
   * @brief DEPRECATED Creates an unbound vertical scroll.
   * @param name the scroll bar name to display
   * @param init the initial value.
   * @param info  the information about the scroll bar.
   * @return int the scroll's id.                            */
  int g2x_CreateAnonymousScrollv(const char *name, double init, const char *info);

  /**@deprecated Use of Annonymous scroll cursor is deprecated.
   * @brief This function returns cursor value. Mainly intended for anonymous cursors.
   * @param id: the id of the target cursor.
   * @return a double representinf the current value of the cursor.
   * @see g2x_CreateAnonymousScrollv
   * @see g2x_CreateAnonymousScrollh                         */
  double g2x_GetScrollCursor(int id);

  /**@brief This function sets the background color of the <id> cursor
   * @param id : the id of the cursor
   * @param col: the color we wish to aply.                  */
  void   g2x_SetScrollColor(int id, G2Xcolor col);

  /**@brief Adujst ScrollBar width.
   * @param width : the desired width / height. Must be within range [4; 16] (default :9) */
  void   g2x_SetScrollWidth(int width) ;


  /*========================================================================*/
  /*                                BOUTTON                                 */
  /*========================================================================*/
  /**@deprecated use G2XExclBut instead.
   * @brief an exclusive buttton.
   * All button created with this structure are mutualy exclusive. This stays for retro-compatibility purposes. */
  typedef struct
  {
    bool on;  /* flag actif/inactif             */
    int  num; /* numero attribue au bouton      */
    int  x,y; /* coordonnees du bouton          */
    int  len; /* taille du nom                  */
    char name[NAMESIZE+1]; /* nom               */
    char info[INFOSIZE+1]; /* info associée     */
  } G2Xbut;


   /**@brief This function returns the number of the current button.
    * @retval -1 - current button not set.
    * @retval [0,inf[ - current button ID.                  */
  int g2x_GetButton(void);

  /**@deprecated use g2x_CreateExclusiveButton instead.
   * @brief  <b> DEPRECATED </b> Creates old school buttons
   * @param  name The name of the button
   * @param  info some info to display in the help window
   * @retval -1 - current button not set.
   * @retval [0,inf[ - current button ID.                  */
  int g2x_CreateButton(const char *name, const char *info);


  /*                                                                        */
  /**@brief describe button mutual exclusive
   * All button must be ascociated to a group. maximum 16 groups for a maximum of 32 buttons.*/
  typedef struct
  { void (*action)(void);  /**@brief the action associated to the button
                            * will only be run when calling @see applyExclusiveButtonGroupe
                            * using this button group's id as parameter       */
    int exclusiveGrp;      /**@brief id of the exclusive group it belongs     */
    bool on;               /**@brief wether the button is active or not
                            * automatically switched to <false> when an other
                            * button of the same group switches to true.      */
    int  num;              /**@brief the button number                       */
    int  x,y;              /**@brief button coordinates on the display       */
    int  len;              /**@brief button label length                     */
    char name[NAMESIZE+1]; /**@brief the button name                         */
    char info[INFOSIZE+1]; /**@brief text for information pannel (key '?')   */
  }G2XExclBut;

  /*                                                                        */
  /**@brief Group of exclusive button
   * Such group can contain up to 32 buttons if all button are in the same group.
   * Maximum number of group is BLOCKSIZE / 2 as there should be at least 2 button per group.
   * @see G2XExclBut                                                        */
  typedef struct
  {
    G2XExclBut* active;    /**@brief pointer to the currently active member */
    char name[NAMESIZE+1]; /**@brief The group's name
                            * @todo implement display                       */
    int len;               /**@brief name length                            */
    G2XExclBut* members;   /**@brief a table of member buttons.             */
    int nbMembers;         /**@brief the number of members.                 */
  } G2XexclusiveGrp;

  bool g2x_CreateExclusiveButtonGroup(const char *name, int* grpId);
  int  g2x_CreateExclusiveButton(const char *name, void (*action)(void), const char *info, int grpId);

  /*========================================================================*/
  /*                               SWITCH                                   */
  /*========================================================================*/
  /**@brief simple on/off switch associated to boolean gloabl <flag> (green=true / red=false)   */
  typedef struct
  {
    bool *flag;            /**@brief pointer to a global boolean var         */
    int  num;              /**@brief the button number                       */
    int  x,y;              /**@brief button coordinates on the display       */
    int  len;              /**@brief button label length                     */
    char name[NAMESIZE+1]; /**@brief the button name                         */
    char info[INFOSIZE+1]; /**@brief text for information pannel (key '?')   */
  } G2Xswitch;


  /**@brief This function create a switch button whose attached value and behaviour should be independant from other buttons.
   * @param txt: a pointer to the string contraining the button label.
   * @param flag: a pointer to the boolean controled by this button.
   * @param info: a pointer to the straing holding the briefc explanation of the purpose of the button.
   * @retval true - the button was succesfully creataed and added to the corresponding talbe.
   * @retval false - something failed.                     */
  bool g2x_CreateSwitch(const char *txt, bool *flag, const char *info);

  /**@brief This function returns the number of the current on/off button.
   * @retval -1 - current button not set.
   * @retval [0-inf] - current button ID.                  */
  int  g2x_GetSwitch(void);

  /*========================================================================*/
  /*                             POPUP (action)                             */
  /*========================================================================*/
  /**@brief single action button.
   * Just like a popup on internet, the action linked to this button is operated imediatly      */
  typedef struct
  {
    void (*action)(void);  /**@brief the linked action as a function pointer */
    bool on;               /**@brief wether the button is active or not      */
    int  num;              /**@brief the button number                       */
    int  x,y;              /**@brief button coordinates on the display       */
    int  len;              /**@brief button label length                     */
    char name[NAMESIZE+1]; /**@brief the button name                         */
    char info[INFOSIZE+1]; /**@brief text for information pannel (key '?')   */
  } G2Xpopup;


  /**@brief This creates a new button bound to an action.
   * @param name: an address on the text displayed on the button
   * @param action: a function to be triggered when clicking the buttion, this function shouldn't expect nor retunr anything.
   * @param info: the address of the text explaining briefly the button purpose (less than 127 char).
   * @retval true on a succesfull creation and storage
   * @retval false if either failed.                  */
  bool g2x_CreatePopUp(const char *name, void (*action)(void), const char *info);

  /**@brief This function return the id of the currently selected pop-up button.
   * @retval -1 - no pop-up selected.
   * @retval [0-inf[ - the id of the pop-up.                  */
  int  g2x_GetPopUp(void);


 #endif

#ifdef __cplusplus
  }
#endif
/*!=============================================================!*/
