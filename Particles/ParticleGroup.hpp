#pragma once

#include <vector>
#include <memory>
#include <random>
#include <iostream>
#include <algorithm>

#include "Vector.hpp"
#include "Particle.hpp"
#include "IDomain.hpp"
#include "Random.hpp"

namespace ps
{
    using domain_ptr = std::shared_ptr<IDomain>;

    class ParticleGroup
    {
    public:
        ParticleGroup();
        explicit ParticleGroup(unsigned int max_particles, unsigned int particle_emission_rate);

        void max_particles(int max) { m_max_particles = max; }
        unsigned int max_particles() const { return m_max_particles; }

        void time_interval(double time_interval) { m_dt = time_interval; }
        double time_interval() const { return m_dt; }

        ParticleGroup& update();
        ParticleGroup& source(domain_ptr source_domain);
        ParticleGroup& sink(domain_ptr sink_domain);
        ParticleGroup& velocity(domain_ptr vel_domain, double speed, bool randomised = false, int variance = 1);
        ParticleGroup& bounce(domain_ptr bounce_domain, double dampening_factor = 1.0);
        ParticleGroup& stick(domain_ptr stick_domain);
        ParticleGroup& constant_force(Vector3d direction);

        void particle_life(int steps, bool randomised, int variance = 1);
        std::vector<Particle> particles() const { return m_particles; };
        void draw_domains(IDomainRenderer& drawer) const;

    private:
        std::vector<Particle> m_particles;
        domain_ptr m_source_domain;
        domain_ptr m_sink_domain;

        domain_ptr m_velocity_domain;
        double m_velocity_speed;
        bool m_randomised_velocity_speed;
        unsigned int m_velocity_variance;

        domain_ptr m_bounce_domain;
        double m_bounce_dampening;

        domain_ptr m_stick_domain;
        Vector3d m_const_force_vector;
        unsigned int m_max_particles;
        unsigned int m_particle_emission_rate;
        double m_dt;
        unsigned int m_sim_steps;

        unsigned int m_particle_life;
        bool m_randomised_particle_life;
        unsigned int m_particle_life_variance;

        Vector3d calculate_force(Vector3d position, Vector3d velocity);
        void advance_particle(Particle& particle);
        unsigned int generate_particle_life() const;
        unsigned int num_particles_alive();

        double generate_particle_speed() const;
    };
}