/*!==================================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G2X - Fonctions de base d'acces public -
                    version 6e - aout 2022
  =================================================================== */

#include <g2x.h>

/*====================================================================*/
/*                    constructeurs/opérateurs                        */
/*====================================================================*/

/**========================================================================= *
 *  DES FONCTIONALITES DE BASE
 * ========================================================================= **/
/**-------------------------------------------------------------------------- *
 *  constructeur 1 : un point et une vitesse "réelle" (décomposée en direction/norme)
 *---------------------------------------------------------------------------**/
extern G2Xpart   g2x_Part_pv(G2Xpoint pos, G2Xvector vit)
{
  double d=g2x_Normalize(&vit);
  return (G2Xpart){pos,vit,d};
}

/**-------------------------------------------------------------------------- *
 *  constructeur 2 : un point et une direction et une norme
 *---------------------------------------------------------------------------**/
extern G2Xpart   g2x_Part_pud(G2Xpoint pos, G2Xvector dir, double amp)
{
  amp*=g2x_Normalize(&dir); // par sécurité !
  return (G2Xpart){pos,dir,amp};
}

/**-------------------------------------------------------------------------- *
 *  constructeur 3 : un bi-point [AB] => A, direction(AB), distance(AB)
 *---------------------------------------------------------------------------**/
extern G2Xpart   g2x_Part_2p(G2Xpoint A, G2Xpoint B)
{
  G2Xvector v=g2x_Vector2p(A,B);
  double d=g2x_Normalize(&v);
  return (G2Xpart){A,v,d};
}

/**-------------------------------------------------------------------------- *
 *  vecteur vitesse "réel" v=d*u
 *---------------------------------------------------------------------------**/
extern G2Xvector g2x_PartFullVect(G2Xpart *a)
{
  return (G2Xvector){(a->d*a->u.x),(a->d*a->u.y)};
}

/**-------------------------------------------------------------------------- *
 *  position de l'extremité du vecteur p+d*u
 *---------------------------------------------------------------------------**/
extern G2Xpoint  g2x_PartNextPos(G2Xpart *a)
{
  return (G2Xpoint){(a->p.x+a->d*a->u.x),(a->p.y+a->d*a->u.y)};
}

/**-------------------------------------------------------------------------- *
 *  déplacement de la particule p <- p+d*u
 *  équivalent à a->p = g2x_PartNextPos(a);
 *---------------------------------------------------------------------------**/
extern void g2x_PartMove(G2Xpart *a)
{
  a->p.x += a->d*a->u.x;
  a->p.y += a->d*a->u.y;
}

/**-------------------------------------------------------------------------- *
 *  rotation d'angle "rad" (donc en radians...) du vecteur directeur u
 *---------------------------------------------------------------------------**/
extern void g2x_PartRotateRad(G2Xpart *a, double rad)
{
  a->u = g2x_Mat_x_Vector(g2x_Rotation(rad),a->u);
}

extern void g2x_PartRotate_DegLUT(G2Xpart *a, double deg)
{
  a->u = g2x_Mat_x_Vector(g2x_Rotation_LUT(deg),a->u);
}


/**-------------------------------------------------------------------------- *
 *  traverse les bords de la fenêtre pour revenir de l'autre côté
 *---------------------------------------------------------------------------**/
extern void g2x_PartCross(G2Xpart *a)
{
       if (a->p.x<g2x_GetXMin()) { a->p.x = g2x_GetXMax()+a->d*a->u.x; return; }
  else if (a->p.x>g2x_GetXMax()) { a->p.x = g2x_GetXMin()+a->d*a->u.x; return; }
       if (a->p.y<g2x_GetYMin()) { a->p.y = g2x_GetYMax()+a->d*a->u.y; return; }
  else if (a->p.y>g2x_GetYMax()) { a->p.y = g2x_GetYMin()+a->d*a->u.y; return; }
}

/**-------------------------------------------------------------------------- *
 *  rebond sur les bords de la fenêtre (façon billard)
 *---------------------------------------------------------------------------**/
