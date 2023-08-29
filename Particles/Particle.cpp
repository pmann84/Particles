#include "Particle.hpp"

namespace ps
{
    Particle::Particle(unsigned int id) : Particle(id, Vector3d(), Vector3d())
    {
    }

    Particle::Particle(unsigned int id, Vector3d position, Vector3d velocity) :
        m_id(id),
        m_position(position),
        m_velocity(velocity),
        m_colour(255, 255, 255),
        m_alpha(0.0),
        m_radius(1.0),
        m_mass(1.0),
        m_life(0),
        m_age(0),
        m_visible(true),
        m_static(false)
    {
    }

    bool Particle::is_dead() const
    {
        return m_age >= m_life;
    }

    void Particle::reset()
    {
        m_age = 0;
        m_life = 0;
    }

    void Particle::birthday()
    {
        m_age += 1;
    }
}