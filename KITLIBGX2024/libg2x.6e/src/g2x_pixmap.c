/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G2X - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ================================================================!*/

#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#include <g2x.h> /* le type G2Xpixmap est dans <g2x_pixmap.h> */


#define _MAX_IMG_NAME_ 32
#define _MAX_NAME_LEN_ 255

typedef struct
{
  G2Xpixmap* img;
  char       name[_MAX_NAME_LEN_+1];
} G2Xpixmapnames;

static int nbimages=0;
static G2Xpixmapnames img_names[_MAX_IMG_NAME_];

/*! ******************************* CREATION ****************************** !*/

/* Lecture image -- doit être au format PNM [cf. https://fr.wikipedia.org/wiki/Portable_pixmap] */
static size_t g2x_PnmPullRAW(G2Xpixmap *img, FILE* pnm_file)
{
  uchar *map=img->map, *end=img->end;
  uchar *p,*q;
  int    i;
  size_t n=img->layer*img->height*img->width;

  switch (img->mode) /* bpm/pgm/ppm ASCII/RAW */
  { /* format ASCII */
    case '1' : /* PBM */
    case '2' : /* PGM */
    case '3' : /* PPM */ for (p=map; !feof(pnm_file) && p<end ;++p) n+=fscanf(pnm_file,"%hhu",p); break;
    /* format RAW */
    case '4' : /* PBM */ n/=8;
    case '5' : /* PGM */
    case '6' : /* PPM */ n=fread(map,sizeof(uchar),n,pnm_file); break;
  }

  if (img->mode=='4') /* cas BINAIRE  : 1 bit -> 8 bits */
  {
    fprintf(stderr,"\e[0;33mFormat PBM : conversion 1 bit -> 8 bits (%d octets lus)->%d pixels\e[0m\n",(int)n,(int)(8*n));
    p=(uchar*)img->map+n-1;
    q=(uchar*)img->end-1;
    do
    {
      for (i=0;i<8;i++) {  q--; *q=(*p&0x1)?0xFF:0x00; *p>>=1;}
      p--;
    }
    while (p!=img->map);
    n*=8;
    return n;
  }
  return n;
}

extern bool g2x_PixmapFree(G2Xpixmap** img)
{
  if (*img==NULL) return false;

  if (nbimages)
  {
    int i=0;
    while (i<nbimages && *img!=img_names[i].img) i++;
    if (i<nbimages)
    {
      if (*img_names[i].name==0)  return true;
     *img_names[i].name=0;
    }
  }
  if ((*img)->map) free((*img)->map);
  free(*img);
  *img=NULL;
  return true;
}

extern bool g2x_PixmapAlloc(G2Xpixmap **img, int width, int height, int layer, int depth)
{
  if ((*img)!=NULL                               &&
      (*img)->map!=NULL                          &&
      (*img)->height*(*img)->width>=height*width &&
      (*img)->layer>=layer                       &&
      (*img)->depth>=depth                        )
  {
    (*img)->height = height;
    (*img)->width  = width;
    (*img)->layer  = layer;
    (*img)->depth  = depth;
    (*img)->end=(uchar*)(*img)->map + (layer*height*width);
    return true;
  }
  if (*img!=NULL) g2x_PixmapFree(img);

  if (!((*img)=(G2Xpixmap*)calloc(1,sizeof(G2Xpixmap))))
  { fprintf(stderr,"\e[1;31m  <g2x_PixmapAlloc> : erreur allocation [%lu oct.]\e[0m\n",sizeof(G2Xpixmap)); return false;}
  (*img)->height = height;
  (*img)->width  = width;
  (*img)->layer  = layer;
  (*img)->depth  = depth;
  if (!((*img)->map = (uchar*)calloc(layer*width*height,sizeof(uchar))))
  {
    free((void**)img);
    fprintf(stderr,"\e[1;31m  <g2x_PixmapAlloc> : erreur allocation [%lu oct.]\e[0m\n",(ulint)(layer*width*height));
    return false;
  }
  (*img)->end=(uchar*)(*img)->map+(layer*height*width);
  return true;
}

