#include "LineDomain.hpp"
#include "Random.hpp"

namespace ps
{
    LineDomain::LineDomain(Vector3d begin, Vector3d end) : m_begin(begin), m_end(end)
    {
    }

    Vector3d LineDomain::GeneratePosition()
    {
        Vector3d line_dir = m_end - m_begin;
        double random_max = line_dir.norm();
        line_dir.normalise();
        double random_min = 0.0;
        double random_length = Random::InRange(random_min, random_max);
        Vector3d new_pt = m_begin + random_length * line_dir;
        return new_pt;
    }

    Vector3d LineDomain::GenerateVelocity(Vector3d start_position, double speed)
    {
        Vector3d pos_for_vel = GeneratePosition();
        Vector3d velocity_vector(pos_for_vel - start_position);
        velocity_vector.normalise();
        velocity_vector *= speed;
        return velocity_vector;
    }

    bool LineDomain::HasEnteredDomain(const Particle& particle) const
    {
        // TODO: Implement intersection detection here
        // Intersection point
        // Px = ((x1*y2 - y1*x2)(x3 - x4) - (x1 - x2)(x3*y4 - y3*x4))/((x1 - x2)(y3 - y4) - (y1 - y2)(x3 - x4))
        // Py = ((x1*y2 - y1*x2)(y3 - y4) - (y1 - y2)(x3*y4 - y3*x4))/((x1 - x2)(y3 - y4) - (y1 - y2)(x3 - x4))
        //double intersect_x =
        return false;
    }

    Vector3d LineDomain::Intersection(const Particle& particle) const
    {
        // TODO: Implement intersection calculation here
        return Vector3d();
    }

    Vector3d LineDomain::Normal()
    {
        // TODO: Implement normal calculation here
        return Vector3d();
    }

    void LineDomain::Draw(IDomainRenderer& renderer)
    {
        renderer.Draw(this);
    }
}