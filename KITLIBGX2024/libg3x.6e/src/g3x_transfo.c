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

extern G3Xhmat g3x_Identity(void)
{
  G3Xhmat Id;
  memset(Id.m,0,16*sizeof(double));
  Id.m[c00] = Id.m[c11] = Id.m[c22] = Id.m[c33] = 1.0;
  return Id;
}

extern G3Xhmat g3x_Translation1v(G3Xvector t)
{
  G3Xhmat A=g3x_Identity();
  A.m[c03] = t.x;
  A.m[c13] = t.y;
  A.m[c23] = t.z;
  return A;
}

extern G3Xhmat g3x_Translation3d(double tx, double ty, double tz)
{
  G3Xhmat A=g3x_Identity();
  A.m[c03] = tx;
  A.m[c13] = ty;
  A.m[c23] = tz;
  return A;
}

extern G3Xhmat g3x_Homothetie1v(G3Xvector h)
{
  G3Xhmat A=g3x_Identity();
  A.m[c00] = h.x;
  A.m[c11] = h.y;
  A.m[c22] = h.z;
  return A;
}

extern G3Xhmat g3x_Homothetie3d(double hx, double hy, double hz)
{
  G3Xhmat A=g3x_Identity();
  A.m[c00] = hx;
  A.m[c11] = hy;
  A.m[c22] = hz;
  return A;
}

extern G3Xhmat g3x_RotationX(double a_radians)
{
  G3Xhmat A=g3x_Identity();
  double c=cos(a_radians), s=sin(a_radians);
  A.m[c11] = +c; A.m[c12] = -s;
  A.m[c21] = +s; A.m[c22] = +c;
  return A;
}

extern G3Xhmat g3x_RotationY(double a_radians)
{
  G3Xhmat A=g3x_Identity();
  double c=cos(a_radians), s=sin(a_radians);
  A.m[c00] = +c; A.m[c02] = +s;
  A.m[c20] = -s; A.m[c22] = +c;
  return A;
}

extern G3Xhmat g3x_RotationZ(double a_radians)
{
  G3Xhmat A=g3x_Identity();
  double c=cos(a_radians), s=sin(a_radians);
  A.m[c00] = +c; A.m[c01] = -s;
  A.m[c10] = +s; A.m[c11] = +c;
  return A;
}

extern G3Xvector g3x_Mat_x_Vector(G3Xhmat A, G3Xvector V)
{
  G3Xvector U;
  U.x = A.m[c00]*V.x + A.m[c01]*V.y + A.m[c02]*V.z;
  U.y = A.m[c10]*V.x + A.m[c11]*V.y + A.m[c12]*V.z;
  U.z = A.m[c20]*V.x + A.m[c21]*V.y + A.m[c22]*V.z;
  return U;
}

extern G3Xpoint g3x_Mat_x_Point(G3Xhmat A, G3Xpoint P)
{
  G3Xpoint M;
  M.x = A.m[c00]*P.x + A.m[c01]*P.y + A.m[c02]*P.z + A.m[c03];
  M.y = A.m[c10]*P.x + A.m[c11]*P.y + A.m[c12]*P.z + A.m[c13];
  M.z = A.m[c20]*P.x + A.m[c21]*P.y + A.m[c22]*P.z + A.m[c23];
  return M;
}

extern G3Xcoord g3x_Mat_x_Coord3d(G3Xhmat A, double x, double y, double z, double delta)
{
  G3Xcoord M;
  M.x = A.m[c00]*x + A.m[c01]*y + A.m[c02]*z + A.m[c03]*delta;
  M.y = A.m[c10]*x + A.m[c11]*y + A.m[c12]*z + A.m[c13]*delta;
  M.z = A.m[c20]*x + A.m[c21]*y + A.m[c22]*z + A.m[c23]*delta;
  return M;
}