extern  void g2x_PixmapInfo(G2Xpixmap *img)
{
  fprintf(stderr,"mode P%c\n",img->mode);
  switch (img->mode)
  {
    case '1' :   fprintf(stderr,"\t<PBM> BitMap  (1 layer) |   1 bit /pix | ASCII data\n"); break;
    case '2' :   fprintf(stderr,"\t<PGM> GrayMap (1 layer) |   8 bits/pix | ASCII data\n"); break;
    case '3' :   fprintf(stderr,"\t<PPM> PixMap  (3 layers) | 3*8 bits/pix | ASCII data\n"); break;
    case '4' :   fprintf(stderr,"\t<PBM> BitMap  (1 layer) |   1 bit /pix | RAW data\n"); break;
    case '5' :   fprintf(stderr,"\t<PGM> GrayMap (1 layer) |   8 bits/pix | RAW data\n"); break;
    case '6' :   fprintf(stderr,"\t<PPM> PixMap  (3 layers) | 3*8 bits/pix | RAW data\n"); break;
    case '7' :   fprintf(stderr,"\t<PAM> AnyMap  (4 layers) | 4*8 bits/pix | RAW data\n"); break;
  }
  fprintf(stderr,"\tsize %d rows | %d lines\n",img->width,img->height);
  fprintf(stderr,"\tdyn. %d\n",img->depth);
  fprintf(stderr,"\tfirst 8x8 block :\n\t");
  for (int l=0; l<8; l++)
  {
    for (int c=0; c<8; c++)
    {
      for (int k=0; k<img->layer; k++)
        fprintf(stderr,"%3d ",img->map[(l*img->width+c)*img->layer+k]);
      fprintf(stderr,"| ");
    }

    fprintf(stderr,"\n\t");
  }
  fprintf(stderr,"\n--------------------\n");
}

/* crée une copie (dst) de la source (src) */
extern  bool g2x_PixmapCpy(G2Xpixmap** dst, G2Xpixmap* src)
{
  if (!g2x_PixmapAlloc(dst,src->width,src->height,src->layer,src->depth)) return false;
  (*dst)->mode = src->mode;
  memcpy((*dst)->map,src->map,src->layer*src->width*src->height*sizeof(uchar));
  return true;
}

/*! ******************************* LECTURE ****************************** !*/
extern bool g2x_PnmLoad(G2Xpixmap** img, char *filename)
{
  int     height,width,depth=1,layer;
  char    mode[3],c,*r,comment[256];
  FILE*   pnm_file;
  size_t  leng;

  if (!(pnm_file=fopen(filename,"r"))) { fprintf(stderr,"<g2x_PnmLoad> erreur ouverture <%s>\n",filename); return false; }

  /*------- le mode, les plans ---------*/
  fscanf(pnm_file,"%s\n",mode);
  if (mode[0]!='P') return false;
  switch (mode[1])
  {
    case '1' : case '2' :
    case '4' : case '5' : layer=1; break; // PBM & PGM
    case '3' : case '6' : layer=3; break; // PPM
    case '7' :            layer=4; break; // PAM
    default  : return false;
  }

  /*------- les commentaires ----------*/
  while ((c=getc(pnm_file))=='#')
  { r=fgets(comment,256,pnm_file);
    fprintf(stderr,"# %s\n",comment);
  }
  ungetc(c,pnm_file);
  /*--------- les tailles -------------*/
  fscanf(pnm_file,"\n%d %d",&width,&height);
  fprintf(stderr,"mode [%s] size [%dx%d]\n",mode,width,height);
  /*-------- la profondeur ------------*/
  if (mode[1]!='1' && mode[1]!='4') { fscanf(pnm_file,"%d\n",&depth); depth++; }

  /* 2023/09/09 : retaillage pour alignement des tailles sur multiple de 8                 *
   * => Cette opération est nécessaire pour compatibilité avec gestion des textures OpenGl */
  if (width%8!=0 || height%8!=0)
  {
    fclose(pnm_file);
    width  -= width %8;
    height -= height%8;
    fprintf(stderr,"\e[46;1;5mWARNING\e[0;0;1m : image must be resized to fit [WxH] modulo 8 => [%dx%d]\n\e[0m",width,height);
    char command[512]="";
    sprintf(command,"pnmcut -width=%d -height=%d %s > /tmp/_g2xtmpfile1_",width,height,filename);
    system(command);
    return g2x_PnmLoad(img,"/tmp/_g2xtmpfile1_");
  }

  if (!g2x_PixmapAlloc(img,width,height,layer,depth)) { fclose(pnm_file); return false; }

  (*img)->mode=mode[1];

  leng=g2x_PnmPullRAW(*img,pnm_file);
  if (leng<layer*height*width)
    fprintf(stderr,"<g2x_PnmLoad> donnees tronquees -- %d octets lus sur %d prevus\n",(int)leng,(int)(layer*height*width));

  fclose(pnm_file);
  return true;
}