extern void g2x_PartBounce(G2Xpart *a)
{
       if (a->p.x<g2x_GetXMin()) { a->u.x *= -1.; a->p.x = g2x_GetXMin()+a->d*a->u.x; return; }
  else if (a->p.x>g2x_GetXMax()) { a->u.x *= -1.; a->p.x = g2x_GetXMax()+a->d*a->u.x; return; }

       if (a->p.y<g2x_GetYMin()) { a->u.y *= -1.; a->p.y = g2x_GetYMin()+a->d*a->u.y; return; }
  else if (a->p.y>g2x_GetYMax()) { a->u.y *= -1.; a->p.y = g2x_GetYMax()+a->d*a->u.y; return; }
}

/**-------------------------------------------------------------------------- *
 *  affichage, juste pour prototypage d'algo
 *---------------------------------------------------------------------------**/
extern void g2x_PartDraw(G2Xpart *a,G2Xcolor col)
{
  g2x_DrawPoint(a->p,col,5);
  g2x_DrawLine(a->p,g2x_PartNextPos(a),col,1);
  if (a->d<1.) return;
  g2x_DrawLine(a->p,(G2Xpoint){a->p.x+a->u.x, a->p.y+a->u.y},col,3);
}

/**========================================================================== *
 *  DES FONCTIONALITES DE PLUS HAUT NIVEAU
 * ========================================================================== **/
/**-------------------------------------------------------------------------- *
 *  intersection des trajectoire et récupération du point de collision
 *---------------------------------------------------------------------------**/
extern bool g2x_PartInterPart(G2Xpart *a, G2Xpart *b, G2Xpoint *I)
{
  G2Xpart ab = g2x_Part_2p(a->p,b->p);

  if (ab.d > (a->d+b->d)) return false;     // points trop eloignes, on passe

  double  sinab = g2x_ProdVect(a->u,b->u); // vecteurs paralleles : on ne detecte pas !!!
  if (G2Xiszero(sinab)) return false;

  double  t;
  t =+g2x_ProdVect(ab.u,b->u)*ab.d/sinab;   // I = a->pos+t*a->d - position de I sur traj. de A
  if (t<0. || t>a->d) return false;

  t = +g2x_ProdVect(ab.u,a->u)*ab.d/sinab;  // I = b->pos+t*b->d - position de I sur traj. de B
  if (t<0. || t>b->d) return false;

  I->x = b->p.x+t*b->u.x;
  I->y = b->p.y+t*b->u.y;

  return true;
}

/**-------------------------------------------------------------------------- *
 *  collision/déviation de 2 particules dans un rayon 'dist'
 *  concrètement, il faudra associer un 'rayon' à chaque particule et
 *  on utilisera dist = (ray_a+ray_b)
 *---------------------------------------------------------------------------**/
extern bool g2x_PartCollPart(G2Xpart *a, G2Xpart *b, double dist)
{
  G2Xpart ab = g2x_Part_2p(a->p,b->p);

  if (ab.d>dist) return false;

  G2Xvector u=a->u;
  a->u = g2x_SubVect(b->u,ab.u);
  g2x_Normalize(&a->u);
  b->u = g2x_AddVect(u,ab.u);
  g2x_Normalize(&b->u);

  return true;
}

/**-------------------------------------------------------------------------- *
 *  <a>  suit <b> : Va = (1.-alf)*Va + alf*dir(AB)*norme(Vb)
 *  si dist(a,b)<dist : pas de poursuite
 *  (usage classique : dist=(ray_a+ray_b -- cf. au dessus)
 *---------------------------------------------------------------------------**/
extern bool g2x_PartPursuit(G2Xpart *a, G2Xpart *b, double alf, double dist)
{
  if (G2Xiszero(alf)) return false;
  G2Xpart ab = g2x_Part_2p(a->p,b->p);
  // si A trop proche de B, on passe
  if (ab.d<dist) return false;

  G2Xvector v = g2x_mapscal2(a->u,a->d); // v = a.u*a.d : vitesse réelle de a
  G2Xvector w = g2x_mapscal2(ab.u,b->d); // dir AB, vitesse réell de b
  v.x = (1.-alf)*v.x + alf*w.x;
  v.y = (1.-alf)*v.y + alf*w.y;
  double d=g2x_Normalize(&v);
  a->u = v;
  a->d = d;
  return true;
}


