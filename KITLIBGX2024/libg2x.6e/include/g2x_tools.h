/**@file    g2x_tools.h
 * @author  Universite Gustave Eiffel
 * @author  E.Incerti - eric.incerti@univ-eiffel.fr
 * @brief   Base public control functions
 * @version 6.e
 * @date    Aug.2022 (doc generation)
 */
/**@cond SHOW_SECURITY_DEFINE */

#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#ifndef _G2X_TOOLS_H_
  #define _G2X_TOOLS_H_
/**@endcond                   */

  #include <g2x.h>

  /* horloge */
  typedef struct
  {
    uint hh; /* heure    [0,23]  */
    uint mm; /* minutes  [0,59]  */
    uint ss; /* secondes [0,59]  */
    uint ds; /* dizieme  de sec. */
    uint cs; /* centieme de sec. */
    uint ms; /* millieme de sec. */
  } G2Xclock;

  /* calcule le temps "processeur" (cumul des threads) */
  /* retour au format hh:mm:ss:cs:ms                   */
  char*  g2x_ProcTimer(void);
  /* chrono "temps réel"                               */
  char*  g2x_Chrono(void);
  /* récupère le chrono courant                        */
  void   g2x_GetTime(G2Xclock* clock);

  /*!****************************************************************************!*/
  /*! générateurs aléatoires                                                     !*/
  /*! DANS CETTE VERSION, C'EST LA GRAINE PAR DEFAUT                             !*/
  /*! => à l'utilisateur de faire appel à rand(seed)                             !*/
  /*!****************************************************************************!*/
  /* intervalle [root*(1.-percent), root*(1.+percent)] */
  double g2x_Rand_Percent(double root, double percent);
  /* intervalle [root-delta, root+delta]               */
  double g2x_Rand_Delta(double root, double delta);
  /* intervalle [min, max]                             */
  double g2x_Rand_MinMax(double min, double max);

 #endif

#ifdef __cplusplus
  }
#endif
/*!=============================================================!*/
