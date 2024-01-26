/*!==================================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G2X - Fonctions de base d'acces public -
                    version 6e - aout 2022
  =================================================================== */
#include <stdio.h>
#include <math.h>



#include <g2x.h>

/*==============================================================================================================*/

static __inline__ bool g2x_IsInWindow(double x, double y)
{
  return (x>=g2x_GetXMin() && x<=g2x_GetXMax() && y>=g2x_GetYMin() && y<=g2x_GetYMax());
}

extern void g2x_SetPointSize(double size) { glPointSize(size); }
extern void g2x_SetColor(G2Xcolor col) { glColor4d(col.r,col.g,col.b,col.a); }

extern void g2x_PlotDft(double x, double y)
{
  glBegin(GL_POINTS);
    glVertex2d(x,y);
  glEnd();
}

extern void g2x_Plot(double x, double y, G2Xcolor color, int width)
{
  if (!g2x_IsInWindow(x,y)) return;
  glPointSize((GLfloat)width);
  G2Xcolor4fv(color);
  glBegin(GL_POINTS);
    glVertex2d(x,y);
  glEnd();
  glPointSize(1.);
}

extern void g2x_DrawPoint(G2Xpoint P, G2Xcolor color, int width) { g2x_Plot(P.x, P.y,color,width); }

/*==============================================================================================================*/
extern void g2x_LineDft(double xa, double ya, double xb, double yb)
{
  glBegin(GL_LINES);
    glVertex2d(xa,ya);
    glVertex2d(xb,yb);
  glEnd();
}

extern void g2x_Line(double xa, double ya, double xb, double yb, G2Xcolor col, int width)
{
  glLineWidth((GLfloat)width);
  G2Xcolor4fv(col);
  glBegin(GL_LINES);
    glVertex2d(xa,ya);
    glVertex2d(xb,yb);
  glEnd();
  glLineWidth(1.);
}

extern void g2x_DrawLine(G2Xpoint A, G2Xpoint B, G2Xcolor color, int width)
{
  g2x_Line(A.x, A.y, B.x, B.y, color, width);
}


extern void g2x_Line_2col(double xa, double ya, G2Xcolor cola, double xb, double yb, G2Xcolor colb, int width)
{
  glLineWidth((GLfloat)width);
  glBegin(GL_LINES);
    G2Xcolor4fv(cola);
    glVertex2d(xa,ya);
    G2Xcolor4fv(colb);
    glVertex2d(xb,yb);
  glEnd();
  glLineWidth(1.);
}

extern void g2x_DrawLine_2col(G2Xpoint A, G2Xcolor colA, G2Xpoint B, G2Xcolor colB, int width)
{
  g2x_Line_2col(A.x, A.y, colA, B.x, B.y, colB, width);
}

/*!============================================================================================================!*/
/*! Cercles, Ellipses, secteurs ....                                                                           !*/
/*!============================================================================================================!*/
extern void g2x_Circle(double xc, double yc, double r, G2Xcolor col, int width)
{
//  if (!g2x_IsInWindow(xc,yc)) return;
  if (G2Xiszero(r)) return;
  double x,y;
  G2Xcolor4fv(col);
  glLineWidth((GLfloat)width);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glBegin(GL_POLYGON);
    int i,n=(int)((g2x_GetXPixSize()*720)/r);
    n = CLIP(1,n,144);
    for (i=0;i<720;i+=n)
    {
      x=xc+r*g2x_Idxcos(i);
      y=yc+r*g2x_Idxsin(i);
      glVertex2d(x,y);
    }
  glEnd();
  glLineWidth(1.);
}

extern void g2x_DrawCircle(G2Xpoint C, double r, G2Xcolor col, int width)
{
  g2x_Circle(C.x,C.y,r,col,width);
}

