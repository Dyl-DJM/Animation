/*!==================================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G3X - Fonctions de base d'acces public -
                     version 6e - Jan.2022
  =================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <g3x.h>

/*====================================================================*/
/*                    constructeurs/opérateurs                        */
/*====================================================================*/

/**========================================================================= *
 *  DES FONCTIONALITES DE BASE
 * ========================================================================= **/
/**-------------------------------------------------------------------------- *
 *  constructeur 1 : un point et une vitesse "réelle" (décomposée en direction/norme)
 *---------------------------------------------------------------------------**/
extern G3Xpart   g3x_Part_pv(G3Xpoint pos, G3Xvector vit)
{
  double d=g3x_Normalize(&vit);
  return (G3Xpart){pos,vit,d};
}

/**-------------------------------------------------------------------------- *
 *  constructeur 2 : un point et une direction et une norme
 *---------------------------------------------------------------------------**/
extern G3Xpart   g3x_Part_pud(G3Xpoint pos, G3Xvector dir, double amp)
{
  amp*=g3x_Normalize(&dir); // par sécurité !
  return (G3Xpart){pos,dir,amp};
}

/**-------------------------------------------------------------------------- *
 *  constructeur 3 : un bi-point [AB] => A, direction(AB), distance(AB)
 *---------------------------------------------------------------------------**/
extern G3Xpart   g3x_Part_2p(G3Xpoint A, G3Xpoint B)
{
  G3Xvector v=g3x_Vector2p(A,B);
  double d=g3x_Normalize(&v);
  return (G3Xpart){A,v,d};
}

/**-------------------------------------------------------------------------- *
 *  vecteur vitesse "réel" v=d*u
 *---------------------------------------------------------------------------**/
extern G3Xvector g3x_PartFullVect(G3Xpart *a)
{
  return (G3Xvector){(a->d*a->u.x),(a->d*a->u.y),(a->d*a->u.z)};
}

/**-------------------------------------------------------------------------- *
 *  position de l'extremité du vecteur p+d*u
 *---------------------------------------------------------------------------**/
extern G3Xpoint  g3x_PartNextPos(G3Xpart *a)
{
  return (G3Xpoint){(a->p.x+a->d*a->u.x),(a->p.y+a->d*a->u.y),(a->p.z+a->d*a->u.z)};
}

/**-------------------------------------------------------------------------- *
 *  déplacement de la particule p <- p+d*u
 *  équivalent à a->p = g3x_PartNextPos(a);
 *---------------------------------------------------------------------------**/
extern void g3x_PartMove(G3Xpart *a)
{
  a->p.x += a->d*a->u.x;
  a->p.y += a->d*a->u.y;
  a->p.z += a->d*a->u.z;
}

/**-------------------------------------------------------------------------- *
 *  rotation d'angles x,y,z "rads" (donc en radians...) du vecteur directeur u
 *---------------------------------------------------------------------------**/
extern void g3x_PartRotate(G3Xpart *a, G3Xvector rads)
{
  a->u = g3x_Mat_x_Vector(g3x_RotationX(rads.x),a->u);
  a->u = g3x_Mat_x_Vector(g3x_RotationY(rads.y),a->u);
  a->u = g3x_Mat_x_Vector(g3x_RotationZ(rads.z),a->u);
}


/**========================================================================== *
 *  DES FONCTIONALITES DE PLUS HAUT NIVEAU
 * ========================================================================== **/
/**-------------------------------------------------------------------------- *
 *  intersection des trajectoire et récupération du point de collision
 *---------------------------------------------------------------------------**/
extern bool g3x_PartInterPart(G3Xpart *a, G3Xpart *b, G3Xpoint *I)
{
  G3Xpart ab = g3x_Part_2p(a->p,b->p);

  // points trop eloignes, on passe
  if (ab.d > (a->d+b->d)) return false;

  // vecteurs paralleles : on ne detecte pas !!!
  double  sinab = g3x_Norm(g3x_ProdVect(a->u,b->u));
  if (G3Xiszero(sinab)) return false;

  double  t;
  // I = a->pos+t*a->d - position de I sur traj. de A
  t =+g3x_Norm(g3x_ProdVect(ab.u,b->u))*ab.d/sinab;
  if (t<0. || t>a->d) return false;
  // I = b->pos+t*b->d - position de I sur traj. de B
  t = +g3x_Norm(g3x_ProdVect(ab.u,a->u))*ab.d/sinab;
  if (t<0. || t>b->d) return false;

  I->x = b->p.x+t*b->u.x;
  I->y = b->p.y+t*b->u.y;
  I->z = b->p.z+t*b->u.z;

  return true;
}

