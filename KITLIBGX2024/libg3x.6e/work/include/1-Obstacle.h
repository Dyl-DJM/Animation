#ifndef _CIRC_H_
  #define _CIRC_H_

  #define SIZE 50

  #include <Object.h>

  typedef struct _obst_
  {
    G3Xpart   par; // une position (centre) et une vitesse (u,d)
    G3Xcolor  col; // une couleur
    double    ray; // un rayon
    Object*   obj;
    struct _obst_ *prev,*next;
  } Obstacle;

  Obstacle* new_random_obst(Obstacle *circ, double rmin, double rmax,Object* obj);
  int       coll_obst(Obstacle *c, Obstacle *d);
  void      obst_move(Obstacle *c);
  void      obst_tex(Obstacle *c, char* texfilename);
  void      draw_obst(Obstacle *c);
  void      free_obst_list(Obstacle **circ_list);
  int       point_int_obst(G3Xpoint *P, Obstacle *c);

#endif