extern bool g2x_ImageLoad(G2Xpixmap** img, char *filename, bool RELOAD)
{
  static char  command[256];
  bool   ok=true;

  if (!RELOAD)
  {
    int i=0;
    while (i<nbimages && strcmp(filename,img_names[i].name)) i++;
    if (i<nbimages)  { *img=img_names[i].img; return true; }
  }
  // on teste d'abord si c'est du format PNM
  if (!g2x_PnmLoad(img,filename))
  { // si c'est pas le cas, on converti en PNM (temporaire)
    sprintf(command,"anytopnm %s > /tmp/_g2xtmpfile0_",filename);
    if ((system(command)))
    {
      fprintf(stderr,"\e[1;31m<g2x_ImageLoad> : le fichier <%s> n'existe pas ou n'est pas lisible\e[0m\n",filename);
      return false;
    }
    // on charge le PNM temporaire
    ok=g2x_PnmLoad(img,"/tmp/_g2xtmpfile0_");
  }
  char *sep=strrchr(filename,'/');
  sep = (sep ? sep+1 : filename);
  fprintf(stderr,"\e[0;33m<g2x_ImageLoad> : Chargement image \e[0;35m%s\e[0;33m (recyclage:[\e[0;45m%s\e[0;33m])\e[0m\n",sep,RELOAD?"non":"oui");

  if (!RELOAD)
  {
    if (nbimages==_MAX_IMG_NAME_)
      fprintf(stderr,"\e[1;31m<g2x_ImageLoad> : impossible de stocker le nom de l'image <%s> - tableau plein\e[0m\n",filename);
    strncpy(img_names[nbimages].name,filename,_MAX_NAME_LEN_);
    img_names[nbimages].img=*img;
    nbimages++;
  }
  return ok;
}


/*! 2023/09/10 - pour palier quelques 'bugs' de la commande <anytopnm> (format TIFF)  *
 *  - 1°) utilise la commande systeme <file> pour identifier le type MIME de l'image  *
 *        (MIME : 'Multipurpose Internet Mail Extensions')                            *
 *  - 2°) cherche le type dans une liste predefinie de formats d'image classique      *
 *  - 3°) utilise le convertisseur <***topnm> adapté, plutôt que <anytopnm>          !*/
