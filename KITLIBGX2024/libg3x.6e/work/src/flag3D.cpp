/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr =!*/
/*!= Université Gustave Eiffel =!*/
/*!= Code "squelette" pour prototypage avec libg3x.6e =!*/
/*!=================================================================!*/

/* le seul #include nécessaire a priori
 * contient les libs C standards et OpenGl */
#include <g3x.h>

#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define MIN_LINES 2
#define MIN_COLS 2

int computeAmountLinks(int nbLines, int nbCols);

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

G3Xvector operator*(float scalar, const G3Xvector &vec)
{
    G3Xvector res;
    res.x = scalar * vec.x;
    res.y = scalar * vec.y;
    res.z = scalar * vec.z;
    return res;
}

G3Xvector operator*(const G3Xvector &vec, float scalar)
{
    G3Xvector res;
    res.x = scalar * vec.x;
    res.y = scalar * vec.y;
    res.z = scalar * vec.z;
    return res;
}

G3Xvector operator/(const G3Xvector &vec, double scalar)
{
    G3Xvector res;
    res.x = vec.x / scalar;
    res.y = vec.y / scalar;
    res.z = vec.z / scalar;
    return res;
}

G3Xvector operator+(const G3Xvector &vec1, const G3Xvector &vec2)
{
    G3Xvector res;
    res.x = vec1.x + vec2.x;
    res.y = vec1.y + vec2.y;
    res.z = vec1.z + vec2.z;
    return res;
}

G3Xvector operator-(const G3Xvector &first, const G3Xvector &second)
{
    G3Xvector res;
    res.x = first.x - second.x;
    res.y = first.y - second.y;
    res.z = first.z - second.z;
    return res;
}

G3Xvector &operator+=(G3Xvector &first, const G3Xvector &second)
{
    first.x += second.x;
    first.y += second.y;
    first.z += second.z;
    return first;
}

G3Xvector operator+=(const G3Xvector &vec, double scalar)
{
    G3Xvector res;
    res.x = vec.x + scalar;
    res.y = vec.y + scalar;
    res.z = vec.z + scalar;
    return res;
}

G3Xvector &operator-=(G3Xvector &first, const G3Xvector &second)
{
    first.x -= second.x;
    first.y -= second.y;
    first.z -= second.z;
    return first;
}

G3Xvector &operator-=(G3Xvector &first, float scalar)
{
    first.x -= scalar;
    first.y -= scalar;
    first.z -= scalar;
    return first;
}

class Particule
{
protected:
    Particule(double x, double y, double radius, G3Xcolor col, bool motion)
        : _type{0}, _col{col}, _rad{radius}, _pos{(G3Xpoint){x, y}}, _m{m}, _f{G3Xvector{0.f, 0.f, 0.f}}, _speed{G3Xvector{0.f, 0.f, 0.f}}, _motion{motion}
    {
    }

public:
    void draw()
    {
        glPushMatrix();
        glTranslatef(_pos.x, _pos.y, _pos.z);
        glEnable(GL_BLEND); /* active le mode 'transparence' (blending) */
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        /* param�tres de couleur/mati�re de l'objet <g3x_colors.h> */
        /* alternative � la m�thode OpenGL, peu intuitive          */
        g3x_Material(_col, 0.8, 0.5, 2, 2, 1.);
        glutSolidSphere(_rad, 40, 40); /* une sphere (cf. manuel OpenGl) */
        glDisable(GL_BLEND);           /* d�sactive le mode blending */
        glPopMatrix();
    }

    void addForce(G3Xvector addedForce)
    {
        printf("Here\n");
        _f += addedForce;
    }

    void update(float h)
    {
        if (!_motion)
        {
            _speed = G3Xvector{0.f, 0.f, 0.f}; // intégration 1 : vitesse m.F(n) = (V(n+1)-V(n))/h -EXplicite
            _f = G3Xvector{0.f, 0.f, 0.f};
            return;
        }

        switch (_type)
        {
        case 0:
        {
            update_leapfrog(h);
            break;
        }
        case 1:
        {
            update_euler_exp(h);
            break;
        }
        }
    }

private:
    void update_leapfrog(float h)
    {
        _speed += (h * _f / _m);       // intégration 1 : vitesse m.F(n) = (V(n+1)-V(n))/h -EXplicite
        _pos += (h * _speed);          // intégration 2 : position V(n+1) = (X(n+1)-X(n))/h -IMplicite
        _f = G3Xvector{0.f, 0.f, 0.f}; // on vide le buffer de force
    }

    // intégrateur Euler Explicite
    // juste pour l’exemple : méthode très instable -> à éviter
    // simple échange des 2 premières lignes par rapport à Leapfrog -> ça change tout
    void update_euler_exp(float h)
    {
        _pos += (h * _speed);          // intégration 1 : position V(n) = (X(n+1)-X(n-1))/h -EXplicite
        _speed += (h * _f / _m);       // intégration 2 : vitesse m.F(n) = (V(n+1)-V(n))/h -EXplicite
        _f = G3Xvector{0.f, 0.f, 0.f}; // on vide le buffer de force
    }

    int _type;
    G3Xcolor _col;
    double _rad = 0.;

    // Mot physique
public:
    G3Xpoint _pos;
    double _m;
    G3Xvector _f;
    G3Xvector _speed;

    bool _motion;
};

class FixPart : public Particule
{
public:
    FixPart(double x, double y, double radius, G3Xcolor col) : Particule(x, y, radius, col, false){};

    void update(float h)
    {
        return;
    }
};

class MotionPart : public Particule
{
public:
    MotionPart(double x, double y, double radius, G3Xcolor col) : Particule(x, y, radius, col, true){};
};

