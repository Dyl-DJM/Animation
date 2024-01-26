/**@file    g2x_pnm.h
 * @author  Universite Gustave Eiffel
 * @author  E.Incerti - eric.incerti@univ-eiffel.fr
 * @brief   Base public control functions
 * @version 6.e
 * @date    Aug.2022 (doc generation)
 */
/**@cond SHOW_SECURITY_DEFINE */

#ifdef __cplusplus
  extern "C" {
#endif

#ifndef _G2X_PIXMAP_
  #define _G2X_PIXMAP_
/**@endcond                   */

  #include <g2x_types.h>

  /* Le type "pixmap" se base sur le format "brut" PNM (Netpnm Project)
   * [cf. https://fr.wikipedia.org/wiki/Portable_pixmap
   *      https://netpbm.sourceforge.net/doc/pnm.html
   *      http://www.jchr.be/python/pnm-pam.htm         ]
   * ATTENTION : l'utilisation de ce format nécessite l'installation de la lib. <netpnm>
   * [apt-get install netpnm]
   * une image de format autre (bmp,gif,jpeg,png,tiff) est d'abord convertie
   * au format PNM adéquat (PBM : binaire 0/1 | PGM : niveau de gris (greymap) | PPM :couleurs RGB)
   * puis chargée dans un G2Xpixmap
   * */
  typedef struct
  {
    int     id;           /* un identificateur (texture) */
    char    mode;         /* P[1-6]                      */
    int     width,height; /* largeur, hauteur            */
    int     layer,depth;  /* nbre de plans, profondeur   */
    uchar  *map,*end;     /* debut et fin du pixmap      */
  } G2Xpixmap;

  /* avec _G2X_NO_RELOAD_ : lorsqu'une image est chargee, son nom est stocke dans un  *
   * tableau, avec le pointeur de pixmap correspondant. Si le meme nom est rappele    *
   * l'image n'est pas rechargee : on redirige vers le pixmap corrspondant            *
   * C'est utile surtout en 3D pour les images de texture qui sont utilisees          *
   * plusieurs fois. --- RISQUE DE BUG A LA LIBERATION ????????                       */
  #define _G2X_RELOAD_    true
  #define _G2X_NO_RELOAD_ false

  /* Libere *pix et le met a NULL */
  bool g2x_PixmapFree(G2Xpixmap** img);

  /* Si *pix=NULL, on alloue, sinon c'est qu'il existe deja, donc on ne fait rien   *
   * Le principe est donc de ne declarer que des G2Xpixmap* initialises a NULL      *
   * paramètres :                                                                   *
   *  - *pix : le pixmap a creer                                                    *
   *  - width/height : dimensions en pixels                                         *
   *  - layer : nombre de plans (1 ou 3)                                            *
   *  - depth : niveau max (1 ou 255)                                               */
  bool g2x_PixmapAlloc(G2Xpixmap** img, int width, int height, int layer, int depth);

  /* crée une copie (dst) de la source (src) */
  bool g2x_PixmapCpy(G2Xpixmap** dst, G2Xpixmap* src);

  /* Charge (alloue, si necessaire) *pix a partir d'un fichier image au format PNM                      *
   * ATTENTION : quel que soit le type du fichier original (pbm,pgm,ppm), on creera un pixmap 24bits    *
   * celà permet, dans le cas d'image binaires ou en niveaux de gris, de "colorer" les pixels à volonté *
   * ---------------------------------------------------------------------------------------------------*
   * ATTENTION : pour des raisons de compatibilité avec OpenGl (passage par une 'texture') les tailles  *
   * du pixmap doivent être des multiples de 8.                                                         *
   * Si nécessaire, l'image d'entrée est retaillée (bords droit/inférieur)                              */
  bool g2x_PnmLoad(G2Xpixmap** img, char *filename);

  /* <DEPRECATED> => utiliser plutôt <g2x_AnyToPixmap>
   * Idem, mais a partir d'un format d'image quelconque             *
   * L'image est transformee en fichier PNM (pnmlib), puis chargee  *
   * le fichier PNM intermediaire est detruit immediatement         */
  bool g2x_ImageLoad(G2Xpixmap** img, char *filename, bool RELOAD);

  /* Charge une image dans un format quelconque (BMP/GIF/JPEG/PNG/PNM/TIFF) *
   * L'image est d'abord convertie au format PNM (dans /tmp) puis chargée   *
   * via la fonction <g2x_PnmLoad>                                          */
  bool g2x_AnyToPixmap(G2Xpixmap** img, char* filename);

  /* Affiche (stderr) quelques infos sur le pixmap */
  void g2x_PixmapInfo(G2Xpixmap *img);

  /* Affichage du pixmap.                                           *
   * le flag <PIXGRID> permet d'afficher les contours des pixels    *
   * lorsque le facteur de zoom est suffisant (8x)                  */
  void g2x_PixmapShow(G2Xpixmap *img, bool PIXGRID);

  /* ces 2 fonctions permettent d'être un peu plus efficace          *
   * en pré-chargeant l'image (<g2x_PixmapPreload>) dans une texture *
   * puis en l'affichant (<g2x_PixmapRecall>) par simple rappel      *
   * => Le pixmpap NE peut PAS être modifié dynamiquement            */
  void g2x_PixmapPreload(G2Xpixmap *img);
  void g2x_PixmapRecall(G2Xpixmap *img, bool PIXGRID);

  /* renvoie la valeur du "plan" du pixel ("line",""col")  / NULL si les coordonnées sont hors zone */
  uchar* g2x_GetPixel(G2Xpixmap* img, int plan, int row, int line);

  /* renvoie la position réelle (coordonnées fenêtre) du centre du pixel img[line][row]  */
  G2Xpoint g2x_PixToPoint(G2Xpixmap *img, int row, int line);

  /* renvoie un pointeur sur le pixel situé en position réelle <pos>
   * les coord. image (row|ŀine) du pixel remontent en passage par adresse */
  uchar*   g2x_PointToPix(G2Xpoint *pos, G2Xpixmap *img, int *row, int *line);

  /* Ecrit les donnees pix dans un fichier PNM au format souhaite (pbm,pgm,ppm) */
  bool g2x_PnmWrite(G2Xpixmap*  img, char *filename, char mode, char* comment);

  /* Enregistrement image dans un format donné ("bmp"/"gif"/"jpeg"/"png"/"tiff"/"pnm")
   * fichier de sortie : pathname/basename.format
   * ATTENTION : dans tous les cas, le pixmap est d'abord converti en PPM 24bits
   *   - BMP  : non compressé
   *   - GIF  : couleurs indexées (256) - standard GIF87a
   *   - JPEG : baseline (non progressif) - qualité 90% - DCT entière
   *   - PNG  : compression niveau 9 (max)
   *   - TIFF : compression <packbits> - format à éviter de manière générale
   *   - PNM  : par défaut, c'est du PPM 24bits                               */
  bool g2x_PixmapToAny(G2Xpixmap* img, char* pathname, char* basename, char* format, char *comment);

#endif

#ifdef __cplusplus
  }
#endif
