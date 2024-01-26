/*!===============================================================
  E.Incerti - Universite Gustave Eiffel - eric.incerti@univ-eiffel.fr
       - Librairie G3X - Fonctions de base d'acces public -
                    version 6e - Jan.2022
  ================================================================!*/

#ifdef __cplusplus
  extern "C" {
#else
  #define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include <g3x_types.h>
#include <g3x_utils.h>
#include <g3x_octree.h>

extern bool g3x_Octree_Init(G3Xoctree* oct, int nblevel)
{
  G3Xoctree *octson=NULL
  static int curlev=0;

  oct->lev = nblevel;
  oct->length=((uint)1)<<(3*curlev);
  /* tableau de noeuds */
  if (!(oct->node=(G3Xocnode*)calloc(oct->length,sizeof(G3Xocnode))))
  {
    fprintf(stderr,"\e[31mg3x_Octree_Init(%d)\e[33m ErrAlloc: oct->node[%d] -\e[0m\n",nblevel,oct->length);
    return false;
  }
  /* si on est au fond */
  if (--nblevel==0) return true;

  /* sinon : creation du niveau inferieur */
  if (!(oct->son=(G3Xoctree*)calloc(1,sizeof(G3Xoctree))))
  {
    fprintf(stderr,"\e[31mg3x_Octree_Init(%d)\e[33m ErrAlloc: oct->son -\e[0m\n",nblevel);
    return false;
  }
  /* construction */
  ++curlev;
  if (!(g3x_Octree_Init(&oct->son,nblevel-1))) return false;
  /* chaînage */
  oct->son->dad = oct;

  /* chaînage des fils */
  G3Xnode *dadnode=oct->node;
  G3Xnode *sonnode=oct->son->node;

  while (dadnode<oct->node+oct->length)
  {
    dadnode->son = sonnode;
    dadnode++;
    sonnode+=8;
  }
}

#ifdef __cplusplus
  }
#endif
