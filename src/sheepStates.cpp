#include <cassert>
#include <iostream>

#include "SheepStates.hpp"
#include "SteeringBehaviour.hpp"
#include "Utility.hpp"
//#include "BlockType.hpp"
#include "MovingTarget.hpp"
#include "World.hpp"

void LookOutForDog::enter(Sheep* sheep)
{
    assert(sheep);
}

void LookOutForDog::execute(Sheep* sheep)
{
    const MovingTarget* dog = sheep->getMovingTarget();

    if(dog
       && !sheep->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Arrive))
    {
        sf::Vector2f sheepPos = sheep->getWorldPosition();

//        float exitRadius = sheep->getExitRadius();

        float exitRadius = 100;

        std::vector<LevelBlock*> levelExit = sheep->getLevelExit();

        bool closeToExit = false;
        float expandedRadius = exitRadius + sheep->getRadius();

        sf::Vector2f targPos;

        for(LevelBlock* lvlBlck : levelExit)
        {
            sf::Vector2f blckPos = lvlBlck->getMiddle();

            sf::Vector2f toBlck = blckPos - sheepPos;
            float magExit = magVec(toBlck);

            if(magExit < expandedRadius)
            {
                closeToExit = true;
                targPos = blckPos;
                break;
            }
        }

        if(closeToExit)
        {
            sheep->changeState(Sheep::States::Exit);
            sheep->setTargetPos(targPos);
            return;
        }

        sf::Vector2f dogPos = dog->targetPosition();
        sf::Vector2f vecToDog = sheepPos - dogPos;

        float magDog = magVec(vecToDog);

        if(magDog <= sheep->mPanicDistance * 2.f)
            sheep->changeState(Sheep::States::Evade);
    }
}

void LookOutForDog::exit(Sheep* sheep)
{

}

void EvadeDog::enter(Sheep* sheep)
{
    assert(sheep);

    std::vector<SteeringBehaviour::Behaviour> behaviours;

    if(!sheep->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Evade))
        behaviours.push_back(SteeringBehaviour::Behaviour::Evade);

    behaviours.push_back(SteeringBehaviour::Behaviour::Flock);

    sheep->setSteeringTypes(behaviours);
    sheep->setText("!!!!");

    sheep->setMaxSpeed(sheep->getMaxRunSpeed());
}

void EvadeDog::execute(Sheep* sheep)
{
    if(sheep->getMovingTarget())
    {
        sf::Vector2f dogPos = sheep->getMovingTarget()->targetPosition();
        sf::Vector2f vecToDog = sheep->getWorldPosition() - dogPos;

        float mag = magVec(vecToDog);

//        sf::Vector2f exitPos = sheep->getExitPos();
//        float exitRadius = sheep->getExitRadius();

//        sf::Vector2f distToExit = sheep->getWorldPosition() - exitPos;
//        float mag2 = magVec(distToExit);
//
//        if(mag2 < exitRadius)
//        {
//            sheep->changeState(Sheep::States::Exit);
//        }
        if(mag > sheep->mPanicDistance)
        {
            sheep->changeState(Sheep::States::Relax);
        }
    }
}

void EvadeDog::exit(Sheep* sheep)
{

}

void Relax::enter(Sheep* sheep)
{
    assert(sheep);

    bool isFlocking = false;

    if(sheep->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Flock))
        isFlocking = true;

    if(!sheep->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Wander))
    {
        std::vector<SteeringBehaviour::Behaviour> behaviours;
        behaviours.push_back(SteeringBehaviour::Behaviour::Wander);

        if(isFlocking)
            behaviours.push_back(SteeringBehaviour::Behaviour::Flock);

        sheep->setSteeringTypes(behaviours);
    }

    sheep->setText("Baaah");
    sheep->setMaxSpeed(sheep->getMaxWalkSpeed());
}

void Relax::execute(Sheep* sheep)
{

}

void Relax::exit(Sheep* sheep)
{

}

void Exit::enter(Sheep* sheep)
{
    assert(sheep);

    if(!sheep->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Arrive))
    {
        std::vector<SteeringBehaviour::Behaviour> behaviours;
        behaviours.push_back(SteeringBehaviour::Behaviour::Arrive);
//        behaviours.push_back(SteeringBehaviour::Behaviour::Flock);

        sheep->setSteeringTypes(behaviours);
    }

    sheep->setText("Maaah");
    sheep->setMaxSpeed(sheep->getMaxRunSpeed());
}

void Exit::execute(Sheep* sheep)
{
//    sheep->setTargetPos(sheep->getExitPos());
}

void Exit::exit(Sheep* sheep)
{

}
