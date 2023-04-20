#pragma once

#include "Particle.h"

using namespace ofxCv;
using namespace cv;

class ParticleSystem {
	public:
		void setup();
		void update();
		void draw();

        vector<Particle *> particles; //particle vector
        int numParticles;
        int totalParticlesCreated;
        float velocity;
        float radius;
        float lifeTime;
        float velocityRnd;
        float velocityMotion;
        float lifetimeRnd;
        float radiusRnd;
        float contourRadius;

        ofPoint initPos; //particle generated location (i.e. contour center)

        float maxForce;
        float maxSpeed;
        

};
