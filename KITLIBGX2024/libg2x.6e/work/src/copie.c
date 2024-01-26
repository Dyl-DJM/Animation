
#include <g2x.h>


//static double wxmin,wymin,wxmax,wymax,xyratio;
/* des numéros de boutons exclusif - ça DOIT être des constantes */
#define _SRC_ 0
#define _CPY_ 1

static G2Xpixmap *pixmap=NULL,  // le pixmap d'entrée (non modifié)
                 *workmap=NULL, // le pixmap de travail (copie)
                 *curmap=NULL;  // le pixamp courant (simple pointeur)
static G2Xpoint *clic=NULL;

/*!-------------------------------------------------------------------------!*/
/*!                                                                         !*/
/*!-------------------------------------------------------------------------!*/
static void init(void)
{
  //g2x_PixmapInfo(pixmap);
  /* crée un copie de travail de l'image courante */
  g2x_PixmapCpy(&workmap,pixmap);
  curmap = pixmap;
  g2x_SetFontAttributes('n','b','l');
}

/*!-------------------------------------------------------------------------!*/
/*! une fonctionalité (mise en négatif) associée à un bouton <popup>        !*/
/*! => en entrée : le pixmap courant, en sortie : toujours <workmap>        !*/
/*!-------------------------------------------------------------------------!*/
void negate(void)
{
  uchar *c,*w;
  for ((c=curmap->map, w=workmap->map); c<curmap->end-curmap->layer; (c++,w++) )
    *w=255-*c;
  curmap = workmap;
}

/*!-------------------------------------------------------------------------!*/
/*! une fonctionalité (différentiel vertical 'emboss') associée à un <popup>!*/
/*! => en entrée : le pixmap courant, en sortie : toujours <workmap>        !*/
/*!-------------------------------------------------------------------------!*/
void emboss(void)
{
  uchar *c,*w;
  for ((c=curmap->map, w=workmap->map); c<curmap->end-curmap->layer; (c++,w++) )
    *w=abs(*c-*(c+curmap->layer));
  curmap = workmap;
}

void select_pixel(void)
{
  int c,l;
  g2x_PointToPix(clic,curmap,&c,&l); // correspondance clic->pixel

  uchar *p=g2x_GetPixel(curmap,0,c,l); // ptr vers le pixel sélectionné
  if (p) g2x_StaticTextBox(20,20,10,G2Xk,G2Xw,2,"pixel (%d,%d) : R:%3d G:%3d B:%3d",c,l,*p,*(p+1),*(p+2));
  G2Xpoint P=g2x_PixToPoint(curmap,c,l);
  fprintf(stderr,"(%lf|%lf)\n",P.x,P.y);
  g2x_DrawCircle(P,10,G2Xr,1);
}

/*!-------------------------------------------------------------------------!*/
/*!                                                                         !*/
/*!-------------------------------------------------------------------------!*/
static void ctrl(void)
{
  // des boutons selecteurs exclusifs => a utiliser avec un switch/case (cf. <g2x_ctrl.h>)
  g2x_CreateButton("SRC","pixmap original  "); // -> _SRC_ : sélectionne <pixmap>
  g2x_CreateButton("CPY","pixmap de travail"); // -> _CPY_ : sélectionne <workmap>

  // des boutons 'popup' : exécutent les fct. passées en paramètre (cf. <g2x_ctrl.h>)
  g2x_CreatePopUp("NEG",negate,"negatif");     // mise en négatif de <curmap> => <workmap>
  g2x_CreatePopUp("EMB",emboss,"emboss");      // differentiel 'emboss' sur <curmap>  => <workmap>
}

/*!-------------------------------------------------------------------------!*/
/*!                                                                         !*/
/*!-------------------------------------------------------------------------!*/
void evts(void)
{
/* les boutons selecteurs exclusifs : bascule entre <pixmap> et <curmap> */
  switch (g2x_GetButton())
  {
    case _SRC_ : curmap=pixmap ; break;
    case _CPY_ : curmap=workmap; break;
  }
}


/*!-------------------------------------------------------------------------!*/
/*!                                                                         !*/
/*!-------------------------------------------------------------------------!*/
static void draw(void)
{
  g2x_PixmapShow(curmap,true);
}


/*===============================================*/
int main(int argc, char* argv[])
{
  if (argc<2)
  {
    fprintf(stderr,"Usage : %s <path_to_image>\n",argv[0]);
    return 1;
  }
  // Pour ce genre d'application, on commence par charger l'image
  // => permet de connaître les dimensions pour ajuster les fenêtres
  if (!g2x_AnyToPixmap(&pixmap,argv[1])) { fprintf(stderr,"\e[43m<%s>\e[0m Erreur d'ouverture image <%s>\n",argv[0],argv[1]); return 1; }

  // création de la fenêtre graphique aux dimensions du pixmap
  // - nom de la fenêtre : nom de l'image (sans le path)
  // - si l'image est trop grande, la fenêtre est réduite aux dimensions de l'écran
  //   mais avec un zoom=1 par défaut (il faut 'dézoomer' pour voir l'image entière

	char *sep=strrchr(argv[1],'/');
	if (sep) argv[1]=sep+1; // suppression du 'path' => il ne reste que le nom de l'image

  g2x_InitWindow(argv[1],pixmap->width,pixmap->height);

  // pour centrer l'image sur la fenêtre réelle
  // sans ça, l'image est calée sur le coin sup. gauche (pixel 0|0 )
  g2x_SetWindowCoord(-pixmap->width/2,-pixmap->height/2,pixmap->width/2,pixmap->height/2);

  // les handlers
  g2x_SetInitFunction(init);
  g2x_SetCtrlFunction(ctrl);
  g2x_SetEvtsFunction(evts);
  g2x_SetDrawFunction(draw);
  g2x_SetAnimFunction(NULL);
  // les pixmaps alloués sont libérés automatiquement - rien de plus à faire
  g2x_SetExitFunction(NULL);

  // lancement
  return g2x_MainStart();
}
