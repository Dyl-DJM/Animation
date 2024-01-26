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

extern void g3x_QuatIdentity(G3Xquat A)
{
  memset(&A,0,sizeof(G3Xquat));
  A.r = 1.0;
}

extern G3Xquat g3x_QuatSet(double r, G3Xvector v)
{
  return (G3Xquat){r,v};
}

extern G3Xquat g3x_QuatSet4(double r, double x, double y, double z)
{
  return (G3Xquat){r,(G3Xvector){x,y,z}};
}

extern G3Xquat g3x_QuatAdd(G3Xquat A, G3Xquat B)
{
  return (G3Xquat){A.r+B.r,g3x_AddVect(A.v,B.v)};
}

extern G3Xquat g3x_QuatProd(G3Xquat A, G3Xquat B)
{
  G3Xquat C;
  C.r = A.r*B.r - G3Xprodscal(A.v,B.v);
  G3Xvector w;
  G3Xprodvect(w,A.v,B.v);
  C.v.x = A.r*B.v.x + B.r*A.v.x + w.x;
  C.v.y = A.r*B.v.y + B.r*A.v.y + w.y;
  C.v.z = A.r*B.v.z + B.r*A.v.z + w.z;
  return C;
}

extern G3Xquat g3x_QuatConj(G3Xquat A)
{
  A.v.x=-A.v.x;
  A.v.y=-A.v.y;
  A.v.z=-A.v.z;
  return A;
}

extern double g3x_QuatSqrNorm(G3Xquat A)
{
  return SQR(A.r)+G3Xsqrnorm(A.v);
}

extern double g3x_QuatNorm(G3Xquat A)
{
  return sqrt(SQR(A.r)+G3Xsqrnorm(A.v));
}

extern G3Xquat g3x_QuatNormalize(G3Xquat A)
{
  double  n=g3x_QuatSqrNorm(A);
  if (G3Xiszero(n)) { memset(&A,0,sizeof(G3Xquat)); return A; }
  n = 1./sqrt(n);
  A.r   *=n;
  A.v=g3x_mapscal3(A.v,n);
  return A;
}

extern G3Xquat g3x_QuatScalMap(G3Xquat A, double a)
{
  A.r*=a;
  A.v=g3x_mapscal3(A.v,a);
  return A;
}


extern G3Xquat g3x_QuatInv(G3Xquat A)
{
  double n = g3x_QuatNorm(A);
  if (G3Xiszero(n)) { memset(&A,0,sizeof(G3Xquat)); return A; }
  A.v.x=-A.v.x;
  A.v.y=-A.v.y;
  A.v.z=-A.v.z;
  g3x_QuatScalMap(A,1./n);
  return A;
}


extern void g3x_QuatToHmat(G3Xquat A, G3Xhmat M)
{
  M[a00]= A.r;   M[a01]=-A.v.x; M[a02]=-A.v.y; M[a03]=-A.v.z;
  M[a10]=+A.v.x; M[a11]= A.r;   M[a12]=-A.v.z; M[a13]=+A.v.y;
  M[a20]=+A.v.y; M[a21]=+A.v.z; M[a22]= A.r;   M[a23]=-A.v.x;
  M[a30]=+A.v.z; M[a33]=-A.v.y; M[a32]=+A.v.x; M[a33]= A.r;
}

extern void g3x_QuatPrint(G3Xquat A)
{
  fprintf(stderr,"Quaternion : [%f|(%f,%f,%f)] ||Q||=%lf\t\r",A.r,A.v.x,A.v.y,A.v.z,g3x_QuatNorm(A));
}


extern void g3x_QuatRot(G3Xquat Qrot, G3Xcoord src, G3Xcoord dest)
{
  G3Xquat Qsrc = g3x_QuatSet(0,src);
  G3Xquat Qdst;

  Qdst  = g3x_QuatProd(Qrot,Qsrc);
  Qrot.v= g3x_mapscal3(Qrot.v,-1.);
  Qdst  = g3x_QuatProd(Qdst,Qrot);
  dest  = Qdst.v;
}

extern void g3x_AxeRadRot(G3Xvector v, double rad, G3Xcoord src, G3Xcoord dest)
{
  G3Xvector u=v;
  g3x_Normalize(&u);/* direction normee */
  double c=cos(0.5*rad);
  double s=sin(0.5*rad);
  u=g3x_mapscal3(u,s);
  G3Xquat Qrot = g3x_QuatSet(c,u);
  G3Xquat Qsrc = g3x_QuatSet(0,src);
  G3Xquat Qdst;

  Qdst = g3x_QuatProd(Qrot,Qsrc);
  u=g3x_mapscal3(u,-1.);
  Qrot = g3x_QuatSet(c,u);
  Qdst = g3x_QuatProd(Qdst,Qrot);
  dest = Qdst.v;
}



/* Quaternion d'alignement du vecteur v sur le vecteur cible */
extern G3Xquat g3x_QuatAlign(G3Xvector v, G3Xvector const cible)
{
  G3Xvector v1,c1,w;
  G3Xquat   Q;

  /* c1 : cible normee */
  c1=cible;
  g3x_Normalize(&c1);

  v1=v;
  g3x_Normalize(&v1);
  /* v1 transforme en (v1+c1)/2 pour avoir l'angle moitie */
  /* angle(v1,c1)=0.5*angle(v,c) et v1 reste norme        */
  v1=g3x_AddVect(v1,c1);
  g3x_Normalize(&v1);

  /* direction de rotation : v1^c1 de norme sin(a) */
  G3Xprodvect(w,v1,c1);

  /* et cos(a)=v1*c1 */
  return g3x_QuatSet(G3Xprodscal(v1,c1),w);
}


#ifdef __cplusplus
  }
#endif
/*=============================================================================*/
