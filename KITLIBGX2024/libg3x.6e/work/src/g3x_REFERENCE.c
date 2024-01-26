/* ===============================================================
  E.Incerti - Université Gustave Eiffel - eric.incerti@univ-eiffel.fr
  ============================================================= */

#include <g3x.h>

/* tailles de la fenêtre graphique (en pixels)     */
static int WWIDTH=768, WHEIGHT=640;

/* DES TAS DE VARIABLES GLOBALES !!!       */
/* des couleurs prédéfinies <g3x_colors.h> */
static G3Xcolor rouge  ={1.0,0.2,0.2,0.0};
static G3Xcolor jaune  ={1.0,1.0,0.2,0.0};
static G3Xcolor vert   ={0.2,1.0,0.2,0.0};
static G3Xcolor cyan   ={0.2,1.0,1.0,0.0};
static G3Xcolor bleu   ={0.3,0.3,1.0,0.0};
static G3Xcolor magenta={1.0,0.2,1.0,0.0};

/* flag d'affichage/masquage utilisées par <void ctrl(void);> */
static bool FLAG_TEAPOT=true;
static bool FLAG_TORUS =true;
static bool FLAG_CONE  =true;
static bool FLAG_ICOS  =true;

/* paramètres géométriques */
static double    angle= 0.00;
static double    rayon= 0.66;
static G3Xvector W={1.,2.,3.};
static double    b=0.1;

/* paramètres de lumière/matière */
static double alpha= 0.5;
static double ambi = 0.2;
static double diff = 0.3;
static double spec = 0.4;
static double shin = 0.5;

/* une table de couleurs HSV */
#define MAXCOL 24
static G3Xcolor colmap[MAXCOL];

static uint id_teapot,id_torus, id_cone;

/*! FONCTION D'INITIALISATION
 *  contient la partie "modélisation"
 *  appelée une seule fois, au début
 * TOUTES LES ALLOCS. DYN. DOIVENT ETRE LA   !*/
void init(void)
{
  /* caméra  <g2x_camlight.h>                                */
  /* param. géométrique de la caméra. cf. gluLookAt(...)     */
  /* FACULTATIF => c'est les valeurs par défaut              */
  g3x_SetPerspective(40.,100.,1.);
  /* position, orientation de la caméra en coord. sphériques */
  /* FACULTATIF => c'est les valeurs par défaut              */
  g3x_SetCameraSpheric(0.25*PI,+0.25*PI,6.);

  /* lumière <g2x_camlight.h>                                */
  /* fixe les param. colorimétriques du spot lumineux        */
  /* lumiere blanche (c'est les valeurs par defaut)          */
  /* FACULTATIF => c'est les valeurs par défaut              */
  g3x_SetLightAmbient (1.,1.,1.);
  g3x_SetLightDiffuse (1.,1.,1.);
  g3x_SetLightSpecular(1.,1.,1.);
  /* fixe la position                                        */
  /* FACULTATIF => c'est les valeurs par défaut              */
  g3x_SetLightPosition (10.,10.,10.);

  /* initialisation d'une carte de <MAXCOL> couleurs HSV */
  /* [0:rouge | 1/3:vert | 2/3:bleu | 1:rouge]           */
  /* cf. <g2x_colors.h> */
  g3x_ColorMapHSV360(colmap,MAXCOL);

  id_teapot = glGenLists(1);
  glNewList(id_teapot, GL_COMPILE);
    glPushMatrix(); /* 'ouverture' de la pile de transfo. OpenGl */
      glTranslatef(0.,0.,1.);    /* une translation */
      glRotatef(angle,0.,0.,1.); /* une rotation autour de l'axe 'z' */
      glRotatef(90,1.,0.,0.);    /* une rotation autour de l'axe 'x' */
      glDisable(GL_CULL_FACE);
      glutSolidTeapot(.5);       /* la teapot OpenGl                 */
      glEnable(GL_CULL_FACE);
    glPopMatrix();
   glEndList();

  id_torus = glGenLists(1);
  glNewList(id_torus, GL_COMPILE);
    glPushMatrix();
      glTranslatef(0.,0.,-1.);
      glRotatef(45.,1.,0.,0.);
      glScalef(0.5,0.5,0.5);     /* une homothétie                   */
      g3x_Material(jaune,ambi,diff,spec,shin,1.);
      glutSolidTorus(.2,1.,20,40); /* un tore (cf. manuel OpenGl) */
    glPopMatrix();
  glEndList();

  id_cone = glGenLists(1);
  glNewList(id_cone, GL_COMPILE);
    glPushMatrix();
      glTranslatef(-2.,0.,0.);
      glScalef(0.5,0.5,0.5);
      g3x_Material(cyan,ambi,diff,spec,shin,1.);
      glutSolidCone(1.,1.,20,20); /* un cône (cf. manuel OpenGl) */
    glPopMatrix();
  glEndList();


}

