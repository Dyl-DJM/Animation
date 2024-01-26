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

extern G2Xhmat g2x_Identity(void)
{
  G2Xhmat Id;
  memset(Id.m,0,9*sizeof(double));
  Id.m[c00] = Id.m[c11] = Id.m[c22] = 1.0;
  return Id;
}

extern G2Xhmat g2x_Translation1v(G2Xvector t)
{
  G2Xhmat A=g2x_Identity();
  A.m[c02] = t.x;
  A.m[c12] = t.y;
  return A;
}

extern G2Xhmat g2x_Translation2d(double tx, double ty)
{
  G2Xhmat A=g2x_Identity();
  A.m[c02] = tx;
  A.m[c12] = ty;
  return A;
}

extern G2Xhmat g2x_Homothetie1v(G2Xvector h)
{
  G2Xhmat A=g2x_Identity();
  A.m[c00] = h.x;
  A.m[c11] = h.y;
  return A;
}

extern G2Xhmat g2x_Homothetie2d(double hx, double hy)
{
  G2Xhmat A=g2x_Identity();
  A.m[c00] = hx;
  A.m[c11] = hy;
  return A;
}

extern G2Xhmat g2x_Rotation(double a_radians)
{
  G2Xhmat A=g2x_Identity();
  double c=cos(a_radians), s=sin(a_radians);
  A.m[c00] = +c; A.m[c01] = -s;
  A.m[c10] = +s; A.m[c11] = +c;
  return A;
}

/** version (en degrés) utilisant les tables (LUT)
  *  de cos/sin prédéfinie -- précision 1/2 degré
  *  moins précis, mais plus rapide         **/
extern G2Xhmat g2x_Rotation_LUT(double a_degre)
{
  G2Xhmat A=g2x_Identity();
  double c=g2x_Degcos(a_degre), s=g2x_Degsin(a_degre);
  A.m[c00] = +c; A.m[c01] = -s;
  A.m[c10] = +s; A.m[c11] = +c;
  return A;
}

extern G2Xvector g2x_Mat_x_Vector(G2Xhmat A, G2Xvector V)
{
  G2Xvector U;
  U.x = A.m[c00]*V.x + A.m[c01]*V.y;
  U.y = A.m[c10]*V.x + A.m[c11]*V.y;
  return U;
}

extern G2Xpoint g2x_Mat_x_Point(G2Xhmat A, G2Xpoint P)
{
  G2Xpoint M;
  M.x = A.m[c00]*P.x + A.m[c01]*P.y + A.m[c02];
  M.y = A.m[c10]*P.x + A.m[c11]*P.y + A.m[c12];
  return M;
}

extern G2Xcoord g2x_Mat_x_Coord2d(G2Xhmat A, double x, double y, double delta)
{
  G2Xcoord M;
  M.x = A.m[c00]*x + A.m[c01]*y + A.m[c02]*delta;
  M.y = A.m[c10]*x + A.m[c11]*y + A.m[c12]*delta;
  return M;
}

extern G2Xhmat g2x_Mat_x_Mat(G2Xhmat A,G2Xhmat B)
{
  G2Xhmat D;
  /* 1ere colonne */
  D.m[c00] = A.m[c00]*B.m[c00] + A.m[c01]*B.m[c10] + A.m[c02]*B.m[c20];
  D.m[c10] = A.m[c10]*B.m[c00] + A.m[c11]*B.m[c10] + A.m[c12]*B.m[c20];
  D.m[c20] = A.m[c20]*B.m[c00] + A.m[c21]*B.m[c10] + A.m[c22]*B.m[c20];
  /* 2eme colonne */
  D.m[c01] = A.m[c00]*B.m[c01] + A.m[c01]*B.m[c11] + A.m[c02]*B.m[c21];
  D.m[c11] = A.m[c10]*B.m[c01] + A.m[c11]*B.m[c11] + A.m[c12]*B.m[c21];
  D.m[c21] = A.m[c20]*B.m[c01] + A.m[c21]*B.m[c11] + A.m[c22]*B.m[c21];
  /* 3eme colonne */
  D.m[c02] = A.m[c00]*B.m[c02] + A.m[c01]*B.m[c12] + A.m[c02]*B.m[c22];
  D.m[c12] = A.m[c10]*B.m[c02] + A.m[c11]*B.m[c12] + A.m[c12]*B.m[c22];
  D.m[c22] = A.m[c20]*B.m[c02] + A.m[c21]*B.m[c12] + A.m[c22]*B.m[c22];
  return D;
}

/* Creation de la matrice 3x3 de rotation autour du G2Xpoint C, d'angle alpha */
extern G2Xhmat g2x_RotationC(G2Xpoint C, float alpha)
{
  return g2x_Mat_x_Mat(g2x_Translation2d(C.x,C.y),g2x_Mat_x_Mat(g2x_Rotation(alpha),g2x_Translation2d(-C.x,-C.y)));
}

extern G2Xhmat g2x_RotationCXY(double Cx, double Cy, float alpha)
{
  return g2x_Mat_x_Mat(g2x_Translation2d(Cx,Cy),
                       g2x_Mat_x_Mat(g2x_Rotation(alpha),g2x_Translation2d(-Cx,-Cy)));
}


extern void g2x_Mat_print(G2Xhmat A)
{
  fprintf(stderr,"Matrice :\n");
  fprintf(stderr,"|%f\t%f\t%f|\n",A.m[c00],A.m[c01],A.m[c02]);
  fprintf(stderr,"|%f\t%f\t%f|\n",A.m[c10],A.m[c11],A.m[c12]);
  fprintf(stderr,"|%f\t%f\t%f|\n",A.m[c20],A.m[c21],A.m[c22]);
  fprintf(stderr,"\n");
}

#ifdef __cplusplus
  }
#endif
/*=============================================================================*/

