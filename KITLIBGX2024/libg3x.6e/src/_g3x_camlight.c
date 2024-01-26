/**===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G3X - Fonctions de base d'acces public -
                    version 6e - Jan.2022
  ================================================================**/

/*====================================================================*/
/*=                               LA CAMERA                          =*/
/*====================================================================*/
static G3Xpoint  cam_default_pos={5.,5.,5.};
static G3Xpoint  cam_default_tar={0.,0.,0.};

static GLdouble  theta_cam_init=+0.25*PI,
                 phi_cam_init  =+0.25*PI,
                 dist_cam_init = 5.;

static G3Xcamera _main_cam_;

extern G3Xcamera* g3x_GetCamera(void) { return &_main_cam_; }
extern double     g3x_GetCameraDist(void) { return _main_cam_.dist; }

extern void g3x_SetCameraTracking(G3Xpoint *position, G3Xpoint *target)
{
  if (position) _main_cam_.pos=position;
  if (target  ) _main_cam_.tar=target;
}

extern void g3x_SetCameraSpheric(double theta, double phi, double dist)
{
  /* matrices de la caméra      */
  _main_cam_.Md = g3x_Identity();
  _main_cam_.Mi = g3x_Identity();
  _main_cam_.Md = g3x_Mat_x_Mat(g3x_RotationZ(PI/2),_main_cam_.Md);
  _main_cam_.Mi = g3x_Mat_x_Mat(_main_cam_.Mi,g3x_RotationZ(-PI/2));
  _main_cam_.Md = g3x_Mat_x_Mat(g3x_RotationY(phi  ),_main_cam_.Md);
  _main_cam_.Mi = g3x_Mat_x_Mat(_main_cam_.Mi,g3x_RotationY(-phi  ));
  _main_cam_.Md = g3x_Mat_x_Mat(g3x_RotationZ(theta),_main_cam_.Md);
  _main_cam_.Mi = g3x_Mat_x_Mat(_main_cam_.Mi,g3x_RotationZ(-theta));
  _main_cam_.Md = g3x_Mat_x_Mat(g3x_Translation3d(0.,0.,dist),_main_cam_.Md);
  _main_cam_.Mi = g3x_Mat_x_Mat(_main_cam_.Mi,g3x_Translation3d(0.,0.,-dist));
  /* paramètres courants        */
  _main_cam_.theta= theta;
  _main_cam_.phi  = phi;
  _main_cam_.dist = dist;
  /* mise à jour de la position */
  _main_cam_.pos->x = _main_cam_.tar->x + dist*cos(theta)*cos(phi);
  _main_cam_.pos->y = _main_cam_.tar->y + dist*sin(theta)*cos(phi);
  _main_cam_.pos->z = _main_cam_.tar->z + dist*sin(phi);
  _main_cam_.upv.z  = cos(phi)>0.?1.0:-1.0;
}

extern void g3x_SetCameraCartesian(G3Xpoint position, G3Xpoint target)
{
 *_main_cam_.pos   = position;
 *_main_cam_.tar   = target;
  _main_cam_.dist  = g3x_Norm(position);
  _main_cam_.phi   = (G3Xiszero(_main_cam_.dist  ) ? 0.     : asin(position.z/_main_cam_.dist   ));
  _main_cam_.theta = (G3Xiszero(position.y) ? 0.5*PI : atan(position.x/position.y));
  g3x_SetCameraSpheric(_main_cam_.theta, _main_cam_.phi, _main_cam_.dist);
}

static __inline__ void g3x_PanScan(G3Xcamera *cam, double dx, double dy)
{
  G3Xhmat M=cam->Mi;
  M = g3x_Mat_x_Mat(g3x_Translation3d(-dx,0.,-dy),M);
  M = g3x_Mat_x_Mat(cam->Md,M);
  *cam->pos = g3x_Mat_x_Point(M,*cam->pos);
  *cam->tar = g3x_Mat_x_Point(M,*cam->tar);
}

extern void g3x_SetCameraTheta(double   theta) { _main_cam_.theta=theta; }
extern void g3x_SetCameraPhi  (double   phi  ) { _main_cam_.phi=phi;}
extern void g3x_SetCameraDist (double   dist ) { _main_cam_.dist=dist; }
extern void g3x_SetCameraTar  (G3Xpoint tar  ) { *(_main_cam_.tar)=tar; }

/* parametres d'ouverture de la camera 3D OpenGl (cf. gluLookAt() */
extern void g3x_SetPerspective(float near, float far, float open)
{
  _main_cam_.near = near;
  _main_cam_.far  = far;
  _main_cam_.open = open;
}

/*====================================================================*/
/*=                               LA LUMIERE                         =*/
/*====================================================================*/
static G3Xpoint  spot_default_pos={5.,5.,5.};

static GLdouble  theta_spot_init=+0.25*PI,
                 phi_spot_init  =+0.25*PI,
                 dist_spot_init = 5.;

static G3Xlight  _main_spot_;

extern G3Xlight* g3x_GetLight(void) { return &_main_spot_; }

extern void g3x_SetLightSpheric(double theta, double phi, double dist)
{
  _main_spot_.theta=theta;
  _main_spot_.phi  =phi;
  _main_spot_.dist =dist;
  _main_spot_.pos.x = _main_spot_.dist*cos(_main_spot_.theta)*cos(_main_spot_.phi);
  _main_spot_.pos.y = _main_spot_.dist*sin(_main_spot_.theta)*cos(_main_spot_.phi);
  _main_spot_.pos.z = _main_spot_.dist*sin(_main_spot_.phi);
}

extern void g3x_SetLightCartesian(G3Xpoint position)
{
  _main_spot_.pos   = position;
  _main_spot_.dist  = g3x_Norm(_main_spot_.pos);
  _main_spot_.phi   = (G3Xiszero(_main_spot_.dist ) ? 0.     : asin(_main_spot_.pos.z/_main_spot_.dist   ));
  _main_spot_.theta = (G3Xiszero(_main_spot_.pos.y) ? 0.5*PI : atan(_main_spot_.pos.x/_main_spot_.pos.y ));
  g3x_SetLightSpheric(_main_spot_.theta, _main_spot_.phi, _main_spot_.dist);
}

extern void g3x_SetSpotSteps(GLfloat dx, GLfloat dy, GLfloat dz)
{
  if (dx!=0.) _main_spot_.dx=dx;
  if (dy!=0.) _main_spot_.dy=dy;
  if (dz!=0.) _main_spot_.dz=dz;
}

extern void g3x_SetLightAmbient  (float r, float g, float b){ _main_spot_.ambi=(G3Xcolor){r,g,b,0.}; }
extern void g3x_SetLightDiffuse  (float r, float g, float b){ _main_spot_.diff=(G3Xcolor){r,g,b,0.}; }
extern void g3x_SetLightSpecular (float r, float g, float b){ _main_spot_.spec=(G3Xcolor){r,g,b,0.}; }
extern void g3x_SetLightPosition (float x, float y, float z){ _main_spot_.pos =(G3Xpoint){x,y,z}   ; }
