/**@file    g2x_polygon.h
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

#ifndef _G2X_POLYGON_H
  #define _G2X_POLYGON_H
/**@endcond                   */

  #include <g2x_geom.h>

  typedef struct _cell
  {
    G2Xpoint s;
    double   o; // ouverture de l'angle
    struct _cell *prev,*next;
  } G2Xvertex, *G2Xpolygon;

  void       g2x_SetPolygonStockSize(uint n);
  void       g2x_FreeStockPolygon(void); /* usage interne - NE PAS UTILISER */
  G2Xvertex* g2x_Vertex_XY(double x, double y);
  G2Xvertex* g2x_Vertex(G2Xpoint P);
  void       g2x_InsereSommet(G2Xpolygon *Pol, G2Xpoint P);
  void       g2x_InsereVertex(G2Xpolygon *Pol, G2Xvertex *newv_rtx);
  G2Xvertex* g2x_InsereApres(G2Xvertex* V, G2Xpoint P);
  G2Xvertex* g2x_InsereVertexApres(G2Xvertex* prev_vrtx, G2Xvertex *new_vrtx);
  void       g2x_SetPolygonOpen(G2Xpolygon pol);
  G2Xvertex* g2x_DelVertex(G2Xvertex** V);
  int        g2x_CleanPolygon(G2Xpolygon Pol);
  void       g2x_PrintPolygon(G2Xpolygon Pol, FILE* output);
  void       g2x_ScanPolygon(G2Xpolygon* Pol, FILE* input);
  void       g2x_RegulPolygon(G2Xpolygon* Pol, uint deg, G2Xpoint C, double r, double alpha);
  void       g2x_TabPolygon(G2Xpolygon* Pol, G2Xpoint S[], uint n, bool closed);
  int        g2x_IsConvex(G2Xpolygon Pol);
  void       g2x_DrawPolygon(G2Xpolygon Pol, G2Xcolor col, uint w);
  void       g2x_DrawSplinePolygon(G2Xpolygon Pol);
  void       g2x_DrawFilledPolygon(G2Xpolygon Pol, G2Xcolor col);
  void       g2x_FreePolygon(G2Xpolygon* Pol);

  bool       g2x_PointDansPolygon(G2Xpoint P, G2Xpolygon Pol);
  bool       g2x_VertexDansPolygon(G2Xvertex *v, G2Xpolygon Pol, int orient, G2Xvertex** som);
  bool       g2x_PointDansPolygon2(G2Xpoint P, G2Xpolygon Pol, G2Xvertex** som);
  G2Xvertex* g2x_PointDansPolygon3(G2Xpoint P, G2Xpolygon Pol);

/*! Intersection de polygones                                     !*/
  G2Xpolygon g2x_PolygonInterPolygon(G2Xpolygon Pol1, G2Xpolygon Pol2);

/*! Clipping d'un segment sur un polygone                         !*/
  bool      g2x_Clipping(G2Xpoint *A, G2Xpoint *B, G2Xpolygon Pol);
  int       g2x_Clipping2(G2Xpoint* A, G2Xpoint* B, G2Xpolygon Pol);

/*! Clipping d'un polygone sur un cercle :
  *  -1 : les 2 sont disjoints - par d'untersection, pas d'inclusion
  *   0 : le polygone est inclus dans le cercle
  *  +1 : le cercle est inclus dans le polygone
  * n>1 : le polygone intersection a n sommets, certains pouvant
          appartenir au polyg. initial.
                                                                  !*/
  int g2x_CercleClipPolyg(G2Xpoint C, double r, G2Xpolygon Poly, G2Xpolygon *Clip);

  /*! Enveloppe convexe (polygone) sur un ensemble de points !*/
  bool g2x_ConvexHull(G2Xpoint *point, int nbpt, G2Xpolygon *EC, int *EClen);

#endif

#ifdef __cplusplus
  }
#endif
/*!===========================================================================!*/