/**-------------------------------------------------------------------------- *
 *  <a>  suit <b> : Va = (1.-alf)*Va + alf*dir(AB)*norme(Vb)
 *  si dist(a,b)<dist : pas de poursuite
 *  (usage classique : dist=(ray_a+ray_b -- cf. au dessus)
 *---------------------------------------------------------------------------**/
extern bool g2x_PartPosTrack(G2Xpart *a, G2Xpoint tar, double alf, double dist)
{
  if (G2Xiszero(alf)) return false;
  G2Xpart ab = g2x_Part_2p(a->p,tar);
  // si A trop proche de B, on passe
  if (ab.d<dist) return false;

  // Ici, la particule a DOIT garder sa vitesse initiale
  // => pas de mise à jour de la norme, JUSTE la direction !
  a->u.x = (1.-alf)*a->u.x + alf*ab.u.x;
  a->u.y = (1.-alf)*a->u.y + alf*ab.u.y;
  g2x_Normalize(&a->u);

  return true;
}



/**-------------------------------------------------------------------------- *
 *  Collision/déviation Particule/Cercle
 *---------------------------------------------------------------------------**/
extern bool g2x_PartCollCircle(G2Xpart *a, G2Xpoint C, double r)
{
  // si l'extremite de la traj. (B=p+d*u) n'est pas dans le cercle : pas de collision
  G2Xpoint  B = g2x_PartNextPos(a);
  if (g2x_SqrDist(C,B)>SQR(r)) return false;

  G2Xpart ac = g2x_Part_2p(a->p,C);

  if (ac.d<r) // si la cellule est déjà dans le cercle, elle est expulsée
  {
    a->u = g2x_mapscal2(ac.u,-1.);
    return 1;
  }

  double t = ac.d*g2x_ProdScal(a->u,ac.u); // distance de A à P, proj de C sur [Au)
  double d = ac.d*g2x_ProdVect(a->u,ac.u); // dist. de C à P

  // la trajectoire ne se dirige pas vers le cercle ou passe a cote
  if (t<0. || fabs(d)>r) return false;

  double e = t-sqrt(SQR(r)-SQR(d)); // dist. de A au point de collision

  // repositionnement de B sur le point de collision
  B.x = a->p.x+e*a->u.x;
  B.y = a->p.y+e*a->u.y;

  // normale sur la nouvelle pos. de A sur le cercle
  G2Xvector N = g2x_Vector2p(C,B);
  g2x_Normalize(&N);

  t =-2.*g2x_ProdScal(N,a->u);
  // redirection de v : symetrique par rapport a n
  a->u.x += t*N.x;
  a->u.y += t*N.y;

  g2x_Normalize(&a->u); // nécessaire ??

  return 1;
}


/**-------------------------------------------------------------------------- *
 *  Collision/déviation Particule/Cercle
 *---------------------------------------------------------------------------**/
extern bool g2x_PartBypassCircle(G2Xpart *a, G2Xpoint C, double r, double coeff)
{
  G2Xpart ac = g2x_Part_2p(a->p,C);

  if (ac.d<r) // si la cellule est déjà dans le cercle, elle est expulsée
  {
    a->u = g2x_mapscal2(ac.u,-1.); // u << -CA/r
    return true;
  }
  double t = ac.d*g2x_ProdScal(a->u,ac.u); // distance de A à P, proj de C sur [Au)
  double d = ac.d*g2x_ProdVect(a->u,ac.u); // dist. de C à P

  // la trajectoire ne se dirige pas vers le cercle ou passe a cote
  if (t<0. || fabs(d)>r) return false;

  // vecteur CP, P : projection de C sur [Au)
  G2Xpart cp = g2x_Part_2p(C,(G2Xpoint){a->p.x+t*a->u.x,
                                        a->p.y+t*a->u.y});

  coeff = CLIP(1.,coeff,25.);
  t=pow(r/t,coeff);
  a->u.x += t*cp.u.x;
  a->u.y += t*cp.u.y;

  g2x_Normalize(&a->u);

  return true;
}
