/*=================================================================*/
/*= E.Incerti - eric.incerti@univ-eiffel.fr                       =*/
/*= Université Gustave Eiffeil                                    =*/
/*= Balistic launch : analytic and step-by-step solutions         =*/
/*=================================================================*/

/* main standard libs included in <g2x.h> */
#include <g2x.h>

/* GRAVITY */
#define g 10.0

/* pixel window dimensions          */
uint   pixwidth=1800,pixheight=900;
/* floating-point window dimensions */
double xmin=-5.,ymin=-5.,xmax=+205.,ymax=+105.;

/* 'Physical' parameters                                                       */
double m  = 1.0;           /* ball mass                                         */
double za = 0.01;          /* air 'viscosity' (elementary kinetic damping)      */
double kc = 0.9, zc=0.9;
double r  = 2;

/* initial position and speed */
G2Xpoint  P0=(G2Xpoint) {  0.,25.};
G2Xvector V0=(G2Xvector){ 10.,40.};
/* current positions Euler, Implicit, Leapfrog */
G2Xpoint  Pe,Pi,Pl;
/* speed, acceleration  Euler, Implicit, Leapfrog, gravity */
G2Xvector Ve,Vi,Vl,Ae,Ai,Al,G;

/* sampling rate for images */
double dt=0.1;
/* simulation time step */
double h =0.1;
/* slow dow simulation (delay, in seconds) */
int tempo=500;

/* ratio DispFreq=(int)(dt/h) */
int    DispFreq;

/*======================================================================*/
/*= Computes an draws analytic solution for initial conditions (p0,vO) =*/
/*======================================================================*/
void Analytic(G2Xpoint* pi, G2Xvector* vi)
{
	G2Xpoint  p,q;
  G2Xvector v;
	double    t = 0;
  double    w=m/za,e;

	/* two sets of "continuous time" formulation : with or without "air friction" */
  switch (G2Xiszero(za)) /* G2Xiszero(x) <=> (fabs(x)<EPSILON?true:false) */
  {
    case true : /* without "air" friction */
      q=*pi;
      do
      {
        t+=dt;
			  /* setup position */
			  p.x = pi->x + vi->x*t;
			  p.y = pi->y + vi->y*t - 0.5*g*t*t;
			  /* setup speed    */
			  v.x = vi->x;
			  v.y = vi->y - g*t;
        /* draw           */
        //g2x_Line(q.x,q.y,p.x,p.y,G2Xwa,1);
        g2x_Plot(p.x,p.y,G2Xwb,3);
        q=p;
     } while (p.y+dt*v.y>0.);
    break;
    default : /* with "air" friction */
      q=*pi;
      do
      {
        t += dt;
        e  = exp(-t/w);
			  /* setup position */
			  p.x = pi->x + w*(      vi->x)*(1.-e);
			  p.y = pi->y + w*(g*w + vi->y)*(1.-e) - (g*w)*t;
			  /* setup speed    */
			  v.x = (    vi->x)*e;
			  v.y = (g*w+vi->y)*e - g*w;
        /* draw           */
        //g2x_Line(q.x,q.y,p.x,p.y,G2Xwa,1);
        g2x_Plot(p.x,p.y,G2Xwb,3);
        q=p;
      } while (p.y+dt*v.y>0.);
  }
  /* setup initial position */
  *pi = q;
  /* setup initial speed    */
  vi->x =  zc*v.x;
  vi->y = -kc*v.y;
}


/*=================================================================*/
/*= step-by-step function called by the simulation loop           =*/
/*=================================================================*/
/*= Explicit method =*/
void Simul_Explicit(void)
{
	/* 1 - set up ball position */
	Pe.x = Pe.x + h*Ve.x;
	Pe.y = Pe.y + h*Ve.y;
	/* 2 - set up ball velocity */
	Ve.x = Ve.x + h*Ae.x;
	Ve.y = Ve.y + h*Ae.y;
  /* 3 - computes the resulting force applied to the particle */
	Ae.x = G.x-(za/m)*Ve.x;
	Ae.y = G.y-(za/m)*Ve.y;
	if (Pe.y<=r) 	/* collision detection */
	{
		Ve.x *=  zc;
		Ve.y *= -kc; /* collision treatment : inverse kinetics */
		Pe.y  = r;
	}
}

/*= Implicit method =*/
void Simul_Implicit(void)
{
	/* 1 - set up ball velocity */
	double w = m/(m+za*h);
	Vi.x = (Vi.x + G.x*h)*w;
	Vi.y = (Vi.y + G.y*h)*w;
	/* 2 - set up ball position */
	Pi.x = Pi.x + h*Vi.x;
	Pi.y = Pi.y + h*Vi.y;
  /* 3 - computes the resulting force applied to the particle */
	Ai.x = G.x-(za/m)*Vi.x;
	Ai.y = G.y-(za/m)*Vi.y;
	if (Pi.y<=r) 	/* collision detection */
	{
		Vi.x *=  zc;
		Vi.y *= -kc; /* collision treatment : inverse kinetics */
		Pi.y  = r;
	}
}

