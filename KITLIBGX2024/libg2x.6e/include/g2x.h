/**@file    g2x.h
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

#ifndef _LIB_G2X_
  #define _LIB_G2X_
/**@endcond                   */

  /* les lib. standards /usr/lib */
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <ctype.h>
  #include <time.h>
  #include <math.h>
  #include <dirent.h>
  #ifndef WIN32
    #include <sys/times.h>
  #else
    #include <times.h>
  #endif
  #include <sys/time.h>

  /* FreeGlut3 /usr/lib
   * si nécessaire, installer freeglut (VERSION 3-dev) :
   * $> sudo apt-get install freeglut3-dev
   **/
  #include <GL/freeglut.h>

  /* les libs. locales        */
  /* les basiques             */
  #include <g2x_types.h>      /* types primaires et macros diverses        */
  #include <g2x_geom.h>       /* primitives et opérateurs géométriques     */
  #include <g2x_tools.h>      /* utilitaires divers                        */
  #include <g2x_colors.h>     /* gestions des couleurs                     */
  #include <g2x_control.h>    /* gestion des boutons, scroll, souris...    */
  #include <g2x_window.h>     /* routines et fonctions de base             */

  /* les "haut niveau"        */
  #include <g2x_draw.h>       /* routines de tracé de primitives           */
  #include <g2x_transfo.h>    /* transfo. en coordonnees homogenes 3x3     */
  #include <g2x_geoalgo.h>    /* quelques algo. geometriques classiques    */
  #include <g2x_polygon.h>    /* manipulation de polygones (listes)        */
  #include <g2x_particle.h>
  #include <g2x_board.h>


  /* les "peripheriques"      */
  #include <g2x_pixmap.h>     /* images au format PNM                      */
  #include <g2x_capture.h>    /* routines de capture d'ecran image/video   */

#endif

#ifdef __cplusplus
  }
#endif
/*!=============================================================!*/
