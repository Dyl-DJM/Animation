/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G3X - Fonctions de base d'acces public -
                    version 6e - Jan.2022
  ================================================================!*/

#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#include <g3x.h>

/* pour compatibilité OpenGl - équivalent glColor4fv */
extern void     g3x_Color3fv(G3Xcolor c) { glColor3f(c.r,c.g,c.b);     }
extern void     g3x_Color4fv(G3Xcolor c) { glColor4f(c.r,c.g,c.b,c.a); }
/* mapping Couleur */
extern G3Xcolor g3x_mapscal4(G3Xcolor C, float    map) { return (G3Xcolor){map*C.r,map*C.g,map*C.b,map*C.a}; }
extern G3Xcolor g3x_mapvect4(G3Xcolor C, G3Xcolor map) { return (G3Xcolor){map.r*C.r,map.g*C.g,map.b*C.b,map.a*C.a}; }

/* pour compatibilité OpenGl */
extern void g3x_Materialfv(GLenum face,GLenum pname, G3Xcolor col)
{
  GLfloat GLCOL[4] = {col.r,col.g,col.b,col.a};
  glMaterialfv(face,pname,GLCOL);
}

/* pour compatibilité OpenGl */
extern void g3x_Enable_Blend(void)
{
  glEnable(GL_LIGHTING);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

/* pour compatibilité OpenGl */
extern void g3x_Material(G3Xcolor col, float ambi, float diff, float spec, float shine, float alpha)
{
  glEnable(GL_LIGHTING);
  GLfloat tmp[4];
  tmp[3]=alpha;
  tmp[0]=ambi*col.r;
  tmp[1]=ambi*col.g;
  tmp[2]=ambi*col.b;
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT  ,tmp);
  tmp[0]=diff*col.r;
  tmp[1]=diff*col.g;
  tmp[2]=diff*col.b;
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE  ,tmp);
  tmp[0]=spec*col.r;
  tmp[1]=spec*col.g;
  tmp[2]=spec*col.b;
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR ,tmp);
  glMaterialf (GL_FRONT_AND_BACK,GL_SHININESS,shine*128.);
}

extern G3Xcolor g3x_Color_NegRGB(G3Xcolor col)
{
  return (G3Xcolor){1.-col.r,1.-col.g,1.-col.b,col.a};
}

extern G3Xcolor g3x_Color_NegRGBA(G3Xcolor col)
{
  return (G3Xcolor){1.-col.r,1.-col.g,1.-col.b,1.-col.a};
}

extern double g3x_ColorDist(G3Xcolor c1, G3Xcolor c2)
{
  double dr=fabs(c1.r-c2.r),
         dg=fabs(c1.g-c2.g),
         db=fabs(c1.b-c2.b);
  return MAX3(dr,dg,db);
}


/* conversion RGBA -> HSVA */
extern G3Xcolor g3x_rgba_hsva_4fv(G3Xcolor col)
{
  float  y=col.r-col.g,
         c=col.g-col.b,
         m=col.b-col.r;

  /* teintes grises R=G=B */
  if (G3Xiszero(y) && G3Xiszero(c) && G3Xiszero(m))
    return (G3Xcolor){0.,0.,col.r,col.a};

  float  min = MIN3(col.r,col.g,col.b);
  float  max = MAX3(col.r,col.g,col.b);
  float  amp = 1./(max-min);
  float  h;
       if (col.r==max) { h = fmod((c*amp),6); }
  else if (col.g==max) { h = m*amp + 1./3.;   }
  else                 { h = y*amp + 2./3.;   }

  return (G3Xcolor){h,(max*amp),(max),(col.a)};
}

/* conversion HSVA -> RGBA */
extern G3Xcolor g3x_hsva_rgba_4fv(G3Xcolor col)
{
  float  h=6.*col.r, s=col.g, v=col.b, a=col.a;
  float  c = s*v;
  float  x = c*( 1.-fabs(fmod(h,2)-1.) );
  float  m = v-c;
  switch ((int)h)
  {                         //  R    G    B   A
    case 0 : return (G3Xcolor){m+c, m+x, m+0, a}; // rouge...jaune
    case 1 : return (G3Xcolor){m+x, m+c, m+0, a}; // jaune...vert
    case 2 : return (G3Xcolor){m+0, m+c, m+x, a}; // vert....cyan
    case 3 : return (G3Xcolor){m+0, m+x, m+c, a}; // cyan....bleu
    case 4 : return (G3Xcolor){m+x, m+0, m+c, a}; // bleu....magenta
    default: return (G3Xcolor){m+c, m+0, m+x, a}; // magenta.rouge
  }
}

/* même chose, mais en-tête différent */
extern G3Xcolor g3x_hsva_rgba_4f(float h, float s, float v, float a)
{
  h*=6;
  float  c = s*v;
  float  x = c*( 1.-fabs(fmod(h,2)-1.) );
  float  m = v-c;
  switch ((int)h)
  {                         //  R    G    B   A
    case 0 : return (G3Xcolor){m+c, m+x, m+0, a}; // rouge...jaune
    case 1 : return (G3Xcolor){m+x, m+c, m+0, a}; // jaune...vert
    case 2 : return (G3Xcolor){m+0, m+c, m+x, a}; // vert....cyan
    case 3 : return (G3Xcolor){m+0, m+x, m+c, a}; // cyan....bleu
    case 4 : return (G3Xcolor){m+x, m+0, m+c, a}; // bleu....magenta
    default: return (G3Xcolor){m+c, m+0, m+x, a}; // magenta.rouge
  }
}