/**-------------------------------------------------------------------------- *
 *  collision/déviation de 2 particules dans un rayon 'dist'
 *  concrètement, il faudra associer un 'rayon' à chaque particule et
 *  on utilisera dist = (ray_a+ray_b)
 *---------------------------------------------------------------------------**/
extern bool g3x_PartCollPart(G3Xpart *a, G3Xpart *b, double dist)
{
  G3Xpart ab = g3x_Part_2p(a->p,b->p);

  if (ab.d>dist) return false;

  G3Xvector u=a->u;
  a->u = g3x_SubVect(b->u,ab.u);
  g3x_Normalize(&a->u);
  b->u = g3x_AddVect(u,ab.u);
  g3x_Normalize(&b->u);

  return true;
}

/**-------------------------------------------------------------------------- *
 *  <a>  suit <b> : Va = (1.-alf)*Va + alf*dir(AB)*norme(Vb)
 *  si dist(a,b)<dist : pas de poursuite
 *  (usage classique : dist=(ray_a+ray_b -- cf. au dessus)
 *---------------------------------------------------------------------------**/
extern bool g3x_PartPursuit(G3Xpart *a, G3Xpart *b, double alf, double dist)
{
  if (G3Xiszero(alf)) return false;
  G3Xpart ab = g3x_Part_2p(a->p,b->p);
  // si A trop proche de B, on passe
  if (ab.d<dist) return false;

  G3Xvector v = g3x_mapscal3(a->u,a->d); // v = a.u*a.d : vitesse réelle de a
  G3Xvector w = g3x_mapscal3(ab.u,b->d); // dir ab, vitesse réelle de b
  v.x = (1.-alf)*v.x + alf*w.x;
  v.y = (1.-alf)*v.y + alf*w.y;
  v.z = (1.-alf)*v.z + alf*w.z;
  double d=g3x_Normalize(&v);
  a->u = v;
  a->d = d;
  return true;
}

/**-------------------------------------------------------------------------- *
 *  <a>  suit <b> : Va = (1.-alf)*Va + alf*dir(AB)*norme(Vb)
 *  si dist(a,b)<dist : pas de poursuite
 *  (usage classique : dist=(ray_a+ray_b -- cf. au dessus)
 *---------------------------------------------------------------------------**/
extern bool g3x_PartPosTrack(G3Xpart *a, G3Xpoint tar, double alf, double dist)
{
  if (G3Xiszero(alf)) return false;
  G3Xpart ab = g3x_Part_2p(a->p,tar);
  // si A trop proche de B, on passe
  if (ab.d<dist) return false;

  // Ici, la particule a DOIT garder sa vitesse initiale
  // => pas de mise à jour de la norme, JUSTE la direction !
  a->u.x = (1.-alf)*a->u.x + alf*ab.u.x;
  a->u.y = (1.-alf)*a->u.y + alf*ab.u.y;
  a->u.z = (1.-alf)*a->u.z + alf*ab.u.z;
  g3x_Normalize(&a->u);

  return true;
}


/**-------------------------------------------------------------------------- *
 *  Collision/déviation Particule/Sphere
 *---------------------------------------------------------------------------**/
