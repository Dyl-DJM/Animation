/**@file    g2x_draw.h
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

#ifndef _G2X_RDRAW_H
  #define _G2X_RDRAW_H
/**@endcond                   */

  /* Fonctions de dessin de base */
  #include <g2x.h>

  /* fixe la couleur de trace courante */
  void g2x_SetColor(G2Xcolor col);

  /* fixe la taille du point courante */
  void g2x_SetPointSize(double size);

  /*!  POINTS SIMPLES                                                           !*/
  /* trace un point en (x,y) avec la couleur et taille courantes                 */
  void g2x_PlotDft(double x, double y);

  /* trace un point en (x,y) avec la couleur et taille donnees                   */
  /* <color> et <width> deviennent les valeurs courantes                         */
  void g2x_Plot(double x, double y, G2Xcolor color, int width);
  void g2x_DrawPoint(G2Xpoint P, G2Xcolor color, int width);

  /*!  LIGNES SIMPLES                                                           !*/
  /* trace une ligne (xa,ya)->(xb,yb) avec la couleur et taille courantes        */
  void g2x_LineDft(double xa, double ya, double xb, double yb);

  /* trace une ligne (xa,ya)->(xb,yb) avec la couleur et taille doonees          */
  /* <color> et <width> deviennent les valeurs courantes                         */
  void g2x_Line(double xa, double ya, double xb, double yb,G2Xcolor color, int width);
  void g2x_DrawLine(G2Xpoint A, G2Xpoint B, G2Xcolor color, int width);

  /* trace une ligne (xa,ya)->(xb,yb) en interpollant les couleurs de cola a colb */
  /* <colb> et <width> deviennent les valeurs courantes                           */
  void g2x_Line_2col(double xa, double ya, G2Xcolor cola, double xb, double yb, G2Xcolor colb, int width);
  void g2x_DrawLine_2col(G2Xpoint A, G2Xcolor colA, G2Xpoint B, G2Xcolor colB, int width);

  /*! CERCLES/DISQUE                                                             !*/
  /* Centre (xc,yc), rayon r, couleur, épaisseur du trait                         */
  void g2x_Circle(double xc, double yc, double r, G2Xcolor col, int width);
  void g2x_DrawCircle(G2Xpoint C, double r, G2Xcolor col, int width);

  void g2x_FillCircle(double xc, double yc, double r, G2Xcolor col);
  void g2x_DrawFillCircle(G2Xpoint C, double r, G2Xcolor col);

  /* tranches et secteurs ('camemberts') */
  void g2x_FillCircleSlice (double xc, double yc, G2Xcolor colC, double xa, double ya, G2Xcolor colA, double deg);
  void g2x_FillCircleSector(double xc, double yc, double xa, double ya, double xb, double yb, G2Xcolor col);

  /*!  ELLIPSES                                                                                     !*/
  /* Centre (xc,yc), rayons (rx,ry), orientation degrot (en degres), couleur, epaisseur du trait.... */
  void g2x_Ellipse(double xc, double yc, double rx, double ry, double degrot, G2Xcolor col, int width);
  void g2x_FillEllipse(double xc, double yc, double rx, double ry, double degrot, G2Xcolor col);
  /* Secteur d'ellipse : angle du secteur degsec (en degres) */
  void g2x_PartialEllipse(double xc, double yc, double rx, double ry, double degsec, double degrot, G2Xcolor col, int width);
  void g2x_FillPartialEllipse(double xc, double yc, double rx, double ry, double degsec, double degrot, G2Xcolor col);

  /*! TRIANGLES                                                                     !*/
  void g2x_Triangle(double xa, double ya, double xb, double yb, double xc, double yc, G2Xcolor col, int width);
  void g2x_DrawTriangle(G2Xpoint A, G2Xpoint B, G2Xpoint C, G2Xcolor color, int width);

  void g2x_FillTriangle(double xa, double ya, double xb, double yb, double xc, double yc, G2Xcolor col);
  void g2x_DrawFillTriangle(G2Xpoint A, G2Xpoint B, G2Xpoint C, G2Xcolor color);

  /* couleurs interpollées sur les sommets                                           */
  void g2x_FillTriangle_3col(double xa, double ya, G2Xcolor cola,
                             double xb, double yb, G2Xcolor colb,
                             double xc, double yc, G2Xcolor colc);
  void g2x_DrawFillTriangle_3col(G2Xpoint A, G2Xcolor colA,
                                 G2Xpoint B, G2Xcolor colB,
                                 G2Xpoint C, G2Xcolor colC);

  /*! RECTANGLE PARALLELE AUX AXES                                                  !*/
  /* coin inf. gauche (xd,yd) Coin sup. droit (xu,yu)                                */
  void g2x_Rectangle(double xdl, double ydl, double xur, double yur, G2Xcolor col, int width);
  void g2x_DrawRectangle(G2Xpoint DL, G2Xpoint UR, G2Xcolor color, int width);
  void g2x_FillRectangle(double xdl, double ydl, double xur, double yur, G2Xcolor col);
  void g2x_DrawFillRectangle(G2Xpoint DL, G2Xpoint UR, G2Xcolor color);


  /*! QUADRILATERES QUELCONQUES (ABCD)                                              !*/
  void g2x_Quad(double xa, double ya, double xb, double yb,
                double xc, double yc, double xd, double yd, G2Xcolor col, int width);
  void g2x_DrawQuad(G2Xpoint A, G2Xpoint B, G2Xpoint C, G2Xpoint D, G2Xcolor color, int width);
  void g2x_FillQuad(double xa, double ya, double xb, double yb,
                    double xc, double yc, double xd, double yd, G2Xcolor col);
  void g2x_DrawFillQuad(G2Xpoint A, G2Xpoint B, G2Xpoint C, G2Xpoint D, G2Xcolor color);

  /* couleurs interpollées sur les sommets                                           */
  void g2x_FillQuad_4col(double xa, double ya,G2Xcolor cola,
                         double xb, double yb,G2Xcolor colb,
                         double xc, double yc,G2Xcolor colc,
                         double xd, double yd,G2Xcolor cold);
  void g2x_DrawFillQuad_4col(G2Xpoint A,G2Xcolor cola,
                             G2Xpoint B,G2Xcolor colb,
                             G2Xpoint C,G2Xcolor colc,
                             G2Xpoint D,G2Xcolor cold);

  /*! "spot" : tache de couleur <col> dégradée vers le transparent                  !*/
  /*! joli mais TRES coûteux, et pas toujours simple à utiliser...                  !*/
  void g2x_Spot(double xc, double yc, double r, G2Xcolor col);

  /* Quelques versions en macros - conservées pour compatibilité avec anciennes versions */
  /* MAIS IL FAUT EVITER DE LES UTILISER                                                 */
  #define G2Xdefpoint(x,y)                    /**/((G2Xpoint){(double)(x),(double)(y)})
  #define G2Xdrawpoint(P,col,size)            /**/g2x_Plot((P).x,(P).y,(col),(size));
  #define G2Xdrawline(A,B,col,size)           /**/g2x_Line(((A).x),((A).y),((B).x),((B).y),(col),(size))
  #define G2Xdrawline2col(A,colA,B,colB,size) /**/g2x_Line_2col(((A).x),((A).y),(colA),((B).x),((B).y),(colB),(size))
  #define G2Xdrawvect(A,U,col,size)           /**/g2x_Line(((A).x),((A).y),((A).x+(U).x),((A).y+(U).y),(col),(size))
  #define G2Xdrawcircle(C,r,col,size)         /**/g2x_Circle((C).x,(C).y,(r),(col),(size))
  #define G2Xdrawfillcircle(C,r,col)          /**/g2x_FillCircle((C).x,(C).y,(r),(col))


#endif

#ifdef __cplusplus
  }
#endif
/*!=============================================================!*/