/* même chose, mais en-tête différent : teinte pure */
extern G3Xcolor g3x_h110_rgba_1f(float h)
{
  h*=6;
  float  x = 1.-fabs(fmod(h,2)-1.);
  x = MIN(x,1.);
  x = MAX(x,0.);
  switch ((int)h)
  {                         // R  G  B  A
    case 0 : return (G3Xcolor){1, x, 0, 0}; // rouge...jaune
    case 1 : return (G3Xcolor){x, 1, 0, 0}; // jaune...vert
    case 2 : return (G3Xcolor){0, 1, x, 0}; // vert....cyan
    case 3 : return (G3Xcolor){0, x, 1, 0}; // cyan....bleu
    case 4 : return (G3Xcolor){x, 0, 1, 0}; // bleu....magenta
    default: return (G3Xcolor){1, 0, x, 0}; // magenta.rouge
  }
}



/*====================================================================*/
/*                  generateurs de cartes de couleurs                 */
/*====================================================================*/
extern void g3x_ColorMapHSV360(G3Xcolor *map, int n)
{
  int       i;
  float     h,x;
  G3Xcolor *c=map;

  for (i=0;i<n;i++)
  {
    h=((6.*i)/n);
    x = 1.-fabs(fmod(h,2)-1.);
    switch ((int)h)
    {                         // R  G  B  A
      case 0 : *c = (G3Xcolor){1, x, 0, 0}; break; // rouge...jaune
      case 1 : *c = (G3Xcolor){x, 1, 0, 0}; break; // jaune...vert
      case 2 : *c = (G3Xcolor){0, 1, x, 0}; break; // vert....cyan
      case 3 : *c = (G3Xcolor){0, x, 1, 0}; break; // cyan....bleu
      case 4 : *c = (G3Xcolor){x, 0, 1, 0}; break; // bleu....magenta
      default: *c = (G3Xcolor){1, 0, x, 0}; break; // magenta.rouge
    }
    c++;
  }
}

extern void g3x_ColorMapHSV360_va(G3Xcolor *map, int n, float v, float a)
{
  int    i;
  float  h, x;
  G3Xcolor *c=map;

  for (i=0;i<n;i++)
  {
    h=((6.*i)/n);
    x = v*SQR(h-(int)h);
    switch((int)h)
    {
      case 0  : *c = (G3Xcolor){v  , x  , 0.0, a}; break;
      case 1  : *c = (G3Xcolor){v-x, v  , 0.0, a}; break;
      case 2  : *c = (G3Xcolor){0.0, v  , x  , a}; break;
      case 3  : *c = (G3Xcolor){0.0, v-x, v  , a}; break;
      case 4  : *c = (G3Xcolor){  x, 0.0, v  , a}; break;
      default : *c = (G3Xcolor){v  , 0.0, v-x, a}; break;
    }
    c++;
  }
}

extern void g3x_ColorMapHSV360_interv(G3Xcolor *map, int n, float min, float max)
{
  int    i;
  float  h,x,v=1.,a=0.;
  G3Xcolor *c=map;

  for (i=0;i<n;i++)
  {
    h=6.*(min+ (i/(n*max)-min));
    x = v*SQR(h-(int)h);
    switch((int)h)
    {
      case 0  : *c = (G3Xcolor){v  , x  , 0.0, a}; break;
      case 1  : *c = (G3Xcolor){v-x, v  , 0.0, a}; break;
      case 2  : *c = (G3Xcolor){0.0, v  , x  , a}; break;
      case 3  : *c = (G3Xcolor){0.0, v-x, v  , a}; break;
      case 4  : *c = (G3Xcolor){  x, 0.0, v  , a}; break;
      default : *c = (G3Xcolor){v  , 0.0, v-x, a}; break;
    }
    c++;
  }
}

extern void g3x_ColorMapHSV360_modulate(G3Xcolor *map, int n, float (*mod_func)(float))
{
  static long double DEGtoNG = 1./60.;
  long double h;
  int i,u;
  float x;
  G3Xcolor *c=map;

  for (i=0;i<n;i++)
  {
    h=((360.*i)/n)*DEGtoNG;
    u = (int)h;
    h-= u;
    x = (float)(h*h); /* x dans [0,1] */
    x = mod_func(x);
    switch(u)
    {
      case 0  : *c = (G3Xcolor){1.0  ,   x  , 0.0  , 0.0}; break;
      case 1  : *c = (G3Xcolor){1.0-x, 1.0  , 0.0  , 0.0}; break;
      case 2  : *c = (G3Xcolor){0.0  , 1.0  ,   x  , 0.0}; break;
      case 3  : *c = (G3Xcolor){0.0  , 1.0-x, 1.0  , 0.0}; break;
      case 4  : *c = (G3Xcolor){  x  , 0.0  , 1.0  , 0.0}; break;
      default : *c = (G3Xcolor){1.0  , 0.0  , 1.0-x, 0.0}; break;
    }
    c++;
  }
}

extern void g3x_SetIndexColor(G3Xcolor* map, int idx, G3Xcolor col)
{
  *(map+idx)=col;
}

#ifdef __cplusplus
  }
#endif