extern bool g3x_PartCollSphere(G3Xpart *a, G3Xpoint C, double r)
{
  // si l'extremite de la traj. (B=p+d*u) n'est pas dans le cercle : pas de collision
  G3Xpoint  B = g3x_PartNextPos(a);
  if (g3x_SqrDist(C,B)>SQR(r)) return false;

  G3Xpart ac = g3x_Part_2p(a->p,C);

  if (ac.d<r) // si la cellule est déjà dans le cercle, elle est expulsée
  {
    a->u = g3x_mapscal3(ac.u,-1.);
    return 1;
  }

  // distance de A à P, proj de C sur [Au)
  double t = ac.d*g3x_ProdScal(a->u,ac.u);
  // dist. de C à P
  double d = ac.d*g3x_Norm(g3x_ProdVect(a->u,ac.u));

  // la trajectoire ne se dirige pas vers le cercle ou passe a cote
  if (t<0. || fabs(d)>r) return false;

  double e = t-sqrt(SQR(r)-SQR(d)); // dist. de A au point de collision

  // repositionnement de B sur le point de collision
  B.x = a->p.x+e*a->u.x;
  B.y = a->p.y+e*a->u.y;
  B.z = a->p.z+e*a->u.z;

  // normale sur la nouvelle pos. de A sur le cercle
  G3Xvector N = g3x_Vector2p(C,B);
  g3x_Normalize(&N);

  t =-2.*g3x_ProdScal(N,a->u);
  // redirection de v : symetrique par rapport a n
  a->u.x += t*N.x;
  a->u.y += t*N.y;
  a->u.z += t*N.z;

  g3x_Normalize(&a->u); // nécessaire ??

  return 1;
}


/**-------------------------------------------------------------------------- *
 *  Collision/déviation Particule/Sphere
 *---------------------------------------------------------------------------**/
extern bool g3x_PartBypassSphere(G3Xpart *a, G3Xpoint C, double r, double coeff)
{
  G3Xpart ac = g3x_Part_2p(a->p,C);

  if (ac.d<r) // si la cellule est déjà dans le cercle, elle est expulsée
  {
    a->u = g3x_mapscal3(ac.u,-1.); // u << -CA/r
    return true;
  }
  double t = ac.d*g3x_ProdScal(a->u,ac.u); // distance de A à P, proj de C sur [Au)
  double d = ac.d*g3x_Norm(g3x_ProdVect(a->u,ac.u)); // dist. de C à P

  // la trajectoire ne se dirige pas vers le cercle ou passe a cote
  if (t<0. || fabs(d)>r) return false;

  // vecteur CP, P : projection de C sur [Au)
  G3Xpart cp = g3x_Part_2p(C,(G3Xpoint){a->p.x+t*a->u.x,
                                        a->p.y+t*a->u.y,
                                        a->p.z+t*a->u.z});

  coeff = CLIP(1.,coeff,25.);
  t=pow(r/t,coeff);
  a->u.x += t*cp.u.x;
  a->u.y += t*cp.u.y;
  a->u.z += t*cp.u.z;

  g3x_Normalize(&a->u);

  return true;
}


extern void g3x_PartBoxBounce(G3Xpart *a, G3Xpoint DLB, G3Xpoint URF)
{
  if      (a->p.x<=DLB.x) { a->u.x = -a->u.x; a->p.x=0.999*DLB.x; }
  else if (a->p.x>=URF.x) { a->u.x = -a->u.x; a->p.x=0.999*URF.x; }
  if      (a->p.y<=DLB.y) { a->u.y = -a->u.y; a->p.y=0.999*DLB.y; }
  else if (a->p.y>=URF.y) { a->u.y = -a->u.y; a->p.y=0.999*URF.y; }
  if      (a->p.z<=DLB.z) { a->u.z = -a->u.z; a->p.z=0.999*DLB.z; }
  else if (a->p.z>=URF.z) { a->u.z = -a->u.z; a->p.z=0.999*URF.z; }
}


/**-------------------------------------------------------------------------- *
 *  traverse les bords de la fenêtre pour revenir de l'autre côté
 *---------------------------------------------------------------------------**/
extern void g3x_PartBoxCross(G3Xpart *a, G3Xcoord DLB, G3Xcoord URF)
{
       if (a->p.x<DLB.x) { a->p.x = URF.x+a->d*a->u.x; return; }
  else if (a->p.x>URF.x) { a->p.x = DLB.x+a->d*a->u.x; return; }
       if (a->p.y<DLB.y) { a->p.y = URF.y+a->d*a->u.y; return; }
  else if (a->p.y>URF.y) { a->p.y = DLB.y+a->d*a->u.y; return; }
       if (a->p.z<DLB.z) { a->p.z = URF.z+a->d*a->u.z; return; }
  else if (a->p.z>URF.z) { a->p.z = DLB.z+a->d*a->u.z; return; }
}
