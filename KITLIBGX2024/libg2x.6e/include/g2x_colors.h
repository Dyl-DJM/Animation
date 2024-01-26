/**
 * @file    g2x_colors.h
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

#ifndef _G2X_COLORS_H
  #define _G2X_COLORS_H
/**@endcond                   */

  #include <g2x_types.h>

  /**
   * @struct G2Xcolor, G2Xfcolor
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
  } G2Xcolor,G2Xfcolor; // 2023.02.07 : alias G2Xfcolor pour mixage avec SDL/libMLV

  /**
   * @struct G2Xucolor (pour compatibilité libMLV/SDL)
   * @brief Couleur RGB.A (Red|Green|Blue|Alpha) sous forme d'un entier 32bits (4 octets 0xRRGGBBAA)
   * >>> chaque composante est codée sur 8 bits [0,255]
   * >>> format Hexa : 0xRRGGBBAA
   * Composition par Modèle Additif [https://fr.wikipedia.org/wiki/Synthèse_additive]
   * Peut aussi être utilisé au format HSV.A [https://fr.wikipedia.org/wiki/Teinte_Saturation_Valeur]
  **/
  typedef uint G2Xucolor; // 2023.02.07 : alias G2Xfcolor pour compatibilité avec libMLV

  G2Xfcolor g2x_uColor_to_fColor(G2Xucolor col);
  G2Xucolor g2x_fColor_to_uColor(G2Xfcolor col);

  /**
   * @brief pour compatibilité OpenGl - équivalents glColor3fv et glColor4fv
   * Existe en version fonctions et macros
  **/
  #define  G2Xcolor3fv(col) glColor3f((col).r,(col).g,(col).b)
   void    g2x_Color3v(G2Xcolor c);
  #define  G2Xcolor4fv(col) glColor4f((col).r,(col).g,(col).b,(col).a)
   void    g2x_Color4fv(G2Xcolor c);

  /**
   * @brief négatif RGB, RGBA : inverse les composantes (x -> (1.-x))
   * @fn G2Xcolor g2x_Color_NegRGB(G2Xcolor col)
   * @fn G2Xcolor g2x_Color_NegRGBA(G2Xcolor col)
  **/
  G2Xcolor g2x_Color_NegRGB(G2Xcolor col);
  G2Xcolor g2x_Color_NegRGBA(G2Xcolor col);

  /**
   * @brief mapping Couleur
   * toutes les valeurs sont automatiquement recalées dans l'intervalle [0,1]
   * @fn G2Xcolor g2x_mapscal4(G2Xcolor C, float    m)
   * renvoie la couleur [m*C.r|m*C.g|m*C.b|m*C.a]
   * @fn G2Xcolor g2x_mapscal4(G2Xcolor C, G2Xcolor m)
   * renvoie la couleur [m.r*C.r|m.g*C.g|m.b*C.b|m.a*C.a]
  **/
  G2Xcolor g2x_mapscal4(G2Xcolor C, float    m);
  G2Xcolor g2x_mapvect4(G2Xcolor C, G2Xcolor m);

  /**
   * @brief conversions RGBA<->HSVA :
   * toutes les valeurs (entrées/sorties) restent dans [0.,1.]
   * @fn G2Xcolor g2x_hsva_rgba_4f(float h, float s, float v, float a);
   * @fn G2Xcolor g2x_rgba_hsva_4fv(G2Xcolor rgba);
   * @fn G2Xcolor g2x_h110_rgba_1f(float h);
   * @fn G2Xcolor g2x_hsva_rgba_4fv(G2Xcolor hsva);
  **/
  G2Xcolor g2x_hsva_rgba_4f(float h, float s, float v, float a);
  G2Xcolor g2x_rgba_hsva_4fv(G2Xcolor rgba);
  G2Xcolor g2x_h110_rgba_1f(float h);
  G2Xcolor g2x_hsva_rgba_4fv(G2Xcolor hsva);

  /**
   * @brief Cartes de couleurs
   * rempli un tableau de n teintes etalonnees du rouge (0°) au rouge (360°)
   * la définition (allocation/libération) du tableau <map> est à la charge de l'utilisateur
   * @fn void   g2x_ColorMapHSV360   (G2Xcolor *map, int n);
   * @fn void   g2x_ColorMapHSV360_va(G2Xcolor *map, int n, float v, float a);
   * @fn void   g2x_ColorMapHSV360_sa(G2Xcolor *map, int n, float s, float a);
   * @fn void   g2x_ColorMapHSV360_interv(G2Xcolor *map, int n, float min, float max);
  **/
  void   g2x_ColorMapHSV360   (G2Xcolor *map, int n);
  void   g2x_ColorMapHSV360_va(G2Xcolor *map, int n, float v, float a);
  void   g2x_ColorMapHSV360_sa(G2Xcolor *map, int n, float s, float a);
  void   g2x_ColorMapHSV360_interv(G2Xcolor *map, int n, float min, float max);
  /*! change la couleur n°<idx> de la table <map> !*/
  void   g2x_SetIndexColor(G2Xcolor* map, int idx, G2Xcolor col);
  /*! distance euclidienne entre 2 couleurs (champs r,g,b seulement) !*/
  double g2x_ColorDist(G2Xcolor c1, G2Xcolor c2);

  /* DES COULEURS PREDEFINIES                                */
  /* TRANSPARENCE 0.00                                       */
  /* intensite 100%                                          */
  #define G2Xw    (G2Xcolor){1.00,1.00,1.00,0.00} /* blanc   */
  #define G2Xr    (G2Xcolor){1.00,0.00,0.00,0.00} /* rouge   */
  #define G2Xo    (G2Xcolor){1.00,0.50,0.25,0.00} /* orange  */
  #define G2Xy    (G2Xcolor){1.00,1.00,0.00,0.00} /* jaune   */
  #define G2Xg    (G2Xcolor){0.00,1.00,0.00,0.00} /* vert    */
  #define G2Xc    (G2Xcolor){0.00,1.00,1.00,0.00} /* cyan    */
  #define G2Xb    (G2Xcolor){0.00,0.00,1.00,0.00} /* bleu    */
  #define G2Xm    (G2Xcolor){1.00,0.00,1.00,0.00} /* magenta */
  #define G2Xk    (G2Xcolor){0.00,0.00,0.00,0.00} /* noir    */
  /* intensite  75%                                          */
  #define G2Xwa   (G2Xcolor){0.75,0.75,0.75,0.00} /*         */
  #define G2Xra   (G2Xcolor){0.75,0.00,0.00,0.00} /*         */
  #define G2Xya   (G2Xcolor){0.75,0.75,0.00,0.00} /*         */
  #define G2Xga   (G2Xcolor){0.00,0.75,0.00,0.00} /*         */
  #define G2Xca   (G2Xcolor){0.00,0.75,0.75,0.00} /*         */
  #define G2Xba   (G2Xcolor){0.00,0.00,0.75,0.00} /*         */
  #define G2Xma   (G2Xcolor){0.75,0.00,0.75,0.00} /*         */
  /* intensite  50%                                          */
  #define G2Xwb   (G2Xcolor){0.50,0.50,0.50,0.00} /*         */
  #define G2Xrb   (G2Xcolor){0.50,0.12,0.12,0.00} /*         */
  #define G2Xyb   (G2Xcolor){0.50,0.50,0.12,0.00} /*         */
  #define G2Xgb   (G2Xcolor){0.12,0.50,0.12,0.00} /*         */
  #define G2Xcb   (G2Xcolor){0.12,0.50,0.50,0.00} /*         */
  #define G2Xbb   (G2Xcolor){0.12,0.12,0.50,0.00} /*         */
  #define G2Xmb   (G2Xcolor){0.50,0.12,0.50,0.00} /*         */
  /* intensite  25%                                          */
  #define G2Xwc   (G2Xcolor){0.25,0.25,0.25,0.00} /*         */
  #define G2Xrc   (G2Xcolor){0.25,0.12,0.12,0.00} /*         */
  #define G2Xyc   (G2Xcolor){0.25,0.25,0.12,0.00} /*         */
  #define G2Xgc   (G2Xcolor){0.12,0.25,0.12,0.00} /*         */
  #define G2Xcc   (G2Xcolor){0.12,0.25,0.25,0.00} /*         */
  #define G2Xbc   (G2Xcolor){0.12,0.12,0.25,0.00} /*         */
  #define G2Xmc   (G2Xcolor){0.25,0.12,0.25,0.00} /*         */
  /* intensite  12.5%                                        */
  #define G2Xwd   (G2Xcolor){0.12,0.12,0.12,0.00} /*         */
  #define G2Xrd   (G2Xcolor){0.12,0.06,0.06,0.00} /*         */
  #define G2Xyd   (G2Xcolor){0.12,0.12,0.06,0.00} /*         */
  #define G2Xgd   (G2Xcolor){0.06,0.12,0.06,0.00} /*         */
  #define G2Xcd   (G2Xcolor){0.06,0.12,0.12,0.00} /*         */
  #define G2Xbd   (G2Xcolor){0.06,0.06,0.12,0.00} /*         */
  #define G2Xmd   (G2Xcolor){0.12,0.06,0.12,0.00} /*         */

  /* TRANSPARENCE 0.25                                       */
  /* intensite 100%                                          */
  #define G2Xw_a  (G2Xcolor){1.00,1.00,1.00,0.75} /* blanc   */
  #define G2Xr_a  (G2Xcolor){1.00,0.00,0.00,0.75} /* rouge   */
  #define G2Xo_a  (G2Xcolor){1.00,0.50,0.00,0.75} /* orange  */
  #define G2Xy_a  (G2Xcolor){1.00,1.00,0.00,0.75} /* jaune   */
  #define G2Xg_a  (G2Xcolor){0.00,1.00,0.00,0.75} /* vert    */
  #define G2Xc_a  (G2Xcolor){0.00,1.00,1.00,0.75} /* cyan    */
  #define G2Xb_a  (G2Xcolor){0.00,0.00,1.00,0.75} /* bleu    */
  #define G2Xm_a  (G2Xcolor){1.00,0.00,1.00,0.75} /* magenta */
  #define G2Xk_a  (G2Xcolor){0.00,0.00,0.00,0.75} /* noir    */
  /* intensite  75%                                          */
  #define G2Xwa_a (G2Xcolor){0.75,0.75,0.75,0.75} /*         */
  #define G2Xra_a (G2Xcolor){0.75,0.00,0.00,0.75} /*         */
  #define G2Xya_a (G2Xcolor){0.75,0.75,0.00,0.75} /*         */
  #define G2Xga_a (G2Xcolor){0.00,0.75,0.00,0.75} /*         */
  #define G2Xca_a (G2Xcolor){0.00,0.75,0.75,0.75} /*         */
  #define G2Xba_a (G2Xcolor){0.00,0.00,0.75,0.75} /*         */
  #define G2Xma_a (G2Xcolor){0.75,0.00,0.75,0.75} /*         */
  /* intensite  50%                                          */
  #define G2Xwb_a (G2Xcolor){0.50,0.50,0.50,0.75} /*         */
  #define G2Xrb_a (G2Xcolor){0.50,0.12,0.12,0.75} /*         */
  #define G2Xyb_a (G2Xcolor){0.50,0.50,0.12,0.75} /*         */
  #define G2Xgb_a (G2Xcolor){0.12,0.50,0.12,0.75} /*         */
  #define G2Xcb_a (G2Xcolor){0.12,0.50,0.50,0.75} /*         */
  #define G2Xbb_a (G2Xcolor){0.12,0.12,0.50,0.75} /*         */
  #define G2Xmb_a (G2Xcolor){0.50,0.12,0.50,0.75} /*         */
  /* intensite  25%                                          */
  #define G2Xwc_a (G2Xcolor){0.25,0.25,0.25,0.75} /*         */
  #define G2Xrc_a (G2Xcolor){0.25,0.12,0.12,0.75} /*         */
  #define G2Xyc_a (G2Xcolor){0.25,0.25,0.12,0.75} /*         */
  #define G2Xgc_a (G2Xcolor){0.12,0.25,0.12,0.75} /*         */
  #define G2Xcc_a (G2Xcolor){0.12,0.25,0.25,0.75} /*         */
  #define G2Xbc_a (G2Xcolor){0.12,0.12,0.25,0.75} /*         */
  #define G2Xmc_a (G2Xcolor){0.25,0.12,0.25,0.75} /*         */
  /* intensite  12.5%                                        */
  #define G2Xwd_a (G2Xcolor){0.12,0.12,0.12,0.75} /*         */
  #define G2Xrd_a (G2Xcolor){0.12,0.06,0.06,0.75} /*         */
  #define G2Xyd_a (G2Xcolor){0.12,0.12,0.06,0.75} /*         */
  #define G2Xgd_a (G2Xcolor){0.06,0.12,0.06,0.75} /*         */
  #define G2Xcd_a (G2Xcolor){0.06,0.12,0.12,0.75} /*         */
  #define G2Xbd_a (G2Xcolor){0.06,0.06,0.12,0.75} /*         */
  #define G2Xmd_a (G2Xcolor){0.12,0.06,0.12,0.75} /*         */

  /* TRANSPARENCE 0.50                                       */
  /* intensite 100%                                          */
  #define G2Xw_b  (G2Xcolor){1.00,1.00,1.00,0.50} /* blanc   */
  #define G2Xr_b  (G2Xcolor){1.00,0.00,0.00,0.50} /* rouge   */
  #define G2Xo_b  (G2Xcolor){1.00,0.50,0.00,0.50} /* orange  */
  #define G2Xy_b  (G2Xcolor){1.00,1.00,0.00,0.50} /* jaune   */
  #define G2Xg_b  (G2Xcolor){0.00,1.00,0.00,0.50} /* vert    */
  #define G2Xc_b  (G2Xcolor){0.00,1.00,1.00,0.50} /* cyan    */
  #define G2Xb_b  (G2Xcolor){0.00,0.00,1.00,0.50} /* bleu    */
  #define G2Xm_b  (G2Xcolor){1.00,0.00,1.00,0.50} /* magenta */
  #define G2Xk_b  (G2Xcolor){0.00,0.00,0.00,0.50} /* noir    */
  /* intensite  75%                                          */
  #define G2Xwa_b (G2Xcolor){0.75,0.75,0.75,0.50} /*         */
  #define G2Xra_b (G2Xcolor){0.75,0.00,0.00,0.50} /*         */
  #define G2Xya_b (G2Xcolor){0.75,0.75,0.00,0.50} /*         */
  #define G2Xga_b (G2Xcolor){0.00,0.75,0.00,0.50} /*         */
  #define G2Xca_b (G2Xcolor){0.00,0.75,0.75,0.50} /*         */
  #define G2Xba_b (G2Xcolor){0.00,0.00,0.75,0.50} /*         */
  #define G2Xma_b (G2Xcolor){0.75,0.00,0.75,0.50} /*         */
  /* intensite  50%                                          */
  #define G2Xwb_b (G2Xcolor){0.50,0.50,0.50,0.50} /*         */
  #define G2Xrb_b (G2Xcolor){0.50,0.12,0.12,0.50} /*         */
  #define G2Xyb_b (G2Xcolor){0.50,0.50,0.12,0.50} /*         */
  #define G2Xgb_b (G2Xcolor){0.12,0.50,0.12,0.50} /*         */
  #define G2Xcb_b (G2Xcolor){0.12,0.50,0.50,0.50} /*         */
  #define G2Xbb_b (G2Xcolor){0.12,0.12,0.50,0.50} /*         */
  #define G2Xmb_b (G2Xcolor){0.50,0.12,0.50,0.50} /*         */
  /* intensite  25%                                          */
  #define G2Xwc_b (G2Xcolor){0.25,0.25,0.25,0.50} /*         */
  #define G2Xrc_b (G2Xcolor){0.25,0.12,0.12,0.50} /*         */
  #define G2Xyc_b (G2Xcolor){0.25,0.25,0.12,0.50} /*         */
  #define G2Xgc_b (G2Xcolor){0.12,0.25,0.12,0.50} /*         */
  #define G2Xcc_b (G2Xcolor){0.12,0.25,0.25,0.50} /*         */
  #define G2Xbc_b (G2Xcolor){0.12,0.12,0.25,0.50} /*         */
  #define G2Xmc_b (G2Xcolor){0.25,0.12,0.25,0.50} /*         */
  /* intensite  12.5%                                        */
  #define G2Xwd_b (G2Xcolor){0.12,0.12,0.12,0.50} /*         */
  #define G2Xrd_b (G2Xcolor){0.12,0.06,0.06,0.50} /*         */
  #define G2Xyd_b (G2Xcolor){0.12,0.12,0.06,0.50} /*         */
  #define G2Xgd_b (G2Xcolor){0.06,0.12,0.06,0.50} /*         */
  #define G2Xcd_b (G2Xcolor){0.06,0.12,0.12,0.50} /*         */
  #define G2Xbd_b (G2Xcolor){0.06,0.06,0.12,0.50} /*         */
  #define G2Xmd_b (G2Xcolor){0.12,0.06,0.12,0.50} /*         */

  /* TRANSPARENCE 0.75                                       */
  /* intensite 100%                                          */
  #define G2Xw_c  (G2Xcolor){1.00,1.00,1.00,0.25} /* blanc   */
  #define G2Xr_c  (G2Xcolor){1.00,0.00,0.00,0.25} /* rouge   */
  #define G2Xo_c  (G2Xcolor){1.00,0.50,0.00,0.25} /* orange  */
  #define G2Xy_c  (G2Xcolor){1.00,1.00,0.00,0.25} /* jaune   */
  #define G2Xg_c  (G2Xcolor){0.00,1.00,0.00,0.25} /* vert    */
  #define G2Xc_c  (G2Xcolor){0.00,1.00,1.00,0.25} /* cyan    */
  #define G2Xb_c  (G2Xcolor){0.00,0.00,1.00,0.25} /* bleu    */
  #define G2Xm_c  (G2Xcolor){1.00,0.00,1.00,0.25} /* magenta */
  #define G2Xk_c  (G2Xcolor){0.00,0.00,0.00,0.25} /* noir    */
  /* intensite  75%                                          */
  #define G2Xwa_c (G2Xcolor){0.75,0.75,0.75,0.25} /*         */
  #define G2Xra_c (G2Xcolor){0.75,0.00,0.00,0.25} /*         */
  #define G2Xya_c (G2Xcolor){0.75,0.75,0.00,0.25} /*         */
  #define G2Xga_c (G2Xcolor){0.00,0.75,0.00,0.25} /*         */
  #define G2Xca_c (G2Xcolor){0.00,0.75,0.75,0.25} /*         */
  #define G2Xba_c (G2Xcolor){0.00,0.00,0.75,0.25} /*         */
  #define G2Xma_c (G2Xcolor){0.75,0.00,0.75,0.25} /*         */
  /* intensite  50%                                          */
  #define G2Xwb_c (G2Xcolor){0.50,0.50,0.50,0.25} /*         */
  #define G2Xrb_c (G2Xcolor){0.50,0.12,0.12,0.25} /*         */
  #define G2Xyb_c (G2Xcolor){0.50,0.50,0.12,0.25} /*         */
  #define G2Xgb_c (G2Xcolor){0.12,0.50,0.12,0.25} /*         */
  #define G2Xcb_c (G2Xcolor){0.12,0.50,0.50,0.25} /*         */
  #define G2Xbb_c (G2Xcolor){0.12,0.12,0.50,0.25} /*         */
  #define G2Xmb_c (G2Xcolor){0.50,0.12,0.50,0.25} /*         */
  /* intensite  25%                                          */
  #define G2Xwc_c (G2Xcolor){0.25,0.25,0.25,0.25} /*         */
  #define G2Xrc_c (G2Xcolor){0.25,0.12,0.12,0.25} /*         */
  #define G2Xyc_c (G2Xcolor){0.25,0.25,0.12,0.25} /*         */
  #define G2Xgc_c (G2Xcolor){0.12,0.25,0.12,0.25} /*         */
  #define G2Xcc_c (G2Xcolor){0.12,0.25,0.25,0.25} /*         */
  #define G2Xbc_c (G2Xcolor){0.12,0.12,0.25,0.25} /*         */
  #define G2Xmc_c (G2Xcolor){0.25,0.12,0.25,0.25} /*         */
  /* intensite  12.5%                                        */
  #define G2Xwd_c (G2Xcolor){0.12,0.12,0.12,0.25} /*         */
  #define G2Xrd_c (G2Xcolor){0.12,0.06,0.06,0.25} /*         */
  #define G2Xyd_c (G2Xcolor){0.12,0.12,0.06,0.25} /*         */
  #define G2Xgd_c (G2Xcolor){0.06,0.12,0.06,0.25} /*         */
  #define G2Xcd_c (G2Xcolor){0.06,0.12,0.12,0.25} /*         */
  #define G2Xbd_c (G2Xcolor){0.06,0.06,0.12,0.25} /*         */
  #define G2Xmd_c (G2Xcolor){0.12,0.06,0.12,0.25} /*         */

#endif

#ifdef __cplusplus
  }
#endif
/*!===========================================================================!*/
