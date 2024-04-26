#pragma once

#include <g3x.h>

#define MIN_LINES 2
#define MIN_COLS 2

// Parameters
constexpr int nbEdgesCorner = 4;
constexpr int nbEdgesInside = 5;
constexpr int nbLines = 50;
constexpr int nbCols = 50;

// Amounts
constexpr int nbParts = nbLines * nbCols;
constexpr int nbLinkInterParts = 2 * (nbEdgesCorner + ((nbCols - MIN_COLS) * nbEdgesInside) + nbEdgesCorner) +
                                 ((nbLines - MIN_LINES) * (nbEdgesInside * nbEdgesCorner + 4 * (nbCols - MIN_COLS)));

/* tailles de la fenêtre graphique (en pixels) */
static int WWIDTH = 512, WHEIGHT = 512;
/* limites de la zone reelle associee a la fenetre
 * ATTENTION : ces valeurs doivent être compatibles avec
 * les tailles WWIDTH et WHEIGHT
 * (wxmax-wxmin)/(wymax-wymin) = WWIDTH/WHEIGHT
 **/
static double wxmin = -10., wymin = -10., wxmax = +10., wymax = +10.;

/* variables géométriques */
static G3Xpoint ctr; /* un point : centre du cercle */
static double ray;   /* un réel : rayon du cercle */
bool state = false;

// Simulator Parameters
static double h = 1. / 100.;
static double Fe = 1. / h;

static double m = 1.;
static double k = 0.05;
static double z = 0.1;

static double gravityIntensity = 0.0001;
static double windIntensity = 0.0001;