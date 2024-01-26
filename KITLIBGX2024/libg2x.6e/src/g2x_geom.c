/*!==================================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G2X - Fonctions de base d'acces public -
                    version 6e - aout 2022
  =================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <g2x.h>


/*====================================================================*/
/*                  fonctions trigo adaptees                          */
/*====================================================================*/

/* fonctions trigo approchees : piochent dans un tableau de 720 val.  */
/* moins precis, mais plus rapide pour une utilisation haute freq.    */
static GLdouble* _cos_=NULL;
static GLdouble* _sin_=NULL;
/*!
static int step[]={1,2,3,4,5,6,8,9,10,12,15,16,18,20,24,30,36,40,45,48,60,72,80,90,120,144,180,240,360,720};
!*/

/* libération des tables prédéfinies de cos. et sin. */
/* appelée par <atexit> via <g2x_InitTrigonometric>  */
static void g2x_FreeTrigonometric(void)
{
  if (_cos_) free(_cos_); _cos_=NULL;
  if (_sin_) free(_sin_); _sin_=NULL;
}

/* creation des tables prédéfinies de cos. et sin.         */
/* cette fonction est appelée automatiquement au lancement */
extern bool g2x_InitTrigonometric(void)
{
  int i;
  GLdouble pas=PI/360;
  if (!(_cos_=(GLdouble*)calloc((720),sizeof(GLdouble))))
  { fprintf(stderr,"\t<erreur allocation _cos_>\n"); return false; }
  if (!(_sin_=(GLdouble*)calloc((720),sizeof(GLdouble))))
  { fprintf(stderr,"\t<erreur allocation _sin_>\n"); return false; }

  GLdouble *c=_cos_;
  GLdouble *s=_sin_;
  for (i=0;i<720;++i) { *c++=cos(i*pas); *s++=sin(i*pas); }
  /* libération automatique en sortie  */
  atexit(g2x_FreeTrigonometric);
  return true;
}

/* les fct cos et sin sur tableau, en indice [0,720], en radians [0,2PI], puis en degres [0,360] */
extern double g2x_Idxcos(int i) { return _cos_[i%720]; }
extern double g2x_Idxsin(int i) { return _sin_[i%720]; }

#define _radtodeg_ 114.591559026
extern double g2x_Radcos(double rad) { return _cos_[((int)(fabs(rad)*_radtodeg_))%720]; }
extern double g2x_Radsin(double rad) { return (rad<0.?-1.:+1.)*_sin_[((int)(fabs(rad)*_radtodeg_))%720]; }

extern double g2x_Degcos(double deg) { return _cos_[((int)(fabs(deg)*2.))%720]; }
extern double g2x_Degsin(double deg) { return (deg<0.?-1.:+1.)*_sin_[((int)(fabs(deg)*2.))%720]; }



/*====================================================================*/
/*         points & vecteurs - constructeurs/opérateurs               */
/*====================================================================*/
extern G2Xpoint g2x_Point2d(double x, double y) { return (G2Xpoint){x,y}; }

/* (x,y) -> V(x,y) */
extern G2Xvector g2x_Vector2d(double x, double y)  { return (G2Xvector){x,y}; }
/* (A,B) -> AB */
extern G2Xvector g2x_Vector2p(G2Xpoint A, G2Xpoint B) { return (G2Xvector){(B.x-A.x),(B.y-A.y)}; }

/* (U,V) -> U+V */
extern G2Xvector g2x_AddVect(G2Xvector U, G2Xvector V) { return (G2Xvector){(U.x+V.x),(U.y+V.y)}; }
/* (U,V) -> U-V */
extern G2Xvector g2x_SubVect(G2Xvector U, G2Xvector V) { return (G2Xvector){(U.x-V.x),(U.y-V.y)}; }

/* mapping Point/Vecteur */
extern G2Xcoord g2x_mapscal2(G2Xcoord C, double   map) { return (G2Xcoord){map*C.x,map*C.y}; }
extern G2Xcoord g2x_mapvect2(G2Xcoord C, G2Xcoord map) { return (G2Xcoord){map.x*C.x,map.y*C.y}; }


/* crée un point aléatoire dans la fenêtre */
extern G2Xpoint g2x_RandPoint(double margin)
{
  return (G2Xpoint){ g2x_Rand_MinMax(g2x_GetXMin()+margin,g2x_GetXMax()-margin) ,
                     g2x_Rand_MinMax(g2x_GetYMin()+margin,g2x_GetYMax()-margin) };
}