extern bool g2x_AnyToPixmap(G2Xpixmap** img, char* filename)
{
  static char MIMET[8][32]={"bmp","gif","jpeg","png","tiff","x-portable-bitmap","x-portable-greymap","x-portable-pixmap"};
  char command[1024]="";
  char buffer[2048]="",*bptr1=NULL,*bptr2=NULL;

  // appel à la commande <file>
  sprintf(command,"file %s -b --mime-type 1> /tmp/G2XMIME",filename);
  system(command);
  FILE *filetype=NULL;

  // extraction du type MIME
  if (!(filetype=fopen("/tmp/G2XMIME","r"))) { fprintf(stderr,"<g2x_AnyToPixmap> Erreur ouverture /tmp/G2XMIME\n"); return false; }
  if (!fgets(buffer,2047,filetype))          { fprintf(stderr,"<g2x_AnyToPixmap> Erreur lecture /tmp/G2XMIME  \n"); return false; }
  if (!(bptr1=strchr(buffer,'/')))           { fprintf(stderr,"<g2x_AnyToPixmap> Erreur lecture type MIME (1) \n"); return false; }
  if (!(bptr2=strchr(bptr1,'\n')))           { fprintf(stderr,"<g2x_AnyToPixmap> Erreur lecture type MIME (2) \n"); return false; }

   bptr1++;
  *bptr2=0;
  // identification du format d'image
  int i;
  for (i=0;i<8;i++)
    if (0==strcmp(bptr1,MIMET[i])) break;

  // formats BMP/GIF/JPEG/PNG/TIFF => conversion en PNM (dans /tmp), puis chargement
  if (i<5)
  {
    sprintf(command,"%stopnm %s > /tmp/_g2xtmpfile0_",MIMET[i],filename);
    system(command);
    return g2x_PnmLoad(img,"/tmp/_g2xtmpfile0_");
  }
  // formats PBM/PGM/PPM : chargement direct
  if (i<8) return g2x_PnmLoad(img,filename);

  fprintf(stderr,"<g2x_AnyToPixmap> format <%s> non pris en charge\n",bptr1);
  return false;
}


/* Chargement du pixamp dans une texture (2022/05)
 * Par défaut, la texture est un rectangle à la taille
 * de l'image qui sera 'resizé' à l'affichage           */
extern void g2x_PixmapPreload(G2Xpixmap *pnm)
{
  pnm->id = glGenLists(1);
  glNewList(pnm->id, GL_COMPILE);
    uint tex_id;
    glGenTextures(1,&tex_id);
    glBindTexture(GL_TEXTURE_2D,tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    switch (pnm->layer)
    {
      case 1 : glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, pnm->width, pnm->height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,pnm->map); break;
      case 3 : glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8     , pnm->width, pnm->height, 0, GL_RGB      , GL_UNSIGNED_BYTE,pnm->map); break;
      default : fprintf(stderr,"\e[1;31m<g2x_PixmapPreload> : format d'image [%d] non reconnu\e[0m\n",pnm->layer); return;
    }
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glColor4f(1.,1.,1.,0.);
    glBegin(GL_QUADS);
      glTexCoord2d(0,1); glVertex2d(-0.5*pnm->width,-0.5*pnm->height);
      glTexCoord2d(1,1); glVertex2d(+0.5*pnm->width,-0.5*pnm->height);
      glTexCoord2d(1,0); glVertex2d(+0.5*pnm->width,+0.5*pnm->height);
      glTexCoord2d(0,0); glVertex2d(-0.5*pnm->width,+0.5*pnm->height);;
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDeleteTextures(1, &tex_id);
  glEndList();
}

/* simple appel à l'objet Gl précompilé (bcp + économique) (2022/05)
 * on lui applique les conditions de zoom_value via les transfos OpenGl
 * pour recaler la texture (rectangle réel) à la taille voulue  */