extern G3Xhmat g3x_Mat_x_Mat(G3Xhmat A,G3Xhmat B)
{
  G3Xhmat D;
  D.m[c00] = A.m[c00]*B.m[c00] + A.m[c01]*B.m[c10] + A.m[c02]*B.m[c20] + A.m[c03]*B.m[c30];
  D.m[c10] = A.m[c10]*B.m[c00] + A.m[c11]*B.m[c10] + A.m[c12]*B.m[c20] + A.m[c13]*B.m[c30];
  D.m[c20] = A.m[c20]*B.m[c00] + A.m[c21]*B.m[c10] + A.m[c22]*B.m[c20] + A.m[c23]*B.m[c30];
  D.m[c30] = A.m[c30]*B.m[c00] + A.m[c31]*B.m[c10] + A.m[c32]*B.m[c20] + A.m[c33]*B.m[c30];
  D.m[c01] = A.m[c00]*B.m[c01] + A.m[c01]*B.m[c11] + A.m[c02]*B.m[c21] + A.m[c03]*B.m[c31];
  D.m[c11] = A.m[c10]*B.m[c01] + A.m[c11]*B.m[c11] + A.m[c12]*B.m[c21] + A.m[c13]*B.m[c31];
  D.m[c21] = A.m[c20]*B.m[c01] + A.m[c21]*B.m[c11] + A.m[c22]*B.m[c21] + A.m[c23]*B.m[c31];
  D.m[c31] = A.m[c30]*B.m[c01] + A.m[c31]*B.m[c11] + A.m[c32]*B.m[c21] + A.m[c33]*B.m[c31];
  D.m[c02] = A.m[c00]*B.m[c02] + A.m[c01]*B.m[c12] + A.m[c02]*B.m[c22] + A.m[c03]*B.m[c32];
  D.m[c12] = A.m[c10]*B.m[c02] + A.m[c11]*B.m[c12] + A.m[c12]*B.m[c22] + A.m[c13]*B.m[c32];
  D.m[c22] = A.m[c20]*B.m[c02] + A.m[c21]*B.m[c12] + A.m[c22]*B.m[c22] + A.m[c23]*B.m[c32];
  D.m[c32] = A.m[c30]*B.m[c02] + A.m[c31]*B.m[c12] + A.m[c32]*B.m[c22] + A.m[c33]*B.m[c32];
  D.m[c03] = A.m[c00]*B.m[c03] + A.m[c01]*B.m[c13] + A.m[c02]*B.m[c23] + A.m[c03]*B.m[c33];
  D.m[c13] = A.m[c10]*B.m[c03] + A.m[c11]*B.m[c13] + A.m[c12]*B.m[c23] + A.m[c13]*B.m[c33];
  D.m[c23] = A.m[c20]*B.m[c03] + A.m[c21]*B.m[c13] + A.m[c22]*B.m[c23] + A.m[c23]*B.m[c33];
  D.m[c33] = A.m[c30]*B.m[c03] + A.m[c31]*B.m[c13] + A.m[c32]*B.m[c23] + A.m[c33]*B.m[c33];
  return D;
}

/* conversion (transposition) au format de amatrice OpenGl */
extern  G3Xhmat g3x_FormatGLmat(G3Xhmat src)
{
  G3Xhmat dst;
  dst.m[c00]=src.m[c00]; dst.m[c10]=src.m[c01]; dst.m[c20]=src.m[c02]; dst.m[c30]=src.m[c03];
  dst.m[c01]=src.m[c10]; dst.m[c11]=src.m[c11]; dst.m[c21]=src.m[c12]; dst.m[c31]=src.m[c13];
  dst.m[c02]=src.m[c20]; dst.m[c12]=src.m[c21]; dst.m[c22]=src.m[c22]; dst.m[c32]=src.m[c23];
  dst.m[c03]=src.m[c30]; dst.m[c13]=src.m[c31]; dst.m[c23]=src.m[c32]; dst.m[c33]=src.m[c33];
  return dst;
}


