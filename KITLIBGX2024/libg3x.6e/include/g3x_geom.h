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

#ifndef _G3X_GEOM_H
  #define _G3X_GEOM_H
/**@endcond                   */

  #include <g3x.h>

  /******************************************************************
   * Coordonnees XYZ - Attention OpenGl gere ca en <double xyz[3]>
   * ----------------------------------------------------------------
   * Le type 'primaire' G3Xcoord s'utilise lorsqu'on manipule des
   * coordonnées qui ne sont pas clairement identifiées comme <Point>
   * ou <Vecteur>
   ******************************************************************/
  typedef struct
  {
    double x,y,z;
  }
  G3Xcoord, G3Xpoint, G3Xvector;

  /* A VOIR : introduire un type "Coord. Sphérique" (r,theta,phi)
   * typedef struct
   * {
   *   double r,t,f;
   * }
   * A voir en vis à vis avec <g3x_quaternions> et <g3x_particules>
   * => bcp de choses proches
   */

            /****************************************************************/
            /*! PRIMITIVES GEOMETRIQUES ET FONCTIONS UTILES                !*/
            /****************************************************************/
            /*! construit le point (x,y,z)                                 !*/
  G3Xpoint  g3x_Point3d(double x, double y, double z);
            /*! construit le point Q=P+V                                   !*/
  G3Xpoint  g3x_PplusV(G3Xpoint P, G3Xvector V);

            /*! construit le vecteur (x,y,z)!*/
  G3Xvector g3x_Vector3d(double x, double y, double z);
            /*! construit le vecteur AB!*/
  G3Xvector g3x_Vector2p(G3Xpoint A, G3Xpoint B);
            /*! crée un vecteur de direction, aléatoire de norme < maxnorm !*/
  G3Xvector g3x_RandVector(double maxnorm);

            /*! construit le vecteur U+V!*/
  G3Xvector g3x_AddVect(G3Xvector U, G3Xvector V);
            /*! construit le vecteur U-V!*/
  G3Xvector g3x_SubVect(G3Xvector U, G3Xvector V);

            /*! renvoie le milieu du segment (AB)      !*/
  G3Xpoint  g3x_Milieu(G3Xpoint A, G3Xpoint B);
            /*! renvoie le barycentre du triangle (ABC)!*/
  G3Xpoint  g3x_Baryc3(G3Xpoint A, G3Xpoint B, G3Xpoint C);

            /*! mapping scalaire  => C*map!*/
  G3Xcoord  g3x_mapscal3(G3Xcoord C, double   map);
            /*! mapping vectoriel => C*map!*/
  G3Xcoord  g3x_mapvect3(G3Xcoord C, G3Xcoord map);

  /* conventions de nommage à revoir */

            /*! calcule le produit scalaire U°V, AB°U=U°AB et AB°AC*/
  double    g3x_ProdScal(G3Xvector U, G3Xvector V);
  double    g3x_ProdScal3p(G3Xpoint A, G3Xpoint B, G3Xpoint C);
  double    g3x_ProdScal2p1v(G3Xpoint A, G3Xpoint B, G3Xvector U);

            /*! produit vectoriel (U^V), (AB^V), (U^AC) et (AB^AC)!*/
  G3Xvector  g3x_ProdVect(G3Xvector U, G3Xvector V);
  G3Xvector  g3x_ProdVect2p1v(G3Xpoint A, G3Xpoint B, G3Xvector V);
  G3Xvector  g3x_ProdVect1v2p(G3Xvector U,G3Xpoint A, G3Xpoint C);
  G3Xvector  g3x_ProdVect3p(G3Xpoint A, G3Xpoint B, G3Xpoint C);

            /*! normalise le vecteur!*/
  G3Xvector g3x_NormalVector(G3Xvector U);
  G3Xvector g3x_NormalVector2p(G3Xpoint A, G3Xpoint B);
            /*! normalise le vecteur V et renvoie la norme!*/
  double    g3x_Normalize(G3Xvector *V);

            /*! distance euclidienne au carre!*/
  double    g3x_SqrDist(G3Xpoint A,G3Xpoint B);
            /*! distance euclidienne!*/
  double    g3x_Dist(G3Xpoint A, G3Xpoint B);

            /*! norme euclidienne au carre (v.x)²+(v.y)²+(v.z)²      !*/
  double    g3x_SqrNorm(G3Xvector V);
            /*! norme euclidienne du vecteur V                       !*/
  double    g3x_Norm(G3Xvector V);
            /*! norme L2 du vecteur V  max(|v.x|,|v.y|,|v.z|)        !*/
  double    g3x_NormL2(G3Xvector V);


  /**---------------------------------------------------------------*
   *  pour compatibilité OpenGl
   * ---------------------------------------------------------------*/
  void g3x_Vertex3dv(G3Xpoint  p); /*! Equivalent glVertex3dv(double p[3])!*/
  void g3x_Normal3dv(G3Xvector v); /*! Equivalent glNormal3dv(double v[3])!*/

  /**---------------------------------------------------------------*
   * FONCTIONS TRIGO USUELLES
   * ---------------------------------------------------------------*/
             /*! appelée automatiquement : NE PAS UTILISER!*/
  bool      g3x_InitTrigonometric(void);

            /*! cos./sin. utilisant des LUT prédéfinies (720 valeurs = 1/2 degré)!*/
            /*! un peu moins précis, mais BEAUCOUP plus rapide                   !*/
  double    g3x_Idxcos(int    idx); /*! angle: valeur d'indice dans [0,720] (0.5 degré) !*/
  double    g3x_Idxsin(int    idx);
  double    g3x_Radcos(double rad); /*! angle: valeur en radian ramenée dans [0,720] !*/
  double    g3x_Radsin(double rad);
  double    g3x_Degcos(double deg); /*! angle: valeur en degré  ramenée dans [0,720] !*/
  double    g3x_Degsin(double deg);

  /* introduire des emulateurs acos et asin sur le même principe */
  /* acos(a) => rechercher val <a> dans les tableaux => retour indice => [0,2PI] */

            /*! calcule le cosinus de l'angle (U,V) -- produit scalaire  normé !*/
  double    g3x_CosVect(G3Xvector U, G3Xvector V);
            /*! calcule le   sinus de l'angle (U,V) -- produit vectoriel normé !*/
  double    g3x_SinVect(G3Xvector U, G3Xvector V);


  /** Quelques versions en macros - conservées pour compatibilité avec anciennes versions *
   *  MAIS IL FAUT EVITER DE LES UTILISER                                                **/
  #define G3Xmilieu(A,B)           /**/((G3Xpoint ){(0.5*((A).x+(B).x)),(0.5*((A).y+(B).y)),,(0.5*((A).z+(B).z))})
  #define G3Xdefvectorxy(x,y)      /**/((G3Xvector){(double)(x),(double)(y),(double)(z)})
  #define G3Xdefvector(A,B)        /**/((G3Xvector){((B).x-(A).x),((B).y-(A).y),((B).z-(A).z)})
  #define G3Xaddvector(U,V)        /**/((G3Xvector){((U).x+(V).x),((U).y+(V).y),((U).z+(V).z)})
  #define G3Xdifvector(U,V)        /**/((G3Xvector){((U).x-(V).x),((U).y-(V).y),((U).z-(V).z)})
  #define G3Xmulvector(U,a)        /**/((G3Xvector){((double)(a))*(U).x,((double)(a))*(U).y,((double)(a))*(U).z})
  #define G3Xpointplusvector(A,V)  /**/((G3Xpoint ){((A).x+(V).x),((A).y+(V).y),((A).z+(V).z)})
  #define G3Xsqrdist(A,B)          /**/(SQR((B).x-(A).x)+SQR((B).y-(A).y)+SQR((B).z-(A).z))
  #define G3Xdist(A,B)             /**/(sqrt(G3Xsqrdist((A),(B))))
  #define G3Xsqrnorm(U)            /**/(SQR((U).x)+SQR((U).y)+SQR((U).z))
  #define G3Xnorm(U)               /**/(sqrt(G3Xsqrnorm(U)))
  #define G3Xprodscal(U,V)         /**/(((U).x*(V).x)+((U).y*(V).y)+((U).z*(V).z))

#endif

#ifdef __cplusplus
  }
#endif
/*!=============================================================!*/
