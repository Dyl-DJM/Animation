/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G2X - Fonctions de base d'acces public -
                    version 6e - aout 2022
            création Hugo PETIOT (stage M1) Mai 2022
  ================================================================!*/

/*===================================*/
/*=  GESTION DES BOUTONS POUSSOIRS  =*/
/*===================================*/

// static G2XExclBut    *_EXCL_BUTTON_=NULL,*TheButton=NULL;
static int GROUP_NUMBER = BLOCSIZE / 2;
static G2XexclusiveGrp* _EXCL_GRPS_ = NULL;
static G2XExclBut** _EXCL_BUTTON_=NULL;
static int       exclbutnum=0;
static int       grpnum = -1;

static int _excl_button_id_on_, _excl_button_id_off_ = 0;

extern bool g2x_CreateExclusiveButtonGroup(const char *name, int* grpId)
{
  if(!_EXCL_GRPS_)
  {
    if (!(_EXCL_GRPS_=(G2XexclusiveGrp*)calloc(GROUP_NUMBER,sizeof(G2XexclusiveGrp)))) return -1;
  }
  grpnum++;
  G2XexclusiveGrp grp = _EXCL_GRPS_[grpnum];
  grp.active = NULL;
  strncpy(grp.name, name, NAMESIZE);
  for (int i = 0; grp.name[i]!= '\0'; i++)
  {
   grp.len += glutBitmapWidth(GLUT_BITMAP_HELVETICA_10,grp.name[i]);
  }
  grp.nbMembers = 0;
 *grpId = grpnum;
  grp.active = NULL;

  return true;
}

/*=  Attribue un numero et un texte au bouton  =*/
extern int g2x_CreateExclusiveButton(const char *name, void (*action)(void), const char *info, int grpId)
{
  int memberNumber;
  G2XexclusiveGrp* targetGrp;
  G2XExclBut* but;
  if(grpnum < grpId)
  {
    fprintf(stderr, "Couldn't create exclusive button <%s>, belonging to group n°%d: inexistant group\n", name, grpId);
    return false;
  }
  if (!_EXCL_BUTTON_)
  {
    if (!(_EXCL_BUTTON_=(G2XExclBut**)calloc(BLOCSIZE,sizeof(G2XExclBut*)))) return false;
  }
  if(!_EXCL_GRPS_[grpId].members){
    if (!(_EXCL_GRPS_[grpId].members=(G2XExclBut*)calloc(BLOCSIZE,sizeof(G2XExclBut)))) return false;
  }
  targetGrp = &_EXCL_GRPS_[grpId];
  memberNumber = targetGrp->nbMembers;
  but = &(targetGrp->members[memberNumber]);

  strncpy(but->name,name,NAMESIZE);
  if (info) strncpy(but->info,info,127);
  but->len = 0;
  for(int i = 0; but->name[i]; i++){
    but->len+=glutBitmapWidth(GLUT_BITMAP_HELVETICA_10,but->name[i]);
  }
  Ydialwidth  = MAX(Ydialwidth,but->len+8);
  Xbutpos     = Ydialwidth/2;
  Xbutw       = Ydialwidth/2-2;
  _EXCL_BUTTON_[exclbutnum++] = but;
  but->num  = (targetGrp->nbMembers)++;
  but->x    = Xbutpos;
  but->y    = Ybutpos; Ybutpos+=22;
  but->on   = false;
  but->action = action;
  but->exclusiveGrp = grpId;

  return true;
}


extern void applyExclusiveButtonGroupe(int grpId){
  if(_EXCL_GRPS_[grpId].active != NULL)
  {
    _EXCL_GRPS_[grpId].active->action();
  }
}

/*= Si un bouton est selectionne, son numero =*/
/*= est mis dans la var. glob. <TheButton>   =*/
static __inline__ bool g2x_SelectExclButton(int x, int y)
{
  if (!_EXCL_BUTTON_){
    return false;
  }
  G2XExclBut **but = _EXCL_BUTTON_;
  for(int i = 0; but[i] != _EXCL_BUTTON_[exclbutnum]; i++){
    if(abs(but[i]->x-x)<2*Xbutw && abs(but[i]->y-y)<10){
      pushmode=GLUT_DOWN;

      int num = but[i]->num;
      G2XexclusiveGrp* grp = &(_EXCL_GRPS_[(but[i])->exclusiveGrp]);

      if(but[i]->on){
        but[i]->on = false;
        grp->active = NULL;
        return true;
      }
      for (int j = 0; j < grp->nbMembers; j++){
        grp->members[j].on = false;
      }
      grp->members[num].on = true;
      grp->active = &(grp->members[num]);
      return true;
    }
  }
  return false;
}

