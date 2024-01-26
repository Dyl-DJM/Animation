#ifndef _CHAIN_H_
  #define _CHAIN_H_

  #include <1-Circ.h>
  #include <2a-Cell.h>

  typedef struct _chain_
  {
    Cell*  head;
    double alf;
    struct _chain_ *prev, *next; // chaînage des "Chain"
  }
  Chain;

  Chain* new_chain(Chain *next_chain, int length, G3Xpart par, double ray, double alf, G3Xcolor col);
  void   chain_free_list(Chain** S);
  void   draw_chain(Chain *s);
  void   draw_snake(Chain *s);

  void   chain_move(Chain *s, Circle *circ, bool ONDULE);

  bool   chain_hit_chain(Chain *s1, Chain *s2);
  void   chain_chase_chain(Chain *s1, Chain *s2);
  void   chain_chase_setofchain(Chain *s1, Chain *slist);
  void   move_snake(Chain* snake);
  Cell*  chain_search_prey(Chain *s, Cell **apples);
  int    chain_eat_prey(Chain *s, Cell *eat, Cell **preys);
#endif