/*! DES ACTIONS ASSOCIEES A DES OPTIONS DE CONTROLE */
/*! action : variation de couleur  !*/
static void action1(void)
{
  rouge.r-=0.01;  rouge.g+=0.01;
  vert.g -=0.01;  vert.b +=0.01;
  bleu.b -=0.01;  bleu.r +=0.01;
}
/*! action : variation de couleur !*/
static void action2(void)
{
  rouge.r+=0.01;  rouge.g-=0.01;
  vert.g +=0.01;  vert.b -=0.01;
  bleu.b +=0.01;  bleu.r -=0.01;
}

/*! action : affiche les parametre camera dans le terminal !*/
static void camera_info(void)
{
  G3Xcamera* cam=g3x_GetCamera();
  fprintf(stderr,"position (x:%lf,y:%lf,z:%lf)\n",cam->pos->x,cam->pos->y,cam->pos->z);
  fprintf(stderr,"cible    (x:%lf,y:%lf,z:%lf)\n",cam->tar->x,cam->tar->y,cam->tar->z);
  fprintf(stderr,"coord. spheriques (d:%lf,theta:%lf,phi:%lf)\n",cam->dist,cam->theta,cam->phi);
}


/*! FONCTION DE CONTROLE  : les boutons, scrollbars, actions .... !*/
/*! exécutée une seule fois, juste après la fonction init()       !*/
/*! types & fonctions définies dans <g2x_ctrls.h>                 !*/
static void ctrl(void)
{
  /* des boutons 'switch' associés à une variable booléene (on/off) */
  g3x_CreateSwitch("teapot",&FLAG_TEAPOT,"affiche/masque la theiere  ");
  g3x_CreateSwitch("torus ",&FLAG_TORUS ,"affiche/masque le tore     ");
  g3x_CreateSwitch("cone  ",&FLAG_CONE  ,"affiche/masque le cone     ");
  g3x_CreateSwitch("icos  ",&FLAG_ICOS  ,"affiche/masque l'icosahedre");

  /* définition des scrollbars */
  int id;
  g3x_SetScrollWidth(6);           /* pour régler la largeur (valeur par défaut 4) */
  /* des scrollbars vericaux       */
  id=g3x_CreateScrollv_d("ray",&rayon,0.1,  2.0,1,"rayon sphere     ");
  g3x_SetScrollColor(id,G3Xrb_c);  /* pour changer la "couleur" du scrollbar       */
  id=g3x_CreateScrollv_d("ang",&angle,0.0,360.0,1,"angle rotation   ");
  g3x_SetScrollColor(id,G3Xgb_c);
  id=g3x_CreateScrollv_d("alf",&alpha,0.0,  1.0,1,"transparence cube");
  g3x_SetScrollColor(id,G3Xbb_c);

  /* et des scrollbars horizontaux */
  g3x_SetScrollWidth(4);
  id=g3x_CreateScrollh_d("shin",&shin,0.0,1.0,1,"intensite lumiere brillance ");
  g3x_SetScrollColor(id,G3Xrc_c);
  id=g3x_CreateScrollh_d("spec",&spec,0.0,1.0,1,"intensite lumiere speculaire");
  g3x_SetScrollColor(id,G3Xgc_c);
  id=g3x_CreateScrollh_d("diff",&diff,0.0,1.0,1,"intensite lumiere diffuse   ");
  g3x_SetScrollColor(id,G3Xbc_c);
  id=g3x_CreateScrollh_d("ambi",&ambi,0.0,1.0,1,"intensite lumiere ambiante  ");

  /* action attachées à des touches clavier */
  g3x_SetKeyAction('g',action1,"variation de couleur (sphere/teapot)");
  g3x_SetKeyAction('G',action2,"variation de couleur (sphere/teapot)");
  g3x_SetKeyAction('c',camera_info,"pos./dir. de la camera => terminal");
}

