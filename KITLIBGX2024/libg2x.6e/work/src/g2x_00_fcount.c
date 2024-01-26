/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr                       =!*/
/*!= Université Gustave Eiffel                                     =!*/
/*!=                                                               =!*/
/*!= cet exemple compte et affiche les appels aux différentes      =!*/
/*!= fonctions / handlers                                          =!*/
/*!=================================================================!*/

#include <g2x.h>

/* les compteurs d'appel */
static int  ifcount=0,
            cfcount=0,
            efcount=0,
            afcount=0,
            dfcount=0,
            qfcount=0;
/* des strings pour les 3 fonctions de boucle */
static char efmsg[32]="",
            afmsg[32]="",
            dfmsg[32]="";

/* la fonction d'initialisation  */
static void init(void)
{
  ifcount++;
  /* format d'écriture texte
   * - taille   'normale'
   * - style    'gras'
   * - position 'aligné à gauche'
   */
  g2x_SetFontAttributes('n','b','l');
}

/* la fonction de contrôle */
static void ctrl(void)
{
  cfcount++;
}

/* la fonction de contrôle */
static void evts(void)
{
  efcount++; sprintf(efmsg,"<evts> %d\n",efcount);
}

/* la fonction d'animation */
static void anim(void)
{
  afcount++; sprintf(afmsg,"<anim> %d\n",afcount);
}

/* la fonction de dessin */
static void draw(void)
{
  dfcount++; sprintf(dfmsg,"<draw> %d\n",dfcount);

  g2x_StaticPrint(10,20,G2Xk,"<anim>");
  switch (g2x_Running())
  {
    case true : g2x_StaticPrint(50,20,G2Xg,"%5d ON ",afcount); break;
    default   : g2x_StaticPrint(50,20,G2Xr,"%5d OFF",afcount);
  }
  g2x_StaticPrint(10,40,G2Xk,"<evts>"); g2x_StaticPrint(50,40,G2Xk,"%5d",efcount);
  g2x_StaticPrint(10,60,G2Xk,"<draw>"); g2x_StaticPrint(50,60,G2Xk,"%5d",dfcount);
}

/* la fonction de sortie */
static void quit(void)
{
  qfcount++;
  /* affichage du bilan d'appels sur la sortie standard */
  fprintf(stdout,"Bilan nombre d'appels : \n\
                 - init : %d\n\
                 - ctrl : %d\n\
                 - evts : %d\n\
                 - anim : %d\n\
                 - draw : %d\n\
                 - quit : %d\n",
                 ifcount,cfcount,efcount,afcount,dfcount,qfcount);
}


/***************************************************************************/
/* La fonction principale : NE CHANGE (presque) JAMAIS                     */
/***************************************************************************/
int main(int argc, char **argv)
{
  int WWIDTH=512, WHEIGHT=512;
  g2x_InitWindow(*argv,WWIDTH,WHEIGHT);

  double wxmin=-10.,wxmax=+10.,
         wymin=-10.,wymax=+10.;
  g2x_SetWindowCoord(wxmin,wymin,wxmax,wymax);

  g2x_SetInitFunction(init); /* fonction d'initialisation */
  g2x_SetCtrlFunction(ctrl); /* fonction de contrôle      */
  g2x_SetEvtsFunction(evts); /* fonction d'événements     */
  g2x_SetDrawFunction(draw); /* fonction de dessin        */
  g2x_SetAnimFunction(anim); /* fonction d'animation      */
  g2x_SetExitFunction(quit); /* fonction de sortie        */

  return g2x_MainStart();
}
