#include <iostream>
using namespace std;

class RandomEvent;
class World;
class Position;
class Creature;

//Random events which may occur world-wide or actor specific.
class RandomEvent {
    private:
        //Integer to hold the current chance of next occurance
        int chance;

        //Adjust the chance to have a new value
        void adjustChance() {
            chance *= rand() % 10;
            chance %= 25;
        }

    public:
        //Create a new RandomEvent
        RandomEvent() {
            int chance = rand() % 25;
        }

        //Create a new creature based on the stored chance
        bool createCreature() {
            adjustChance();
            return(chance > 10);
        }

        //Kills a current creature based on the stored chance
        bool die() {
            adjustChance();
            return (chance < 3);
        }

        //Gets the current chance
        int getChance() {
            return chance;
        }
};

//Class which contains the World, which holds all creatures and an event item
class World {
    //All variables first
    private:
        //Variable to hold random events which occur world-wide
        RandomEvent *event;
    public:
        //Variable to hold array of creatures that exist in the world
        Creature *creatures[10];
    
    //Now methods
        //Constructor which allocates memory
        World() {
            event = new RandomEvent();
        }

        Creature* createCreature();

};

//Class to hold position data
class Position {
    private:
        //Ints for x and y
        int Xpos, Ypos;
        //World the position is located in
        World* inside;

    public:
        //Constructors
        Position() {
            Xpos = 0;
            Ypos = 0;
        }
        Position(World* w) {
            Xpos = 0;
            Ypos = 0;
            inside = w;
        }

        //Adjust the values
        void adjustX(int amount) {
            Xpos += amount;
        }
        void adjustY(int amount) {
            Ypos += amount;
        }
        void moveTo(Position p) {
            Xpos = p.getX();
            Ypos = p.getY();
            inside = p.inWorld();
        }
        void changeWorld(World* w) {
            inside = w;
        }

        //Get the values
        int getX() {
            return Xpos;
        }
        int getY() {
            return Ypos;
        }
        World* inWorld() {
            return inside;
        }
};

class Creature {
    private:
        //Position of the creature within the world
        Position position;
        //The random event item related to this creature
        RandomEvent* events;
    public:
        Creature() {
            events = new RandomEvent();
        }
        Creature(Position at) {
            position = at;
            events = new RandomEvent();
        }
        void setWorld(World* w) {
            position.changeWorld(w);
        }
        World* getWorld() {
            return position.inWorld();
        }
        Creature* createCreature() {
            Creature* newCreature = NULL;
            if(events->createCreature()) {
                newCreature = new Creature();
                if(&position != NULL) {
                    newCreature->move(position);
                    newCreature->position.adjustX(1);
                }
            }
            return newCreature;
        }
        void move(Position dest) {
            position.moveTo(dest);
        }
        Position getPosition() {
            return position;
        }
        void die() {
            if(events->die()) {
                delete events;
                delete this;
            }
        }
};

Creature* World::createCreature() {
    Creature* newCreature = NULL;
    if(event->createCreature()) {
        newCreature = new Creature();
        newCreature->setWorld(this);
    }
    return newCreature;
}

int main() {
    //Declare a world
    World* w = new World();

    //A starting position
    Position p = *new Position(w);

    //And a couple creatures
    Creature* creatureOne = new Creature(p);
    Creature* creatureTwo = new Creature(p);
    Creature* creatureThr = new Creature(p);

    //Do the have kids?
    Creature* newKid = creatureOne->createCreature();
    while(newKid == NULL) {
        cout << "Failed to creature creature from creature" << endl;
        newKid = creatureOne->createCreature();
    }
    cout << "Created a new child at " << newKid->getPosition().getX() << ", " << newKid->getPosition().getY() << endl;

    //Does the world have kids?
    newKid = w->createCreature();
    while(newKid == NULL) {
        cout << "Failed to creature creature from world" << endl;
        newKid = w->createCreature();
    }
    cout << "Created a new child at " << newKid->getPosition().getX() << ", " << newKid->getPosition().getY() << endl;
    
}