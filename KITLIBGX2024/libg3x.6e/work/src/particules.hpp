#pragma once

#include "tools.hpp"
#include "config.hpp"

class Particule
{
protected:
    Particule(double x, double y, double radius, G3Xcolor col, bool motion)
        : _type{0}, _col{col}, _rad{radius}, _pos{(G3Xpoint){x, y}}, _f{G3Xvector{0.f, 0.f, 0.f}}, _speed{G3Xvector{0.f, 0.f, 0.f}}, _motion{motion}
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
    static double _m;
    G3Xvector _f;
    G3Xvector _speed;

    bool _motion;
};

double Particule::_m = initialM;

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