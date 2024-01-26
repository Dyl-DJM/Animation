/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G2X - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ================================================================!*/

#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#include <g2x.h>

/* pour compatibilité OpenGl - équivalent glColor4fv */
extern void     g2x_Color3fv(G2Xcolor c) { glColor3f(c.r,c.g,c.b);     }
extern void     g2x_Color4fv(G2Xcolor c) { glColor4f(c.r,c.g,c.b,c.a); }


/* conversion pour compatibilité libMLV/SDL */
extern G2Xfcolor g2x_uColor_to_fColor(G2Xucolor col)
{
  return (G2Xfcolor){((col>>24)&0x000000FF)/255.,
                     ((col>>16)&0x000000FF)/255.,
                     ((col>> 8)&0x000000FF)/255.,
                  1.-((col    )&0x000000FF)/255.};
}

extern G2Xucolor g2x_fColor_to_uColor(G2Xfcolor col)
{
  return (G2Xucolor)((((uint)(col.r*255))<<24) |
                     (((uint)(col.g*255))<<16) |
                     (((uint)(col.b*255))<< 8) |
                     (((uint)((1.-col.a)*255))));
}


/* mapping Couleur */
extern G2Xcolor g2x_mapscal4(G2Xcolor C, float    map) { return (G2Xcolor){map*C.r,map*C.g,map*C.b,map*C.a}; }
extern G2Xcolor g2x_mapvect4(G2Xcolor C, G2Xcolor map) { return (G2Xcolor){map.r*C.r,map.g*C.g,map.b*C.b,map.a*C.a}; }

extern G2Xcolor g2x_Color_NegRGB(G2Xcolor col)
{
  return (G2Xcolor){1.-col.r,1.-col.g,1.-col.b,col.a};
}

extern G2Xcolor g2x_Color_NegRGBA(G2Xcolor col)
{
  return (G2Xcolor){1.-col.r,1.-col.g,1.-col.b,1.-col.a};
}

/*
extern double g2x_ColorDist(G2Xcolor c1, G2Xcolor c2)
{
  double dr=fabs(c1.r-c2.r),
         dg=fabs(c1.g-c2.g),
         db=fabs(c1.b-c2.b);
  return MAX3(dr,dg,db);
}
*/
extern double g2x_ColorDist(G2Xcolor c1, G2Xcolor c2)
{
  double dr=(c1.r-c2.r),
         dg=(c1.g-c2.g),
         db=(c1.b-c2.b);
  return sqrt(dr*dr+dg*dg+db*db);
}


/* conversion RGBA -> HSVA */
extern G2Xcolor g2x_rgba_hsva_4fv(G2Xcolor col)
{
  float  y=col.r-col.g,
         c=col.g-col.b,
         m=col.b-col.r;

  /* teintes grises R=G=B */
  if (G2Xiszero(y) && G2Xiszero(c) && G2Xiszero(m))
    return (G2Xcolor){0.,0.,col.r,col.a};

  float  min = MIN3(col.r,col.g,col.b);
  float  max = MAX3(col.r,col.g,col.b);
  float  amp = 1./(max-min);
  float  h;
       if (col.r==max) { h = fmod((c*amp),6); }
  else if (col.g==max) { h = m*amp + 1./3.;   }
  else                 { h = y*amp + 2./3.;   }

  return (G2Xcolor){h,(max*amp),(max),(col.a)};
}

/* conversion HSVA -> RGBA */
extern G2Xcolor g2x_hsva_rgba_4fv(G2Xcolor col)
{
  float  h=6.*col.r, s=col.g, v=col.b, a=col.a;
  float  c = s*v;
  float  x = c*( 1.-fabs(fmod(h,2)-1.) );
  float  m = v-c;
  switch ((int)h)
  {                         //  R    G    B   A
    case 0 : return (G2Xcolor){m+c, m+x, m+0, a}; // rouge...jaune
    case 1 : return (G2Xcolor){m+x, m+c, m+0, a}; // jaune...vert
    case 2 : return (G2Xcolor){m+0, m+c, m+x, a}; // vert....cyan
    case 3 : return (G2Xcolor){m+0, m+x, m+c, a}; // cyan....bleu
    case 4 : return (G2Xcolor){m+x, m+0, m+c, a}; // bleu....magenta
    default: return (G2Xcolor){m+c, m+0, m+x, a}; // magenta.rouge
  }
}

/* même chose, mais en-tête différent */
extern G2Xcolor g2x_hsva_rgba_4f(float h, float s, float v, float a)
{
  h*=6;
  float  c = s*v;
  float  x = c*( 1.-fabs(fmod(h,2)-1.) );
  float  m = v-c;
  switch ((int)h)
  {                         //  R    G    B   A
    case 0 : return (G2Xcolor){m+c, m+x, m+0, a}; // rouge...jaune
    case 1 : return (G2Xcolor){m+x, m+c, m+0, a}; // jaune...vert
    case 2 : return (G2Xcolor){m+0, m+c, m+x, a}; // vert....cyan
    case 3 : return (G2Xcolor){m+0, m+x, m+c, a}; // cyan....bleu
    case 4 : return (G2Xcolor){m+x, m+0, m+c, a}; // bleu....magenta
    default: return (G2Xcolor){m+c, m+0, m+x, a}; // magenta.rouge
  }
}

