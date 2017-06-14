#ifndef __PARTICLE_EFFECTS_H__
#define __PARTICLE_EFFECTS_H__

#include "gf2d_vector.h"
#include "gf2d_color.h"
#include "gf2d_particles.h"

/**
 * @brief load a default set of assets used for particle effects
 */
void pe_load_assets();

void pe_thrust(
    ParticleEmitter *pe,
    Vector2D position,
    Vector2D positionVariance,
    Vector2D velocity,
    Vector2D velocityVariance,
    Vector2D acceleration,
    Vector2D accelerationVariance,
    Color color,
    int count);

void pe_blood_spray(ParticleEmitter *pe, Vector2D position,Vector2D direction,Color color,int amount);

void pe_explosion(
    ParticleEmitter *pe,
    Vector2D position,
    Color color,
    int count);

#endif