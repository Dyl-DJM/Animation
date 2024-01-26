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
static G2Xpoint  P;

/* la fonction d'initialisation : appelée 1 seule fois, au début */
static void init(void)
{
  g2x_SetWindowCoord(wxmin,wymin,wxmax,wymax);
  g2x_SetBkGdCol(0.); /* couleur de fond : noir */
  P = (G2Xpoint){0.,0.};
}

static void ctrl(void)
{
  g2x_SetControlPoint(&P,10);
}

static void evts(void)
{
}

/* la fonction de dessin : appelée en boucle */
static void draw(void)
{

  G2Xvector u = (G2Xvector)P;
  if (g2x_SqrNorm(u)>1.) g2x_Normalize(&u);
  P = (G2Xpoint)u;

  /* saturation : distance courante au centre */
  double s = g2x_Normalize(&u);
  /* teinte : angle courant par rapport au vecteur [0,0]->[1,0] (rouge) */
  double h = PI - acos(g2x_ProdScal((G2Xvector){1.,0.},u));
         h*= (g2x_ProdVect((G2Xvector){1.,0.},u)<0.?+1.:-1.);
         h = 0.5*(1+h/PI);

  G2Xcolor RGB = g2x_hsva_rgba_4f(h,s,1.,0.);
  G2Xcolor NEG = g2x_Color_NegRGB(RGB);

  g2x_FillRectangle(-1.,-1.,1.,1,NEG);
  g2x_FillCircle(0.,0.,1.,RGB);

  g2x_Axes();

  g2x_Line(P.x,P.y,P.x,0.,NEG,1);
  g2x_Line(P.x,P.y,0.,P.y,NEG,1);
  g2x_Line(0.,0.,u.x,u.y,NEG,1);
  g2x_Plot(P.x,P.y,G2Xk,8);
  g2x_Print(P.x,P.y,G2Xk," (%.1lf,%.1lf)",h,s);
  g2x_Circle(0.,0.,1.,NEG,1.);

  g2x_StaticPrint(10,35,G2Xk,"R=%3d | G=%3d | B=%3d",(int)rint(255*RGB.r),(int)rint(255*RGB.g),(int)rint(255*RGB.b));
  g2x_StaticPrint(10,15,G2Xw,"R=%3d | G=%3d | B=%3d",(int)rint(255*NEG.r),(int)rint(255*NEG.g),(int)rint(255*NEG.b));
  fprintf(stderr,"dist = %lf\t\r",g2x_ColorDist(RGB,NEG));
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
  g2x_SetCtrlFunction(ctrl); /* fonction de controle      */
  g2x_SetEvtsFunction(evts); /* fonction d'événements     */
  g2x_SetDrawFunction(draw); /* fonction de dessin        */
  g2x_SetAnimFunction(NULL); /* fonction d'animation      */
  g2x_SetExitFunction(NULL); /* fonction de sortie        */

  /* 4°) lancement de la boucle principale */
  return g2x_MainStart();
  /* RIEN APRES CA */
}
