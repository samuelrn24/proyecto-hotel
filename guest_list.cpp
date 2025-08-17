// guest_list.cpp
// Implementation file for the GuestList class defined in guest_list.h.

#include "guest_list.h"
#include <fstream>
#include <cctype>

// Utilizamos el espacio de nombres estándar globalmente para evitar
// anteponer "std::" a cada elemento de la biblioteca estándar.
using namespace std;

// Constructor sets all head/tail pointers to nullptr.
GuestList::GuestList() : headChron(nullptr), tailChron(nullptr), headAlpha(nullptr) {}

// Destructor cleans up all allocated nodes by walking the chronological
// list. Because each node is referenced by both lists, deleting via
// one ordering suffices. After deletion all pointers are reset to
// nullptr to avoid dangling pointers in case of debugging.
GuestList::~GuestList() {
    GuestNode* current = headChron;
    while (current) {
        GuestNode* nextNode = current->nextChron;
        delete current;
        current = nextNode;
    }
    headChron = tailChron = headAlpha = nullptr;
}

// Helper: inserts a node into the alphabetical list maintaining
// ascending order by name. Uses lexicographic comparison on the
// guest names. Updates the headAlpha pointer if the new node
// becomes the new head. The prevAlpha and nextAlpha pointers of
// neighbouring nodes are adjusted accordingly.
void GuestList::insertIntoAlphaList(GuestNode* newNode) {
    if (!headAlpha) {
        // First node in alphabetical list
        headAlpha = newNode;
        return;
    }
    // Check if the new node should become the new head
    if (newNode->name < headAlpha->name) {
        newNode->nextAlpha = headAlpha;
        headAlpha->prevAlpha = newNode;
        headAlpha = newNode;
        return;
    }
    // Otherwise find the insertion point
    GuestNode* current = headAlpha;
    while (current) {
        // Insert before the first node whose name is greater
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
            // Reached end without inserting; append here
            current->nextAlpha = newNode;
            newNode->prevAlpha = current;
            return;
        }
        current = current->nextAlpha;
    }
}

// Reads guest data from a text file. Alternating lines of name and
// room number are expected. Leading/trailing whitespace on each
// line is preserved for names but stripped before conversion to
// integer for room numbers. Incomplete pairs (e.g., odd number of
// lines) are ignored. If the file cannot be opened the function
// quietly returns leaving the list empty.
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
            // Si hay un nombre sin número de habitación, se ignora
            break;
        }
        // Eliminar espacios en blanco al inicio y final de la línea de habitación
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

// Appends a guest to the file in the same two-line format used by
// loadFromFile(). Returns true on success or false on failure. The
// function does not throw exceptions so that the caller can handle
// failures gracefully (e.g., by displaying an error message).
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

// Adds a new guest node to both lists and optionally writes to file.
void GuestList::addGuest(const string &name, int room, const string &filename) {
    // Create new node
    GuestNode* newNode = new GuestNode(name, room);
    // Insert into chronological list at the end
    if (!headChron) {
        headChron = tailChron = newNode;
    } else {
        tailChron->nextChron = newNode;
        newNode->prevChron = tailChron;
        tailChron = newNode;
    }
    // Insert into alphabetical list
    insertIntoAlphaList(newNode);
    // Append to file if requested
    if (!filename.empty()) {
        bool ok = appendToFile(filename, name, room);
        if (!ok) {
            cerr << "No se pudo escribir en el archivo: " << filename << "\n";
        }
    }
}

// Searches the alphabetical list for a guest with the given name.
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

// Searches the chronological list for a guest occupying the given
// room number.
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

// Prints the list of guests in alphabetical order.
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

// Prints the list of guests in the order they were added.
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

// Returns true if the specified room is occupied.
bool GuestList::isRoomOccupied(int room) const {
    return searchByRoom(room) != nullptr;
}

// Suggests an adjacent room if the desired room is occupied. It checks
// room-1 if greater than zero and not occupied. If room-1 is not
// available it checks room+1. If neither is available returns -1.
int GuestList::suggestAdjacentRoom(int desiredRoom) const {
    // Check previous room if it's positive
    if (desiredRoom > 1) {
        int previous = desiredRoom - 1;
        if (!isRoomOccupied(previous)) {
            return previous;
        }
    }
    // Check next room
    int nextRoom = desiredRoom + 1;
    if (!isRoomOccupied(nextRoom)) {
        return nextRoom;
    }
    return -1;
}

// Given a room number prints the names of guests occupying the
// immediate neighbouring rooms. If the room does not exist or is
// unoccupied a message is printed. For neighbours that do not
// exist or are unoccupied, messages are printed accordingly.
void GuestList::consultNeighbors(int room) const {
    GuestNode* target = searchByRoom(room);
    if (!target) {
        cout << "No se encontró al huésped en la habitación " << room << ".\n";
        return;
    }
    int prevRoom = room - 1;
    int nextRoom = room + 1;
    cout << "Consultando vecinos de la habitación " << room << ":\n";
    // Previous neighbour
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
    // Next neighbour
    GuestNode* nextNode = searchByRoom(nextRoom);
    if (nextNode) {
        cout << "Habitación " << nextRoom << ": " << nextNode->name << "\n";
    } else {
        cout << "Habitación " << nextRoom << ": disponible o no existe.\n";
    }
}