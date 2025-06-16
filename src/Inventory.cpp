#include "Inventory.h"
void Inventory::addItem(Item* item) {
    items.push_back(item);
}

void Inventory::removeItem(int id) {
    items.erase(std::remove_if(items.begin(), items.end(), [&](Item* i) {
        return i->id == id;
        }), items.end());
}

void Inventory::showInventory() {
    std::cout << "Inventario:\n";
    for (const auto& item : items) {
        std::cout << "- " << item->name << "\n";
    }
}

