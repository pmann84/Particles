#include "ParticleGroup.hpp"

namespace ps
{
    ParticleGroup::ParticleGroup() : ParticleGroup(100, 10)
    {
    }

    ParticleGroup::ParticleGroup(unsigned max_particles, unsigned particle_emission_rate) :
        m_particles(max_particles),
        m_const_force_vector(
              0.0, 0.0,
              0.0),
        m_max_particles(
              max_particles),
        m_particle_emission_rate(
              particle_emission_rate),
        m_dt(0.01),
        m_sim_steps(0),
        m_particle_life(100),
        m_randomised_particle_life(
              false),
        m_particle_life_variance(
              1)
    {
        for (unsigned int i = 0; i < m_particles.size(); ++i) {
            // Create particle initialised to nothing
            Particle new_particle(i);
            new_particle.age(0);
            m_particles[i] = new_particle;
        }
    }

    ParticleGroup& ParticleGroup::update()
    {
        std::for_each(m_particles.begin(), m_particles.end(),
                      [this](Particle &particle) {
                          // Particle destruction
                          bool particle_entered_sink = false;
                          if (m_sink_domain) {
                              particle_entered_sink = m_sink_domain->HasEnteredDomain(particle);
                          }
                          if (particle.is_dead() || particle_entered_sink) {
                              // Kill any particles that are past their life or hit the sink
                              particle.reset();
                          }
                      });

        // Figure out number of particles to create - will now always be > 0
        const unsigned int num_p_alive = num_particles_alive();
        const int num_particles_available_to_create = m_max_particles - num_p_alive;
        int num_particles_to_create = num_particles_available_to_create > m_particle_emission_rate
                                      ? m_particle_emission_rate
                                      : num_particles_available_to_create;

        for (auto &particle: m_particles) {
            // Particle Simulation
            if (!particle.is_dead()) {
                if (!particle.is_static()) {
                    if (m_stick_domain && m_stick_domain->HasEnteredDomain(particle)) {
                        // Update position to be intersection on the domain
                        Vector3d new_pos = m_stick_domain->Intersection(particle);
                        particle.position(new_pos);
                        // Set velocity and accn to 0 so it doesn't move
                        particle.velocity(Vector3d());
                        // Finally set this to static so that it won't be updated!!!
                        particle.set_static(true);
                    } else {
                        if (m_bounce_domain && m_bounce_domain->HasEnteredDomain(particle)) {
                            // Update position to be intersection on the domain
                            Vector3d new_pos = m_bounce_domain->Intersection(particle);
                            //new_pos += vector3d(0.0, 0.001, 0.0);
                            particle.position(new_pos);
                            if (particle.velocity().norm() > 1e-16) {
                                // Update velocity to be appropriate for a bounce off the domain
                                Vector3d domain_norm = m_bounce_domain->Normal();
                                // Full particle collision calc
                                Vector3d mom_change =
                                        ((1.0 + m_bounce_dampening) * inner_product(particle.velocity(), domain_norm) *
                                         particle.mass()) * domain_norm;
                                Vector3d new_vel = particle.velocity() - mom_change;
                                particle.velocity(new_vel);
                            } else {
                                // Set velocity and accn to 0 so it doesnt move
                                particle.velocity(Vector3d());
                                // Finally set this to static so that it won't be updated!!!
                                particle.set_static(true);
                            }
                        }
                        // Advance particle position using *PHYSICS*
                        advance_particle(particle);
                    }
                }
                // Increment the age of the particle
                particle.birthday();
            } else {
                if (num_particles_to_create > 0) {
                    // Particle Creation
                    // Generate particles from source (if any)
                    // Get position from source domain
                    const Vector3d pos = m_source_domain->GeneratePosition();
                    // Get velocity using velocity domain
                    const Vector3d vel = m_velocity_domain->GenerateVelocity(pos, generate_particle_speed());
                    // Generate particle life
                    const unsigned int life = generate_particle_life();
                    // Update a particle that is dead
                    particle.previous_position(pos);
                    particle.position(pos);
                    particle.velocity(vel);
                    particle.life(life);
                    particle.birthday();
                    particle.set_static(false);
                    // Decrease counter
                    num_particles_to_create--;
                }
            }
        }
        m_sim_steps++;

//        std::cout << "Particles Alive: " << num_particles_alive() << std::endl;
        return *this;
    }

