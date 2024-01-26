/*!==================================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G3X - Fonctions de base d'acces public -
                    version 6.0 - Apr.2020
  =====================================================================!*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <g3x.h>

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
/* appelée par <atexit> via <g3x_InitTrigonometric>  */
static void g3x_FreeTrigonometric(void)
{
  if (_cos_) free(_cos_); _cos_=NULL;
  if (_sin_) free(_sin_); _sin_=NULL;
}

/* creation des tables prédéfinies de cos. et sin.         */
/* cette fonction est appelée automatiquement au lancement */
extern bool g3x_InitTrigonometric(void)
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
  atexit(g3x_FreeTrigonometric);
  return true;
}

/* les fct cos et sin sur tableau, en indice [0,720], en radians [0,2PI], puis en degres [0,360] */
extern double g3x_Idxcos(int i) { return _cos_[i%720]; }
extern double g3x_Idxsin(int i) { return _sin_[i%720]; }

#define _radtodeg_ 114.591559026
extern double g3x_Radcos(double rad) { return _cos_[((int)(fabs(rad)*_radtodeg_))%720]; }
extern double g3x_Radsin(double rad) { return (rad<0.?-1.:+1.)*_sin_[((int)(fabs(rad)*_radtodeg_))%720]; }

extern double g3x_Degcos(double deg) { return _cos_[((int)(fabs(deg)*2.))%720]; }
extern double g3x_Degsin(double deg) { return (deg<0.?-1.:+1.)*_sin_[((int)(fabs(deg)*2.))%720]; }


/*====================================================================*/
/* compatibilité OpenGl                                               */
/*====================================================================*/
/* Equivalent glVertex3dv(double p[3]) */
extern void g3x_Vertex3dv(G3Xpoint p) { glVertex3d(p.x,p.y,p.z); };

/* Equivalent glNormal3dv(double v[3]) */
extern void g3x_Normal3dv(G3Xvector v) { glNormal3d(v.x,v.y,v.z); };


/*====================================================================*/
/*         points & vecteurs - constructeurs/opérateurs               */
/*====================================================================*/
extern G3Xpoint g3x_Point3d(double x, double y, double z)
{
  return (G3Xpoint){x,y,z};
}

extern G3Xpoint g3x_PplusV(G3Xpoint P, G3Xvector V)
{
  return (G3Xpoint){P.x+V.x,P.y+V.y,P.z+V.z};
}
/* (A,B) -> AB */
extern G3Xvector g3x_Vector2p(G3Xpoint A, G3Xpoint B)
{
  return (G3Xvector){(B.x-A.x),(B.y-A.y),(B.z-A.z)};
}

/* (x,y,z) -> V(x,y,z) */
extern G3Xvector g3x_Vector3d(double x, double y, double z)
{
  return (G3Xvector){x,y,z};
}

/* (U,V) -> U+V */
extern G3Xvector g3x_AddVect(G3Xvector U, G3Xvector V)
{
  return (G3Xvector){(U.x+V.x),(U.y+V.y),(U.z+V.z)};
}
/* (U,V) -> U-V */
extern G3Xvector g3x_SubVect(G3Xvector U, G3Xvector V)
{
  return (G3Xvector){(U.x-V.x),(U.y-V.y),(U.z-V.z)};
}

/* mapping scalaire Point/Vecteur */
extern G3Xcoord g3x_mapscal3(G3Xcoord C, double   map)
{
  return (G3Xcoord){map*C.x,map*C.y,map*C.z};
}

/* mapping vectoriel Point/Vecteur */
extern G3Xcoord g3x_mapvect3(G3Xcoord C, G3Xcoord map)
{
  return (G3Xcoord){map.x*C.x,map.y*C.y,map.z*C.z};
}


/* crée un vecteur de direction, aléatoire de norme < n  */
extern G3Xvector g3x_RandVector(double maxnorm)
{
  return (G3Xvector){ g3x_Rand_MinMax(-maxnorm,+maxnorm),
                      g3x_Rand_MinMax(-maxnorm,+maxnorm),
                      g3x_Rand_MinMax(-maxnorm,+maxnorm) };
}

extern G3Xvector g3x_NormalVector(G3Xvector V)
{
  g3x_Normalize(&V);
  return V;
}

extern G3Xvector g3x_NormalVector2p(G3Xpoint A, G3Xpoint B)
{
  G3Xvector V=g3x_Vector2p(A,B);
  g3x_Normalize(&V);
  return V;
}

