/* ===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G3X - Fonctions de base d'acces public -
                    version 6e - 2022/2023
  ============================================================= */


/*=================================================*/
/*=           fonctions d'ecriture                =*/
/*=================================================*/
typedef struct
{
  const    uchar *str;
  double   x,y;
  G3Xcolor col;
  int      len;
} G3Xtext;

static char  _print_posit_='l';
static char  _print_style_='m';
static char  _print_size_ ='n';
static void* _print_font_ =  GLUT_BITMAP_HELVETICA_12;
static int   _v_offset_=0;
static int   _f_width_,_f_height_;

extern void g3x_SetFontAttributes(char size, char style, char pos)
{
  int xoff=0;
  if (pos) _print_posit_ = pos;
  switch (style)
  {
    case 'n' :
    case 'b' : _print_style_=style; break;
    case 'B' : xoff=1;_print_style_=style; break;
    case  0  : break;
  }
  switch (size)
  {
    case 's' : _print_font_=GLUT_BITMAP_HELVETICA_10;   _v_offset_= 3; _f_height_= 5; break;
    case 'm' : _print_font_=GLUT_BITMAP_HELVETICA_12;   _v_offset_= 4; _f_height_= 6; break;
    case 'l' : _print_font_=GLUT_BITMAP_HELVETICA_18;   _v_offset_= 6; _f_height_= 9; break;
    case 'L' : _print_font_=GLUT_BITMAP_TIMES_ROMAN_24; _v_offset_= 8; _f_height_=12; break;
    case  0  : break;
  }
}



/*====================================================================*/
#include <stdarg.h> /* pour fcts a nbre d'args. variable */

static __inline__ void g3x_StaticTextPrint(G3Xtext T)
{
  switch (_print_posit_)
  {
    case 'c' : T.x-=0.5*T.len; break;
    case 'r' : T.x-=(T.len+4); break;
    default  :                 break;
  }
  T.y-=_v_offset_;

  g3x_Color4fv(T.col);
  glRasterPos2d(T.x,T.y);
  glutBitmapString(_print_font_,T.str);

  if (_print_style_!='b' && _print_style_!='B') return;
  glRasterPos2d(T.x+1,T.y);
  glutBitmapString(_print_font_,T.str);

  if (_print_style_!='B') return;
  glRasterPos2d(T.x+2,T.y);
  glutBitmapString(_print_font_,T.str);
}


/*====================================================================*/
extern void g3x_StaticPrint(int x, int y, G3Xcolor col, const char *format, ...)
{
  va_list ap;
  char    msg[1024];
  const uchar *str=(uchar*)msg;
  va_start(ap, format);
  vsprintf(msg, format, ap);
  va_end(ap);
  G3Xtext TEXT=(G3Xtext){str,x,y,col,glutBitmapLength(_print_font_,str)};
  g3x_StaticTextPrint(TEXT);
}

/*!
 * affichage texte dans case, plus ou moins pré-formaté
 !*/
extern void g3x_StaticTextBox(int x, int y, int margin, G3Xcolor txtcol, G3Xcolor bkgcol, int borderw, const char *format, ...)
{
  va_list ap;
  char    msg[1024];
  const uchar *str=(uchar*)msg;
  va_start(ap, format);
  vsprintf(msg, format, ap);
  va_end(ap);

  G3Xtext TEXT=(G3Xtext){str,x,y,txtcol,glutBitmapLength(_print_font_,str)};

  /*
  x-=margin;
  double xsize = TEXT.len+2*margin;
  double ysize = (1      +  margin)*_v_offset_;
  double dxsize = xsize*Xpixsize;
  double dysize = ysize*Ypixsize;

  double xoffset;
  switch (_print_posit_)
  {
    case 'l' : xoffset= 0.;         break;
    case 'c' : xoffset= dxsize*0.5; break;
    case 'r' : xoffset= dxsize;     break;
  }

  double dx=Xwmin+x*Xpixsize-xoffset;
  double dy=Ywmin+y*Ypixsize-dysize/2.;
  g3x_FillRectangle(dx,dy,dx+dxsize,dy+dysize,bkgcol);
  g3x_Rectangle    (dx,dy,dx+dxsize,dy+dysize,txtcol,borderw);
  */
  g3x_StaticTextPrint(TEXT);
}






/*====================================================================*/
/* variante ancienne version - un peu obsolète                        */
extern void g3x_Write(const char *msg, int x, int y, G3Xcolor col)
{
  int i=0;
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);
  glMatrixMode(GL_PROJECTION);
  glOrtho(-0.5,639.5,-0.5,479.5,-1.0,1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glColor4f(col.r,col.g,col.b,col.a);
  glRasterPos2i(x,y);
  while (msg[i]!='\0')
  {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,msg[i]);
    i++;
  }
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
}
