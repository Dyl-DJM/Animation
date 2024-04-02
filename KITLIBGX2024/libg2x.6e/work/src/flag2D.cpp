/*!=================================================================!*/
/*!= E.Incerti - eric.incerti@univ-eiffel.fr =!*/
/*!= Université Gustave Eiffel =!*/
/*!= Code "squelette" pour prototypage avec libg2x.6e =!*/
/*!=================================================================!*/

/* le seul #include nécessaire a priori
 * contient les libs C standards et OpenGl */
#include <g2x.h>

#include <assert.h>

#define MIN_LINES 2
#define MIN_COLS 2

int computeAmountLinks(int nbLines, int nbCols);

// Parameters
constexpr int nbEdgesCorner = 2;
constexpr int nbEdgesInside = 3;
constexpr int nbLines = 13;
constexpr int nbCols = 17;

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
static G2Xpoint ctr; /* un point : centre du cercle */
static double ray;   /* un réel : rayon du cercle */

// Simulator Parameters

static double Fe = 100;
static double h = 1 / Fe;
static double m = 1.;
static double k = 0.001;
static double z = 0.0001;

// int computeAmountLinks(int nbLines, int nbCols){
// int firstLine = nbEdgesCorner + ((nbCols - MIN_COLS) * nbEdgesInside) + nbEdgesCorner;
// int otherLines = (nbLines - MIN_LINES) * (nbEdgesInside * nbEdgesCorner + 4 * (nbCols - MIN_COLS));
// int lastLine = firstLine;

// return firstLine + otherLines + lastLine;
// }

G2Xvector operator*(float scalar, const G2Xvector &vec)
{
    G2Xvector res;
    res.x = scalar * vec.x;
    res.y = scalar * vec.y;
    return res;
}

G2Xvector operator*(const G2Xvector &vec, float scalar)
{
    G2Xvector res;
    res.x = scalar * vec.x;
    res.y = scalar * vec.y;
    return res;
}

G2Xvector operator/(const G2Xvector &vec, double scalar)
{
    G2Xvector res;
    res.x = vec.x / scalar;
    res.y = vec.y / scalar;
    return res;
}

G2Xvector operator+(const G2Xvector &vec1, const G2Xvector &vec2)
{
    G2Xvector res;
    res.x = vec1.x + vec2.x;
    res.y = vec1.y + vec2.y;
    return res;
}

G2Xvector operator-(const G2Xvector &first, const G2Xvector &second)
{
    G2Xvector res;
    res.x = first.x - second.x;
    res.y = first.y - second.y;
    return res;
}

G2Xvector &operator+=(G2Xvector &first, const G2Xvector &second)
{
    first.x += second.x;
    first.y += second.y;
    return first;
}

G2Xvector operator+=(const G2Xvector &vec, double scalar)
{
    G2Xvector res;
    res.x = vec.x + scalar;
    res.y = vec.y + scalar;
    return res;
}

G2Xvector &operator-=(G2Xvector &first, const G2Xvector &second)
{
    first.x -= second.x;
    first.y -= second.y;
    return first;
}

G2Xvector &operator-=(G2Xvector &first, float scalar)
{
    first.x -= scalar;
    first.y -= scalar;
    return first;
}

class Particule
{
protected:
    Particule(double x, double y, double radius, G2Xcolor col, bool motion)
        : _type{0}, _col{col}, _rad{radius}, _pos{(G2Xpoint){x, y}}, _m{m}, _f{G2Xvector{0.f, 0.f}}, _speed{G2Xvector{0.f, 0.f}}, _motion{motion}
    {
    }

public:
    void draw()
    {
        g2x_DrawFillCircle(_pos, _rad, _col);
    }

    void update(float h)
    {
        if (!_motion)
        {
            _speed = G2Xvector{0.f, 0.f}; // intégration 1 : vitesse m.F(n) = (V(n+1)-V(n))/h -EXplicite
            _f = G2Xvector{0.f, 0.f};
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
        _speed += (h * _f / _m);  // intégration 1 : vitesse m.F(n) = (V(n+1)-V(n))/h -EXplicite
        _pos += (h * _speed);     // intégration 2 : position V(n+1) = (X(n+1)-X(n))/h -IMplicite
        _f = G2Xvector{0.f, 0.f}; // on vide le buffer de force
    }

    // intégrateur Euler Explicite
    // juste pour l’exemple : méthode très instable -> à éviter
    // simple échange des 2 premières lignes par rapport à Leapfrog -> ça change tout
    void update_euler_exp(float h)
    {
        _pos += (h * _speed);     // intégration 1 : position V(n) = (X(n+1)-X(n-1))/h -EXplicite
        _speed += (h * _f / _m);  // intégration 2 : vitesse m.F(n) = (V(n+1)-V(n))/h -EXplicite
        _f = G2Xvector{0.f, 0.f}; // on vide le buffer de force
    }

    int _type;
    G2Xcolor _col;
    double _rad = 0.;

    // Mot physique
public:
    G2Xpoint _pos;
    double _m;
    G2Xvector _f;
    G2Xvector _speed;

    bool _motion;
};

class FixPart : public Particule
{
public:
    FixPart(double x, double y, double radius, G2Xcolor col) : Particule(x, y, radius, col, false){};

