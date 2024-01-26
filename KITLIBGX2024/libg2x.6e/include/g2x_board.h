/**
 * @file    g2x_gameboard.h
 * @author  Universite Gustave Eiffel
 * @author  E.Incerti - eric.incerti@univ-eiffel.fr
 * @brief   Plateau de jeu style échec, dame, démineur....
 * @version 6.e
 * @date    Aug.2022 (doc generation)
 */
/**@cond SHOW_SECURITY_DEFINE */

#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#ifndef _G2X_GAMEBOARD_H
  #define _G2X_GAMEBOARD_H
/**@endcond                   */

  #include <g2x_types.h>

  #define G2X_DEFSQRPIX 64

  /**
   * @struct G2Xboard
   * @brief
  **/
  typedef struct _g2x_board_
  {
    int        nbline,nbrow; // nombre de ligne/colonne du plateau
    double     sqrdim;       // dimension d'une case du plateau
    G2Xcolor **sqrcol;       // les cases tableau double de taille nbline*nbrow
  } G2Xboard;

  bool     g2x_CreateGameBoard (G2Xboard *board, int height, int width);
  void     g2x_FreeGameBoard   (G2Xboard *board);
  bool     g2x_InitGameBoard   (G2Xboard *board, char* windname, int nbline, int nbrow);

  G2Xpoint g2x_GetBoardPosition(G2Xboard *board, int  l, int  c);
  bool     g2x_GetClickedSquare(G2Xboard *board, int *line, int *col, G2Xpoint *clic);

  void     g2x_DrawSingleSquare(G2Xboard *board, int y, int x, G2Xcolor col, bool FILLED);
  void     g2x_DrawGameBoard   (G2Xboard *board);

#endif

#ifdef __cplusplus
  }
#endif
/*!===========================================================================!*/