void evts(void)
{
  /* affichage lié à un bouton 'switch' (cf void ctrl(void) */
  if (FLAG_TEAPOT)
  {
    g3x_Material(bleu,ambi,diff,spec,shin,0.3);
    glCallList(id_teapot);
  }

  if (FLAG_TORUS)
  {
    glPushMatrix();
      glRotatef(-angle,0.,0.,1.);
      glCallList(id_torus);
    glPopMatrix();
  }

  if (FLAG_CONE)
  {
    glPushMatrix();
      glRotatef(angle,0.,0.,1.);
      glCallList(id_cone);
    glPopMatrix();
  }

  if (FLAG_ICOS)
  {
    glPushMatrix();
      glTranslatef(2.,0.,0.);
      glRotatef(angle,0.,0.,1.);
      glScalef(0.5,0.5,0.5);
      g3x_Material(magenta,ambi,diff,spec,shin,1.);
      glutSolidIcosahedron(); /* un icosahèdre [20 faces] (cf. manuel OpenGl) */
    glPopMatrix();
  }
}


/*! FONCTION DE DESSIN PRINCIPALE (c'est le "moteur de rendu")
 *  LA SEULE FONCTION INDISPENSABLE
 *  En 3D, c'est là (et uniquement là) qu'on fait de l'OpenGl
 *  ATTENTION : cette fonction est appelée en duo
 *  avec la fonction d'animation <void anim(void)> dans une
 *  boucle infinie =>surtout pas d'allocation de mémoire ici
 *  Elle ne fait QUE des opérations géom. (transformation)
 *  et TOUT l'affichage : aucun calcul ici                 */
static void draw(void)
{
  glEnable(GL_BLEND); /* active le mode 'transparence' (blending) */
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    /* paramètres de couleur/matière de l'objet <g2x_colors.h> */
    /* alternative à la méthode OpenGL, peu intuitive          */
    g3x_Material(rouge,ambi,diff,spec,shin,1.);
    glutSolidSphere(rayon,40,40); /* une sphere (cf. manuel OpenGl) */
    /* un cube transparent */
    /*--------------LIMITES DE LA TRANSPARENCE Gl ---------*/
    /* -> TESTER :  DEFINIR LE CUBE TRANSP. AVANT LA SPHERE */
    /* -> TESTER :  RENDRE LA SPHERE TRANSPARENTE           */
    g3x_Material(vert,ambi,diff,spec,shin,alpha);
    glutSolidCube(1.); /* un cube (cf. manuel OpenGl) */
  glDisable(GL_BLEND); /* désactive le mode blending */

  /* les cubes animes -> cf fonction <anim> */
  int i=0;
  double a=360./MAXCOL;
  glPushMatrix();
    glRotatef(-2.*b,0.,0.,W.z);
    while (i<MAXCOL)
    {
      g3x_Material(colmap[i],ambi,diff,spec,shin,1.);
      glRotatef(a,0.,0.,1.);
      glPushMatrix();
        glTranslatef(1.,0.,0.);
        glRotatef(i*a,1.,0.,0.);
        glRotatef(20.*b,W.x,W.y,W.z);
        glutSolidCube(1.25*PI/MAXCOL);
      glPopMatrix();
      i++;
    }
  glPopMatrix();
}

/*! FONCTION D'ANIMATION (c'est le "moteur de jeu")
 *  ATTENTION : cette fonction est appelée en duo
 *  avec la fonction de desssin <void anim(void)> dans une
 *  boucle infinie =>surtout pas d'allocation de mémoire ici
 *  Elle ne fait QUE du calcul : aucun affichage ici
 *  Elle est bien sûr facultative, si rien ne bouge !*/
void anim(void)
{
  static double pas=0.01; /* vitesse de rotation */
  b+=pas;
  W.x+=pas;
  W.y-=pas;
  W.z+=pas;
  if (W.x<-60. || W.x>+60.) pas=-pas;
}

/*!    ACTION A EXECUTER EN SORTIE   !*/
/*! libération de mémoire, nettoyage !*/
/*! -> utilise la pile de <atexit()> !*/
static void quit(void)
{
  /* rien à faire ici puisqu'il n'y a pas d'allocation dynamique */
  fprintf(stdout,"\n rien a faire, bye !\n");
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
  g3x_SetCtrlFunction(ctrl); /* fonction de contrôle      */
  g3x_SetEvtsFunction(evts); /* fonction d'événements     */
  g3x_SetDrawFunction(draw); /* fonction de dessin        */
  g3x_SetAnimFunction(anim); /* fonction d'animation      */
  g3x_SetExitFunction(quit); /* fonction de sortie        */

  /* 3°) lancement de la boucle principale */
  return g3x_MainStart();
  /* RIEN APRES CA */
}
