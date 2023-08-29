#pragma once

#include "Vector.hpp"

// DESIGN
// PARTICLE - A single particle that stores all the particles attributes e.g. size, position, velocity etc
// GROUPS - A group of particles that are all acted upon by the same forces. Initially empty and should take a (list of) source(s) (in the form of DOMAINS) to generate particles and sink(s) to kill particles.
//			Should set a max size, and cannot create more particles than the max allowed. This should control the advancing of the simulation for the group
// DOMAINS - Domains are used to generate particles, usually a 3/2d volume e.g. point, line, disc, sphere etc
// MODIFIER - Specifies how attributes should behave in the simulation, so this can be acted upon by physical forces e.g. gravity or something that has no physical analogy
// Utilities - Static methods to control advancing the simulation of several groups, etc

// TODO: NEW FEATURES
// TODO: Implement line and point intersection logic
// TODO: Implement camera controls
// TODO: Implement bounce modifier
// TODO: Implement avoid modifier
// TODO: Implement orbit modifier
// TODO: Implement Plane Domain
// TODO: Implement Cylinder Domain
// TODO: Implement sphere Domain
// TODO: Implement Box (3D) domain
// TODO: Implement Cone domain
// TODO: Implement Triangle domain
// TODO: Implement Rectangle (2d) domain
// TODO: Make multiple sources work correctly
// TODO: Make multiple sinks work correctly
// TODO: Make multiple domains work correctly (bounce, stick etc)
// TODO: Change the disk domain to be defined by a normal, inner & outer radius, angle range (for segments)
//
// TODO: BUGS

namespace ps
{
    class Particle
    {
    public:
        Particle() = default;
        explicit Particle(unsigned int id);
        Particle(unsigned int id, Vector3d position, Vector3d velocity);

        Vector3d position() const { return m_position; }
        void position(Vector3d position) { m_position = position; }

        Vector3d previous_position() const { return m_previous_position; }
        void previous_position(Vector3d previous_position) { m_previous_position = previous_position; }

        Vector3d velocity() const { return m_velocity; }
        void velocity(Vector3d velocity) { m_velocity = velocity; }

        Vector3i colour() const
        {
            // TODO: Make this a proper switch - for debug only!
            double increment = static_cast<double>(m_age) / static_cast<double>(m_life);
            int final_colour = 255.0 * (1.0 - increment);
            return Vector3i(final_colour, final_colour, final_colour);
            // return m_colour;
        }
        void colour(Vector3i colour) { m_colour = colour; }

        double alpha() const { return m_alpha; }
        void alpha(double alpha) { m_alpha = alpha; }

        double radius() const { return m_radius; }
        void radius(double radius) { m_radius = radius; }

        double mass() const { return m_mass; }
        void mass(double mass) { m_mass = mass; }

        unsigned int life() const { return m_life; }
        void life(unsigned int life) { m_life = life; }

        unsigned int age() const { return m_age; }
        void age(unsigned int age) { m_age = age; }

        bool visible() const { return m_visible; }
        void visible(bool visible) { m_visible = visible; }

        bool is_static() const { return m_static; }
        void set_static(bool is_static) { m_static = is_static; }

        bool is_dead() const;
        void reset();
        void birthday();

        friend std::ostream& operator<< (std::ostream& stream, const Particle& particle)
        {
            stream << "Position: " << particle.position() << "; Velocity: " << particle.velocity();
            return stream;
        }

    private:
        unsigned int m_id;
        Vector3d m_position;
        Vector3d m_previous_position;
        Vector3d m_velocity;
        Vector3i m_colour;
        double m_alpha;
        double m_radius;
        double m_mass;
        unsigned int m_life;
        unsigned int m_age;
        bool m_visible;
        bool m_static; // Indicates whether this particle is subject to the forces of the system
    };
}