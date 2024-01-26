#include <g2x.h>

typedef G2Xcoord complex;
// nombres complexes
static __inline__ complex Clx_cjg(complex a)           { return (complex){a.x,-a.y}; }
static __inline__ complex Clx_map(double  m, complex a){ return (complex){m*a.x,m*a.y}; }
static __inline__ complex Clx_add(complex a, complex b){ return (complex){a.x+b.x,a.y+b.y}; }
static __inline__ complex Clx_sub(complex a, complex b){ return (complex){a.x-b.x,a.y-b.y}; }
static __inline__ complex Clx_mul(complex a, complex b){ return (complex){a.x*b.x-a.y*b.y , a.x*b.y+a.y*b.x}; }
static __inline__ double  Clx_sqrmod(complex a)        { return SQR(a.x)+SQR(a.y); }

static int       max_it = 200;
static int       width  = 512;
static int       height = 512;
static double    inv_max_it,coef=1.;
static int      *valmap=NULL;
static G2Xcolor *colmap=NULL;
static complex   c;

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
      z = (complex){xdl+r*dx,ydl+l*dy};
     *v = F(z,c,0);
      v++;
    }
}

/*--------------------------------------------------------------*/
/* conversion des lim. de convergence en teinte HSV             */
/*--------------------------------------------------------------*/
static void val_to_col(int* valmap, G2Xcolor *colmap, int size)
{
  double    u,v;
  int      *val=valmap;
  G2Xcolor *col=colmap;
  while (val<valmap+size)
  {
     u = (*val)*inv_max_it;
     v = pow(u,coef);
     if (v==0.) *col = G2Xk;
     else *col = g2x_hsva_rgba_4f(v,1.-v,1.-v,0.);
     col++;
     val++;
   }
}

/*--------------------------------------------------------------*/
/* Chargement de l'image dans une texture                       */
/*--------------------------------------------------------------*/
static void LoadColMap(G2Xcolor *colmap, int width, int height)
{
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glEnable(GL_TEXTURE_2D);
  GLuint tex_id=0;
  glGenTextures(1,&tex_id);
  glBindTexture(GL_TEXTURE_2D,tex_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,width,height,0,GL_RGBA,GL_FLOAT,colmap);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glColor4f(1.,1.,1.,0.);
  glBegin(GL_QUADS);
    glTexCoord2d(0,0); glVertex2d(g2x_GetXMin(),g2x_GetYMin());
    glTexCoord2d(1,0); glVertex2d(g2x_GetXMax(),g2x_GetYMin());
    glTexCoord2d(1,1); glVertex2d(g2x_GetXMax(),g2x_GetYMax());
    glTexCoord2d(0,1); glVertex2d(g2x_GetXMin(),g2x_GetYMax());
  glEnd();
  glDisable(GL_TEXTURE_2D);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

/*--------------------------------------------------------------*/
/*--------------------------------------------------------------*/
static void init(void)
{
  width  = g2x_GetPixWidth() ;
  height = g2x_GetPixHeight();
  c   = (complex){0.3,0.5};
  valmap = (int     *)calloc(width*height,sizeof(int     ));
  colmap = (G2Xcolor*)calloc(width*height,sizeof(G2Xcolor));
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
  if (width<g2x_GetPixWidth() || height<g2x_GetPixHeight())
  {
    width  = g2x_GetPixWidth() ;
    height = g2x_GetPixHeight();
    valmap = (int     *)realloc(valmap,width*height*sizeof(int     ));
    colmap = (G2Xcolor*)realloc(colmap,width*height*sizeof(G2Xcolor));
  }
  julia(valmap,width        ,height
              ,g2x_GetXMin(),g2x_GetYMin()
              ,g2x_GetXMax(),g2x_GetYMax(),c);
  val_to_col(valmap,colmap,width*height);
  LoadColMap(colmap,width,height);
}

static void quit(void)
{
  free(valmap);
  free(colmap);
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
