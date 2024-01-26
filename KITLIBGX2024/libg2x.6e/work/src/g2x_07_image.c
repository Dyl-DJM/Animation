
#include <g2x.h>

//static double wxmin,wymin,wxmax,wymax,xyratio;
/* des numéros de boutons exclusif - ça DOIT être des constantes */
#define _SRC_ 0
#define _CPY_ 1

static G2Xpixmap *pixmap =NULL,  // le pixmap d'entrée (non modifié)
                 *workmap=NULL, // le pixmap de travail (copie)
                 *curmap =NULL;  // le pixamp courant (simple pointeur)
static G2Xpoint  *mouse;

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
  // les boutons selecteurs exclusifs : bascule entre <pixmap> et <curmap>
  switch (g2x_GetButton())
  {
    case _SRC_ : curmap=pixmap ; break;
    case _CPY_ : curmap=workmap; break;
  }
}

/*!-------------------------------------------------------------------------!*/
/*! sélection au clic souris d'un pixel de l'image                          !*/
/*! - affiche la (les) valeur(s) du pixel en bas de la fenêtre              !*/
/*! - trace un carré rouge autour du pixel                                  !*/
/*!-------------------------------------------------------------------------!*/
void select_pixel(void)
{
  int c,l;
  uchar *p=g2x_PointToPix(mouse,curmap,&c,&l); // correspondance clic->pixel
  if (p) g2x_StaticPrint(20,20,G2Xk,"pixel (%d,%d) : R:%3d G:%3d B:%3d",c,l,*p,*(p+1),*(p+2));

  G2Xpoint P=g2x_PixToPoint(curmap,c,l);
  double ps=0.5*g2x_GetZoom()*g2x_GetXPixSize();
  g2x_Rectangle(P.x-ps,P.y-ps,P.x+ps,P.y+ps,G2Xr,3);
}


/*!-------------------------------------------------------------------------!*/
/*!                                                                         !*/
/*!-------------------------------------------------------------------------!*/
static void draw(void)
{
  g2x_PixmapShow(curmap,true);
  // marquage d'un pixel
  if ((mouse=g2x_GetClic())) select_pixel();
}

/*!-------------------------------------------------------------------------!*/
/*!                                                                         !*/
/*!-------------------------------------------------------------------------!*/
static void quit(void)
{
  g2x_PixmapToAny(workmap,"","test0","pnm","");
  g2x_PixmapToAny(workmap,"TESTS/","test2","bmp","");
  g2x_PixmapToAny(workmap,"../work/TESTS","test3","jpeg","");
  g2x_PixmapToAny(workmap,"TESTS/","test4","png","");
  g2x_PixmapToAny(workmap,"./TESTS","test5","gif","");
  g2x_PixmapToAny(workmap,"./TESTS/","test6","tiff","");
  // libération des 2 pixmap
  g2x_PixmapFree(&pixmap);
  g2x_PixmapFree(&workmap);
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
  // => ça permet de connaître les dimensions pour ajuster les fenêtres
  // => et en cas de pépin, on s'arrête là.
  if (!g2x_AnyToPixmap(&pixmap,argv[1]))
  { fprintf(stderr,"\e[43m<%s>\e[0m Erreur d'ouverture image <%s>\n",argv[0],argv[1]); return 1; }

  int w = pixmap->width; // largeur
  int h = pixmap->height;// hauteur

  char *sep=strrchr(argv[1],'/');
	if (sep) argv[1]=sep+1; // suppression du 'path' => il ne reste que le nom de l'image

  // création de la fenêtre graphique aux dimensions du pixmap
  // - nom de la fenêtre : nom de l'image (sans le path)
  // - si l'image est trop grande, la fenêtre est réduite aux dimensions de l'écran
  //   mais avec un zoom=1 par défaut (=> il faut 'dézoomer' pour voir l'image entière)
  g2x_InitWindow(argv[1],w,h);

  // pour centrer l'image sur la fenêtre réelle
  // sans ça, l'image est calée sur le coin sup. gauche (pixel 0|0 )
  g2x_SetWindowCoord(-w/2,-h/2,+w/2,+h/2);

  // les handlers
  g2x_SetInitFunction(init);
  g2x_SetCtrlFunction(ctrl);
  g2x_SetEvtsFunction(evts);
  g2x_SetDrawFunction(draw);
  g2x_SetAnimFunction(NULL);
  g2x_SetExitFunction(quit);

  // lancement
  return g2x_MainStart();
}
