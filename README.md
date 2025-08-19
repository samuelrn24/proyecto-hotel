# proyecto-hotel - practica1

## Student
- **Full name:** Nicolas Reyes, Samuel Ramírez and Tomás Vera
- **Class number:** Estructuras de Datos y Algoritmos 1 - S2566-0936

---

## Environment (Versions)
This implementation uses only the C++ standard library.

- **Operating System (used):** macOS 15.6
- **Compiler** g++ 11+ (MinGW-w64) o MSVC 2019/2022
- **C++ standard:** C++17
- **Editor/IDE:** Visual Studio Code

---

## Code explanation
The program manages a hotel guest register using two doubly linked lists over the same nodes: one in arrival order and another in alphabetical order. Each node stores a name, room number, and four pointers; the GuestList class keeps headChron/tailChron and headAlpha to handle insertions, searches, and traversals. On startup it loads name–room pairs from a text file; when adding a guest, it appends to the chronological list, inserts in order into the alphabetical list, and persists the entry to the file. The system lets you search by name or room, list guests in both orders, consult neighboring rooms, and, if the requested room is occupied, suggest an available adjacent one. A simple menu in main.cpp orchestrates all interactions.
