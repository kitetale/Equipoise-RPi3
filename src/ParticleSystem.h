#pragma once

#include "Particle.h"


class ParticleSystem {
	public:
                ParticleSystem();
                ~ParticleSystem();
		void setup(float w, float h, ofPoint initPos, float maxForce, float maxSpeed);
		void update(vector<ofPolyline> c);
		void draw();
                void addParticle(ofPoint pos, ofPoint vel, ofColor color, float radius, float lifetime);
                void addParticles(int n);
                
                ofPoint randomVector();
                float randomRange(float percentage, float value);
                
                vector<Particle *> particles; //particle vector
                int nParticles;
                int totalParticlesCreated;
                float velocity;
                float radius;
                float lifeTime;
                float velocityRnd;
                float velocityMotion;
                float lifetimeRnd;
                float radiusRnd;
                float contourRadius;
                
                float w,h;

                ofPoint initPos; //particle generated location (i.e. contour center)

                float maxForce;
                float maxSpeed;
        

};
