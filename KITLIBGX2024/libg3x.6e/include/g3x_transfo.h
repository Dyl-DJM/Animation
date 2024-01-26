/**@file    g3x.h
 * @author  Universite Gustave Eiffel
 * @author  E.Incerti - eric.incerti@univ-eiffel.fr
 * @brief   Base public control functions
 * @version 6.e
 * @date    Aug.2022 (doc generation)
 */
/**@cond SHOW_SECURITY_DEFINE */
#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#ifndef _G3X_TRANSFO_H
  #define _G3X_TRANSFO_H
/**@endcond                   */

  #include <g3x_types.h>

  /*! MATRICE EN COORDONNEES HOMOGENE
   *  type matrice en coordonnees homogenes
   *  ATTENTION  : OpenGl gere ça directement en <double mat[16]
   *               et traite ça par colonne plutôt que par ligne
  !*/
  typedef struct { double m[16]; } G3Xhmat;
  /*! | 00 04 08 | 12 |!*/
  /*! | 01 05 09 | 13 |!*/
  /*! | 02 06 10 | 14 |!*/
  /*! | 03 07 11 | 15 |!*/

   /*! 1° colonne!*/
  #define c00  0
  #define c10  1
  #define c20  2
  #define c30  3
  /*! 2° colonne!*/
  #define c01  4
  #define c11  5
  #define c21  6
  #define c31  7
  /*! 3° colonne!*/
  #define c02  8
  #define c12  9
  #define c22 10
  #define c32 11
  /*! 4° colonne!*/
  #define c03 12
  #define c13 13
  #define c23 14
  #define c33 15


  /*=****************************************!*/
  /*!  Identite                             !*/
  /*=****************************************!*/
  G3Xhmat   g3x_Identity(void);
  /*=****************************************!*/
  /*!  MATRICES DE TRANSFORMATION STANDARD  !*/
  /*=****************************************!*/
  G3Xhmat   g3x_Translation1v(G3Xvector t);
  G3Xhmat   g3x_Translation3d(double tx, double ty, double tz);
  G3Xhmat   g3x_Homothetie1v(G3Xvector h);
  G3Xhmat   g3x_Homothetie3d(double hx, double hy, double hz);
  G3Xhmat   g3x_RotationZ(double a_radians);
  G3Xhmat   g3x_RotationY(double a_radians);
  G3Xhmat   g3x_RotationX(double a_radians);
  /*=****************************************!*/
  /*! Fonctions utiltaires sur les matrices !*/
  /*=****************************************!*/
  /*! Produit Matrice(A)*Vecteur(V) -> Vecteur!*/
  G3Xvector g3x_Mat_x_Vector(G3Xhmat A, G3Xvector V);
  /*! Produit Matrice(A)*Point(P) -> Point    !*/
  G3Xpoint  g3x_Mat_x_Point(G3Xhmat A, G3Xpoint P);
  /*! version generique des 2 precedentes     !*/
  G3Xcoord  g3x_Mat_x_Coord3d(G3Xhmat A, double x, double y, double z, double delta);
  /*! Produit Matrice(A)*Matrice(B)->Matrice  !*/
  G3Xhmat   g3x_Mat_x_Mat(G3Xhmat A,G3Xhmat B);
  /*! Affichage de la matrice!*/
  void g3x_Mat_print(G3Xhmat A);
  /*! conversion (transposition) au format de amatrice OpenGl!*/
  G3Xhmat g3x_FormatGLmat(G3Xhmat src);

  bool g3x_Mat_Invert(G3Xhmat M, G3Xhmat *Mi);

#endif

#ifdef __cplusplus
  }
#endif
/*============================================================================!*/
