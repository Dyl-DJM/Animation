
/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr                       =!*/
/*!= Université Gustave Eiffel                                     =!*/
/*!= Code "squelette" pour prototypage avec libg2x.6e              =!*/
/*!=================================================================!*/

#include <g2x.h>

/* tailles de la fenêtre graphique (en pixels)     */
static int WWIDTH=512, WHEIGHT=512;
/* limites de la zone reelle associee a la fenetre */
static double   wxmin=-2.,wymin=-2.,wxmax=+1.,wymax=+1.;

/* -----------------------------------------------------------------------
 * VARIABLES GLOBALES
 * ----------------------------------------------------------------------- */

/* des points (cf. <g2x_geom.h>)    */
static G2Xpoint A,B,C,D,E;

/* parametres pour l'animation */
static double   xa1,ya1,xa2,ya2,pasxa1,pasya1,pasxa2,pasya2;

/* des parametres controlables pour les scrollbars */
static double   rA,rE,cE,bkg;
/* une couleur  (cf. <g2x_color.h>) */
static G2Xcolor col;

/* des flags booleen pour les boutons 'switch' */
static bool _DISKFLAG_  =false;
static bool _SQUAREFLAG_=false;

/* des numéros de boutons exclusif - ça DOIT être des constantes */
#define _BUT1_ 0
#define _BUT2_ 1

/* -----------------------------------------------------------------------
 * FONCTIONS D'INTERACTIONS ASSOCIEES A DES TOUCHES, BOUTONS
 * ----------------------------------------------------------------------- */
/* une fonction associee a une touche clavier 'normale' : 's' [cf void ctrl(void)]*/
static void toggle_square(void) { _SQUAREFLAG_=!_SQUAREFLAG_; }

/* une fonction associee a une touche clavier 'spéciale' : <F1> [cf void ctrl(void)] */
/* -> affiche l'état sur <stderr> */
static void toggle_panscan(void)
{
  static bool flag = true;
  g2x_SetPanScanAndZoom((flag=!flag));  /* (cf. <g2x_window.h>) */
  fprintf(stderr,"PANSCAN %sLOCKED\e[0;0m                                     \r",
                  flag?"\e[1;34mUN":"\e[1;31m");
}

/* un fonction associee a un bouton 'popup' : */
/* remise aux positions initiales             */
static void reset(void)
{
  xa1=0.5*wxmin; ya1=0.5*wymin;
  xa2=0.5*wxmax; ya2=0.5*wymax;
  A.x=xa1; A.y=ya1;
  B.x=xa2; B.y=ya1;
  C.x=xa1; C.y=ya2;
  D.x=xa2; D.y=ya2;
  E.x=0.0; E.y=0.0;
}

/* la fonction d'initialisation */
static void init(void)
{
  reset();
  rA=0.2;
  rE=0.2;
  cE=0.5;
  bkg=1.;
  col=(G2Xcolor){0.,0.,1.};
  pasxa1=pasxa2=(xa2-xa1)*0.004;
  pasya1=pasya2=(ya2-ya1)*0.004;

  fprintf(stderr,"(%lf,%lf)->(%lf,%lf)\n",g2x_GetXMin(),g2x_GetYMin(),g2x_GetXMax(),g2x_GetYMax());
}

