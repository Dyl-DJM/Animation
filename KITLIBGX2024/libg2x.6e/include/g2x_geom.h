/**@file    g2x_geom.h
 * @author  Universite Gustave Eiffel
 * @author  E.Incerti - eric.incerti@univ-eiffel.fr
 * @brief   Base public control functions
 * @version 6.e
 * @date    Aug.2022 (doc generation)
 */
/**@cond SHOW_SECURITY_DEFINE */
#ifdef __cplusplus
  extern "C" {
#endif

#ifndef _G2X_GEOM_H
  #define _G2X_GEOM_H
/**@endcond                   */

  #include <g2x.h>

  /******************************************************************
   * Coordonnees XY
   * ----------------------------------------------------------------
   * Le type 'primaire' G3Xcoord s'utilise lorsqu'on manipule des
   * coordonnées qui ne sont pas clairement identifiées comme <Point>
   * ou <Vecteur>
   ******************************************************************/
  typedef struct
  {
    double x,y;
  }
  G2Xcoord, G2Xpoint, G2Xvector;

  /** PRIMITIVES GEOMETRIQUES ET FONCTIONS UTILES                  **/
            /** construit le point (x,y) **/
  G2Xpoint  g2x_Point2d(double x, double y);
            /** construit le point Q=P+V **/
  G2Xpoint  g2x_PplusV(G2Xpoint P, G2Xvector V);
            /** crée un point aléatoire dans la fenêtre (avec une marge de securite) **/
  G2Xpoint  g2x_RandPoint(double margin);

            /** construit le vecteur (x,y) **/
  G2Xvector g2x_Vector2d(double x, double y);
            /** construit le vecteur AB **/
  G2Xvector g2x_Vector2p(G2Xpoint A, G2Xpoint B);
            /** crée un vecteur de direction aléatoire de norme definie  **/
  G2Xvector g2x_RandVector(double norm);

            /** construit le vecteur U+V **/
  G2Xvector g2x_AddVect(G2Xvector U, G2Xvector V);
            /** construit le vecteur U-V **/
  G2Xvector g2x_SubVect(G2Xvector U, G2Xvector V);

            /** rotation d'angle "radians" sur le vecteur U **/
  G2Xvector g2x_VectRotate(G2Xvector U, double radians);

            /** mapping scalaire  => C*map **/
  G2Xcoord  g2x_mapscal2(G2Xcoord C, double   map);
            /** mapping vectoriel => C*map **/
  G2Xcoord  g2x_mapvect2(G2Xcoord C, G2Xcoord map);

            /** renvoie une copie 'normee' de U (V=U/||U||) **/
  G2Xvector g2x_NormalVector(G2Xvector U);
            /** renvoie le vecteur norme AB/d(A,B)          **/
  G2Xvector g2x_NormalVector2p(G2Xpoint A, G2Xpoint B);
            /** normalise le vecteur V et renvoie la norme  **/
  double    g2x_Normalize(G2Xvector *V);
      /** calcule le produit scalaire U°V et AB°AC**/
  double    g2x_ProdScal(G2Xvector U, G2Xvector V);
  double    g2x_ProdScal3p(G2Xpoint A, G2Xpoint B, G2Xpoint C);

            /** produit vectoriel (U^V).z et (AB^AC).z  : en 2D c'est un scalaire !!!! **/
  double    g2x_ProdVect(G2Xvector U, G2Xvector V);
  double    g2x_ProdVect3p(G2Xpoint A, G2Xpoint B, G2Xpoint C);

            /** renvoie le milieu du segment (AB)       **/
  G2Xpoint  g2x_Milieu(G2Xpoint A, G2Xpoint B);
            /** renvoie le barycentre du triangle (ABC) **/
  G2Xpoint  g2x_Baryc3(G2Xpoint A, G2Xpoint B, G2Xpoint C);

            /** distance euclidienne au carre **/
  double    g2x_SqrDist(G2Xpoint A,G2Xpoint B);
            /** distance euclidienne **/
  double    g2x_Dist(G2Xpoint A, G2Xpoint B);

            /** norme euclidienne du vecteur V au carre (v.x)²+(v.y)² **/
  double    g2x_SqrNorm(G2Xvector V);
            /** norme euclidienne du vecteur V                        **/
  double    g2x_Norm(G2Xvector V);
            /** norme L2 du vecteur V  max(|v.x|,|v.y|)               **/
  double    g2x_NormL2(G2Xvector V);

            /** retourne +1 si (ABC) orienté trigo+
               0 .........alignés
              -1 .........orienté trigo- **/
  int       g2x_Orient(G2Xpoint A, G2Xpoint B, G2Xpoint C);


  /**---------------------------------------------------------------*
   * FONCTIONS TRIGO USUELLES
   * ---------------------------------------------------------------*/
             /** appelée automatiquement : NE PAS UTILISER **/
  bool      g2x_InitTrigonometric(void);

            /** cos./sin. utilisant des LUT prédéfinies (720 valeurs = 1/2 degré)  *
             *  un peu moins précis, mais BEAUCOUP plus rapide                     **/
  double    g2x_Idxcos(int    idx); /** angle : valeur d'indice ramenée dans [0,720] (1/2 degré) **/
  double    g2x_Idxsin(int    idx);
  double    g2x_Radcos(double rad); /** angle : valeur en radian ramenée dans [0,720] **/
  double    g2x_Radsin(double rad);
  double    g2x_Degcos(double deg); /** angle : valeur en degré ramenée dans [0,720] **/
  double    g2x_Degsin(double deg);

            /** calcule le cosinus de l'angle (U,V) -- produit scalaire  normé **/
  double    g2x_CosVect(G2Xvector U, G2Xvector V);
            /** calcule le   sinus de l'angle (U,V) -- produit vectoriel normé **/
  double    g2x_SinVect(G2Xvector U, G2Xvector V);


  /** Quelques versions en macros - conservées pour compatibilité avec anciennes versions *
   *  MAIS IL FAUT EVITER DE LES UTILISER                                                **/
  #define G2Xmilieu(A,B)                      /***/((G2Xpoint ){(0.5*((A).x+(B).x)),(0.5*((A).y+(B).y))})
  #define G2Xdefvectorxy(x,y)                 /***/((G2Xvector){(double)(x),(double)(y)})
  #define G2Xdefvector(A,B)                   /***/((G2Xvector){((B).x-(A).x),((B).y-(A).y)})
  #define G2Xnormalvector2P(A,B)              /***/((G2Xvector){((A).y-(B).y),((B).x-(A).x)})
  #define G2Xnormalvector(V)                  /***/((G2Xvector){(-(V).y),((V).x)})
  #define G2Xaddvector(U,V)                   /***/((G2Xvector){((U).x+(V).x),((U).y+(V).y)})
  #define G2Xdifvector(U,V)                   /***/((G2Xvector){((U).x-(V).x),((U).y-(V).y)})
  #define G2Xmulvector(U,a)                   /***/((G2Xvector){((double)(a))*(U).x,((double)(a))*(U).y})
  #define G2Xpointplusvector(A,V)             /***/((G2Xpoint ){((A).x+(V).x),((A).y+(V).y)})
  #define G2Xsqrdist(A,B)                     /***/(SQR((B).x-(A).x)+SQR((B).y-(A).y))
  #define G2Xdist(A,B)                        /***/(sqrt(G2Xsqrdist((A),(B))))
  #define G2Xsqrnorm(U)                       /***/(SQR((U).x)+SQR((U).y))
  #define G2Xnorm(U)                          /***/(sqrt(G2Xsqrnorm(U)))
  #define G2Xprodscal(U,V)                    /***/(((U).x*(V).x)+((U).y*(V).y))
  #define G2Xpntprodscal(A,B,C)               /***/((((B).x-(A).x)*((C).x-(A).x)) + (((B).y-(A).y)*((C).y-(A).y)))
  #define G2Xprodvect(U,V)                    /***/( ((U).x*(V).y)-((U).y*(V).x))
  #define G2Xprodvect3P(A,B,C)                /***/((((B).x-(A).x)*((C).y-(A).y) - ((B).y-(A).y)*((C).x-(A).x)))
  #define G2Xorient(A,B,C)                    /***/((((B).x-(A).x)*((C).y-(A).y) - ((B).y-(A).y)*((C).x-(A).x))>0.?+1:-1)
  #define G2Xorientv(u,v)                     /***/( ((u).x*(v).y>(u).y*(v).x)?+1:-1)

#endif

#ifdef __cplusplus
  }
#endif
/*!=============================================================!**/
