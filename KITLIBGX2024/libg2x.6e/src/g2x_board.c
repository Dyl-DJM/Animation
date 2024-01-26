#include <g2x.h>

static double wxmin,wxmax,wymin,wymax,yxratio;

extern __inline__ G2Xpoint g2x_GetBoardPosition(G2Xboard *board, int l, int c)
{
  return (G2Xpoint){(-board->nbrow+2*c+1)*board->sqrdim,(-board->nbline+2*l+1)*board->sqrdim };
}

extern __inline__ void g2x_DrawSingleSquare(G2Xboard *board, int y, int x, G2Xcolor col, bool FILLED)
{
  G2Xpoint DL,UR;
  DL = UR = g2x_GetBoardPosition(board,y,x);
  DL.x-=board->sqrdim; DL.y-=board->sqrdim;
  UR.x+=board->sqrdim; UR.y+=board->sqrdim;
  if (FILLED) { g2x_DrawFillRectangle(DL,UR,col);
                if (board->sqrdim>8*g2x_GetXPixSize())
                  g2x_DrawRectangle(DL,UR,G2Xk,1);
              }
  else g2x_DrawRectangle(DL,UR,col,4);
}

extern bool g2x_GetClickedSquare(G2Xboard *board, int *line, int *col, G2Xpoint *clic)
{
  G2Xpoint* mouse=g2x_GetClic();
  if (!mouse) return false;
  *line  = (int)((mouse->y-wymin)*0.5/board->sqrdim);
  *col   = (int)((mouse->x-wxmin)*0.5/board->sqrdim);
  *clic = g2x_GetBoardPosition(board,*line,*col);
  g2x_DrawSingleSquare(board,*line,*col,G2Xc,1);
  return true;
}


extern void g2x_DrawGameBoard(G2Xboard *board)
{
  int y,x;
  for (y=0;y<board->nbline;y++)
    for (x=0;x<board->nbrow;x++)
      g2x_DrawSingleSquare(board,y,x,board->sqrcol[y][x],true);
}


extern void g2x_FreeGameBoard(G2Xboard *board)
{
  if (board->sqrcol)
  {
    for ( G2Xcolor** l=board->sqrcol; l< board->sqrcol+board->nbline; l++)
      if (*l) free(*l);
    free(board->sqrcol);
    board->sqrcol = NULL;
  }
}


extern bool g2x_CreateGameBoard(G2Xboard *board, int nbline, int nbrow)
{
  // allocation de la grille : positions, couleurs
  board->sqrcol = (G2Xcolor**)calloc(nbline,sizeof(G2Xcolor*));
  if (!(board->sqrcol)) { g2x_FreeGameBoard(board); return false; }
  G2Xcolor **cline =board->sqrcol;
  while (cline<board->sqrcol+nbline)
  {
    *cline = (G2Xcolor*)calloc(nbrow,sizeof(G2Xcolor));
    if (!(cline)) { g2x_FreeGameBoard(board); return false; }
    cline++;
  }

  board->nbline  = nbline;
  board->nbrow   = nbrow;
  board->sqrdim  = 1./nbline;

  /* initialisation des points & des couleurs */
  double v,w=0.25,z;
  for (int l=0; l<board->nbline; l++)
  {
    z=w;
    for (int c=0; c<board->nbrow; c++)
    {
      v = 0.75+w;
      w = -w;
      board->sqrcol[l][c] = (G2Xcolor){v,v,v,0.80};
    }
    w = -z;
  }
  return true;
}


extern bool g2x_InitGameBoard(G2Xboard *board, char* windname, int nbline, int nbrow)
{
  yxratio = (nbrow*1.)/nbline;
  /* Dimension de la fenêtre à l'ouverture */
  int height = MIN(G2X_DEFSQRPIX*nbrow  , 960);//960=512+256+128+64
  int width  = MIN((int)(yxratio*height),1440);
  g2x_InitWindow(windname,width,height);

  /* Dimension réelles de la zone graphique initiale : (-1,-1)->(+1,+1) */
  wymin   = -1.; wxmin   = wymin*yxratio;
  wymax   = +1.; wxmax   = wymax*yxratio;
  g2x_SetWindowCoord(wxmin,wymin,wxmax,wymax);

  return g2x_CreateGameBoard(board,nbline,nbrow);
}