/* la fonction de contrôle (cf. <g2x_ctrl.h>) */
static void ctrl(void)
{
  /* les 5 points controlables à la souris (clic&drag) (cf. <g2x_ctrl.h>) */
  g2x_SetControlPoint(&A,10);
  g2x_SetControlPoint(&B,10);
  g2x_SetControlPoint(&C,10);
  g2x_SetControlPoint(&D,10);
  g2x_SetControlPoint(&E,10);

  /* les boutons 'switch' (on/off) - associes a des flag booleen (cf. <g2x_ctrl.h>) */
  g2x_CreateSwitch("D.",&_DISKFLAG_  ,"affiche/masque le disque de centre A");
  g2x_CreateSwitch("S.",&_SQUAREFLAG_,"affiche/masque le 'quad' <ABDC>     ");

  /* les boutons selecteurs exclusifs => a utiliser avec un switch/case (cf. <g2x_ctrl.h>) */
  g2x_CreateButton("spot","affiche/masque le 'spot' de centre E      "); // -> _BUT1_
  g2x_CreateButton("tri.","affiche/masque le triangle tricolore <BCD>"); // -> _BUT2_

  /* un bouton "popup" : exécute une action 1 fois à chaque clic (cf. <g2x_ctrl.h>) */
  g2x_CreatePopUp("reset",reset,"reset positions");

  /* les scrollbars : 1 horiz. / 2 vertic., tous attaché à des var. réelles (cf. <g2x_ctrl.h>) */
  int id;
  id=g2x_CreateScrollh_d("rA" ,&rA,0.1,1.0,"rayon du disque de centre A      ");
  //g2x_SetScrollColor(id,G2Xrb_c); /* change la couleur de fond du scrollbar */

  id=g2x_CreateScrollv_d("rE" ,&rE,0.1,1.0,"rayon du 'spot' de centre E      ");
  //g2x_SetScrollColor(id,G2Xrb_c);

  id=g2x_CreateScrollv_d("col",&cE,0.0,1.0,"couleur du spot 0.:bleu->1.:rouge");
  //g2x_SetScrollColor(id,G2Xgb_c);

  /* une action attachee a une touce clavier 'normale' (cf. <g2x_ctrl.h>)  */
  g2x_SetKeyAction('s',toggle_square,"affiche/masque le 'quad' <ABDC>");
  /* une action attachee a une touce clavier 'spéciale' (cf. <g2x_ctrl.h>)  */
  g2x_SetSpecialKeyAction(SKEY_F1,toggle_panscan,"bloque/débloque le zoom et le panscan");
}

/* la fonction de gestion des événements (clavier/souris) */
static void evts(void)
{
  /* la souris :
   * ATTENTION : g2x_GetClic() renvoie NULL si pas de clic
   * donc ne SURTOUT PAS utiliser directement (clic->x,clic->y) ==> Seg.Fault !!!
   * => TOUJOURS dans un  if ( (clic=g2x_GetClic())) ...
   * => g2x_GetMousePosition() : position courante, indep. du clic
   */
  static G2Xpoint  mpos;     // la position de la souris
  static G2Xpoint *clic=NULL;// si clic gauche -> récup. position
         G2Xcolor  mcol=G2Xb;// couleur du texte

  if ( (clic=g2x_GetClic())) { mpos = *clic; mcol=G2Xr; }
  else mpos=g2x_GetMousePosition();

  /* selection des attributs de la fonte taille, style, position (cf. <g2x_window.h>) */
  g2x_SetFontAttributes('s','b','c'); // small|bold
  g2x_Print(mpos.x,mpos.y,mcol,"(%+.2lf,%+.2lf)",mpos.x,mpos.y);
  g2x_SetFontAttributes('m','n','l'); // medium|normal (default)

  /* les boutons 'switch' */
  if (_SQUAREFLAG_) g2x_FillQuad(A.x,A.y,B.x,B.y,D.x,D.y,C.x,C.y,G2Xya_b);  /* tracé de formes diverses... */
  if (_DISKFLAG_)   g2x_FillCircle(A.x,A.y,rA,G2Xca_b);                     /* ... cf. <g2x_draw.h>        */
  /* les boutons selecteurs exclusifs */
  switch (g2x_GetButton())
  {
    case _BUT1_ : col.r=cE; col.g=1.-cE; g2x_Spot(E.x,E.y,rE,col); break;
    case _BUT2_ : g2x_FillTriangle_3col(B.x,B.y,G2Xr_b ,C.x,C.y,G2Xg_b ,D.x,D.y,G2Xb_b); break;
  }
}

