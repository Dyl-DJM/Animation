/*=================================================================*/
/*= E.Incerti - eric.incerti@univ-eiffel.fr                       =*/
/*= Université Gustave Eiffel                                     =*/
/*= Code d'illustration pour prototypage avec libg2x.6e           =*/
/*=================================================================*/

#include <g2x.h>

static int WWIDTH=512, WHEIGHT=512;
static double wxmin=-10.,wymin=-10.,wxmax=+10.,wymax=+10.;

static void g2x_drawfunction(double (*f)(double), double xmin, double xmax, G2Xcolor col, double e)
{
  double dx = e*g2x_GetXPixSize();
  double xa=xmin,xb=xa+dx;
  while (xb<=xmax) { g2x_Line(xa,f(xa),xb,f(xb),col,e); xa=xb; xb+=dx; }
}

static double m,e,rac2pi;

double gaussienne(double x)
{
  return exp(-0.5*SQR(x-m)/e)/(e*rac2pi);
}

static void init(void)
{
  m=0;
  e=1.;
  rac2pi = sqrt(2.*PI);
  g2x_SetWindowCoord(wxmin,wymin,wxmax,wymax);
}

static void ctrl(void)
{
  g2x_CreateScrollh_d("m",&m,-5.,+5,"");
  g2x_CreateScrollv_d("e",&e, 0.,+5,"");
}

static void draw(void)
{
  g2x_MultiGrad(G2Xk,3,1.,G2Xwc,.1,G2Xwb,.01,G2Xwa);
  g2x_drawfunction(gaussienne,g2x_GetXMin(),g2x_GetXMax(),G2Xr,2);
}

/***************************************************************************/
/* La fonction principale : NE CHANGE (presque) JAMAIS                     */
/***************************************************************************/
int main(int argc, char **argv)
{
  g2x_InitWindow(argv[0],WWIDTH,WHEIGHT);

  g2x_SetInitFunction(init);
  g2x_SetDrawFunction(draw);
  g2x_SetCtrlFunction(ctrl);

  return g2x_MainStart();
}
