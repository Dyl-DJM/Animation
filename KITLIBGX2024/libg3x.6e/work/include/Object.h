
#ifndef _OBJECT_H
  #define _OBJECT_H

  #include <g3x.h>

  #define MAXRES 720

  typedef struct _obj_
  {
    size_t     n1,n2,n3;
    G3Xpoint  *vrtx;
    G3Xvector *norm;
    void     (*draw_points)(struct _obj_*, G3Xvector, double);
    void     (*draw_norms )(struct _obj_*, G3Xvector, double);
    void     (*draw_quads )(struct _obj_*, G3Xvector, double);
  } Object;

  void NormVrtx3dv(G3Xvector *N, G3Xpoint *P, int i);
  void Normale(G3Xvector *N, G3Xpoint *P, int i, double amp);

  bool object_init(Object* obj, size_t size);
  void object_free(Object* obj);

  bool load_sphere  (Object *obj);
/*
  bool load_carre   (Object *obj);
  bool load_cube    (Object *obj);
  bool load_cylinder(Object *obj);
  bool load_cone    (Object *obj);
  bool load_torus   (Object *obj);
*/

#endif
