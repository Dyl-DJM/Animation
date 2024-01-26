#include <g2x.h>

typedef G2Xcoord complex;
// nombres complexes
static __inline__ complex Clx_cjg(complex a)           { return (complex){a.x,-a.y}; }
static __inline__ complex Clx_map(double  m, complex a){ return (complex){m*a.x,m*a.y}; }
static __inline__ complex Clx_add(complex a, complex b){ return (complex){a.x+b.x,a.y+b.y}; }
static __inline__ complex Clx_sub(complex a, complex b){ return (complex){a.x-b.x,a.y-b.y}; }
static __inline__ complex Clx_mul(complex a, complex b){ return (complex){a.x*b.x-a.y*b.y , a.x*b.y+a.y*b.x}; }
static __inline__ double  Clx_sqrmod(complex a)        { return SQR(a.x)+SQR(a.y); }

static int        max_it = 200;
static int        width  = 256;
static int        height = 256;
static double     inv_max_it,coef=1.;
static int       *valmap=NULL;
static G2Xcolor  *colmap=NULL;
static G2Xpixmap *pixmap=NULL;
static complex    c;

/*--------------------------------------------------------------*/
/* fonction récursive                                           */
/*--------------------------------------------------------------*/
int  F(complex z, complex c, int  nb_it)
{
  /* divergence */
  if (Clx_sqrmod(z)>4.) return nb_it;
  /* limite de convergence atteinte */
  if (nb_it>=max_it) return 0;

  nb_it++;
  z = Clx_mul(z,z);

  double tmp;
  tmp = c.x;
  c.x = c.y;
  c.y = tmp;

  z = Clx_add(z,c);
  return F(z,c,nb_it);
}


/*--------------------------------------------------------------*/
/* ensemble de Julia(c)                                         */
/*--------------------------------------------------------------*/
void julia(int *valmap, int width, int height, double xdl,double ydl, double xur, double yur, complex c)
{
  complex   z;
  int       l,r;
  double dx =(xur-xdl)/(width -1);
  double dy =(yur-ydl)/(height-1);
  int   *v=valmap;
  for (l=0; l<height; l++)
    for (r=0; r<width; r++)
    {
      z = (complex){xdl+r*dx,ydl+l*dy}; // dans [-1.,+1]²
     *v = F(z,c,0);
      v++;
    }
}

/*--------------------------------------------------------------*/
/* conversion des lim. de convergence en teinte HSV             */
/*--------------------------------------------------------------*/
static void val_to_col(int* valmap, G2Xpixmap *pixmap)
{
  double    u,v;
  G2Xcolor  col;
  int      *val=valmap;
  uchar    *pix=pixmap->map;
  while (pix<pixmap->end)
  {
     u = (*val)*inv_max_it;
     v = pow(u,coef);
     if (v!=0.) col = g2x_hsva_rgba_4f(v,1.-v,1.-v,0.);
    *pix=(uchar)(255.*col.r); pix++;
    *pix=(uchar)(255.*col.g); pix++;
    *pix=(uchar)(255.*col.b); pix++;
     val++;
   }
}

/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
static void init(void)
{
  if (!g2x_PixmapAlloc(&pixmap,width,height,3,255)) exit(1);
//  width  = g2x_GetPixWidth() ;
//  height = g2x_GetPixHeight();
  c   = (complex){0.3,0.5};
  valmap = (int     *)calloc(width*height,sizeof(int     ));
}

static void ctrl(void)
{
  g2x_CreateScrollv_d("re" ,&c.x,-1.0,+1.0,"c.re");
  g2x_CreateScrollv_d("im" ,&c.y,-1.0,+1.0,"c.im");
  g2x_CreateScrollh_i("max" ,&max_it,10,1000,"iterations max");
  g2x_CreateScrollh_d("coef",&coef   ,0.,2.,"exposant");
}

static void draw(void)
{
  inv_max_it=1./max_it;
  julia(valmap,width        ,height
              ,g2x_GetXMin(),g2x_GetYMin()
              ,g2x_GetXMax(),g2x_GetYMax(),c);
  val_to_col(valmap,pixmap);
  g2x_PixmapShow(pixmap,true);
}

static void quit(void)
{
  free(valmap);
  g2x_PixmapFree(&pixmap);
}

int main(int argc, char **argv)
{
  g2x_InitWindow(*argv,width,height);
  g2x_SetWindowCoord(-1.0,-1.0,+1.0,+1.0);
  g2x_SetInitFunction(init);
  g2x_SetCtrlFunction(ctrl);
  g2x_SetDrawFunction(draw);
  g2x_SetExitFunction(quit);

  return g2x_MainStart();
}
