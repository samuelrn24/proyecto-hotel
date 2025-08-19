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
- **Editor/IDE:** Visual Studio Code - CLion

---

## Code explanation
The program manages a hotel guest register using two doubly linked lists over the same nodes: one in arrival order and another in alphabetical order. Each node stores a name, room number, and four pointers; the `GuestList` class keeps `headChron/tailChron` and `headAlpha` to handle insertions, searches, and traversals. On startup it loads name–room pairs from a text file; when adding a guest, it appends to the chronological list, inserts in order into the alphabetical list, and persists the entry to the file. The system lets you search by name or room, list guests in both orders, consult neighboring rooms, and, if the requested room is occupied, suggest an available adjacent one. A simple menu in `main.cpp` orchestrates all interactions.

El programa gestiona un registro de huéspedes de hotel usando **dos listas doblemente enlazadas** sobre los mismos nodos: una en **orden de llegada** y otra en **orden alfabético**. Cada nodo almacena nombre, número de habitación y cuatro punteros para pertenecer a ambas listas; la clase `GuestList` mantiene `headChron`/`tailChron` y `headAlpha` para administrar inserciones, búsquedas y recorridos. Al iniciar, se **cargan** pares *nombre–habitación* desde un archivo de texto; al **agregar** un huésped, se anexa al final de la lista cronológica, se inserta ordenadamente en la alfabética y se **persiste** en el archivo. El sistema permite **buscar** por nombre o habitación, **listar** en ambos órdenes, **consultar vecinos** de una habitación y, si una habitación está ocupada, **sugerir** una adyacente libre. Un menú en `main.cpp` coordina todas estas operaciones de forma interactiva.

---

## Video Explanation
