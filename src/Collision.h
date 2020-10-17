//
// Created by Pavel Gnusin on 17.10.2020.
//

#ifndef COLLISION_H
#define COLLISION_H


#include <SDL2/SDL.h>

class Collision {
public:
    static bool CheckRectangleCollision(const SDL_Rect rectangleA, const SDL_Rect rectangleB);

private:

};


#endif //COLLISION_H