extern void g2x_PixmapRecall(G2Xpixmap *pnm, bool PIX_GRID)
{
  double dx = g2x_GetZoom()*g2x_GetXPixSize(); // facteur d'échelle en x
  double dy = g2x_GetZoom()*g2x_GetYPixSize(); // facteur d'échelle en y
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glPushMatrix();
    glScalef(dx,dy,1.);
    glCallList(pnm->id);
    if (PIX_GRID && g2x_GetZoom()>=8)
    { // tracé du cadre du pixel si facteur de zoom > 4 : 1 pixel > carré 4x4 avec 1 pixel de bord
      int x,y;
      for (y=-pnm->height/2;y<pnm->height/2;y++) g2x_Line(-pnm->width/2,y,+pnm->width/2,y,G2Xwa_a,1);
      for (x=-pnm->width/2 ;x<pnm->width/2 ;x++) g2x_Line(x,-pnm->height/2,x,pnm->height/2,G2Xwa_a,1);
    }
  glPopMatrix();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

/* affichage d'une image non bufferisé => moins performant,
 * mais adapté à des traitements dynamiques. (2022/10) */
extern void g2x_PixmapShow(G2Xpixmap *pnm, bool PIX_GRID)
{
  uint tex_id;
  double dx = g2x_GetZoom()*g2x_GetXPixSize(); // facteur d'échelle en x
  double dy = g2x_GetZoom()*g2x_GetYPixSize(); // facteur d'échelle en y
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glPushMatrix();
    glScalef(dx,dy,1.);
    glGenTextures(1,&tex_id);
    glBindTexture(GL_TEXTURE_2D,tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    switch (pnm->layer)
    {
      case 1  : glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, pnm->width, pnm->height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,pnm->map); break;
      case 3  : glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8     , pnm->width, pnm->height, 0, GL_RGB      , GL_UNSIGNED_BYTE,pnm->map); break;
      default : fprintf(stderr,"\e[1;31m<g2x_PixmapPreload> : format d'image [%d] non reconnu\e[0m\n",pnm->layer); return;
    }
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glColor4f(1.,1.,1.,0.);
    glBegin(GL_QUADS);
      glTexCoord2d(0,1); glVertex2d(-0.5*pnm->width,-0.5*pnm->height);
      glTexCoord2d(1,1); glVertex2d(+0.5*pnm->width,-0.5*pnm->height);
      glTexCoord2d(1,0); glVertex2d(+0.5*pnm->width,+0.5*pnm->height);
      glTexCoord2d(0,0); glVertex2d(-0.5*pnm->width,+0.5*pnm->height);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glDisable(GL_TEXTURE_2D);

    if (PIX_GRID && g2x_GetZoom()>=8)
    { // trace du cadre d'un bloc de pixels de taille <step*step> si facteur de zoom > cutzoom
      int step=1;
      glLineWidth(1.);
      G2Xcolor4fv(G2Xwb_b);
      glBegin(GL_LINES);
        for (int y=-pnm->height/2 ;y<pnm->height/2 ;y+=step)
        {
          glVertex2d(-pnm->width/2,y);
          glVertex2d(+pnm->width/2,y);
        }
        for (int x=-pnm->width /2 ;x<pnm->width /2 ;x+=step)
        {
          glVertex2d(x,-pnm->height/2);
          glVertex2d(x,+pnm->height/2);
        }
      glEnd();
    }
  glPopMatrix();
}

/* [OBSOLETE ET/OU PAS ADAPTE]
 * affichage d'un image non bufferisé => moins performant,
 * mais adapté à des traitements dynamiques. (2022/10)  */
extern void g2x_ShowPixLayer(G2Xpixmap *pnm, uchar MODE, uchar LAYERS)
{
  if (pnm->layer!=3)
  {
    fprintf(stderr,"<g2x_ShowPixLayer> image mono-plan => fonction non adaptée\n");
    return;
  }
  uint tex_id;
  double dx = g2x_GetZoom()*g2x_GetXPixSize(); // facteur d'échelle en x
  double dy = g2x_GetZoom()*g2x_GetYPixSize(); // facteur d'échelle en y
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glPushMatrix();
  glScalef(dx,dy,1.);
  glGenTextures(1,&tex_id);
  glBindTexture(GL_TEXTURE_2D,tex_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8     , pnm->width, pnm->height, 0, GL_RGB      , GL_UNSIGNED_BYTE,pnm->map);

  glEnable(GL_TEXTURE_2D);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glColor4f(1.,1.,1.,0.);
  glBegin(GL_QUADS);
    glTexCoord2d(0,1); glVertex2d(-0.5*pnm->width,-0.5*pnm->height);
    glTexCoord2d(1,1); glVertex2d(+0.5*pnm->width,-0.5*pnm->height);
    glTexCoord2d(1,0); glVertex2d(+0.5*pnm->width,+0.5*pnm->height);
    glTexCoord2d(0,0); glVertex2d(-0.5*pnm->width,+0.5*pnm->height);;
  glEnd();
  if (g2x_GetZoom()>=8)
  { // tracé du cadre du pixel si facteur de zoom > 4 : 1 pixel > carré 4x4 avec 1 pixel de bord
    int x,y;
    for (y=-pnm->height/2;y<pnm->height/2;y++) g2x_Line(-pnm->width/2,y,+pnm->width/2,y,G2Xwa_a,1);
    for (x=-pnm->width/2 ;x<pnm->width/2 ;x++) g2x_Line(x,-pnm->height/2,x,pnm->height/2,G2Xwa_a,1);
  }
  glPopMatrix();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glDisable(GL_TEXTURE_2D);
}

