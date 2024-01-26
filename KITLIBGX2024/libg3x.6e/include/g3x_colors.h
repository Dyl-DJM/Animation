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

#ifndef _G3X_COLORS_H
  #define _G3X_COLORS_H
/**@endcond                   */

  #include <g3x_types.h>

  /**
   * @struct G3Xcolor, G3Xfcolor
   * @brief Couleur RGB.A (Red|Green|Blue|Alpha) sous forme d'une structure à 4 champs réels (float)
   * >>> Les 4 valeurs réelles doivent être dans l'intervalle [0,1]
   * Composition par Modèle Additif [https://fr.wikipedia.org/wiki/Synthèse_additive]
   * Peut aussi être utilisé au format HSV.A [https://fr.wikipedia.org/wiki/Teinte_Saturation_Valeur]
   * Attention OpenGl gere les couleurs via des tableaux <float rgba[4]> et non des structures
   * La libg2x propose quelques fonctions/macros de conversion
  **/
  typedef struct
  {
    float  r,g,b,a;
  } G3Xcolor,G3Xfcolor; // alias G3Xfcolor pour mixage avec SDL

  /**
   * @struct G3Xucolor (pour compatibilité SDL)
   * @brief Couleur RGB.A (Red|Green|Blue|Alpha) sous forme d'un entier 32bits (4 octets 0xRRGGBBAA)
   * >>> chaque composante est codée sur 8 bits [0,255]
   * >>> format Hexa : 0xRRGGBBAA
   * Composition par Modèle Additif [https://fr.wikipedia.org/wiki/Synthèse_additive]
   * Peut aussi être utilisé au format HSV.A [https://fr.wikipedia.org/wiki/Teinte_Saturation_Valeur]
  **/
  typedef uint G3Xucolor; // pour compatibilité avec SDL

  G3Xfcolor g3x_uColor_to_fColor(G3Xucolor col);
  G3Xucolor g3x_fColor_to_uColor(G3Xfcolor col);

  /**
   * @brief pour compatibilité OpenGl - équivalents glColor3fv et glColor4fv
   * Existe en version fonctions et macros
  **/
  #define G3Xcolor3fv(col) glColor3f((col).r,(col).g,(col).b)
  void    g3x_Color3v(G3Xcolor c);
  #define G3Xcolor4fv(col) glColor4f((col).r,(col).g,(col).b,(col).a)
  void    g3x_Color4fv(G3Xcolor c);

  /*! pour compatibilité OpenGl - équivalent glMaterialfv!*/
  void g3x_Materialfv(GLenum face,GLenum pname, G3Xcolor col);

  void g3x_Enable_Blend(void);

  /*! @brief ALTERNATIVE a la gestion de couleurs OpenGl
   *  mise en forme des parametres de couleur/matiere des objets
   *  tous les parametres dans [0,1]
   *  col    : teinte RGB
   *  ambi   : reaction a la lumiere ambiante (blanche par defaut)
   *  diff   : reaction a la lumiere diffuse (blanche par defaut)
   *  spec   : reaction a la lumiere de speculosite (blanche par defaut)
   *  shine  : coeff. de speculosite (max par defaut -- 256.    )
   *  transp : valeur de transparence (0.: objet totalement transp. / 1.:opaque)
  !*/
  void g3x_Material(G3Xcolor col, float ambi, float diff, float spec, float shine, float alpha);

  /**
   * @brief négatif RGB, RGBA : inverse les composantes (x -> (1.-x))
   * @fn G3Xcolor g3x_Color_NegRGB(G3Xcolor col)
   * @fn G3Xcolor g3x_Color_NegRGBA(G3Xcolor col)
  **/
  G3Xcolor g3x_Color_NegRGB(G3Xcolor col);
  G3Xcolor g3x_Color_NegRGBA(G3Xcolor col);

  /**
   * @brief mapping Couleur
   * toutes les valeurs sont automatiquement recalées dans l'intervalle [0,1]
   * @fn G3Xcolor g3x_mapscal4(G3Xcolor C, float    m)
   * renvoie la couleur [m*C.r|m*C.g|m*C.b|m*C.a]
   * @fn G3Xcolor g3x_mapscal4(G3Xcolor C, G3Xcolor m)
   * renvoie la couleur [m.r*C.r|m.g*C.g|m.b*C.b|m.a*C.a]
  **/
  G3Xcolor g3x_mapscal4(G3Xcolor C, float    m);
  G3Xcolor g3x_mapvect4(G3Xcolor C, G3Xcolor m);

  /**
   * @brief conversions RGBA<->HSVA :
   * toutes les valeurs (entrées/sorties) restent dans [0.,1.]
   * @fn G3Xcolor g3x_hsva_rgba_4f(float h, float s, float v, float a);
   * @fn G3Xcolor g3x_rgba_hsva_4fv(G3Xcolor rgba);
   * @fn G3Xcolor g3x_h110_rgba_1f(float h);
   * @fn G3Xcolor g3x_hsva_rgba_4fv(G3Xcolor hsva);
  **/
  G3Xcolor g3x_hsva_rgba_4f(float h, float s, float v, float a);
  G3Xcolor g3x_rgba_hsva_4fv(G3Xcolor rgba);
  G3Xcolor g3x_h110_rgba_1f(float h);
  G3Xcolor g3x_hsva_rgba_4fv(G3Xcolor hsva);

  /**
   * @brief Cartes de couleurs
   * rempli un tableau de n teintes etalonnees du rouge (0°) au rouge (360°)
   * la définition (allocation/libération) du tableau <map> est à la charge de l'utilisateur
   * @fn void   g3x_ColorMapHSV360   (G3Xcolor *map, int n);
   * @fn void   g3x_ColorMapHSV360_va(G3Xcolor *map, int n, float v, float a);
   * @fn void   g3x_ColorMapHSV360_sa(G3Xcolor *map, int n, float s, float a);
   * @fn void   g3x_ColorMapHSV360_interv(G3Xcolor *map, int n, float min, float max);
  **/
  void   g3x_ColorMapHSV360   (G3Xcolor *map, int n);
  void   g3x_ColorMapHSV360_va(G3Xcolor *map, int n, float v, float a);
  void   g3x_ColorMapHSV360_sa(G3Xcolor *map, int n, float s, float a);
  void   g3x_ColorMapHSV360_interv(G3Xcolor *map, int n, float min, float max);
  /*! change la couleur n°<idx> de la table <map> !*/
  void   g3x_SetIndexColor(G3Xcolor* map, int idx, G3Xcolor col);
  /*! distance euclidienne entre 2 couleurs (champs r,g,b seulement) !*/
  double g3x_ColorDist(G3Xcolor c1, G3Xcolor c2);

  /*! DES COULEURS PREDEFINIES                               !*/
  /*! TRANSPARENCE 0.00                                      !*/
  /*! intensite 100%                                         !*/
  #define G3Xw    (G3Xcolor){1.00,1.00,1.00,0.00} /*! blanc  !*/
  #define G3Xr    (G3Xcolor){1.00,0.00,0.00,0.00} /*! rouge  !*/
  #define G3Xo    (G3Xcolor){1.00,0.50,0.00,0.00} /*! orange !*/
  #define G3Xy    (G3Xcolor){1.00,1.00,0.00,0.00} /*! jaune  !*/
  #define G3Xg    (G3Xcolor){0.00,1.00,0.00,0.00} /*! vert   !*/
  #define G3Xc    (G3Xcolor){0.00,1.00,1.00,0.00} /*! cyan   !*/
  #define G3Xb    (G3Xcolor){0.00,0.00,1.00,0.00} /*! bleu   !*/
  #define G3Xm    (G3Xcolor){1.00,0.00,1.00,0.00} /*! magenta!*/
  #define G3Xk    (G3Xcolor){0.00,0.00,0.00,0.00} /*! noir   !*/
  /*! intensite  75%                                         !*/
  #define G3Xwa   (G3Xcolor){0.75,0.75,0.75,0.00} /*!        !*/
  #define G3Xra   (G3Xcolor){0.75,0.00,0.00,0.00} /*!        !*/
  #define G3Xya   (G3Xcolor){0.75,0.75,0.00,0.00} /*!        !*/
  #define G3Xga   (G3Xcolor){0.00,0.75,0.00,0.00} /*!        !*/
  #define G3Xca   (G3Xcolor){0.00,0.75,0.75,0.00} /*!        !*/
  #define G3Xba   (G3Xcolor){0.00,0.00,0.75,0.00} /*!        !*/
  #define G3Xma   (G3Xcolor){0.75,0.00,0.75,0.00} /*!        !*/
  /*! intensite  50%                                         !*/
  #define G3Xwb   (G3Xcolor){0.50,0.50,0.50,0.00} /*!        !*/
  #define G3Xrb   (G3Xcolor){0.50,0.12,0.12,0.00} /*!        !*/
  #define G3Xyb   (G3Xcolor){0.50,0.50,0.12,0.00} /*!        !*/
  #define G3Xgb   (G3Xcolor){0.12,0.50,0.12,0.00} /*!        !*/
  #define G3Xcb   (G3Xcolor){0.12,0.50,0.50,0.00} /*!        !*/
  #define G3Xbb   (G3Xcolor){0.12,0.12,0.50,0.00} /*!        !*/
  #define G3Xmb   (G3Xcolor){0.50,0.12,0.50,0.00} /*!        !*/
  /*! intensite  25%                                         !*/
  #define G3Xwc   (G3Xcolor){0.25,0.25,0.25,0.00} /*!        !*/
  #define G3Xrc   (G3Xcolor){0.25,0.12,0.12,0.00} /*!        !*/
  #define G3Xyc   (G3Xcolor){0.25,0.25,0.12,0.00} /*!        !*/
  #define G3Xgc   (G3Xcolor){0.12,0.25,0.12,0.00} /*!        !*/
  #define G3Xcc   (G3Xcolor){0.12,0.25,0.25,0.00} /*!        !*/
  #define G3Xbc   (G3Xcolor){0.12,0.12,0.25,0.00} /*!        !*/
  #define G3Xmc   (G3Xcolor){0.25,0.12,0.25,0.00} /*!        !*/
  /*! intensite  12.5%                                       !*/
  #define G3Xwd   (G3Xcolor){0.12,0.12,0.12,0.00} /*!        !*/
  #define G3Xrd   (G3Xcolor){0.12,0.06,0.06,0.00} /*!        !*/
  #define G3Xyd   (G3Xcolor){0.12,0.12,0.06,0.00} /*!        !*/
  #define G3Xgd   (G3Xcolor){0.06,0.12,0.06,0.00} /*!        !*/
  #define G3Xcd   (G3Xcolor){0.06,0.12,0.12,0.00} /*!        !*/
  #define G3Xbd   (G3Xcolor){0.06,0.06,0.12,0.00} /*!        !*/
  #define G3Xmd   (G3Xcolor){0.12,0.06,0.12,0.00} /*!        !*/


  /*! TRANSPARENCE 0.25                                      !*/
  /*! intensite 100%                                         !*/
  #define G3Xw_a  (G3Xcolor){1.00,1.00,1.00,0.75} /*! blanc  !*/
  #define G3Xr_a  (G3Xcolor){1.00,0.00,0.00,0.75} /*! rouge  !*/
  #define G3Xo_a  (G3Xcolor){1.00,0.50,0.00,0.75} /*! orange !*/
  #define G3Xy_a  (G3Xcolor){1.00,1.00,0.00,0.75} /*! jaune  !*/
  #define G3Xg_a  (G3Xcolor){0.00,1.00,0.00,0.75} /*! vert   !*/
  #define G3Xc_a  (G3Xcolor){0.00,1.00,1.00,0.75} /*! cyan   !*/
  #define G3Xb_a  (G3Xcolor){0.00,0.00,1.00,0.75} /*! bleu   !*/
  #define G3Xm_a  (G3Xcolor){1.00,0.00,1.00,0.75} /*! magenta!*/
  #define G3Xk_a  (G3Xcolor){0.00,0.00,0.00,0.75} /*! noir   !*/
  /*! intensite  75%                                         !*/
  #define G3Xwa_a (G3Xcolor){0.75,0.75,0.75,0.75} /*!        !*/
  #define G3Xra_a (G3Xcolor){0.75,0.00,0.00,0.75} /*!        !*/
  #define G3Xya_a (G3Xcolor){0.75,0.75,0.00,0.75} /*!        !*/
  #define G3Xga_a (G3Xcolor){0.00,0.75,0.00,0.75} /*!        !*/
  #define G3Xca_a (G3Xcolor){0.00,0.75,0.75,0.75} /*!        !*/
  #define G3Xba_a (G3Xcolor){0.00,0.00,0.75,0.75} /*!        !*/
  #define G3Xma_a (G3Xcolor){0.75,0.00,0.75,0.75} /*!        !*/
  /*! intensite  50%                                         !*/
  #define G3Xwb_a (G3Xcolor){0.50,0.50,0.50,0.75} /*!        !*/
  #define G3Xrb_a (G3Xcolor){0.50,0.12,0.12,0.75} /*!        !*/
  #define G3Xyb_a (G3Xcolor){0.50,0.50,0.12,0.75} /*!        !*/
  #define G3Xgb_a (G3Xcolor){0.12,0.50,0.12,0.75} /*!        !*/
  #define G3Xcb_a (G3Xcolor){0.12,0.50,0.50,0.75} /*!        !*/
  #define G3Xbb_a (G3Xcolor){0.12,0.12,0.50,0.75} /*!        !*/
  #define G3Xmb_a (G3Xcolor){0.50,0.12,0.50,0.75} /*!        !*/
  /*! intensite  25%                                         !*/
  #define G3Xwc_a (G3Xcolor){0.25,0.25,0.25,0.75} /*!        !*/
  #define G3Xrc_a (G3Xcolor){0.25,0.12,0.12,0.75} /*!        !*/
  #define G3Xyc_a (G3Xcolor){0.25,0.25,0.12,0.75} /*!        !*/
  #define G3Xgc_a (G3Xcolor){0.12,0.25,0.12,0.75} /*!        !*/
  #define G3Xcc_a (G3Xcolor){0.12,0.25,0.25,0.75} /*!        !*/
  #define G3Xbc_a (G3Xcolor){0.12,0.12,0.25,0.75} /*!        !*/
  #define G3Xmc_a (G3Xcolor){0.25,0.12,0.25,0.75} /*!        !*/
  /*! intensite  12.5%                                       !*/
  #define G3Xwd_a (G3Xcolor){0.12,0.12,0.12,0.75} /*!        !*/
  #define G3Xrd_a (G3Xcolor){0.12,0.06,0.06,0.75} /*!        !*/
  #define G3Xyd_a (G3Xcolor){0.12,0.12,0.06,0.75} /*!        !*/
  #define G3Xgd_a (G3Xcolor){0.06,0.12,0.06,0.75} /*!        !*/
  #define G3Xcd_a (G3Xcolor){0.06,0.12,0.12,0.75} /*!        !*/
  #define G3Xbd_a (G3Xcolor){0.06,0.06,0.12,0.75} /*!        !*/
  #define G3Xmd_a (G3Xcolor){0.12,0.06,0.12,0.75} /*!        !*/

  /*! TRANSPARENCE 0.50                                      !*/
  /*! intensite 100%                                         !*/
  #define G3Xw_b  (G3Xcolor){1.00,1.00,1.00,0.50} /*! blanc  !*/
  #define G3Xr_b  (G3Xcolor){1.00,0.00,0.00,0.50} /*! rouge  !*/
  #define G3Xo_b  (G3Xcolor){1.00,0.50,0.00,0.50} /*! orange !*/
  #define G3Xy_b  (G3Xcolor){1.00,1.00,0.00,0.50} /*! jaune  !*/
  #define G3Xg_b  (G3Xcolor){0.00,1.00,0.00,0.50} /*! vert   !*/
  #define G3Xc_b  (G3Xcolor){0.00,1.00,1.00,0.50} /*! cyan   !*/
  #define G3Xb_b  (G3Xcolor){0.00,0.00,1.00,0.50} /*! bleu   !*/
  #define G3Xm_b  (G3Xcolor){1.00,0.00,1.00,0.50} /*! magenta!*/
  #define G3Xk_b  (G3Xcolor){0.00,0.00,0.00,0.50} /*! noir   !*/
  /*! intensite  75%                                         !*/
  #define G3Xwa_b (G3Xcolor){0.75,0.75,0.75,0.50} /*!        !*/
  #define G3Xra_b (G3Xcolor){0.75,0.00,0.00,0.50} /*!        !*/
  #define G3Xya_b (G3Xcolor){0.75,0.75,0.00,0.50} /*!        !*/
  #define G3Xga_b (G3Xcolor){0.00,0.75,0.00,0.50} /*!        !*/
  #define G3Xca_b (G3Xcolor){0.00,0.75,0.75,0.50} /*!        !*/
  #define G3Xba_b (G3Xcolor){0.00,0.00,0.75,0.50} /*!        !*/
  #define G3Xma_b (G3Xcolor){0.75,0.00,0.75,0.50} /*!        !*/
  /*! intensite  50%                                         !*/
  #define G3Xwb_b (G3Xcolor){0.50,0.50,0.50,0.50} /*!        !*/
  #define G3Xrb_b (G3Xcolor){0.50,0.12,0.12,0.50} /*!        !*/
  #define G3Xyb_b (G3Xcolor){0.50,0.50,0.12,0.50} /*!        !*/
  #define G3Xgb_b (G3Xcolor){0.12,0.50,0.12,0.50} /*!        !*/
  #define G3Xcb_b (G3Xcolor){0.12,0.50,0.50,0.50} /*!        !*/
  #define G3Xbb_b (G3Xcolor){0.12,0.12,0.50,0.50} /*!        !*/
  #define G3Xmb_b (G3Xcolor){0.50,0.12,0.50,0.50} /*!        !*/
  /*! intensite  25%                                         !*/
  #define G3Xwc_b (G3Xcolor){0.25,0.25,0.25,0.50} /*!        !*/
  #define G3Xrc_b (G3Xcolor){0.25,0.12,0.12,0.50} /*!        !*/
  #define G3Xyc_b (G3Xcolor){0.25,0.25,0.12,0.50} /*!        !*/
  #define G3Xgc_b (G3Xcolor){0.12,0.25,0.12,0.50} /*!        !*/
  #define G3Xcc_b (G3Xcolor){0.12,0.25,0.25,0.50} /*!        !*/
  #define G3Xbc_b (G3Xcolor){0.12,0.12,0.25,0.50} /*!        !*/
  #define G3Xmc_b (G3Xcolor){0.25,0.12,0.25,0.50} /*!        !*/
  /*! intensite  12.5%                                       !*/
  #define G3Xwd_b (G3Xcolor){0.12,0.12,0.12,0.50} /*!        !*/
  #define G3Xrd_b (G3Xcolor){0.12,0.06,0.06,0.50} /*!        !*/
  #define G3Xyd_b (G3Xcolor){0.12,0.12,0.06,0.50} /*!        !*/
  #define G3Xgd_b (G3Xcolor){0.06,0.12,0.06,0.50} /*!        !*/
  #define G3Xcd_b (G3Xcolor){0.06,0.12,0.12,0.50} /*!        !*/
  #define G3Xbd_b (G3Xcolor){0.06,0.06,0.12,0.50} /*!        !*/
  #define G3Xmd_b (G3Xcolor){0.12,0.06,0.12,0.50} /*!        !*/

  /*! TRANSPARENCE 0.75                                      !*/
  /*! intensite 100%                                         !*/
  #define G3Xw_c  (G3Xcolor){1.00,1.00,1.00,0.25} /*! blanc  !*/
  #define G3Xr_c  (G3Xcolor){1.00,0.00,0.00,0.25} /*! rouge  !*/
  #define G3Xo_c  (G3Xcolor){1.00,0.50,0.00,0.25} /*! orange !*/
  #define G3Xy_c  (G3Xcolor){1.00,1.00,0.00,0.25} /*! jaune  !*/
  #define G3Xg_c  (G3Xcolor){0.00,1.00,0.00,0.25} /*! vert   !*/
  #define G3Xc_c  (G3Xcolor){0.00,1.00,1.00,0.25} /*! cyan   !*/
  #define G3Xb_c  (G3Xcolor){0.00,0.00,1.00,0.25} /*! bleu   !*/
  #define G3Xm_c  (G3Xcolor){1.00,0.00,1.00,0.25} /*! magenta!*/
  #define G3Xk_c  (G3Xcolor){0.00,0.00,0.00,0.25} /*! noir   !*/
  /*! intensite  75%                                         !*/
  #define G3Xwa_c (G3Xcolor){0.75,0.75,0.75,0.25} /*!        !*/
  #define G3Xra_c (G3Xcolor){0.75,0.00,0.00,0.25} /*!        !*/
  #define G3Xya_c (G3Xcolor){0.75,0.75,0.00,0.25} /*!        !*/
  #define G3Xga_c (G3Xcolor){0.00,0.75,0.00,0.25} /*!        !*/
  #define G3Xca_c (G3Xcolor){0.00,0.75,0.75,0.25} /*!        !*/
  #define G3Xba_c (G3Xcolor){0.00,0.00,0.75,0.25} /*!        !*/
  #define G3Xma_c (G3Xcolor){0.75,0.00,0.75,0.25} /*!        !*/
  /*! intensite  50%                                         !*/
  #define G3Xwb_c (G3Xcolor){0.50,0.50,0.50,0.25} /*!        !*/
  #define G3Xrb_c (G3Xcolor){0.50,0.12,0.12,0.25} /*!        !*/
  #define G3Xyb_c (G3Xcolor){0.50,0.50,0.12,0.25} /*!        !*/
  #define G3Xgb_c (G3Xcolor){0.12,0.50,0.12,0.25} /*!        !*/
  #define G3Xcb_c (G3Xcolor){0.12,0.50,0.50,0.25} /*!        !*/
  #define G3Xbb_c (G3Xcolor){0.12,0.12,0.50,0.25} /*!        !*/
  #define G3Xmb_c (G3Xcolor){0.50,0.12,0.50,0.25} /*!        !*/
  /*! intensite  25%                                         !*/
  #define G3Xwc_c (G3Xcolor){0.25,0.25,0.25,0.25} /*!        !*/
  #define G3Xrc_c (G3Xcolor){0.25,0.12,0.12,0.25} /*!        !*/
  #define G3Xyc_c (G3Xcolor){0.25,0.25,0.12,0.25} /*!        !*/
  #define G3Xgc_c (G3Xcolor){0.12,0.25,0.12,0.25} /*!        !*/
  #define G3Xcc_c (G3Xcolor){0.12,0.25,0.25,0.25} /*!        !*/
  #define G3Xbc_c (G3Xcolor){0.12,0.12,0.25,0.25} /*!        !*/
  #define G3Xmc_c (G3Xcolor){0.25,0.12,0.25,0.25} /*!        !*/
  /*! intensite  12.5%                                       !*/
  #define G3Xwd_c (G3Xcolor){0.12,0.12,0.12,0.25} /*!        !*/
  #define G3Xrd_c (G3Xcolor){0.12,0.06,0.06,0.25} /*!        !*/
  #define G3Xyd_c (G3Xcolor){0.12,0.12,0.06,0.25} /*!        !*/
  #define G3Xgd_c (G3Xcolor){0.06,0.12,0.06,0.25} /*!        !*/
  #define G3Xcd_c (G3Xcolor){0.06,0.12,0.12,0.25} /*!        !*/
  #define G3Xbd_c (G3Xcolor){0.06,0.06,0.12,0.25} /*!        !*/
  #define G3Xmd_c (G3Xcolor){0.12,0.06,0.12,0.25} /*!        !*/

#endif

#ifdef __cplusplus
  }
#endif
/*!===========================================================================!*/
