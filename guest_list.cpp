#include "guest_list.h"
#include <fstream>
#include <cctype>

using namespace std;

GuestList::GuestList() : headChron(nullptr), tailChron(nullptr), headAlpha(nullptr) {}

GuestList::~GuestList() {
    GuestNode* current = headChron;
    while (current) {
        GuestNode* nextNode = current->nextChron;
        delete current;
        current = nextNode;
    }
    headChron = tailChron = headAlpha = nullptr;
}

void GuestList::insertIntoAlphaList(GuestNode* newNode) {
    if (!headAlpha) {
        headAlpha = newNode;
        return;
    }
    if (newNode->name < headAlpha->name) {
        newNode->nextAlpha = headAlpha;
        headAlpha->prevAlpha = newNode;
        headAlpha = newNode;
        return;
    }
    GuestNode* current = headAlpha;
    while (current) {
        if (newNode->name < current->name) {
            newNode->nextAlpha = current;
            newNode->prevAlpha = current->prevAlpha;
            if (current->prevAlpha) {
                current->prevAlpha->nextAlpha = newNode;
            }
            current->prevAlpha = newNode;
            return;
        }
        if (!current->nextAlpha) {
            current->nextAlpha = newNode;
            newNode->prevAlpha = current;
            return;
        }
        current = current->nextAlpha;
    }
}

void GuestList::loadFromFile(const string &filename) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cerr << "No se pudo abrir el archivo: " << filename << "\n";
        return;
    }
    string name;
    string roomLine;
    while (getline(fin, name)) {
        if (!getline(fin, roomLine)) {
            break;
        }
        size_t start = 0;
        while (start < roomLine.size() && isspace(static_cast<unsigned char>(roomLine[start]))) {
            start++;
        }
        size_t end = roomLine.size();
        while (end > start && isspace(static_cast<unsigned char>(roomLine[end - 1]))) {
            end--;
        }
        string numberStr = roomLine.substr(start, end - start);
        int room = 0;
        try {
            room = stoi(numberStr);
        } catch (...) {
            cerr << "Número de habitación inválido: " << roomLine << "\n";
            continue;
        }
        addGuest(name, room, "");
    }
    fin.close();
}

bool GuestList::appendToFile(const string &filename, const string &name, int room) const {
    if (filename.empty()) {
        return true;
    }
    ofstream fout(filename, ios::app);
    if (!fout.is_open()) {
        return false;
    }
    fout << name << "\n" << room << "\n";
    fout.close();
    return true;
}

void GuestList::addGuest(const string &name, int room, const string &filename) {
    GuestNode* newNode = new GuestNode(name, room);
    if (!headChron) {
        headChron = tailChron = newNode;
    } else {
        tailChron->nextChron = newNode;
        newNode->prevChron = tailChron;
        tailChron = newNode;
    }
    insertIntoAlphaList(newNode);
    if (!filename.empty()) {
        bool ok = appendToFile(filename, name, room);
        if (!ok) {
            cerr << "No se pudo escribir en el archivo: " << filename << "\n";
        }
    }
}

GuestNode* GuestList::searchByName(const string &name) const {
    GuestNode* current = headAlpha;
    while (current) {
        if (current->name == name) {
            return current;
        }
        current = current->nextAlpha;
    }
    return nullptr;
}

GuestNode* GuestList::searchByRoom(int room) const {
    GuestNode* current = headChron;
    while (current) {
        if (current->room == room) {
            return current;
        }
        current = current->nextChron;
    }
    return nullptr;
}

void GuestList::displayAlphabetical() const {
    if (!headAlpha) {
        cout << "La lista está vacía.\n";
        return;
    }
    cout << "Lista de huéspedes en orden alfabético:\n";
    GuestNode* current = headAlpha;
    while (current) {
        cout << "Nombre: " << current->name << ", Habitación: " << current->room << "\n";
        current = current->nextAlpha;
    }
}

void GuestList::displayArrival() const {
    if (!headChron) {
        cout << "La lista está vacía.\n";
        return;
    }
    cout << "Lista de huéspedes en orden de llegada:\n";
    GuestNode* current = headChron;
    while (current) {
        cout << "Nombre: " << current->name << ", Habitación: " << current->room << "\n";
        current = current->nextChron;
    }
}

bool GuestList::isRoomOccupied(int room) const {
    return searchByRoom(room) != nullptr;
}

int GuestList::suggestAdjacentRoom(int desiredRoom) const {
    if (desiredRoom > 1) {
        int previous = desiredRoom - 1;
        if (!isRoomOccupied(previous)) {
            return previous;
        }
    }
    int nextRoom = desiredRoom + 1;
    if (!isRoomOccupied(nextRoom)) {
        return nextRoom;
    }
    return -1;
}

void GuestList::consultNeighbors(int room) const {
    GuestNode* target = searchByRoom(room);
    if (!target) {
        cout << "No se encontró al huésped en la habitación " << room << ".\n";
        return;
    }
    int prevRoom = room - 1;
    int nextRoom = room + 1;
    cout << "Consultando vecinos de la habitación " << room << ":\n";
    //Vecino Anterior
    if (prevRoom > 0) {
        GuestNode* prevNode = searchByRoom(prevRoom);
        if (prevNode) {
            cout << "Habitación " << prevRoom << ": " << prevNode->name << "\n";
        } else {
            cout << "Habitación " << prevRoom << ": disponible o no existe.\n";
        }
    } else {
        cout << "No hay habitación anterior a la " << room << ".\n";
    }
    //Vecino posterior
    GuestNode* nextNode = searchByRoom(nextRoom);
    if (nextNode) {
        cout << "Habitación " << nextRoom << ": " << nextNode->name << "\n";
    } else {
        cout << "Habitación " << nextRoom << ": disponible o no existe.\n";
    }
}