/* crée un vecteur de direction, aléatoire de norme définie  */
extern G2Xvector g2x_RandVector(double norm)
{
  G2Xvector V=(G2Xvector){ g2x_Rand_MinMax(-1.,+1.),
                           g2x_Rand_MinMax(-1.,+1.) };
  g2x_Normalize(&V);
  return g2x_mapscal2(V,norm);
}


extern G2Xvector g2x_NormalVector(G2Xvector V)
{
  g2x_Normalize(&V);
  return V;
}

extern G2Xvector g2x_NormalVector2p(G2Xpoint A, G2Xpoint B)
{
  G2Xvector V=g2x_Vector2p(A,B);
  g2x_Normalize(&V);
  return V;
}

/* normalise le vecteur V et renvoie la norme */
extern double g2x_Normalize(G2Xvector *V)
{
  double norm = SQR(V->x)+SQR(V->y);

  if (G2Xiszero(norm)) { V->x=V->y=0.; return 0.; }

  norm = sqrt(norm);
  V->x/=norm;
  V->y/=norm;

  return norm;
}

// applique une rotation d'angle "radians" à la direction de U
extern G2Xvector g2x_VectRotate(G2Xvector U, double radians)
{
  U = g2x_Mat_x_Vector(g2x_Rotation(radians),U);
  return U;
}


/* produit scalaire  : cree le scalaire U°V */
extern double g2x_ProdScal(G2Xvector U, G2Xvector V) { return (U.x*V.x + U.y*V.y); }
/* produit scalaire : cree le scalaire AB°AC */
extern double g2x_ProdScal3p(G2Xpoint A, G2Xpoint B, G2Xpoint C) { return g2x_ProdScal(g2x_Vector2p(A,B),g2x_Vector2p(A,C)); }

/* produit vectoriel (U^V).z   : en 2D c'est un scalaire !!!! */
extern double g2x_ProdVect(G2Xvector U, G2Xvector V) { return (U.x*V.y - U.y*V.x); }
/* produit vectoriel (AB^AC).z : en 2D c'est un scalaire !!!! */
extern double g2x_ProdVect3p(G2Xpoint A, G2Xpoint B, G2Xpoint C) { return g2x_ProdVect(g2x_Vector2p(A,B),g2x_Vector2p(A,C)); }

/* renvoie le milieu du segment (AB) */
extern G2Xpoint g2x_Milieu(G2Xpoint A, G2Xpoint B) { return (G2Xpoint){(A.x+B.x)*0.5, (A.y+B.y)*0.5 }; }
/* renvoie le barycentre du triangle (ABC) */
extern G2Xpoint g2x_Baryc3(G2Xpoint A, G2Xpoint B, G2Xpoint C) { return (G2Xpoint){(A.x+B.x+C.x)/3., (A.y+B.y+C.y)/3. }; }

/* distance euclidienne au carre */
extern double g2x_SqrDist(G2Xpoint A,G2Xpoint B) { return (SQR(B.x-A.x)+SQR(B.y-A.y)); }
/* distance euclidienne */
extern double g2x_Dist(G2Xpoint A, G2Xpoint B) { return sqrt((SQR(B.x-A.x)+SQR(B.y-A.y))); }

/* norme du vecteur V au carre */
extern double g2x_SqrNorm(G2Xvector V) { return (SQR(V.x)+SQR(V.y)); }
/* norme du vecteur V  */
extern double g2x_Norm(G2Xvector V)   { return sqrt(SQR(V.x)+SQR(V.y)); }

/* norme L2 du vecteur V  */
extern double g2x_NormL2(G2Xvector V) { return MAX(fabs(V.x),fabs(V.y)); }

/* retourne +1 si (ABC) orienté trigo+
             0 .........alignés
            -1 .........orienté trigo- */
extern int g2x_Orient(G2Xpoint A, G2Xpoint B, G2Xpoint C)
{
  double o = g2x_ProdVect3p(A,B,C);
  if (G2Xiszero(o)) return 0;
  return (o>0.? +1.:-1.);
}

/* renvoie le cosinus de l'angle UV (produit scalaire normé) */
extern double g2x_CosVect(G2Xvector U, G2Xvector V)
{
  double nunv=(SQR(U.x)+SQR(U.y))*(SQR(V.x)+SQR(V.y));
  if (nunv<1.e-6) return 0;
  return (U.x*V.x + U.x*V.y)/sqrt(nunv);
}

/* renvoie le sinus de l'angle UV (produit vectoriel normé) */
extern double g2x_SinVect(G2Xvector U, G2Xvector V)
{
  double nunv=(SQR(U.x)+SQR(U.y))*(SQR(V.x)+SQR(V.y));
  if (nunv<1.e-6) return 0;
  return (U.x*V.y - U.y*V.x)/sqrt(nunv);
}
