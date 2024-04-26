/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr =!*/
/*!= Université Gustave Eiffel =!*/
/*!= Code "squelette" pour prototypage avec libg3x.6e =!*/
/*!=================================================================!*/

/* le seul #include nécessaire a priori
 * contient les libs C standards et OpenGl */
#include <g3x.h>

#include "particules.hpp"
#include "link.hpp"
#include "storage.hpp"

#include <assert.h>
#include <unistd.h>
#include <stdlib.h>

static Storage<Particule, nbParts> parts;
static Storage<Link, nbLinkInterParts> links;
static Storage<GravityLink, nbParts> gravityLinks;
static Storage<WindLink, nbParts> windLinks;

/* -----------------------------------------------------------------------
 * ici, en général pas mal de variables GLOBALES
 * - les variables de données globales (points, vecteurs....)
 * - les FLAGS de dialogues
 * - les paramètres de dialogue
 * - ......
 * Pas trop le choix, puisque TOUT passe par des fonctions <void f(void)>
 * ----------------------------------------------------------------------- */

/* la fonction d'initialisation : appelée 1 seule fois, au début */
static void init(void)
{
    srand(time(NULL));
    /* cam�ra  <g2x_camlight.h>                                */
    /* param. g�om�trique de la cam�ra. cf. gluLookAt(...)     */
    /* FACULTATIF => c'est les valeurs par d�faut              */
    g3x_SetPerspective(40., 100., 1.);
    /* position, orientation de la cam�ra en coord. sph�riques */
    /* FACULTATIF => c'est les valeurs par d�faut              */
    g3x_SetCameraSpheric(0.25 * PI, +0.25 * PI, 6.);

    /* lumi�re <g2x_camlight.h>                                */
    /* fixe les param. colorim�triques du spot lumineux        */
    /* lumiere blanche (c'est les valeurs par defaut)          */
    /* FACULTATIF => c'est les valeurs par d�faut              */
    g3x_SetLightAmbient(1., 1., 1.);
    g3x_SetLightDiffuse(1., 1., 1.);
    g3x_SetLightSpecular(1., 1., 1.);
    /* fixe la position                                        */
    /* FACULTATIF => c'est les valeurs par d�faut              */
    g3x_SetLightPosition(1., 1., 1.);

    ctr = (G3Xpoint){0., 0., 0.}; /* positionnement de centre */
    ray = .2;                     /* rayon initial */

    double startX = wxmin + 2;
    double endX = wxmax - 2;
    double coefX = abs(startX - endX) / (nbCols - 1);

    double startY = wymin + 2;
    double endY = wymax - 2;
    double coefY = abs(startY - endY) / (nbLines - 1);

    for (int i = 0; i < nbLines; i++)
    {
        for (int j = 0; j < nbCols; j++)
        {
            if (j == 0)
            {
                parts.pushElement(FixPart(startX + j * coefX, startY + i * coefY, ray, G3Xg));
            }
            else
            {
                parts.pushElement(MotionPart(startX + j * coefX, startY + i * coefY, ray, G3Xr));
            }

            auto id = (i * nbCols + j);

            // Link de gravité
            gravityLinks.pushElement(GravityLink(parts.tab + id));

            // Link de vent
            windLinks.pushElement(WindLink(parts.tab + id));
        }
    }

    for (int i = 0; i < nbLines; i++)
    {
        for (int j = 0; j < nbCols; j++)
        {
            auto id = (i * nbCols + j);
            auto idNext = id + 1;
            auto idAbove = id - nbCols;
            auto idRightDiag = idAbove + 1;
            auto idLeftDiag = idAbove - 1;

            if (j != nbCols - 1) // Not in the last column
            {
                links.pushElement(Link(parts.tab + id, parts.tab + idNext));
            }

            if (i != 0) // Not in the first line
            {
                links.pushElement(Link(parts.tab + id, parts.tab + idAbove));

                if (j != nbCols - 1)
                {
                    links.pushElement(Link(parts.tab + id, parts.tab + idRightDiag));
                }

                if (j != 0)
                {
                    links.pushElement(Link(parts.tab + id, parts.tab + idLeftDiag));
                }
            }
        }
    }
}

