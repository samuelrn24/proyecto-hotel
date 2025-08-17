// guest_list.h
// Header file defining the data structures and operations for managing
// a hotel guest register using doubly linked lists. The list keeps
// two independent orderings: chronological (arrival order) and
// alphabetical. Each node contains pointers for both orderings so
// that traversals can be performed in either order without copying
// the underlying data.

#ifndef GUEST_LIST_H
#define GUEST_LIST_H

#include <string>
#include <iostream>

// Se utiliza la directiva de espacio de nombres estándar para no
// tener que anteponer "std::" a cada uso de elementos de la STL.
using namespace std;

// A node representing a hotel guest. Each node stores the guest's
// name, their room number and four pointers: two for the chronological
// list (prevChron/nextChron) and two for the alphabetical list
// (prevAlpha/nextAlpha). The pointers allow independent traversal
// without duplicating information.
struct GuestNode {
    string name;       // Name of the guest
    int room;          // Room number occupied by the guest
    GuestNode* prevChron;  // Previous node in chronological order
    GuestNode* nextChron;  // Next node in chronological order
    GuestNode* prevAlpha;  // Previous node in alphabetical order
    GuestNode* nextAlpha;  // Next node in alphabetical order
    // Constructor initialises the node with a name and room number and
    // sets all pointers to nullptr. The pointers will be patched into
    // the lists by the GuestList class when the node is added.
    GuestNode(const string &n, int r)
        : name(n), room(r), prevChron(nullptr), nextChron(nullptr),
          prevAlpha(nullptr), nextAlpha(nullptr) {}
};

// GuestList encapsulates the logic for managing the doubly linked
// lists. It supports loading guest data from a text file, saving
// newly added guests back to the file, inserting guests into both
// orderings, searching, displaying and neighbour queries.
class GuestList {
private:
    GuestNode* headChron;    // Head pointer for arrival (chronological) list
    GuestNode* tailChron;    // Tail pointer for arrival list
    GuestNode* headAlpha;    // Head pointer for alphabetical list
    // Inserts a node into the alphabetical list maintaining order. This
    // routine updates the headAlpha pointer if necessary.
    void insertIntoAlphaList(GuestNode* newNode);
public:
    // Constructor initialises the list heads to nullptr. On
    // construction the list is empty.
    GuestList();
    // Destructor releases all nodes from memory by walking the
    // chronological list. Since each node appears exactly once in
    // this list no node will be deleted twice.
    ~GuestList();
    // Reads guest data from a text file. The expected file format is
    // alternating lines of guest name and room number. For each pair
    // of lines a new node is created and inserted into both lists.
    // If the file cannot be opened the list remains empty.
    void loadFromFile(const string &filename);
    // Appends a single guest entry to the end of the text file. The
    // name and room number are written on separate lines to match
    // loadFromFile(). Returns true if the append succeeds, false
    // otherwise (for example, when the file cannot be opened for
    // writing).
    bool appendToFile(const string &filename, const string &name, int room) const;
    // Adds a new guest to both the chronological and alphabetical
    // lists. The new node is appended to the end of the chronological
    // list and inserted into the proper position in the alphabetical
    // list. If a filename is provided the new guest is also appended
    // to the file.
    void addGuest(const string &name, int room, const string &filename);
    // Searches for a guest by name. Returns a pointer to the node if
    // found or nullptr otherwise. Case–sensitive comparison is used.
    GuestNode* searchByName(const string &name) const;
    // Searches for a guest by room number. Returns a pointer to the
    // node if found or nullptr otherwise.
    GuestNode* searchByRoom(int room) const;
    // Displays the list of guests in alphabetical order. Each line
    // printed contains the guest name and their room number. If the
    // list is empty a message is printed.
    void displayAlphabetical() const;
    // Displays the list of guests in order of arrival. Each line
    // printed contains the guest name and their room number. If the
    // list is empty a message is printed.
    void displayArrival() const;
    // Returns true if the supplied room number is currently occupied
    // by some guest, or false otherwise.
    bool isRoomOccupied(int room) const;
    // Given a desired room number this method returns an available
    // immediate neighbour: it checks the previous room (room-1) if
    // room > 1, then the next room (room+1). If neither is available
    // it returns -1 to signal that no adjacent suggestions exist.
    int suggestAdjacentRoom(int desiredRoom) const;
    // For a given room number prints the names of the guests in the
    // rooms immediately before and after it. If a neighbouring room
    // does not exist or is not occupied an appropriate message is
    // printed. If the requested room is not found a message is also
    // printed.
    void consultNeighbors(int room) const;
};

#endif // GUEST_LIST_H