/**@file    g2x_transfo.h
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

#ifndef _G2X_TRANSFO_H
  #define _G2X_TRANSFO_H
/**@endcond                   */

  #include <g2x.h>

  /*! MATRICE EN COORDONNEES HOMOGENE
   *  type matrice en coordonnees homogenes
   *  ATTENTION  : OpenGl gere ça directement en <double mat[9]
   *               et traite ça par colonne plutôt que par ligne
  !*/
  typedef struct { double m[9]; } G2Xhmat;
  /* | 00 03 | 06 | */
  /* | 01 04 | 07 | */
  /* | 02 05 | 08 | */

   /* 1° colonne */
  #define c00  0
  #define c10  1
  #define c20  2
  /* 2° colonne */
  #define c01  3
  #define c11  4
  #define c21  5
  /* 3° colonne */
  #define c02  6
  #define c12  7
  #define c22  8

  /*!****************************************!*/
  /*!   Identite                             !*/
  /*!****************************************!*/
  G2Xhmat   g2x_Identity(void);
  /*!****************************************!*/
  /*!   MATRICES DE TRANSFORMATION STANDARD  !*/
  /*!****************************************!*/
  G2Xhmat   g2x_Translation1v(G2Xvector t);
  G2Xhmat   g2x_Translation2d(double tx, double ty);
  G2Xhmat   g2x_Homothetie1v(G2Xvector h);
  G2Xhmat   g2x_Homothetie2d(double hx, double hy);
  G2Xhmat   g2x_Rotation(double a_radians);
  /** version (en degrés) utilisant les tables (LUT)
   *  de cos/sin prédéfinie -- précision 1/2 degré
   *  moins précis, mais plus rapide         **/
  G2Xhmat   g2x_Rotation_LUT(double a_degre);

  /*!****************************************!*/
  /*!  Fonctions utiltaires sur les matrices !*/
  /*!****************************************!*/
  /* Produit Matrice(A)*Vecteur(V) -> Vecteur */
  G2Xvector g2x_Mat_x_Vector(G2Xhmat A, G2Xvector V);
  /* Produit Matrice(A)*Point(P) -> Point     */
  G2Xpoint  g2x_Mat_x_Point(G2Xhmat A, G2Xpoint P);
  /* version generique des 2 precedentes      */
  G2Xcoord  g2x_Mat_x_Coord2d(G2Xhmat A, double x, double y, double delta);
  /* Produit Matrice(A)*Matrice(B)->Matrice   */
  G2Xhmat   g2x_Mat_x_Mat(G2Xhmat A,G2Xhmat B);
  /* Affichage de la matrice */
  void      g2x_Mat_print(G2Xhmat A);
  /* Creation de la matrice 3x3 de rotation autour du G2Xpoint C, d'angle alpha */
  G2Xhmat   g2x_RotationC(G2Xpoint C, float alpha);
  G2Xhmat   g2x_RotationCXY(double Cx, double Cy, float alpha) ;

#endif

#ifdef __cplusplus
  }
#endif
/*!===========================================================================!*/
