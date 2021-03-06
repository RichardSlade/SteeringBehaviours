#ifndef ENTITYSTATS_HPP
#define ENTITYSTATS_HPP

#include <fstream>
#include <string>

struct EntityStats
{
public:
    float       mass;
    float       walkMaxSpeed;
    float       runMaxSpeed;
    float       maxForce;
    float       maxTurnRate;

                EntityStats(std::string fileName)
                {
                    std::ifstream in(fileName);

                    if(!in.is_open())
                        throw std::runtime_error("ERROR: Could not open file" + fileName);

                    in >> *this;
                    in.close();
                }

    friend void operator>>(std::ifstream &in, EntityStats &stats)
    {
        in.ignore(255, ' ');
        in >> stats.mass;

        in.ignore(255, ' ');
        in >> stats.walkMaxSpeed;

        in.ignore(255, ' ');
        in >> stats.runMaxSpeed;

        in.ignore(255, ' ');
        in >> stats.maxForce;

        in.ignore(255, ' ');
        in >> stats.maxTurnRate;
    }

};

#endif // ENTITYSTATS_HPP
