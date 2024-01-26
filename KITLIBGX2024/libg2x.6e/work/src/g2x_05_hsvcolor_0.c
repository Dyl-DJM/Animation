/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr                       =!*/
/*!= Université Gustave Eiffel                                     =!*/
/*!= Code "squelette" pour prototypage avec libg2x.6e              =!*/
/*!=================================================================!*/

#include <g2x.h>

/* tailles de la fenêtre graphique (en pixels)     */
static int WWIDTH=512, WHEIGHT=512;
/* limites de la zone reelle associee a la fenetre */
static double wxmin=-1.,wymin=-1.,wxmax=+1.,wymax=+1.;

/* variables globales */
static bool      FLAG=false;
static double    h,s;
static G2Xvector u;
static G2Xpoint *clic=NULL;

/* la fonction d'initialisation : appelée 1 seule fois, au début */
static void init(void)
{
  g2x_SetWindowCoord(wxmin,wymin,wxmax,wymax);
  g2x_SetBkGdCol(0.); /* couleur de fond : noir */
}

/* la fonction de gestion des événements */
static void evts(void)
{
  FLAG=false;
  if ((clic=g2x_GetClic()) && g2x_SqrNorm(*(G2Xvector*)clic)<1.)
  {
    FLAG = true;
    u = *(G2Xvector*)clic;
    /* saturation : distance courante au centre */
    s = g2x_Normalize(&u);
    /* teinte : angle courant par rapport au vecteur [0,0]->[1,0] (rouge) */
    h = PI - acos(g2x_ProdScal((G2Xvector){1.,0.},u));
    h*= (g2x_ProdVect((G2Xvector){1.,0.},u)<0.?+1.:-1.);
    h = 0.5*(1+h/PI);
  }
}

/* la fonction de dessin : appelée en boucle */
static void draw(void)
{
  if (FLAG)
  {
    G2Xcolor RGB = g2x_hsva_rgba_4f(h,s,1.,0.);
    g2x_FillCircle(0.,0.,1.,RGB);
    g2x_Axes();
    g2x_Line(clic->x,clic->y,clic->x,0.,G2Xk,1);
    g2x_Line(clic->x,clic->y,0.,clic->y,G2Xk,1);
    g2x_Line(0.,0.,u.x,u.y,G2Xk,1);
    g2x_Print(clic->x,clic->y,G2Xk,"(%.1lf,%.1lf)",h,s);
  }
  g2x_Circle(0.,0.,1.,G2Xw,1);
}


/***************************************************************************/
/* La fonction principale : NE CHANGE (presque) JAMAIS                     */
/***************************************************************************/
int main(int argc, char **argv)
{
  /* 1°) creation de la fenetre - titre et tailles (pixels) */
  g2x_InitWindow(*argv,WWIDTH,WHEIGHT);

  /* 3°) association des fonctions */
  g2x_SetInitFunction(init); /* fonction d'initialisation */
  g2x_SetCtrlFunction(NULL); /* fonction de controle      */
  g2x_SetEvtsFunction(evts); /* fonction d'événements     */
  g2x_SetDrawFunction(draw); /* fonction de dessin        */
  g2x_SetAnimFunction(NULL); /* fonction d'animation      */
  g2x_SetExitFunction(NULL); /* fonction de sortie        */

  /* 4°) lancement de la boucle principale */
  return g2x_MainStart();
  /* RIEN APRES CA */
}
