#include "Inventory.h"
#include "Player.h"
void Inventory::PickupWeapon(Weapon* weapon, Player* player) {
auto* newNode = new LLNode<Weapon>(weapon);

if (!head) {
    head = tail = current = newNode;
    head->Next = head; // Circular
    head->Prev = head;
}
else {
    tail->Next = newNode;
    newNode->Prev = tail;
    newNode->Next = head;
    head->Prev = newNode;
    tail = newNode;
}
current = newNode;
player->SetWeapon(weapon); // Integración con sistema de armas
}
//std::vector<Weapon*> Inventory::GetInventory() const {
//    std::vector<Weapon*> weapons;
//    if (!head) return weapons;
//
//    LLNode<Weapon>* node = head;
//    do {
//        weapons.push_back(node->data);
//        node = node->Next;
//    } while (node != head);
//
//    return weapons;
//}
void Inventory::NextWeapon(Player* player) {
    if (current && current->Next) {
        current = current->Next;
        std::vector<Weapon*> GetInventory();
        if (IsKeyPressed(KEY_TAB)) { // O la tecla que prefieras
            player->GetInventory()->NextWeapon(player);
        }
        // En tu función de renderizado del UI
        Weapon* currentWeapon = player->GetInventory()->GetCurrentWeapon();
        if (currentWeapon) {
            //DrawText(currentWeapon->GetName().c_str(), x, y, fontSize, color);
            // O mostrar la imagen: 
            //DrawTexture(currentWeapon->GetTexture(), x, y, color);
        }
        player->SetWeapon(current->data);
    }
}

Weapon* Inventory::GetCurrentWeapon() const {
    if (current == nullptr)
    {
        return nullptr;
   }
   /* else
    {
        return current->data;
    }*/
}