extern void g3x_Mat_print(G3Xhmat A)
{
  fprintf(stderr,"Matrice :\n");
  fprintf(stderr,"|%f\t%f\t%f\t%f|\n",A.m[c00],A.m[c01],A.m[c02],A.m[c03]);
  fprintf(stderr,"|%f\t%f\t%f\t%f|\n",A.m[c10],A.m[c11],A.m[c12],A.m[c13]);
  fprintf(stderr,"|%f\t%f\t%f\t%f|\n",A.m[c20],A.m[c21],A.m[c22],A.m[c23]);
  fprintf(stderr,"|%f\t%f\t%f\t%f|\n",A.m[c30],A.m[c31],A.m[c32],A.m[c33]);
  fprintf(stderr,"\n");
}


/*
 ** Invert 4x4 matrix.
 ** Contributed by David Moore (See Mesa bug #6748)
 */
 extern bool g3x_Mat_Invert(G3Xhmat M, G3Xhmat *Mi)
 {
    double inv[16], det;
    int i;

    inv[ 0] =   M.m[ 5]*M.m[10]*M.m[15] - M.m[ 5]*M.m[11]*M.m[14] - M.m[ 9]*M.m[ 6]*M.m[15]
              + M.m[ 9]*M.m[ 7]*M.m[14] + M.m[13]*M.m[ 6]*M.m[11] - M.m[13]*M.m[ 7]*M.m[10];
    inv[ 4] = - M.m[ 4]*M.m[10]*M.m[15] + M.m[ 4]*M.m[11]*M.m[14] + M.m[ 8]*M.m[ 6]*M.m[15]
              - M.m[ 8]*M.m[ 7]*M.m[14] - M.m[12]*M.m[ 6]*M.m[11] + M.m[12]*M.m[ 7]*M.m[10];
    inv[ 8] =   M.m[ 4]*M.m[ 9]*M.m[15] - M.m[ 4]*M.m[11]*M.m[13] - M.m[ 8]*M.m[ 5]*M.m[15]
              + M.m[ 8]*M.m[ 7]*M.m[13] + M.m[12]*M.m[ 5]*M.m[11] - M.m[12]*M.m[ 7]*M.m[ 9];
    inv[12] = - M.m[ 4]*M.m[ 9]*M.m[14] + M.m[ 4]*M.m[10]*M.m[13] + M.m[ 8]*M.m[ 5]*M.m[14]
              - M.m[ 8]*M.m[ 6]*M.m[13] - M.m[12]*M.m[ 5]*M.m[10] + M.m[12]*M.m[ 6]*M.m[ 9];
    inv[ 1] =  -M.m[ 1]*M.m[10]*M.m[15] + M.m[ 1]*M.m[11]*M.m[14] + M.m[ 9]*M.m[ 2]*M.m[15]
              - M.m[ 9]*M.m[ 3]*M.m[14] - M.m[13]*M.m[ 2]*M.m[11] + M.m[13]*M.m[ 3]*M.m[10];
    inv[ 5] =   M.m[ 0]*M.m[10]*M.m[15] - M.m[ 0]*M.m[11]*M.m[14] - M.m[ 8]*M.m[ 2]*M.m[15]
              + M.m[ 8]*M.m[ 3]*M.m[14] + M.m[12]*M.m[ 2]*M.m[11] - M.m[12]*M.m[ 3]*M.m[10];
    inv[ 9] =  -M.m[ 0]*M.m[ 9]*M.m[15] + M.m[ 0]*M.m[11]*M.m[13] + M.m[ 8]*M.m[ 1]*M.m[15]
              - M.m[ 8]*M.m[ 3]*M.m[13] - M.m[12]*M.m[ 1]*M.m[11] + M.m[12]*M.m[ 3]*M.m[ 9];
    inv[13] =   M.m[ 0]*M.m[ 9]*M.m[14] - M.m[ 0]*M.m[10]*M.m[13] - M.m[ 8]*M.m[ 1]*M.m[14]
              + M.m[ 8]*M.m[ 2]*M.m[13] + M.m[12]*M.m[ 1]*M.m[10] - M.m[12]*M.m[ 2]*M.m[ 9];
    inv[ 2] =   M.m[ 1]*M.m[ 6]*M.m[15] - M.m[ 1]*M.m[ 7]*M.m[14] - M.m[ 5]*M.m[ 2]*M.m[15]
              + M.m[ 5]*M.m[ 3]*M.m[14] + M.m[13]*M.m[ 2]*M.m[ 7] - M.m[13]*M.m[ 3]*M.m[ 6];
    inv[ 6] = - M.m[ 0]*M.m[ 6]*M.m[15] + M.m[ 0]*M.m[ 7]*M.m[14] + M.m[ 4]*M.m[ 2]*M.m[15]
              - M.m[ 4]*M.m[ 3]*M.m[14] - M.m[12]*M.m[ 2]*M.m[ 7] + M.m[12]*M.m[ 3]*M.m[ 6];
    inv[10] =   M.m[ 0]*M.m[ 5]*M.m[15] - M.m[ 0]*M.m[ 7]*M.m[13] - M.m[ 4]*M.m[ 1]*M.m[15]
              + M.m[ 4]*M.m[ 3]*M.m[13] + M.m[12]*M.m[ 1]*M.m[ 7] - M.m[12]*M.m[ 3]*M.m[ 5];
    inv[14] = - M.m[ 0]*M.m[ 5]*M.m[14] + M.m[ 0]*M.m[ 6]*M.m[13] + M.m[ 4]*M.m[ 1]*M.m[14]
              - M.m[ 4]*M.m[ 2]*M.m[13] - M.m[12]*M.m[ 1]*M.m[ 6] + M.m[12]*M.m[ 2]*M.m[ 5];
    inv[ 3] =  -M.m[ 1]*M.m[ 6]*M.m[11] + M.m[ 1]*M.m[ 7]*M.m[10] + M.m[ 5]*M.m[ 2]*M.m[11]
              - M.m[ 5]*M.m[ 3]*M.m[10] - M.m[ 9]*M.m[ 2]*M.m[ 7] + M.m[ 9]*M.m[ 3]*M.m[ 6];
    inv[ 7] =   M.m[ 0]*M.m[ 6]*M.m[11] - M.m[ 0]*M.m[ 7]*M.m[10] - M.m[ 4]*M.m[ 2]*M.m[11]
              + M.m[ 4]*M.m[ 3]*M.m[10] + M.m[ 8]*M.m[ 2]*M.m[ 7] - M.m[ 8]*M.m[ 3]*M.m[ 6];
    inv[11] = - M.m[ 0]*M.m[ 5]*M.m[11] + M.m[ 0]*M.m[ 7]*M.m[ 9] + M.m[ 4]*M.m[ 1]*M.m[11]
              - M.m[ 4]*M.m[ 3]*M.m[ 9] - M.m[ 8]*M.m[ 1]*M.m[ 7] + M.m[ 8]*M.m[ 3]*M.m[ 5];
    inv[15] =   M.m[ 0]*M.m[ 5]*M.m[10] - M.m[ 0]*M.m[ 6]*M.m[ 9] - M.m[ 4]*M.m[ 1]*M.m[10]
              + M.m[ 4]*M.m[ 2]*M.m[ 9] + M.m[ 8]*M.m[ 1]*M.m[ 6] - M.m[ 8]*M.m[ 2]*M.m[ 5];

     det = M.m[ 0]*inv[ 0] + M.m[ 1]*inv[ 4] + M.m[ 2]*inv[ 8] + M.m[ 3]*inv[12];
     if (det == 0)
         return false;

     det = 1.0/det;
     for (i=0;i<16;i++) Mi->m[i] = inv[i]*det;

     return true;
 }

#ifdef __cplusplus
  }
#endif
/*=============================================================================*/

