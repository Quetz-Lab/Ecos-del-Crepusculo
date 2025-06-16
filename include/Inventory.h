#pragma once
#include <vector>
#include <iostream>
#include "Item.h"
class Inventory
{
    std::vector<Item*> items;

    void addItem(Item* item);
    void removeItem(int id);
    void showInventory();


};

