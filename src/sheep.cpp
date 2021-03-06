#include <iostream>
#include <cmath>
#include <vector>

#include "Sheep.hpp"
//#include "Dog.hpp"
#include "Utility.hpp"
#include "World.hpp"

//const float Sheep::mSightRange = 250.f;
//const float Sheep::mAngleOfVision = 180.f;
//const float Sheep::mPanicDistance = Sheep::mSightRange;
//const float Sheep::mSheepMaxRotation = 0.08f;

Sheep::Sheep(World* world
                 , const sf::Texture& texture
                 , const sf::Font& font
                 , sf::Vector2f startPos
                 , State<Sheep>* globalState
                 , State<Sheep>* initState
                 , EntityStats stats
                 , const Params& params
                 , float scale)
: MovingEntity(world
               , texture
               , font
               , startPos
               , stats
               , params
               , MovingEntity::EntityType::Sheep
               , scale)

, mSightRange(params.SheepSightRange)
, mAngleOfVision(params.SheepAngleOfVision)
, mPanicDistance(params.SheepPanicDistance)
, mStateMachine(this, globalState, initState)
{
    mText.setPosition(-10.f, -40.f);

    mCurrentBlock = mWorld->insertEntityIntoLevel(this);
}

void Sheep::updateCurrent(sf::Time dt)
{
    mStateMachine.update();

    MovingEntity::updateCurrent(dt);

    mCurrentBlock->deleteEntity(this);
    mCurrentBlock = mWorld->insertEntityIntoLevel(this);

    if(mCurrentBlock->getType() == LevelBlock::Type::ExitBlock)
//       && checkSteeringBehaviour(SteeringBehaviour::Behaviour::Arrive))
    {
        mToRemove = true;
//        mTarget = nullptr;
        mMovingTarget = nullptr;
        mCurrentBlock->deleteEntity(this);

        mWorld->incSheepHerded();
    }
}

void Sheep::drawCurrent(sf::RenderTarget& target
                        , sf::RenderStates states) const
{
    target.draw(mSprite, states);
    target.draw(mText);
}

void Sheep::changeState(Sheep::States newState)
{
    mStateMachine.changeState(mWorld->getSheepState(newState));
}