/* normalise le vecteur V et renvoie la norme */
extern double g3x_Normalize(G3Xvector *V)
{
  double norm = SQR(V->x)+SQR(V->y)+SQR(V->z);
  if (norm<1.E-20) norm=1.;
  else norm = sqrt(norm);
  V->x/=norm;
  V->y/=norm;
  V->z/=norm;
  return norm;
}

/* produit scalaire  : cree le scalaire U°V */
extern double g3x_ProdScal(G3Xvector U, G3Xvector V)
{
  return (U.x*V.x+U.y*V.y+U.z*V.z);
}

/* produit scalaire : cree le scalaire AB°U */
extern double g3x_ProdScal2p1v(G3Xpoint A, G3Xpoint B, G3Xvector U)
{
  return ((B.x-A.x)*U.x+(B.y-A.y)*U.y+(B.x-A.x)*U.z);
}

/* produit scalaire : cree le scalaire AB°AC */
extern double g3x_ProdScal3p(G3Xpoint A, G3Xpoint B, G3Xpoint C)
{
  return ((B.x-A.x)*(C.x-A.x)+(B.y-A.y)*(C.y-A.y)+(B.x-A.x)*(C.z-A.z));
}

/* produit vectoriel (U^V) */
extern G3Xvector g3x_ProdVect(G3Xvector U, G3Xvector V)
{
  return (G3Xvector){(U.y*V.z-U.z*V.y),(U.z*V.x-U.x*V.z),(U.x*V.y-U.y*V.x)};
}

/* produit vectoriel (AB^AC) */
extern G3Xvector g3x_ProdVect3p(G3Xpoint A, G3Xpoint B, G3Xpoint C)
{
  return g3x_ProdVect(g3x_Vector2p(A,B),g3x_Vector2p(A,C));
}

/* produit vectoriel (AB^U) */
extern G3Xvector g3x_ProdVect2p1v(G3Xpoint A, G3Xpoint B, G3Xvector U)
{
  return g3x_ProdVect(g3x_Vector2p(A,B),U);
}

/* produit vectoriel (U^AB) */
extern G3Xvector g3x_ProdVect1v2p(G3Xvector U, G3Xpoint A, G3Xpoint B)
{
  return g3x_ProdVect(U,g3x_Vector2p(A,B));
}

/* renvoie le milieu du segment (AB) */
extern G3Xpoint g3x_Milieu(G3Xpoint A, G3Xpoint B)
{
  return (G3Xpoint){(A.x+B.x)*0.5, (A.y+B.y)*0.5, (A.z+B.z)*0.5 };
}

/* renvoie le barycentre du triangle (ABC) */
extern G3Xpoint g3x_Baryc3(G3Xpoint A, G3Xpoint B, G3Xpoint C)
{
  return (G3Xpoint){(A.x+B.x+C.x)/3., (A.y+B.y+C.y)/3., (A.z+B.z+C.z)/3. };
}

/* distance euclidienne au carre */
extern double g3x_SqrDist(G3Xpoint A,G3Xpoint B)
{
  return (SQR(B.x-A.x)+SQR(B.y-A.y)+SQR(B.z-A.z));
}

/* distance euclidienne */
extern double g3x_Dist(G3Xpoint A, G3Xpoint B)
{
  return sqrt((SQR(B.x-A.x)+SQR(B.y-A.y)+SQR(B.z-A.z)));
}

/* norme du vecteur V au carre */
extern double g3x_SqrNorm(G3Xvector V)
{
  return (SQR(V.x)+SQR(V.y)+SQR(V.z));
}

/* norme du vecteur V  */
extern double g3x_Norm(G3Xvector V)
{
  return sqrt(SQR(V.x)+SQR(V.y)+SQR(V.z));
}

/* norme L2 du vecteur V  */
extern double g3x_NormL2(G3Xvector V)
{
  return MAX3(fabs(V.x),fabs(V.y),fabs(V.z));
}

/* renvoie le cosinus de l'angle UV (produit scalaire normé) */
extern double g3x_CosVect(G3Xvector U, G3Xvector V)
{
  double nunv=(SQR(U.x)+SQR(U.y))*(SQR(V.x)+SQR(V.y));
  if (nunv<1.e-6) return 0;
  return (U.x*V.x + U.x*V.y)/sqrt(nunv);
}

/* renvoie le sinus de l'angle UV (produit vectoriel normé) */
extern double g3x_SinVect(G3Xvector U, G3Xvector V)
{
  double nunv=(SQR(U.x)+SQR(U.y))*(SQR(V.x)+SQR(V.y));
  if (nunv<1.e-6) return 0;
  return (U.x*V.y - U.y*V.x)/sqrt(nunv);
}