/* la fonction de dessin (cf <g2x_draw.h>) */
static void draw()
{
  g2x_Line(A.x,A.y,E.x,E.y,G2Xr,2); /* trace une ligne entre A et E */
  g2x_DrawLine(B,E,G2Xg,1);         /* variante                     */

  g2x_Plot(A.x,A.y,G2Xr,4);         /* trace un point */

  g2x_SetFontAttributes('m','n','r');
  g2x_Print(A.x,A.y,G2Xk,"  A:%+.2lf,%+.2lf",A.x,A.y);

  g2x_DrawPoint(B,G2Xr,4);          /* variante       */
  g2x_Print(B.x,B.y,G2Xk,"  B:%+.2lf,%+.2lf",B.x,B.y);

  g2x_Plot(C.x,C.y,G2Xr,4);

  g2x_SetFontAttributes(0,0,'l');
  g2x_Print(C.x,C.y,G2Xk,"  C:%+.2lf,%+.2lf",C.x,C.y);

  g2x_Plot(D.x,D.y,G2Xr,4);
  g2x_Print(D.x,D.y,G2Xk,"  D:%+.2lf,%+.2lf",D.x,D.y);

  g2x_Plot(E.x,E.y,G2Xr,4);
  g2x_Print(E.x,E.y,G2Xk,"  E:%+.2lf,%+.2lf",E.x,E.y);


  /* manip. texte - cf. <g2x_window.h> */
  g2x_SetFontAttributes('L','B','l'); // LARGE|BOLD|left
  g2x_StaticPrint(g2x_GetPixWidth()/2,g2x_GetPixHeight()/2,G2Xwc_a,"TEXTE FIXE");

  g2x_SetFontAttributes('L','B','c');
  g2x_StaticTextBox(150, 30,3,G2Xr,G2Xo_c,2,"size : 'L', style : 'B', pos : 'c'");
  g2x_SetFontAttributes('l','b',0);
  g2x_StaticTextBox(150, 70,3,G2Xb,G2Xwc_a,1,"size : 'l', style : 'b', pos : 'c'");
  g2x_SetFontAttributes('m','n','l');
  g2x_StaticTextBox(150,100,3,G2Xw,G2Xb,0,"size : 'm', style : 'n', pos : 'l'");
  g2x_SetFontAttributes('s','b','r');
  g2x_StaticTextBox(150,130,3,G2Xk,G2Xwc_a,1,"size : 's', style : 'b', pos : 'r'");

//  g2x_Axes();
}

/* la fonction d'animation - run/stop : <SPACEBAR> */
static void anim(void)
{
  /* avancement des parametres */
  xa1+=pasxa1; ya1+=pasya1;
  xa2-=pasxa2; ya2-=pasya2;
  /* change de direction sur les bords de la fenetre */
  if (xa1>g2x_GetXMax() || xa1<g2x_GetXMin()) pasxa1=-pasxa1;
  if (xa2>g2x_GetXMax() || xa2<g2x_GetXMin()) pasxa2=-pasxa2;
  if (ya1>g2x_GetYMax() || ya1<g2x_GetYMin()) pasya1=-pasya1;
  if (ya2>g2x_GetYMax() || ya2<g2x_GetYMin()) pasya2=-pasya2;
  /* mise a jour des points : 3 façons de faire */
  /*1*/A = (G2Xpoint){xa1,ya1};
  /*2*/B.x=xa1; B.y=ya2;
  /*3*/C = g2x_Point2d(xa2,ya1);
  D.x=xa2; D.y=ya2;
}

static void quit()
{
  fprintf(stdout,"\n--bye--\n");
}

/***************************************************************************/
/* La fonction principale : NE CHANGE JAMAIS                               */
/***************************************************************************/
int main(int argc, char **argv)
{
  /* 1°) creation de la fenetre - titre et tailles (pixels) */
  g2x_InitWindow(*argv,WWIDTH,WHEIGHT);
  /* 2°) définition de la zone de travail en coord. réeelles *
   *     ATTENTION : veiller à respecter les proportions
   *                 (wxmax-wxmin)/(wymax-wymin) = WWIDTH/WHEIGHT
   */
  g2x_SetWindowCoord(wxmin,wymin,wxmax,wymax);

  /* 3°) association des fonctions */
  g2x_SetInitFunction(init); /* fonction d'initialisation */
  g2x_SetCtrlFunction(ctrl); /* fonction de contrôle      */
  g2x_SetEvtsFunction(evts); /* fonction d'événements     */
  g2x_SetDrawFunction(draw); /* fonction de dessin        */
  g2x_SetAnimFunction(anim); /* fonction d'animation      */
  g2x_SetExitFunction(quit); /* fonction de sortie        */

  /* 4°) lancement de la boucle principale */
  return g2x_MainStart();
  /* RIEN APRES CA */
}