/*= Leapfrog method =*/
void Simul_LeapFrog(void)
{
  /* 1 - computes the resulting force applied to the particle */
	Al.x = G.x-(za/m)*Vl.x;
	Al.y = G.y-(za/m)*Vl.y;
	/* 2 - set up ball velocity */
	Vl.x = Vl.x + h*Al.x;
	Vl.y = Vl.y + h*Al.y;
	/* 3 - set up ball position */
	Pl.x = Pl.x + h*Vl.x;
	Pl.y = Pl.y + h*Vl.y;
	if (Pl.y<=r) 	/* collision detection */
	{
		Vl.x *=  zc;
		Vl.y *= -kc; /* collision treatment : inverse kinetics */
		Pl.y  = r;
	}
}


/*=================================================================*/
/*= sic.                                                          =*/
/*=================================================================*/
void reset(void)
{
	Pe=Pi=Pl=P0;
	Ve=Vi=Vl=V0;
	Ae.x=Ai.x=Al.x=0.;
	Ae.y=Ai.y=Al.y=0.;
}

/*=================================================================*/
/*= Initialize                                                    =*/
/*=================================================================*/
void init(void)
{
  m  = 1.0;        /* ball mass                                    */
  za = 0.01;       /* air 'viscosity' (elementary kinetic damping) */
  kc = 0.9;
  zc = 0.9;
  r  = 2.0;
  /* initial position and speed */
  P0=(G2Xpoint) {  0.,25.};
  V0=(G2Xvector){ 10.,40.};
  G =(G2Xvector){  0.,-g};
  dt=0.05; /* analytic  time step */
  h =0.05; /* simulation time step */
	reset();
}

/*===========================================================================*/
/*=                                                                         =*/
/*===========================================================================*/
void ctrl(void)
{
  /* scrollbars and buttons */
	g2x_CreateScrollv_d("za" ,&za   ,0.   ,.1  ,"za");
	g2x_CreateScrollv_i("tmp",&tempo,0    ,1000,"tempo");
	g2x_CreateScrollh_d("dt" ,&dt   ,0.01,.5   ,"dt");
	g2x_CreateScrollh_d(" h" ,&h    ,0.01,.5   ,"h");
	g2x_CreatePopUp("reset",reset,"reset");
}


/*===========================================================================*/
/*=                                                                         =*/
/*===========================================================================*/
void draw(void)
{
	G2Xpoint  p=P0;
	G2Xvector v=V0;
	g2x_Axes();

  while (!G2Xiszero(v.x) && p.x<xmax) Analytic(&p,&v);

	g2x_Circle(Pl.x,Pl.y,r,G2Xr,5);
	g2x_Circle(Pe.x,Pe.y,r,G2Xc,3);
	g2x_Circle(Pi.x,Pi.y,r,G2Xg,2);
  /*-------------------------------------*/
	g2x_StaticPrint( 10, 10,G2Xk,"dt=%.1e  h=%.1e  DispFreq=%d",dt,h,DispFreq);
	g2x_StaticPrint(100,600,G2Xc,"Explicit");
	g2x_StaticPrint(180,600,G2Xg,"Implicit");
	g2x_StaticPrint(250,600,G2Xr,"LeapFrog");
}

/*===========================================================================*/
/*=                                                                         =*/
/*===========================================================================*/
void anim(void)
{
  usleep(SQR(tempo)*h); // temporisation

  Simul_Explicit();
  Simul_Implicit();
  Simul_LeapFrog();

	/* the ball leaves the window, back to initial conditions */
	if (Pe.y<=0.   || Pi.y<=0.   || Pl.y<=0. ||
	    Pe.x>=xmax || Pi.x>=xmax || Pl.x>=xmax) reset();
}

/*===========================================================================*/
/*= Cleaning function                                                       =*/
/*===========================================================================*/
void quit(void)
{
  /* nothing to do here */
}

/*===========================================================================*/
/*=                                                                         =*/
/*===========================================================================*/
int main(int argc, char* argv[])
{
  /* window statement */
  g2x_InitWindow("Balistic",pixwidth,pixheight);
  g2x_SetWindowCoord(xmin,ymin,xmax,ymax);
  g2x_SetBkGdCol(0.);
  /* handlers */
  g2x_SetInitFunction(init);
  g2x_SetCtrlFunction(ctrl);
  g2x_SetEvtsFunction(NULL);
	g2x_SetDrawFunction(draw);
	g2x_SetAnimFunction(anim);
  g2x_SetExitFunction(quit);
  /* simulation loop -- passed to glutMainLoop() */
  return g2x_MainStart();
}
