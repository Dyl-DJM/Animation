#ifndef _CELL_H_
  #define _CELL_H_

  #include <1-Circ.h>

  #define RES 20

  typedef struct _cell_
  {
    G3Xpart   h,b,t;
    G3Xcolor  col;    // une couleur
    double    alf;
    struct _cell_ *prev,*next;
  } Cell;

  void   cell_setup(Cell *e);
  void   cell_head_insert(Cell **L, Cell *new);
  void   cell_insert_between(Cell *prev, Cell *new, Cell *next);
  void   cell_unlist(Cell **list, Cell *c);
  Cell  *cell_create(G3Xpart par, G3Xcolor col, double ray, double alf);
  void   cell_free_list(Cell** cell_list);
  void   cell_force_vit(Cell *c, double vit);
  void   cell_draw_pred(Cell *p, G3Xcolor col);
  void   cell_draw_prey(Cell *p);
  bool   cell_inter_circ(Cell *a, G3Xpoint C, double r);
  bool   cell_hit_circles(Cell *a, Circle *circ);
  bool   cell_bypass_circles(Cell *a, Circle *circ);
  bool   cell_hit_cell(Cell *a, Cell *b);
  void   cell_move(Cell *a);
  void   cell_bounce(Cell *a);
  void   cell_cross(Cell *a);
  bool   cell_pursuit (Cell *a, Cell *b);
  double cell_chase(Cell *a, Cell *b);
  double cell_escape(Cell *a, Cell *b);
  double cell_cone(Cell *a, G3Xpoint C, double r, G3Xvector *w);
  void   cell_swarm(Cell *clist, Cell *lead);
#endif