/*! ****************************** ECRITURE ****************************** !*/
/* Ecriture image au format PNM *
 * fopen, fflush et fclose effectués par fonction appelante */
static size_t g2x_PnmPushRAW(G2Xpixmap *img, FILE* pnm_file)
{
  uchar *map=img->map, *end=img->end;
  uchar *p,*q,a;
  size_t n=img->layer*img->height*img->width,i=0;

  if (img->mode=='4')
  {
    fprintf(stderr,"\e[0;33mFormat PBM : conversion 8 bits -> 1 bit\e[0m\n");
    p=map;
    i=0;
    do { fprintf(stderr,"\e[0;33m%hhu%s",*p++,(++i)%img->width?"":"\e[0m\n");} while (p!=end);
    p=map;
    q=map;
    n=0;
    do
    {
      a = 0;
      int b=8;
      while (b) a|=(*q++&1)<<(--b);
      /*
      a|=(*q++&1)<<7; a|=(*q++&1)<<6; a|=(*q++&1)<<5; a|=(*q++&1)<<4;
      a|=(*q++&1)<<3; a|=(*q++&1)<<2; a|=(*q++&1)<<1; a|=(*q++&1)<<0;
      */
      p--;
     *p++=a;
      n++;
    }
    while (q!=end);
    end=p;
  }

  switch (img->mode)
  {
    case '1' :
    case '2' :
    case '3' : for (p=map;p<end;++p) n+=fprintf(pnm_file,"%hhu ",*p); break;
    case '4' :
    case '5' :
    case '6' : n=fwrite(map,sizeof(uchar),n,pnm_file); break;
  }
  return n;
}


extern bool g2x_PnmWrite(G2Xpixmap* img, char *filename, char mode, char* comment)
{
  FILE  *pnm_file;
  time_t date=time(NULL);
  size_t leng;

  if (img==NULL)
  { fprintf(stderr,"\e[0;31m<g2x_PnmWrite> : Rien a ecrire sur %s\e[0m\n",filename); return false; }

  if (!(pnm_file=fopen(filename,"w")))
  { fprintf(stderr,"\e[1;31m<g2x_PnmWrite> : Erreur ouverture %s\e[0m\n",filename); return false; }

  fprintf(pnm_file,"P%c\n",mode);
  fprintf(pnm_file,"#--------------------------------------\n");
  fprintf(pnm_file,"# %s - %s",filename,ctime(&date));
  if (strlen(comment)!=0) fprintf(pnm_file,"# %s\n",comment);
  fprintf(pnm_file,"#--------------------------------------\n");
  fprintf(pnm_file,"%d %d\n",img->width,img->height);

  if (mode!='1' && mode!='4') fprintf(pnm_file,"%d\n",img->depth-1);
  fflush(pnm_file);

  leng=g2x_PnmPushRAW(img,pnm_file);
  if (leng<img->layer*img->height*img->width)
  { fprintf(stderr,"<g2x_PnmWrite> : donnee tronquees -- %d octets ecrits sur %d prevus\n",(int)leng,(int)(img->layer*img->height*img->width));}

  fflush(pnm_file);
  fclose(pnm_file);
  return true;
}


