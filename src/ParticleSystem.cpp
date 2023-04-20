#include "ParticleSystem.h"

//--------------------------------------------------------------
ParticleSystem::ParticleSystem(){
    velocity = 0.0;
    radius = 5.0;
    lifetime = 5.0; 

    velocityRnd = 20.0;
    velocityMotion = 50.0;
    lifetimeRnd = 20.0;
    radiusRnd = 50.0;

    totalParticlesCreated = 0;
    nParticles = 300;
    contourRadius = 15;

    particles.clear();
}

ParticleSystem::~ParticleSystem(){
    // Delete all the particles
    for (int i = 0; i < particles.size(); i++) {
        delete particles.at(i);
        particles.at(i) = NULL;
    }
    particles.clear();
}

void ParticleSystem::setup(float w, float h, ofPoint initPos, float maxForce, float maxSpeed){
    this->initPos = initPos;
    this->w = w;
    this->h = h;
    this->maxForce = maxForce;
    this->maxSpeed = maxSpeed;

    addParticles(nParticles);
}

bool comparisonFunction(Particle * a, Particle * b) {
    return a->pos.x < b->pos.x;
}

void ParticleSystem::update(vector<ofPolyline> c){
    //sort particles for more effective particle/particle interactions
    //sort(particles.begin(), particles.end(), comparisonFunction); 

    // 1. delete inactive particles
    // int i = 0;
    // while(i < particles.size()){
    //     if (!particles[i]->isAlive){
    //         delete particles.at(i);
    //         particles.erase(particles.begin() + i);
    //         numParticles--;
    //     } else {
    //         i++;
    //     }
    // }

     for (int i=0; i < particles.size(); i++){
        particles[i].follow(c,contourRadius);
        particles[i].update();
     }
}

void ParticleSystem::draw(){
    for(int i = 0; i < particles.size(); i++){
        particles[i]->draw();
    }
}


void ParticleSystem::addParticle(ofPoint pos, ofPoint vel, ofColor color, float radius, float lifetime){
    Particle * newParticle = new Particle();
    float id = totalParticlesCreated;

    newParticle->w = w;
    newParticle->h = h;

    newParticle->setup(id,pos,vel,color,radius,lifetime);
    particles.push_back(newParticle);

    numParticles++;
    totalParticlesCreated++;
}

void ParticleSystem::addParticles(int n){
    for(int i = 0; i < n; i++){
        ofPoint pos = ofPoint(ofRandom(w), ofRandom(h)); // random point on screen
        //ofPoint pos = initPos; //center of contour blob
        ofPoint vel = randomVector()*(velocity+randomRange(velocityRnd, velocity));

        float initRadius = radius + randomRange(radiusRnd, radius);
        float lifetime = this->lifetime + randomRange(lifetimeRnd, this->lifetime);

        addParticle(pos, vel, ofColor(255,140,0), initRadius, lifetime);
    }
}