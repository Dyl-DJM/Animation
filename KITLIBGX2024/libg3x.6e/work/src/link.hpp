#pragma once

#include "particules.hpp"
#include "config.hpp"

class Link
{
public:
    Link(Particule *left, Particule *right)
        : _leftEntry{left},
          _rightEntry{right},
          _l{0},
          _l0{distance(left->_pos, right == NULL ? left->_pos : right->_pos)},
          _type{0}
    {
    }

    Link(Particule *M)
        : _leftEntry{M},
          _rightEntry{NULL},
          _l{0},
          _l0{distance(M->_pos, M->_pos)},
          _type{1}
    {
    }

    void update()
    {
        switch (_type)
        {
        case 0:
        {
            cond_ressort_frein();
            break;
        }

        case 1:
        {
            update_external(); // Gravité
            break;
        }
        }
    }

    Particule *_leftEntry;
    Particule *_rightEntry;

    static double _k;
    static double _z;

    double _l;
    double _s;
    double _l0;
    int _type;

private:
    static double distance(G3Xpoint a, G3Xpoint b)
    {
        return sqrt(pow(b.x - a.x, 2) + pow(b.y - a.y, 2) + pow(b.z - a.z, 2));
    }

    void update_external()
    {
        _leftEntry->_f += (_constF * _leftEntry->_m);
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

double Link::_k = initialK;
double Link::_z = initialZ;

class GravityLink : public Link
{
public:
    GravityLink(Particule *M) : Link(M)
    {
    }

    void update()
    {
        _constF = G3Xvector{0.f, -_gravityIntensity, 0.f};
        Link::update();
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

    void update()
    {
        _leftEntry->_f += (G3Xvector{0.f, 0.f, -_windIntensity} * 1 / _leftEntry->_m);
    }

    static double _windIntensity;
};

double WindLink::_windIntensity = 0.f;