/*= libere les bouttons        =*/
static __inline__ void g2x_FreeExclButtons()
{
  if(_EXCL_GRPS_){
    for(int i = 0; i < grpnum; i++){
      free(_EXCL_GRPS_[i].members);
    }
    free(_EXCL_GRPS_);
  }
  if (_EXCL_BUTTON_)
  {
    free(_EXCL_BUTTON_);
  }
}

static __inline__ void g2x_InitExclButOn()
{
  _excl_button_id_on_ = glGenLists(GROUP_NUMBER);
  float hue_graduation = 1 / (float)GROUP_NUMBER;
  for (int i = 0; i < GROUP_NUMBER; i++){
    glNewList(_excl_button_id_on_+i, GL_COMPILE);
      glPolygonMode(GL_FRONT,GL_FILL);
      glPolygonMode(GL_BACK, GL_LINE);
      glBegin(GL_TRIANGLE_FAN);
//        g2x_Color4fv(G2Xy_b);
        g2x_Color4fv(g2x_hsva_rgba_4f(hue_graduation * i, 0.8, 0.75, 1));

        glVertex2i(0,0);
//        g2x_Color4fv(G2Xg_a);
//        G2Xcolor col = g2x_hsva_rgba_4f(hue_graduation * i, 0.8, 0.75, 1);
        g2x_Color4fv(g2x_hsva_rgba_4f(hue_graduation * i, 0.8, 0.75, 1));

        glVertex2i(-Xbutw,-8);
        glVertex2i(+Xbutw,-8);
        glVertex2i(+Xbutw,+8);
        glVertex2i(-Xbutw,+8);
        glVertex2i(-Xbutw,-8);
      glEnd();
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEndList();
  }
}

static __inline__ void g2x_InitExclButOff()
{
  float hue_graduation = 1 / (float)GROUP_NUMBER;

  _excl_button_id_off_ = glGenLists(GROUP_NUMBER);
  for(int i = 0; i < GROUP_NUMBER; i++){
    glNewList(_excl_button_id_off_+i, GL_COMPILE);
      glPolygonMode(GL_FRONT,GL_FILL);
      glPolygonMode(GL_BACK, GL_LINE);// permet de faire apparaitre les buggs d'orrientation.
      glBegin(GL_TRIANGLE_FAN);
        g2x_Color4fv(g2x_hsva_rgba_4f(hue_graduation * i, 0.5, 0.75, 1));
        glVertex2i(0,0);
        g2x_Color4fv(g2x_hsva_rgba_4f(hue_graduation * i, 0.5, 0.75, 1));
        glVertex2i(-Xbutw,-8);
        glVertex2i(+Xbutw,-8);
        glVertex2i(+Xbutw,+8);
        glVertex2i(-Xbutw,+8);
        glVertex2i(-Xbutw,-8);
      glEnd();
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEndList();
  }
}

/*=  dessinne tous les boutons  =*/
static __inline__ void g2x_DrawExclusiveButtons(void)
{
  G2XExclBut  **but=_EXCL_BUTTON_;
  G2Xcolor col;
  char *c;

  while (but<_EXCL_BUTTON_+exclbutnum)
  {
    int grp = (*but)->exclusiveGrp;
    glPushMatrix();
      glTranslatef(Xbutpos,curheight-(*but)->y,0.);
      glCallList((*but)->on?_excl_button_id_on_+grp:_excl_button_id_off_+grp);
      glRasterPos2i(12,-4);
      col   = G2Xk;
      col.a = 1.-col.a;
      g2x_Color4fv(col);
      glRasterPos2i(-(*but)->len/2,-4);
      for (c=(*but)->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
      glRasterPos2i(1-(*but)->len/2,-4);
      for (c=(*but)->name; *c!='\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*c);
    glPopMatrix();
    but++;
  }
}