/* la fonction de contrôle : appelée 1 seule fois, juste APRES <init> */
static void ctrl(void)
{
    /*! Interface de dialogue (partie statique) : création des boutons, scrollbars....
    * Tout ce qu'il y a ici pourrait être directement écrit dans la fonction init(),
    * mais c'est plus 'propre' et plus pratique de séparer.
    !*/
    g3x_CreateScrollh_d("h", &h, 0.01, 1, 1, "Pas d'échantillon");
    g3x_CreateScrollh_d("z", &Link::_z, 0.0, 0.1, 1, "Indice de viscosité");
    g3x_CreateScrollh_d("k", &Link::_k, 0.05, 0.5, 1, "Indice de raideur");
    g3x_CreateScrollh_d("m", &Particule::_m, 0.5, 3., 1., "Valeur de masse");
    g3x_CreateScrollv_d("w", &WindLink::_windIntensity, -0.001f, 0.001f, 1, "Wind");
    g3x_CreateScrollv_d("g", &GravityLink::_gravityIntensity, 0, 0.001f, 1., "Gravity");

    Fe = 1. / h;
}

/* la fonction de contrôle : appelée 1 seule fois, juste APRES <init> */
static void evts(void)
{
    /*! Interface de dialogue (partie dynamique) : les touches clavier, la souris ....
    * Tout ce qu'il y a ici pourrait être directement écrit dans la fonction draw(),
    * mais c'est plus 'propre' et plus pratique de séparer.
    !*/
}

/* la fonction de dessin : appelée en boucle (indispensable) */
static void draw(void)
{
    /*! C'est la fonction de dessin principale : elle ne réalise que de l'affichage
    * sa mise à jour est automatique :
    * - si un paramètre de contrôle est modifié
    * - si la fonction <anim()> (calcul) est activée
    * ATTENTION : surtout pas d'alloc. mémoire ici !!!
    !*/

    for (int i = 0; i < parts.size; i++)
    {
        (parts.tab[i]).draw();
    }
}

/* la fonction d'animation : appelée en boucle draw/anim/draw/anim... (facultatif) */
static void anim(void)
{
    /*! C'est la fonction de 'calcul' qui va modifier les 'objets' affichés
    * par la fonction de dessin (déplacement des objets, calculs divers...)
    * Si elle n'est pas définie, c'est qu'il n'y a pas d'animation.
    * ATTENTION : surtout pas d'alloc. mémoire ici !!!
    !*/

    for (int i = 0; i < links.size; i++)
    {
        (links.tab[i]).update();
    }
    for (int i = 0; GravityLink::_gravityIntensity != 0 && i < gravityLinks.size; i++)
    {
        (gravityLinks.tab[i]).update();
    }

    for (int i = 0; WindLink::_windIntensity != 0 && i < windLinks.size; i++)
    {
        (windLinks.tab[i]).update();
    }

    for (int i = 0; i < parts.size; i++)
    {
        (parts.tab[i]).update(h);
    }

    // usleep(Fe);
}

/* la fonction de sortie (facultatif) */
static void quit(void)
{
    /*! Ici, les opérations à réaliser en sortie du programme
    * - libération de la mémoire éventuellement alloueé dans <init()>
    * - fermeture de fichiers ....
    * - bilan et messages...
    * Au final cette fonction est exécutée par un appel à <atexit()>
    !*/
}

/***************************************************************************/
/* La fonction principale : NE CHANGE (presque) JAMAIS                     */
/***************************************************************************/
int main(int argc, char **argv)
{
    /* 1�) creation de la fenetre - titre et tailles (pixels) */
    g3x_InitWindow(*argv, WWIDTH, WHEIGHT);

    /* 2�) association des fonctions */
    g3x_SetInitFunction(init); /* fonction d'initialisation */
    g3x_SetCtrlFunction(ctrl); /* fonction de contr�le      */
    g3x_SetEvtsFunction(evts); /* fonction d'�v�nements     */
    g3x_SetDrawFunction(draw); /* fonction de dessin        */
    g3x_SetAnimFunction(anim); /* fonction d'animation      */
    g3x_SetExitFunction(quit); /* fonction de sortie        */

    /* 3�) lancement de la boucle principale */
    return g3x_MainStart();
    /* RIEN APRES CA */
}