    ParticleGroup& ParticleGroup::source(domain_ptr source_domain)
    {
        m_source_domain = source_domain;
        return *this;
    }

    ParticleGroup& ParticleGroup::sink(domain_ptr sink_domain)
    {
        m_sink_domain = sink_domain;
        return *this;
    }

    ParticleGroup& ParticleGroup::velocity(domain_ptr vel_domain, double speed, bool randomised, int variance)
    {
        m_velocity_domain = vel_domain;
        m_velocity_speed = speed;
        m_randomised_velocity_speed = randomised;
        m_velocity_variance = variance;
        return *this;
    }

    ParticleGroup& ParticleGroup::bounce(domain_ptr bounce_domain, double dampening_factor)
    {
        m_bounce_domain = bounce_domain;
        m_bounce_dampening = dampening_factor;
        return *this;
    }

    ParticleGroup& ParticleGroup::stick(domain_ptr stick_domain)
    {
        m_stick_domain = stick_domain;
        return *this;
    }

    ParticleGroup& ParticleGroup::constant_force(Vector3d direction)
    {
        m_const_force_vector = direction;
        return *this;
    }

    void ParticleGroup::particle_life(int steps, bool randomised = false, int variance)
    {
        m_particle_life = steps;
        m_randomised_particle_life = randomised;
        m_particle_life_variance = variance;
    }

    void ParticleGroup::draw_domains(IDomainRenderer &renderer) const
    {
        if (m_source_domain) {
            m_source_domain->Draw(renderer);
        }
        if (m_velocity_domain) {
            m_velocity_domain->Draw(renderer);
        }
        if (m_sink_domain) {
            m_sink_domain->Draw(renderer);
        }
        if (m_stick_domain) {
            m_stick_domain->Draw(renderer);
        }
        if (m_bounce_domain) {
            m_bounce_domain->Draw(renderer);
        }
    }

    Vector3d ParticleGroup::calculate_force(Vector3d position, Vector3d velocity)
    {
        return m_const_force_vector;
    }

    void ParticleGroup::advance_particle(Particle &particle)
    {
        // Advance particle position using *PHYSICS* - here most actions
        // can be calculated in terms of forces that can be used in newtons
        // 3rd law solution, algorithm is thus (midpoint/euler-richardson):
        // First update old position
        particle.previous_position(particle.position());
        // Get accn at current pos, vel and time
        Vector3d acc_n = calculate_force(particle.position(), particle.velocity()) / particle.mass();
        // Calculate velocity at mid point of time step
        Vector3d vel_mid = particle.velocity() + 0.5 * acc_n * m_dt;
        // Calculate position at mid point of time step
        Vector3d pos_mid = particle.position() + 0.5 * particle.velocity() * m_dt;
        // Calculate acc at mid point of time step, using vel_mid, pos_mid and t + 0.5dt
        Vector3d acc_mid = calculate_force(pos_mid, vel_mid) / particle.mass();
        // Now calculate the final pos and vel from mid points!
        Vector3d vel_np1 = particle.velocity() + 0.5 * acc_mid * m_dt;
        Vector3d pos_np1 = particle.position() + 0.5 * vel_mid * m_dt;
        // Update the particle position
        particle.position(pos_np1);
        particle.velocity(vel_np1);
    }

    unsigned int ParticleGroup::generate_particle_life() const
    {
        return m_randomised_particle_life ?
            Random::Normal<unsigned int>(m_particle_life, m_particle_life_variance) :
            m_particle_life;
    }

    unsigned ParticleGroup::num_particles_alive()
    {
        unsigned int num_particles_alive = 0;
        for (auto &particle: m_particles) {
            if (!particle.is_dead()) {
                num_particles_alive++;
            }
        }
        return num_particles_alive;
    }

    double ParticleGroup::generate_particle_speed() const
    {
        return m_randomised_velocity_speed ?
            Random::Normal<double>(m_velocity_speed, m_velocity_variance) :
            m_velocity_speed;
    }
}