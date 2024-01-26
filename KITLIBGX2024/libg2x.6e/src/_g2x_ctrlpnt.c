/*!==================================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G2X - Fonctions de base d'acces public -
                    version 6e - aout 2022
  ===================================================================!*/

/*============================================*/
/*=   GESTIONS DES POINTS DE CONTROLE REELS  =*/
/*============================================*/

/*=  Les points de controles sont ranges dans un tableau semi-dynamique automatique  =*/
static int        nbblocs=0;   // nombre de blocs d'allocation (<BLOCSIZE>)
static G2Xctrlpt* CTRLPTS=NULL;// tableau des <ctrlpt>
static int        nbcpt=0;     // nbre total de points de controle
static G2Xpoint*  CPT=NULL;    // adresse du point courant selectionne
static G2Xpoint   CPTpos;      // sa position

/*= revoie <true> si il existe au moins un point 'cliquable' =*/
/*= utilite douteuse.....                                    =*/
extern bool g2x_GetCpt() { return nbcpt?true:false;}

/*= ajoute le point d'adresse <P> dans la liste
    en lui attribuant le 'rayon' de detection <ray> (pixels) =*/
extern bool g2x_SetControlPoint(G2Xpoint* P, int  ray)
{
  if (nbblocs>=MAXBLOC)
  {
    fprintf(stderr,"\e[35m<g2x_SetControlPoint> : \e[1;31m Nombre max (%d) de point de controle atteint\e0;0m\n",nbcpt);
    return false;
  }
  if (nbcpt%BLOCSIZE==0)
  {
    CTRLPTS=(G2Xctrlpt*)realloc(CTRLPTS,(nbblocs+1)*BLOCSIZE*sizeof(G2Xctrlpt));
    if (!CTRLPTS) return false;
    memset(CTRLPTS+nbblocs*BLOCSIZE,0,BLOCSIZE*sizeof(G2Xctrlpt));
    nbblocs++;
  }
  CTRLPTS[nbcpt].add = P;
  CTRLPTS[nbcpt].ray = ray;
  nbcpt++;
  return true;
}

/*= Renvoie l'adresse du point de controle selectionne       =*/
extern G2Xpoint* g2x_GetControlPoint(void)
{ return (pushmode==GLUT_UP)||(CPT==NULL)?NULL:(CPT); }

/*= Renvoie l'adresse du point de controle selectionne       =*/
extern G2Xpoint* g2x_GetControlPoint2(G2Xpoint *old)
{ return (pushmode==GLUT_UP)||(CPT==NULL)?NULL:(*old=CPTpos,CPT); }

/*= Annule le deplacement du point de controle selectionne   =*/
extern void g2x_ReplaceControlPoint(void)
{ *CPT=CPTpos; }

/*= Supprime le point de controle                            =*/
extern void g2x_CleanControlPoint(G2Xpoint* pt)
{
  int n=0;
  while(n<nbcpt)
  {
    if (CTRLPTS[n].add==pt)
    {
      nbcpt--;
      CTRLPTS[n].add=CTRLPTS[nbcpt].add; CTRLPTS[nbcpt].add=NULL;
      CTRLPTS[n].ray=CTRLPTS[nbcpt].ray; CTRLPTS[nbcpt].ray=0;
      return;
    }
    n++;
  }
}

/*= Libere la liste des points de controle                   =*/
extern void g2x_CleanAllControlPoint(void)
{
  if (CTRLPTS) { free(CTRLPTS); CTRLPTS=NULL; }
  nbblocs=0;
  nbcpt=0;
  CPT=NULL;
}
