#define _USE_MATH_DEFINES
#include <cmath>

#include "DiskDomain.hpp"
#include "DomainExceptions.hpp"
#include "Random.hpp"

namespace ps
{
    DiskDomain::DiskDomain(Vector3d centre, double inner_radius, double outer_radius, double zenith, double azimuth) :
        m_disc_centre(centre), m_inner_radius(inner_radius), m_outer_radius(outer_radius), m_theta(zenith), m_phi(azimuth)
    {
    }

    Vector3d DiskDomain::GeneratePosition()
    {
        // For OpenGL to get a flat circle with normal facing upwards (y direction in OGL)
        // we must choose zenith = M_PI / 2, and azimuth = M_PI / 2

        // Check first to see if it's a circle or an annulus
        double random_radius;
        if (m_inner_radius == 0.0f) {
            random_radius = Random::InRange(0.0f, m_outer_radius);
        } else {
            random_radius = Random::InRange(m_inner_radius, m_outer_radius);
        }

        // Generate random angle
        double random_angle = Random::InRange(0.0, 2 * M_PI);
        return GeneratePosition(random_radius, random_angle);
    }

    Vector3d DiskDomain::GenerateVelocity(Vector3d start_position, double speed)
    {
        Vector3d pos_for_vel = GeneratePosition();
        Vector3d velocity_vector(pos_for_vel - start_position);
        velocity_vector.normalise();
        velocity_vector *= speed;
        return velocity_vector;
    }

    bool DiskDomain::HasEnteredDomain(const Particle &particle) const
    {
        try {
            Vector3d intersection = Intersection(particle);
            const double radius_from_centre = (intersection - m_disc_centre).norm();
            return radius_from_centre > m_inner_radius && radius_from_centre < m_outer_radius;
        }
        catch (NoDomainIntersectionException& e) {
            return false;
        }
    }

    // Throws exception on no intersection
    Vector3d DiskDomain::Intersection(const Particle& particle) const
    {
        // 0 = disjoint(no intersection)
        // 1 =  intersection in the unique point *I0
        // 2 = the  segment lies in the plane
        Vector3d start = particle.previous_position();
        Vector3d end = particle.position();
        Vector3d normal(cos(m_phi) * sin(m_theta), sin(m_theta) * sin(m_phi), cos(m_theta));

        // plane point = m_disc_centre
        Vector3d u = end - start;
        Vector3d w = start - m_disc_centre;

        double D = inner_product(normal, u);
        double N = -inner_product(normal, w);

        if (std::abs(D) < 0.00000001) // segment is parallel to plane
        {
            if (N == 0) {
                // segment lies in plane (2)
                return particle.position();
            }
            throw NoDomainIntersectionException(); // no intersection (0)
        }
        // they are not parallel
        // compute intersect param
        const double sI = N / D;
        if (sI < 0 || sI > 1) {
            throw NoDomainIntersectionException(); // no intersection (0)
        }
        // intersection in the unique point(1)
        Vector3d intersect_point = start + sI * u; // compute segment intersect point
        return intersect_point;
    }

    Vector3d DiskDomain::Normal()
    {
        return {cos(m_phi) * sin(m_theta), sin(m_theta) * sin(m_phi), cos(m_theta)};
    }

    void DiskDomain::Draw(IDomainRenderer& renderer)
    {
        renderer.Draw(this);
    }

    Vector3d DiskDomain::GeneratePosition(double radius, double angle) const
    {
        // Generate cartesian coordinates - x = sqrt(r) * cos(th), y = sqrt(r) * sin(th)
        Vector3d new_pos(radius * cos(angle), radius * sin(angle), 0.0);

        // Calculate the normal and perp vectors
        // vector3d normal(cos(m_phi)*sin(m_theta), sin(m_theta)*sin(m_phi), cos(m_theta));
        Vector3d perp(-sin(m_phi), cos(m_phi), 0);
        Vector3d ncrossp(cos(m_theta) * cos(m_phi), cos(m_theta) * sin(m_phi), -sin(m_theta));

        // Calculate the final position
        Vector3d newpos(radius * cos(angle) * perp + radius * sin(angle) * ncrossp + m_disc_centre);
        return newpos;
    }
}