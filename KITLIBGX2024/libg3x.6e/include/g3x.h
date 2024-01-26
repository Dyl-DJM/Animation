/**@file    g3x.h
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

#ifndef _LIB_G3X_
  #define _LIB_G3X_
/**@endcond                   */

  /* les lib. standards /usr/lib */
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <ctype.h>
  #include <time.h>
  #include <math.h>
  #ifndef WIN32
    #include <sys/times.h>
  #else
    #include <times.h>
  #endif
  #include <sys/time.h>

  /* FreeGlut /usr/lib
   * si nécessaire, installer freeglut (VERSION 3-dev) :
   * $> sudo apt-get install freeglut3-dev
   **/
  #include <GL/freeglut.h>

  /* les libs. locales        */
  /* les basiques             */
  #include <g3x_types.h>      /* types primaires et macros diverses        */
  #include <g3x_geom.h>       /* primitives et opérateurs géométriques     */
  #include <g3x_colors.h>     /* gestions des couleurs                     */
  #include <g3x_tools.h>      /* utilitaires divers                        */
  #include <g3x_control.h>    /* gestion des boutons, scroll, souris...    */
  #include <g3x_transfo.h>    /* transfo. en coordonnees homogenes 4x4     */
  #include <g3x_camlight.h>   /* gestion de la caméra et de la lumière     */

  #include <g3x_window.h>     /* routines et fonctions de base             */

  /* les "peripheriques"      */
  #include <g3x_pnm.h>        /* image au format PNM                       */
  #include <g3x_capture.h>    /* routines de capture d'ecran image/video   */

  /* les "haut niveau"         */
  #include <g3x_transfo.h>     /* transfo. en coordonnees homogenes 4x4     */
  #include <g3x_particule.h>   /* particules "cinématique" (pos.,dir.,norme)*/
  #include <g3x_quaternions.h> /* quaternions                               */

  #endif

#ifdef __cplusplus
  }
#endif
/*==============================================================!*/
