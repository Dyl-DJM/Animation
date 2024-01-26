/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr                       =!*/
/*!= Université Gustave Eiffel                                     =!*/
/*!= Code "squelette" pour prototypage avec libg2x.6e              =!*/
/*!= démo de définition/utilisation des outils de contrôle         =!*/
/*!=                                                               =!*/
/*!=                                                               =!*/
/*!=================================================================!*/

#include <g2x.h>

/* tailles de la fenêtre graphique (en pixels)     */
static int WWIDTH=512, WHEIGHT=512;
/* limites de la zone reelle associee a la fenetre */
static double wxmin=-10.,wymin=-10.,wxmax=+10.,wymax=+10.;

/* variables géométriques */
static G2Xpoint ctr; /* un point : centre du cercle */
static double   ray; /* un réel  : rayon du cercle  */

/* la fonction d'initialisation : appelée 1 seule fois, au début     */
static void init(void)
{
  g2x_SetWindowCoord(wxmin,wymin,wxmax,wymax);

  ctr = (G2Xpoint){0.,0.}; /* positionnement de centre */
  ray = 1.;                /* rayon initial            */
}

/* la fonction de contrôle : appelée 1 seule fois, juste APRES <init> */
static void ctrl(void)
{
  /* le centre devient actif (drag_&_drop), "cliquable" dans un rayon de 10 pixels */
  g2x_SetControlPoint(&ctr,10);
  /* un scrollbar vertical associé à la variable <ray> : varie dans [0.1,5.]       */
  g2x_CreateScrollv_d("ray",&ray,0.1,5.,"rayon du cercle");
}

/* la fonction de gestion des événements (clavier/souris) */
static void evts(void)
{
  static G2Xpoint *clic=NULL;
  if ((clic=g2x_GetClic()) && g2x_SqrDist(*clic,ctr)<SQR(ray))
    g2x_DrawFillCircle(ctr,ray,g2x_h110_rgba_1f(g2x_Rand_MinMax(0.,1.)));
}

/* la fonction de dessin : appelée en boucle (indispensable) */
static void draw(void)
{
  /* le cercle, en rouge, avec une épaisseur de trait de 2 */
  g2x_DrawCircle(ctr,ray,G2Xr,2);
  /* ou g2x_Cricle(ctr.x, ctr.y, ray, G2Xr, 2); */
  g2x_DrawPoint(ctr,G2Xo,3);
  /* ou g2x_Plot(ctr.x, ctr.y,G2Xr, 3); */
}

/* pas de fonction d'animation ni de fonction de sortie */

/***************************************************************************/
/* La fonction principale : NE CHANGE (presque) JAMAIS                     */
/***************************************************************************/
int main(int argc, char **argv)
{
  /* 1°) creation de la fenetre - titre et tailles (pixels) */
  g2x_InitWindow(*argv,WWIDTH,WHEIGHT);
  /* 2°) définition de la zone de travail en coord. réeelles *
   *     ATTENTION : veiller à respecter les proportions
   *                 (wxmax-wxmin)/(wymax-wymin) = WWIDTH/WHEIGHT
   *     si cette fonction n'est pas appelée, la zone réelle
   *     par défaut est wxmin=0., wymin=0., wxmax=WWIDTH, wymax=WHEIGHT
   */
  g2x_SetWindowCoord(wxmin,wymin,wxmax,wymax);

  /* 3°) association des fonctions */
  g2x_SetInitFunction(init); /* fonction d'initialisation */
  g2x_SetCtrlFunction(ctrl); /* fonction de contrôle      */
  g2x_SetEvtsFunction(evts); /* fonction d'événements     */
  g2x_SetDrawFunction(draw); /* fonction de dessin        */
  g2x_SetAnimFunction(NULL); /* fonction d'animation      */
  g2x_SetExitFunction(NULL); /* fonction de sortie        */

  /* 4°) lancement de la boucle principale */
  return g2x_MainStart();
  /* RIEN APRES CA */
}
