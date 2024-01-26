/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr                       =!*/
/*!= Université Gustave Eiffel                                     =!*/
/*!= démo de définition/utilisation des outils de contrôle         =!*/
/*!=  fonctions <ctrl> et <evts>                                   =!*/
/*!=================================================================!*/

#include <g2x.h>

/* variables globale */
static bool     FILL;
static double   rx,ry;
static int      shape;
static G2Xpoint mouse,C;  /* type défini dans <g2x_geom.h> */

/* la fonction d'initialisation : appelée 1 seule fois, au début     */
static void init(void)
{
  FILL = false;
  rx=ry=5.;
  shape=0;
  C=(G2Xpoint){0.,0.};
}

/* la fonction de contrôle : appelée 1 seule fois, juste APRES <init> */
static void ctrl(void)
{
  g2x_CreateSwitch("F",&FILL,"Plein ou Trait");
  g2x_CreateButton("Ell","Trace une Ellipse  "); // bouton 0
  g2x_CreateButton("Rec","Trace un  Rectangle"); // bouton 1
  g2x_CreateScrollh_d("rx",&rx,0.,+10.0,"rayon en x");
  g2x_CreateScrollv_d("ry",&ry,0.,+10.0,"rayon en y");
  g2x_SetControlPoint(&C,10);
  /* sélection de la fonte pour les textes */
  g2x_SetFontAttributes('m','b','l');    /* taille : médium | style 'gras' (bold) | position horiz. (l | r |c)  */
}

/* la fonction de contrôle : appelée 1 seule fois, juste APRES <init> */
static void evts(void)
{
  static G2Xpoint* clic=NULL;
  shape = 2*g2x_GetButton() + FILL; // cercle ou rectangle + plein ou trait
  if ((clic=g2x_GetClic())!=NULL) C=*clic;
}

/* la fonction de dessin : appelée en boucle (indispensable) */
static void draw(void)
{
  switch (shape) // la valeur de <shape> est éventuellement modifiée par la fonction <evts>Z
  {
    case 0  : g2x_Ellipse      (C.x,C.y,rx,ry,0,G2Xo,1); break;
    case 1  : g2x_FillEllipse  (C.x,C.y,rx,ry,0,G2Xo  ); break;
    case 2  : g2x_Rectangle    (C.x-rx,C.y-ry,C.x+rx,C.y+ry,G2Xb,1); break;
    case 3  : g2x_FillRectangle(C.x-rx,C.y-ry,C.x+rx,C.y+ry,G2Xb  ); break;
    default : g2x_Plot(C.x,C.y,G2Xk,5);
  }
  g2x_StaticTextBox(10,15,5,G2Xk,G2Xwc_a,1,"(%+.1lf,%+.1lf)",C.x,C.y);
}

/***************************************************************************/
/* La fonction principale : NE CHANGE (presque) JAMAIS                     */
/***************************************************************************/
int main(int argc, char **argv)
{
  /* tailles de la fenêtre graphique (en pixels)     */
  int    WWIDTH=512, WHEIGHT=512;
  double wxmin=-10.,wymin=-10.,wxmax=+10.,wymax=+10.;

  g2x_InitWindow(*argv,WWIDTH,WHEIGHT);
  g2x_SetWindowCoord(wxmin,wymin,wxmax,wymax);

  g2x_SetInitFunction(init); /* fonction d'initialisation */
  g2x_SetCtrlFunction(ctrl); /* fonction de contrôle      */
  g2x_SetEvtsFunction(evts); /* fonction d'événements     */
  g2x_SetDrawFunction(draw); /* fonction de dessin        */
  g2x_SetAnimFunction(NULL); /* fonction d'animation      */
  g2x_SetExitFunction(NULL); /* fonction de sortie        */

  return g2x_MainStart();
}
