#include <g3x.h>

/* tailles de la fenêtre graphique (en pixels)     */
static int WWIDTH=768, WHEIGHT=640;

/* des couleurs prédéfinies */
static G3Xcolor rouge  ={1.0,0.0,0.0,1.0};
static G3Xcolor jaune  ={1.0,1.0,0.0,1.0};
static G3Xcolor vert   ={0.0,1.0,0.0,1.0};
static G3Xcolor bleu   ={0.0,0.0,1.0,1.0};


G3Xpoint  A,B,I,K;
G3Xvector u,v;
bool      flag=false;

bool distdroites_2(G3Xpoint A, G3Xvector u, G3Xpoint *I, G3Xpoint B, G3Xvector v, G3Xpoint *K)
{
  double c = g3x_ProdScal(u,v);
  double d = (1.-c*c);
  if (fabs(d)<1.E-8) return false;
  d = 1./d;
  G3Xvector AB=g3x_Vector2p(A,B);
  double a =+g3x_ProdScal(AB,u);
  double b =-g3x_ProdScal(AB,v);
  double ta = (a+b*c)*d;
  double tb = (b+a*c)*d;
  I->x = A.x+ta*u.x;
  I->y = A.y+ta*u.y;
  I->z = A.z+ta*u.z;
  K->x = B.x+tb*v.x;
  K->y = B.y+tb*v.y;
  K->z = B.z+tb*v.z;
  return true;
}


/*! fonction d'initialisation : passée à <g3x_SetInitFunction>      !*/
/*! ne dessine rien, fait éventuellement des calculs (1 seule fois) !*/
void init(void)
{
  A=(G3Xpoint ){-1.0,-1.0,-1.0};
  u=(G3Xvector){+0.5,+0.4,+0.2};
  g3x_Normalize(&u);
  B=(G3Xpoint ){+1.0,+1.0,+1.0};
  v=(G3Xvector){-0.2,-0.4,-0.5};
  g3x_Normalize(&v);

  /* l'algo. qui détermine les points I,K */
  flag = distdroites_2(A,u,&I,B,v,&K);
}

/*! fonction de dessin : passée à <g3x_SetDrawFunction> !*/
/*! ne fait aucun calcul, que du dessin                 !*/
void draw(void)
{
  glDisable(GL_LIGHTING);     /* <BALISE.GL>  "débranche" la lumière, pour permettre le tracé en mode point/ligne */

    /*! tracé de la "droite" [A,u) - En fait on trace le segment [A-10*u,A+10*u]                                 !*/
    g3x_Color4fv(bleu);       /* <VAR.ENV.GL> fonction g3x, emule la fonction glColor4fv(GLfloat [4])             */
    glLineWidth(1.0);         /* <VAR.ENV.GL> épaisseur de référence du trait (1.0 = val. par défaut)             */
    glBegin(GL_LINES);        /* <BALISE.GL>  début du bloc de tracé, en mode ligne                               */
      glVertex3d(A.x-10*u.x,A.y-10*u.y,A.z-10*u.z); /* 2 points, à relier par une ligne                           */
      glVertex3d(A.x+10*u.x,A.y+10*u.y,A.z+10*u.z);
    glEnd();                  /* <BALISE.GL>  fin du bloc                                                         */

    /*! tracé du segment [A,A+u] pour visualiser la postion de A et la direction de u                            !*/
    g3x_Color4fv(jaune);
    glPointSize(4.0);         /* <VAR.ENV.GL> taille de référence du point                                        */
    glBegin(GL_POINTS);       /* <BALISE.GL>  début du bloc de tracé, en mode "point"                             */
      g3x_Vertex3dv(A);       /*              fonction g3x, emule la fonction glVertex3dv(GLdouble [3])           */
    glEnd();
    glLineWidth(2.0);         /* <VAR.ENV.GL> on change l'épaisseur du trait,                                     */
    glBegin(GL_LINES);        /* <BALISE.GL>  début du bloc                                                       */
      glVertex3d(A.x,A.y,A.z);
      glVertex3d(A.x+u.x,A.y+u.y,A.z+u.z);
    glEnd();                  /* <BALISE.GL>  fin du bloc                                                         */

    /*! tracé de la "droite" [B,v)                                                                               !*/
    g3x_Color4fv(vert);
    glLineWidth(1.0);
    glBegin(GL_LINES);
      glVertex3d(B.x-10*v.x,B.y-10*v.y,B.z-10*v.z);
      glVertex3d(B.x+10*v.x,B.y+10*v.y,B.z+10*v.z);
    glEnd();
    /*! tracé du segment [B,B+v]                                                                                 !*/
    g3x_Color4fv(jaune);
    glPointSize(4.0);
    glBegin(GL_POINTS);
      g3x_Vertex3dv(A);
    glEnd();
    glLineWidth(2.0);
    glBegin(GL_LINES);
      glVertex3d(B.x,B.y,B.z);
      glVertex3d(B.x+v.x,B.y+v.y,B.z+v.z);
    glEnd();

  if (flag==true) /* si le calcul à donné un résultat : on trace [I,K] */
  {
    g3x_Color4fv(rouge);
    glPointSize(3.0);
    glBegin(GL_POINTS);
      g3x_Vertex3dv(I);
      g3x_Vertex3dv(K);
    glEnd();
    glLineWidth(1.);
    glBegin(GL_LINES);
      g3x_Vertex3dv(I);
      g3x_Vertex3dv(K);
    glEnd();
  }

  glEnable(GL_LIGHTING);      /* <BALISE.GL>  "rebranche" la lumière, pour permettre le tracé en mode "3D"      */
}


/***************************************************************************/
/* La fonction principale : NE CHANGE (presque) JAMAIS                     */
/***************************************************************************/
int main(int argc, char **argv)
{
  /* 1°) creation de la fenetre - titre et tailles (pixels) */
  g3x_InitWindow(*argv,WWIDTH,WHEIGHT);

  /* 2°) association des fonctions */
  g3x_SetInitFunction(init); /* fonction d'initialisation */
  g3x_SetCtrlFunction(NULL); /* fonction de contrôle      */
  g3x_SetEvtsFunction(NULL); /* fonction d'événements     */
  g3x_SetDrawFunction(draw); /* fonction de dessin        */
  g3x_SetAnimFunction(NULL); /* fonction d'animation      */
  g3x_SetExitFunction(NULL); /* fonction de sortie        */

  /* 3°) lancement de la boucle principale */
  return g3x_MainStart();
  /* RIEN APRES CA */
}