/* 2023/09/12 Ecriture image dans un format parmis BMP/GIF/JPEG/PNG/PNM/TIFF */
extern bool g2x_PixmapToAny(G2Xpixmap* img, char* path, char* base, char* ext, char *comment)
{
  static char MIMET[9][32]={"bmp","gif","jpeg","png","tiff","pbm","pgm","pnm","ppm"};
  char full[1024]="";
  char command[1024]="";
  bool ok=false;

  int f;
  for (f=0;f<9;f++) if (0==strcmp(ext,MIMET[f])) break;

  if (!(f<9)) { fprintf(stderr,"<g2x_PixmapToAny> format <%s> non pris en charge\n",ext); return false; }

  // vérification du path
  int l=strlen(path);
  if (l==0)
    sprintf(full,"./");
  else
  {
    char* ptr=strrchr(path,'/');
    if ((ptr-path)<l)
      sprintf(full,"%s/",path);
    else
      sprintf(full,"%s",path);
  }
  if (!opendir(full))
  { fprintf(stderr,"<g2x_PixmapToAny> erreur ouverture répertoire cible <%s>\n",full); return false; }

  // formation du nom cible complet path/base.ext
  sprintf(full,"%s%s.%s",full,base,ext);

  // dans tous les cas, on passe par un fichier PNM temporaire (toujours en données brutes).
  switch(img->mode)
  {
    case 1  : case 4 : ok=g2x_PnmWrite(img,"/tmp/_g2xtmpfile2_",'4',comment); break;
    case 2  : case 5 : ok=g2x_PnmWrite(img,"/tmp/_g2xtmpfile2_",'5',comment); break;
    default :          ok=g2x_PnmWrite(img,"/tmp/_g2xtmpfile2_",'6',comment); break;
  }
  if (!ok) { fprintf(stderr,"<g2x_PixmapToAny> erreur ecriture '/tmp/_g2xtmpfile2_'\n"); return false; }

  // conversion PNM -> format de sortie avec codecs spécifiques
  switch (f)
  {
    case 0  : sprintf(command,"ppmtobmp /tmp/_g2xtmpfile2_ 1> %s",full);                 break;//BMP
    case 1  : sprintf(command,"pnmquant 256 /tmp/_g2xtmpfile2_ | ppmtogif 1> %s",full);  break;//GIF
    case 2  : sprintf(command,"pnmtojpeg --quality=90 /tmp/_g2xtmpfile2_ 1> %s",full);   break;//JPEG
    case 3  : sprintf(command,"pnmtopng -compression 9 /tmp/_g2xtmpfile2_ 1> %s",full); break;//PNG
    case 4  : sprintf(command,"pnmtotiff /tmp/_g2xtmpfile2_ 1> %s",full);                break;//TIFF
    default : sprintf(command,"cp -f /tmp/_g2xtmpfile2_ %s",full);                       break;//PNM
  }
  system(command);
  return true;
}



/*! ------------------------------ ACCES COORD. PIXEL <=> COORD. REELLES ------------------------------ !*/

/* renvoie la valeur du "plan" du pixel ("line",""col")  / NULL si les coordonnées sont hors zone */
extern __inline__ uchar* g2x_GetPixel(G2Xpixmap* pix, int lay, int row, int line)
{
  if (pix==NULL ||
      lay <0 || lay >=pix->layer  ||
      row <0 || row >=pix->width  ||
      line<0 || line>=pix->height) return NULL;
  return (pix->map + pix->layer*(line*pix->width+row) + lay);
}


/* renvoie la position réelle (coordonnées fenêtre) du centre du pixel img[line][row]  */
extern __inline__ G2Xpoint g2x_PixToPoint(G2Xpixmap *img, int row, int line)
{
  return (G2Xpoint){ (row -img->width /2+0.5),
                    -(line-img->height/2+0.5) };
}

/* renvoie un pointeur sur le pixel situé en position réelle <pos>
 * les coord. image (row|ŀine) du pixel remontent en passage par adresse */
extern __inline__ uchar*   g2x_PointToPix(G2Xpoint *pos, G2Xpixmap *img, int *row, int *line)
{
  *row  = (int)(pos->x<0.?floor(pos->x)  :ceil(pos->x)-1)+img->width/2;
  *line =-(int)(pos->y<0.?floor(pos->y)+1:ceil(pos->y)  )+img->height/2;
  return img->map + img->layer*((*line)*img->width+(*row));
}


#ifdef __cplusplus
  }
#endif