extern void g2x_FillCircle(double xc, double yc, double r, G2Xcolor col)
{
  double x,y;

  //if (!g2x_IsInWindow(xc,yc)) return;
  if (G2Xiszero(r)) return;

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  G2Xcolor4fv(col);
  glBegin(GL_POLYGON);
    int i,n=(int)((g2x_GetXPixSize()*720)/r);
    n = CLIP(1,n,144);
    for (i=0;i<720;i+=n)
    {
      x=xc+r*g2x_Idxcos(i);
      y=yc+r*g2x_Idxsin(i);
      glVertex2d(x,y);
    }
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void g2x_DrawFillCircle(G2Xpoint C, double r, G2Xcolor col)
{
  g2x_FillCircle(C.x,C.y,r,col);
}


extern void g2x_Ellipse(double xc, double yc, double rx, double ry, double degrot, G2Xcolor col, int width)
{
  if (G2Xiszero(rx*ry)) return;
  if (degrot<0.) degrot=360.+degrot;
  G2Xcolor4fv(col);
  glLineWidth((GLfloat)width);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glBegin(GL_LINE_LOOP);
    double x,y,xa,ya;
    double ca=g2x_Idxcos((int)(2*degrot));
    double sa=g2x_Idxsin((int)(2*degrot));
    int    i,n=(int)((g2x_GetXPixSize()*1440)/(rx+ry));
    n = CLIP(1,n,144);
    for (i=0;i<720;i+=n)
    {
      xa=rx*g2x_Idxcos(i);
      ya=ry*g2x_Idxsin(i);
      x=xc+ca*xa+sa*ya;
      y=yc+sa*xa-ca*ya;
      glVertex2d(x,y);
    }
  glEnd();
  glLineWidth(1.);
}

extern void g2x_FillEllipse(double xc, double yc, double rx, double ry, double degrot, G2Xcolor col)
{
  if (G2Xiszero(rx*ry)) return;
  if (degrot<0.) degrot=360.+degrot;
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  G2Xcolor4fv(col);
  glBegin(GL_POLYGON);
    double x,y,xa,ya;
    double ca=g2x_Idxcos((int)(2*degrot));
    double sa=g2x_Idxsin((int)(2*degrot));
    int    i,n=(int)((g2x_GetXPixSize()*1440)/(rx+ry));
    n = CLIP(1,n,144);
    for (i=0;i<720;i+=n)
    {
      xa=rx*g2x_Idxcos(i);
      ya=ry*g2x_Idxsin(i);
      x=xc+ca*xa+sa*ya;
      y=yc+sa*xa-ca*ya;
      glVertex2d(x,y);
    }
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void g2x_PartialEllipse(double xc, double yc, double rx, double ry, double degsec, double degrot, G2Xcolor col, int width)
{
  if (G2Xiszero(rx*ry)) return;
  G2Xcolor4fv(col);
  glLineWidth((GLfloat)width);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  glBegin(GL_LINE_LOOP);
    double x,y,xa,ya;
    double ca=g2x_Idxcos((int)(2*degrot));
    double sa=g2x_Idxsin((int)(2*degrot));
    int    i,n=(int)((g2x_GetXPixSize()*1440)/(rx+ry));
    int    deb=720-2*degsec;
    n = CLIP(1,n,144);
    glVertex2d(xc,yc);
    for (i=deb;i<=720;i+=n)
    {
      xa=rx*g2x_Idxcos(i);
      ya=ry*g2x_Idxsin(i);
      x=xc+ca*xa+sa*ya;
      y=yc+sa*xa-ca*ya;
      glVertex2d(x,y);
    }
  glEnd();
  glLineWidth(1.);
}

extern void g2x_FillPartialEllipse(double xc, double yc, double rx, double ry, double degsec, double degrot, G2Xcolor col)
{
  if (G2Xiszero(rx*ry)) return;
  if (degrot<0.) degrot=360.+degrot;
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  G2Xcolor4fv(col);
  glBegin(GL_POLYGON);
    double x,y,xa,ya;
    double ca=g2x_Idxcos((int)(2*degrot));
    double sa=g2x_Idxsin((int)(2*degrot));
    int    i,n=(int)((g2x_GetXPixSize()*1440)/(rx+ry));
    int    deb=720-2*degsec;
    n = CLIP(1,n,144);
    glVertex2d(xc,yc);
    for (i=deb;i<=720;i+=n)
    {
      xa=rx*g2x_Idxcos(i);
      ya=ry*g2x_Idxsin(i);
      x=xc+ca*xa+sa*ya;
      y=yc+sa*xa-ca*ya;
      glVertex2d(x,y);
    }
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void g2x_FillCircleSlice(double xc, double yc, G2Xcolor colC, double xa, double ya, G2Xcolor colA, double deg)
{
  double r,d,a,x,y;

  r=sqrt(SQR(xc-xa)+SQR(yc-ya));
  if (G2Xiszero(r)) return;

  a = acos((xa-xc)/r);
  a*=(asin((ya-yc)/r)<0.?-1.:+1.);

  deg*=DegToRad;
  d=a-0.5*deg;

  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  glBegin(GL_POLYGON);
    G2Xcolor4fv(colC);
    glVertex2d(xc,yc);
    G2Xcolor4fv(colA);
    while (d<a+0.5*deg)
    {
      x=xc+r*g2x_Radcos(d);
      y=yc+r*g2x_Radsin(d);
      glVertex2d(x,y);
      d+=deg*0.1;
    }
    x=xc+r*g2x_Radcos(a+0.5*deg);
    y=yc+r*g2x_Radsin(a+0.5*deg);
    glVertex2d(x,y);
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void g2x_FillCircleSector(double xc, double yc, double xa, double ya, double xb, double yb, G2Xcolor col)
{
  double r,d,a,b,x,y;

  r=sqrt(SQR(xc-xa)+SQR(yc-ya));
  if (G2Xiszero(r)) return;
  a = acos((xa-xc)/r);
  a*=(asin((ya-yc)/r)<0.?-1.:+1.);

  b = acos((xb-xc)/r);
  b*=(asin((yb-yc)/r)<0.?-1.:+1.);
  if (b<a) {d=a; a=b; b=d;}
  d=a;
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glBegin(GL_POLYGON);
    G2Xcolor4fv(col);
    while (d<b)
    {
      x=xc+r*g2x_Radcos(d);
      y=yc+r*g2x_Radsin(d);
      glVertex2d(x,y);
      d+=(b-a)*0.1;
    }
    x=xc+r*g2x_Radcos(b);
    y=yc+r*g2x_Radsin(b);
    glVertex2d(x,y);
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}


/*!============================================================================================================!*/
/*! Triangles                                                                                                  !*/
/*!============================================================================================================!*/
extern void g2x_Triangle(double xa, double ya, double xb, double yb, double xc, double yc, G2Xcolor col, int width)
{
  glLineWidth((GLfloat)width);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  G2Xcolor4fv(col);
  glBegin(GL_TRIANGLES);
    glVertex2d(xa,ya);
    glVertex2d(xb,yb);
    glVertex2d(xc,yc);
  glEnd();
  glLineWidth(1.);
}

extern void g2x_DrawTriangle(G2Xpoint A, G2Xpoint B, G2Xpoint C, G2Xcolor col, int width)
{
  g2x_Triangle(A.x,A.y,B.x,B.y,C.x,C.y,col,width);
}


extern void g2x_FillTriangle(double xa, double ya, double xb, double yb, double xc, double yc, G2Xcolor col)
{
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  G2Xcolor4fv(col);
  glBegin(GL_TRIANGLES);
    glVertex2d(xa,ya);
    glVertex2d(xb,yb);
    glVertex2d(xc,yc);
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void g2x_DrawFillTriangle(G2Xpoint A, G2Xpoint B, G2Xpoint C, G2Xcolor col)
{
  g2x_FillTriangle(A.x,A.y,B.x,B.y,C.x,C.y,col);
}


extern void g2x_FillTriangle_3col(double xa, double ya, G2Xcolor cola,
                                  double xb, double yb, G2Xcolor colb,
                                  double xc, double yc, G2Xcolor colc)
{
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glBegin(GL_TRIANGLES);
    G2Xcolor4fv(cola); glVertex2d(xa,ya);
    G2Xcolor4fv(colb); glVertex2d(xb,yb);
    G2Xcolor4fv(colc); glVertex2d(xc,yc);
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void g2x_DrawFillTriangle_3col(G2Xpoint A, G2Xcolor colA, G2Xpoint B, G2Xcolor colB, G2Xpoint C, G2Xcolor colC)
{
  g2x_FillTriangle_3col(A.x,A.y,colA,B.x,B.y,colB,C.x,C.y,colC);
}

/*!============================================================================================================!*/
/*! Rectangles parallèles aux axes                                                                             !*/
/*!============================================================================================================!*/
extern void g2x_Rectangle(double xd, double yd, double xu, double yu, G2Xcolor col, int width)
{
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  G2Xcolor4fv(col);
  glLineWidth((GLfloat)width);
  glRectd(xd,yd,xu,yu);
  glLineWidth(1.);
}

extern void g2x_DrawRectangle(G2Xpoint DL, G2Xpoint UR, G2Xcolor color, int width)
{
  g2x_Rectangle(DL.x,DL.y,UR.x,UR.y,color,width);
}

extern void g2x_FillRectangle(double xd, double yd, double xu, double yu, G2Xcolor col)
{
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  G2Xcolor4fv(col);
  glRectd(xd,yd,xu,yu);
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void g2x_DrawFillRectangle(G2Xpoint DL, G2Xpoint UR, G2Xcolor color)
{
  g2x_FillRectangle(DL.x,DL.y,UR.x,UR.y,color);
}


/*!============================================================================================================!*/
/*! Quadrilatères quelconques                                                                                  !*/
/*!============================================================================================================!*/
extern void g2x_Quad(double xa, double ya,
                     double xb, double yb,
                     double xc, double yc,
                     double xd, double yd, G2Xcolor col, int width)
{
  G2Xcolor4fv(col);
  glLineWidth((GLfloat)width);
  glBegin(GL_QUADS);
    glVertex2d(xa,ya);
    glVertex2d(xb,yb);
    glVertex2d(xc,yc);
    glVertex2d(xd,yd);
  glEnd();
}

extern void g2x_FillQuad(double xa, double ya,
                         double xb, double yb,
                         double xc, double yc,
                         double xd, double yd,G2Xcolor col)
{
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  G2Xcolor4fv(col);
  glBegin(GL_QUADS);
    glVertex2d(xa,ya);
    glVertex2d(xb,yb);
    glVertex2d(xc,yc);
    glVertex2d(xd,yd);
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}

extern void g2x_FillQuad_4col(double xa, double ya,G2Xcolor cola,
                              double xb, double yb,G2Xcolor colb,
                              double xc, double yc,G2Xcolor colc,
                              double xd, double yd,G2Xcolor cold)
{
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glBegin(GL_QUADS);
    G2Xcolor4fv(cola); glVertex2d(xa,ya);
    G2Xcolor4fv(colb); glVertex2d(xb,yb);
    G2Xcolor4fv(colc); glVertex2d(xc,yc);
    G2Xcolor4fv(cold); glVertex2d(xd,yd);
  glEnd();
  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}



/*!============================================================================================================!*/
/*! "spots" : coûte TRES cher !!!!  => A EVITER                                                                !*/
/*!============================================================================================================!*/
#define SPOTPOINTSIZE 2
extern void g2x_Spot(double xc, double yc, double r, G2Xcolor col)
{
  int i,j,rix,riy;
  double x,y,yy,rr;
  float  d;
    glPointSize(SPOTPOINTSIZE);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glBegin(GL_POINTS);
  rr=r*r;
  riy=(int)(r/g2x_GetYPixSize());
  rix=(int)(r/g2x_GetXPixSize());
  for (j=-riy; j<=+riy; j++)
  {
    y=j*SPOTPOINTSIZE*g2x_GetYPixSize();
    yy=y*y;
    for (i=-rix; i<=+rix; i++)
    {
       x=i*SPOTPOINTSIZE*g2x_GetXPixSize();
       if ((d=(float)(x*x+yy))>rr) continue;
       d=pow((1.-d/rr),0.333);
       glColor4f(d*col.r,d*col.g,d*col.b,d*(1.-col.a));
       glVertex2d(xc+x,yc+y);
    }
  }
  glEnd();
  glPointSize(1.);
  glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA);
}

extern void g2x_SpotBar(double xa, double ya, double xb, double yb, double r, G2Xcolor col)
{

  double x,y,yy,rr;
  float  d;
  glBegin(GL_LINES);
  rr=r*r;
  for (y=-r; y<=+r; y+=g2x_GetYPixSize())
  {
    yy=y*y;
    for (x=-r; x<=+r; x+=g2x_GetXPixSize())
    {
       if ((d=(double)(x*x+yy))>rr) continue;
       d=(1.-d/rr);
       glColor4f(d*col.r,d*col.g,d*col.b,d*col.a);
       glVertex2d(xa+x,ya+y);
       glVertex2d(xb+x,yb+y);
    }
  }
  glEnd();
}

extern void g2x_SpotMap(double xc, double yc, double r, G2Xcolor col, float (*map)(float))
{
  double x,y,yy,rr;
  float  d;

  glPointSize(SPOTPOINTSIZE);
  glBegin(GL_POINTS);
  rr=r*r;
  for (y=-r; y<=+r; y+=SPOTPOINTSIZE*g2x_GetYPixSize())
  {
    yy=y*y;
    for (x=-r; x<=+r; x+=SPOTPOINTSIZE*g2x_GetXPixSize())
    {
       if ((d=(float)(x*x+yy))>rr) continue;
       d=map(1.-d/rr);
       glColor4f(col.r,col.g,col.b,d*col.a);
       glVertex2d(xc+x,yc+y);
    }
  }
  glEnd();
  glPointSize(1.);
}

extern void g2x_Spotf2(double xc, double yc, double r, G2Xcolor col, int blend_mode)
{
  double x,y,yy,rr;
  float  d;

  switch (blend_mode)
  {
    case 1 : glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA); break;

    case 2 : glBlendFunc(GL_DST_COLOR,GL_ONE_MINUS_SRC_COLOR); break;
    case 3 : glBlendFunc(GL_DST_ALPHA,GL_ONE_MINUS_SRC_COLOR); break;

    case 4 : glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_DST_COLOR); break;
    case 5 : glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_DST_COLOR); break;

    case 6 : glBlendFunc(GL_ONE_MINUS_SRC_COLOR,GL_DST_ALPHA); break;
    case 7 : glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_DST_ALPHA); break;

    case 8 : glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); break;

  }
  glBegin(GL_POINTS);
  rr=r*r;
  for (y=-r; y<=+r; y+=g2x_GetYPixSize())
  {
    yy=y*y;
    for (x=-r; x<=+r; x+=g2x_GetXPixSize())
    {
       if ((d=(double)(x*x+yy))>rr) continue;
       d=(1.-d/rr);
       d=d*d;
       glColor4d(d*col.r,d*col.g,d*col.b,d*col.a);
       glVertex2d(xc+x,yc+y);
    }
  }
  glEnd();
  glBlendFunc(GL_SRC_ALPHA,GL_DST_ALPHA);
}