class Link
{
public:
    Link(Particule *left, Particule *right)
        : _leftEntry{left},
          _rightEntry{right},
          _k{k},
          _l{0},
          _l0{distance(left->_pos, right == NULL ? left->_pos : right->_pos)},
          _type{0}
    {
    }

    Link(Particule *M)
        : _leftEntry{M},
          _rightEntry{NULL},
          _k{k},
          _l{0},
          _l0{distance(M->_pos, M->_pos)},
          _type{1}
    {
    }

    void update(float z, float m)
    {
        _z = z;

        switch (_type)
        {
        case 0:
        {
            cond_ressort_frein();
            break;
        }

        case 1:
        {
            update_external(m); // Gravité
            break;
        }
        }
    }

    void draw()
    {
        G3Xpoint secondPoint;
        // if (_rightEntry == NULL)
        // {
        //     secondPoint = _leftEntry->_pos + (10000 * constF);
        // }
        // else
        // {
        //     secondPoint = _rightEntry->_pos;
        // }
        // g3x_DrawLine(_leftEntry->_pos, secondPoint, G3Xb, 2);
    }

    Particule *_leftEntry;
    Particule *_rightEntry;

    double _k;
    double _l;
    double _z;
    double _s;
    double _l0;
    int _type;

private:
    static double distance(G3Xpoint a, G3Xpoint b)
    {
        return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
    }

    void update_external(float m)
    {
        _leftEntry->_f += (_constF * m);
    }

    void hook()
    {
        double d = distance(_rightEntry->_pos, _leftEntry->_pos);
        G3Xvector u = (_rightEntry->_pos - _leftEntry->_pos) / d;
        G3Xvector f = _k * (d - _l0) * u;
        _leftEntry->_f += f;
        _rightEntry->_f -= f;
    }

    void frein_cinetique()
    {
        G3Xvector speed_diff = _rightEntry->_speed - _leftEntry->_speed;
        G3Xvector f = -_z * speed_diff;
        _leftEntry->_f -= f;
        _rightEntry->_f += f;
    }

    void ressort_frein()
    {
        double d = distance(_leftEntry->_pos, _rightEntry->_pos);
        G3Xvector speed_diff = _rightEntry->_speed - _leftEntry->_speed;
        G3Xvector u = (_rightEntry->_pos - _leftEntry->_pos) / d;
        G3Xvector f = -_k * (d - _l0) * u - _z * speed_diff;
        _leftEntry->_f -= f;
        _rightEntry->_f += f;
    }

    void cond_ressort_frein()
    {
        double d = distance(_rightEntry->_pos, _leftEntry->_pos);
        if (d < _s * _l0)
        {
            return;
        }
        G3Xvector speed_diff = _rightEntry->_speed - _leftEntry->_speed;
        G3Xvector u = (_rightEntry->_pos - _leftEntry->_pos) / d;
        G3Xvector f = -_k * (d - _l0) * u - (_z * speed_diff);
        _leftEntry->_f -= f;
        _rightEntry->_f += f;
    }

protected:
    G3Xvector _constF = G3Xvector{0.f, 0.f, 0.f};
};

class GravityLink : public Link
{
public:
    GravityLink(Particule *M) : Link(M)
    {
    }

    void update(float a, float b)
    {
        _constF = G3Xvector{0.f, -_gravityIntensity, 0.f};
        Link::update(a, b);
    }

    static double _gravityIntensity;
};

double GravityLink::_gravityIntensity = 0.0001f;

class WindLink : public Link
{
public:
    WindLink(Particule *M) : Link(M)
    {
    }

    void update(float a, float b)
    {
        _leftEntry->_f += (G3Xvector{0.f, 0.f, -_windIntensity} * 1 / m);
    }

    static double _windIntensity;
};

double WindLink::_windIntensity = 0.f;

template <class T, std::size_t MaxSize>
struct Storage
{

    Storage()
        : tab{(T *)malloc(sizeof(T) * MaxSize)}
    {
    }

    const T &operator[](std::size_t i) const
    {
        assert(i < size && "Index out of range.\n");
        return tab[i];
    }

    T &operator[](std::size_t i)
    {
        assert(i < size && "Index out of range.\n");
        return tab[i];
        stackLastIdx = i > stackLastIdx ? i : stackLastIdx;
    }

    void pushElement(T elem)
    {
        size++;
        assert(size <= MaxSize && "Couldn't add more elements.\n");
        assert(stackLastIdx < size && "Index out of range.\n");
        tab[stackLastIdx] = elem;
        stackLastIdx++;
    }

    unsigned int stackLastIdx = 0;
    unsigned int size = 0;
    T *tab;
};

static Storage<Particule, nbParts> parts;
static Storage<Link, nbLinkInterParts> links;
static Storage<GravityLink, nbParts> gravityLinks;
static Storage<WindLink, nbParts> windLinks;

void applyWindParts(int intensity, float force)
{
    for (int i = 0; i <= intensity; i++)
    {
        int x = rand() % nbCols;
        int y = rand() % nbLines;
        auto id = (y * nbCols + x);

        parts[id].addForce(G3Xvector{0.f, force, 0.f});
    }
}

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
    g3x_CreateScrollh_d("z", &z, 0.0, 0.1, 1, "Indice de viscosité");
    g3x_CreateScrollh_d("k", &k, 0.05, 0.5, 1, "Indice de raideur");
    g3x_CreateScrollh_d("m", &m, 0.5, 3., 1., "Valeur de masse");
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

    usleep(Fe);
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
        (links.tab[i]).update(z, m);
    }
    for (int i = 0; i < gravityLinks.size; i++)
    {
        (gravityLinks.tab[i]).update(z, m);
    }

    for (int i = 0; i < windLinks.size; i++)
    {
        (windLinks.tab[i]).update(z, m);
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