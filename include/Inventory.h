#pragma once
#include <vector>
#include "Weapon.h"
#include "LinkedList.h"
#include "LLNode.h"
using namespace Quetz_LabEDC;
 // Forward declaration
class Inventory {
public:
    LLNode<Weapon>* head = nullptr;
    LLNode<Weapon>* tail = nullptr;
    LLNode<Weapon>* current = nullptr;

	Inventory() : head(nullptr), tail(nullptr), current(nullptr){}

    void PickupWeapon(Weapon* weapon, Player* player);
    void NextWeapon(Player* player);
    //std::vector<Weapon*> GetInventory() const;
    Weapon* GetCurrentWeapon() const ;
};