/* même chose, mais en-tête différent : teinte pure */
extern G2Xcolor g2x_h110_rgba_1f(float h)
{
  h*=6;
  float  x = 1.-fabs(fmod(h,2)-1.);
  switch ((int)h)
  {                         // R  G  B  A
    case 0 : return (G2Xcolor){1, x, 0, 0}; // rouge...jaune
    case 1 : return (G2Xcolor){x, 1, 0, 0}; // jaune...vert
    case 2 : return (G2Xcolor){0, 1, x, 0}; // vert....cyan
    case 3 : return (G2Xcolor){0, x, 1, 0}; // cyan....bleu
    case 4 : return (G2Xcolor){x, 0, 1, 0}; // bleu....magenta
    default: return (G2Xcolor){1, 0, x, 0}; // magenta.rouge
  }
}


/*====================================================================*/
/*                  generateurs de cartes de couleurs                 */
/*====================================================================*/
extern void g2x_ColorMapHSV360(G2Xcolor *map, int n)
{
  int       i;
  float     h,x;
  G2Xcolor *c=map;

  for (i=0;i<n;i++)
  {
    h=((6.*i)/n);
    x = 1.-fabs(fmod(h,2)-1.);
    switch ((int)h)
    {                       // R  G  B  A
      case 0 : *c = (G2Xcolor){1, x, 0, 0}; break; // rouge...jaune
      case 1 : *c = (G2Xcolor){x, 1, 0, 0}; break; // jaune...vert
      case 2 : *c = (G2Xcolor){0, 1, x, 0}; break; // vert....cyan
      case 3 : *c = (G2Xcolor){0, x, 1, 0}; break; // cyan....bleu
      case 4 : *c = (G2Xcolor){x, 0, 1, 0}; break; // bleu....magenta
      default: *c = (G2Xcolor){1, 0, x, 0}; break; // magenta.rouge
    }
    c++;
  }
}

extern void g2x_ColorMapHSV360_va(G2Xcolor *map, int n, float v, float a)
{
  int    i;
  float  h, x;
  G2Xcolor *c=map;

  for (i=0;i<n;i++)
  {
    h=((6.*i)/n);
    x = v*SQR(h-(int)h);
    switch((int)h)
    {
      case 0  : *c = (G2Xcolor){v  , x  , 0.0, a}; break;
      case 1  : *c = (G2Xcolor){v-x, v  , 0.0, a}; break;
      case 2  : *c = (G2Xcolor){0.0, v  , x  , a}; break;
      case 3  : *c = (G2Xcolor){0.0, v-x, v  , a}; break;
      case 4  : *c = (G2Xcolor){  x, 0.0, v  , a}; break;
      case 5  : *c = (G2Xcolor){v  , 0.0, v-x, a}; break;
      default : *c = (G2Xcolor){v  , x  , 0.0, a}; break;
    }
    c++;
  }
}

extern void g2x_ColorMapHSV360_sa(G2Xcolor *map, int n, float s, float a)
{
  int    i;
  float  h, x;
  G2Xcolor *c=map;

  for (i=0;i<n;i++)
  {
    h=((6.*i)/n);
    x = s*SQR(h-(int)h);
    switch((int)h)
    {
      case 0  : *c = (G2Xcolor){  s   ,  x   ,(1.-s), a}; break;
      case 1  : *c = (G2Xcolor){ s-x  ,  s   ,(1.-s), a}; break;
      case 2  : *c = (G2Xcolor){(1.-s),  s   ,   x  , a}; break;
      case 3  : *c = (G2Xcolor){(1.-s), s-x  ,   s  , a}; break;
      case 4  : *c = (G2Xcolor){  x   ,(1.-s),   s  , a}; break;
      default : *c = (G2Xcolor){  s   ,(1.-s),  s-x , a}; break;
    }
    c++;
  }
}
extern void g2x_ColorMapHSV360_interv(G2Xcolor *map, int n, float min, float max)
{
  int    i;
  float  h,x,v=1.,a=0.;
  G2Xcolor *c=map;

  for (i=0;i<n;i++)
  {
    h=6.*(min+ (i/(n*max)-min));
    x = v*SQR(h-(int)h);
    switch((int)h)
    {
      case 0  : *c = (G2Xcolor){v  , x  , 0.0, a}; break;
      case 1  : *c = (G2Xcolor){v-x, v  , 0.0, a}; break;
      case 2  : *c = (G2Xcolor){0.0, v  , x  , a}; break;
      case 3  : *c = (G2Xcolor){0.0, v-x, v  , a}; break;
      case 4  : *c = (G2Xcolor){  x, 0.0, v  , a}; break;
      default : *c = (G2Xcolor){v  , 0.0, v-x, a}; break;
    }
    c++;
  }
}


extern void g2x_SetIndexColor(G2Xcolor* map, int idx, G2Xcolor col)
{
  *(map+idx)=col;
}

#ifdef __cplusplus
  }
#endif
