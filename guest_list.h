#ifndef GUEST_LIST_H
#define GUEST_LIST_H

#include <string>
#include <iostream>

using namespace std;

struct GuestNode {
    string name;   
    int room;       
    GuestNode* prevChron; 
    GuestNode* nextChron;  
    GuestNode* prevAlpha;  
    GuestNode* nextAlpha;  

    GuestNode(const string &n, int r)
        : name(n), room(r), prevChron(nullptr), nextChron(nullptr),
          prevAlpha(nullptr), nextAlpha(nullptr) {}
};

class GuestList {
private:
    GuestNode* headChron;    
    GuestNode* tailChron;    
    GuestNode* headAlpha;   
    void insertIntoAlphaList(GuestNode* newNode);
public:
    GuestList();

    ~GuestList();

    void loadFromFile(const string &filename);

    bool appendToFile(const string &filename, const string &name, int room) const;

    void addGuest(const string &name, int room, const string &filename);

    GuestNode* searchByName(const string &name) const;

    GuestNode* searchByRoom(int room) const;

    void displayAlphabetical() const;

    void displayArrival() const;

    bool isRoomOccupied(int room) const;
    
    int suggestAdjacentRoom(int desiredRoom) const;

    void consultNeighbors(int room) const;
};

#endif