    void update(float h)
    {
        return;
    }
};

class MotionPart : public Particule
{
public:
    MotionPart(double x, double y, double radius, G2Xcolor col) : Particule(x, y, radius, col, true){};
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
            hook();
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
        G2Xpoint secondPoint;
        if (_rightEntry == NULL)
        {
            secondPoint = _leftEntry->_pos + (10000 * constF);
        }
        else
        {
            secondPoint = _rightEntry->_pos;
        }
        g2x_DrawLine(_leftEntry->_pos, secondPoint, G2Xb, 2);
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
    static double distance(G2Xpoint a, G2Xpoint b)
    {
        return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
    }

    void update_external(float m)
    {
        _leftEntry->_f += (constF * m);
    }

    void hook()
    {
        double d = distance(_rightEntry->_pos, _leftEntry->_pos);
        G2Xvector u = (_rightEntry->_pos - _leftEntry->_pos) / d;
        G2Xvector f = _k * (d - _l0) * u;
        _leftEntry->_f += f;
        _rightEntry->_f -= f;
    }

    void frein_cinetique()
    {
        G2Xvector speed_diff = _rightEntry->_speed - _leftEntry->_speed;
        G2Xvector f = -_z * speed_diff;
        _leftEntry->_f -= f;
        _rightEntry->_f += f;
    }

    void ressort_frein()
    {
        double d = distance(_leftEntry->_pos, _rightEntry->_pos);
        G2Xvector speed_diff = _rightEntry->_speed - _leftEntry->_speed;
        G2Xvector u = (_rightEntry->_pos - _leftEntry->_pos) / d;
        G2Xvector f = -_k * (d - _l0) * u - _z * speed_diff;
        _leftEntry->_f -= f;
        _rightEntry->_f += f;
    }

    void cond_ressort_frein()
    {
        double d = distance(_rightEntry->_pos, _leftEntry->_pos);
        if (d > _s * _l0)
        {
            return;
        }
        G2Xvector speed_diff = _rightEntry->_speed - _leftEntry->_speed;
        G2Xvector u = (_rightEntry->_pos - _leftEntry->_pos) / d;
        G2Xvector f = -_k * (d - _l0) * u - (_z * speed_diff);
        _leftEntry->_f -= f;
        _rightEntry->_f += f;
    }

    static G2Xvector constF;
};

G2Xvector Link::constF = G2Xvector{0, -0.0001f};

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
    /*! Ici, pas de dessin, pas de calcul dynamique, rien que des initialisations
    * -> allocation(s) de mémoire, init. des paramètres globaux,
    * construction de modèles....
    * ouverture de fichiers, pré-chargement de données....
    *
    * Tout ce qu'il y a ici pourrait être écrit directement dans le main()
    * juste après l'appel à g2x_InitWindow()
    !*/

    g2x_SetWindowCoord(wxmin, wymin, wxmax, wymax);

    ctr = (G2Xpoint){0., 0.}; /* positionnement de centre */
    ray = .2;                 /* rayon initial */

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
                parts.pushElement(FixPart(startX + j * coefX, startY + i * coefY, ray, G2Xg));
            }
            else
            {
                parts.pushElement(MotionPart(startX + j * coefX, startY + i * coefY, ray, G2Xr));
            }

            auto id = (i * nbCols + j);

            // Link de gravité
            links.pushElement(Link(parts.tab + id));
        }
    }

    for (int i = 0; i < nbLines; i++)
    {
        for (int j = 0; j < nbCols; j++)
        {
            auto id = (i * nbCols + j);
            auto idNext = id + 1;
            auto idAbove = id - nbCols;

            if (j != nbCols - 1)
            {
                links.pushElement(Link(parts.tab + id, parts.tab + idNext));
            }

            if (i != 0)
            {
                links.pushElement(Link(parts.tab + id, parts.tab + idAbove));
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
    g2x_CreateScrollh_d("h", &h, (1 / Fe), (1 / Fe) * 100, "Indice de raideur");
    g2x_CreateScrollh_d("z", &z, 0.0001, 0.1, "Indice de viscosité");
    g2x_CreateScrollh_d("m", &m, 0, 10., "Valeur de masse");
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

    for (int i = 0; i < links.size; i++)
    {
        (links.tab[i]).draw();
    }

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
        (links.tab[i]).update(z, m);
    }
    for (int i = 0; i < parts.size; i++)
    {
        (parts.tab[i]).update(h);
    }
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
/* La fonction principale : NE CHANGE (presque) JAMAIS */
/***************************************************************************/
int main(int argc, char **argv)
{

    char *title = "Rope Simulation";

    /* 1°) creation de la fenetre - titre et tailles (pixels) */
    g2x_InitWindow(title, WWIDTH, WHEIGHT);
    /* 2°) définition de la zone de travail en coord. réeelles *
     * ATTENTION : veiller à respecter les proportions
     * (wxmax-wxmin)/(wymax-wymin) = WWIDTH/WHEIGHT
     * si cette fonction n'est pas appelée, la zone réelle
     * par défaut est wxmin=0., wymin=0., wxmax=WWIDTH, wymax=WHEIGHT
     */
    g2x_SetWindowCoord(wxmin, wymin, wxmax, wymax);

    /* 3°) association des fonctions */
    g2x_SetInitFunction(init); /* fonction d'initialisation */
    g2x_SetCtrlFunction(ctrl); /* fonction de contrôle */
    g2x_SetEvtsFunction(evts); /* fonction d'événements */
    g2x_SetDrawFunction(draw); /* fonction de dessin */
    g2x_SetAnimFunction(anim); /* fonction d'animation */
    g2x_SetExitFunction(quit); /* fonction de sortie */

    /* 4°) lancement de la boucle principale */
    return g2x_MainStart();
    /* RIEN APRES CA */
}