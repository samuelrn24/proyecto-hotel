#include "guest_list.h"
#include <iostream>
#include <limits>
#include <fstream> 

using namespace std;

static void clearInput() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    const string filename = "hotel.txt";

    ifstream fin(filename);
    if (!fin.is_open()) {
        ofstream fout(filename);
        fout.close();
        cout << "Se creó el archivo hotel.txt en la carpeta del programa.\n";
    } else {
        fin.close();
    }

    GuestList list;
    list.loadFromFile(filename);

    int option = 0;
    do {
        cout << "\n--- Menú de gestión de huéspedes ---\n";
        cout << "1. Ingresar nuevo huésped\n";
        cout << "2. Buscar huésped\n";
        cout << "3. Mostrar lista en orden alfabético\n";
        cout << "4. Mostrar lista en orden de llegada\n";
        cout << "5. Consultar vecinos de una habitación\n";
        cout << "6. Salir\n";
        cout << "Seleccione una opción: ";
        if (!(cin >> option)) {
            cin.clear();
            clearInput();
            cout << "Entrada inválida. Intente de nuevo.\n";
            continue;
        }
        clearInput();
        switch (option) {
        case 1: {
            // Add new guest
            string name;
            cout << "Ingrese el nombre del huésped: ";
            getline(cin, name);
            if (name.empty()) {
                cout << "El nombre no puede estar vacío.\n";
                break;
            }
            int room;
            bool validRoom = false;
            while (!validRoom) {
                cout << "Ingrese el número de habitación deseado: ";
                if (!(cin >> room)) {
                    cin.clear();
                    clearInput();
                    cout << "Número de habitación inválido.\n";
                    continue;
                }
                clearInput();
                if (room <= 0) {
                    cout << "El número de habitación debe ser positivo.\n";
                    continue;
                }
                if (!list.isRoomOccupied(room)) {
                    validRoom = true;
                } else {
                    cout << "La habitación " << room << " ya está ocupada.\n";
                    int suggestion = list.suggestAdjacentRoom(room);
                    if (suggestion > 0) {
                        cout << "Se sugiere la habitación " << suggestion << ". ¿Desea ocuparla? (s/n): ";
                        char resp;
                        cin >> resp;
                        clearInput();
                        if (resp == 's' || resp == 'S') {
                            room = suggestion;
                            validRoom = true;
                        } else {
                            cout << "Seleccione otro número de habitación.\n";
                        }
                    } else {
                        cout << "Ni la habitación anterior ni la siguiente están disponibles. "
                                "Seleccione otro número de habitación.\n";
                    }
                }
            }
            // Now we have a valid room
            list.addGuest(name, room, filename);
            cout << "Huésped agregado exitosamente.\n";
            break;
        }
        case 2: {
            // Search guest
            int searchOption;
            cout << "Buscar por: 1) Nombre  2) Habitación\n";
            cout << "Seleccione una opción: ";
            if (!(cin >> searchOption)) {
                cin.clear();
                clearInput();
                cout << "Entrada inválida.\n";
                break;
            }
            clearInput();
            if (searchOption == 1) {
                string queryName;
                cout << "Ingrese el nombre a buscar: ";
                getline(cin, queryName);
                GuestNode* found = list.searchByName(queryName);
                if (found) {
                    cout << "El huésped " << queryName << " ocupa la habitación " << found->room << ".\n";
                } else {
                    cout << "Ese huésped no fue encontrado.\n";
                }
            } else if (searchOption == 2) {
                int queryRoom;
                cout << "Ingrese el número de habitación a buscar: ";
                if (!(cin >> queryRoom)) {
                    cin.clear();
                    clearInput();
                    cout << "Número de habitación inválido.\n";
                    break;
                }
                clearInput();
                GuestNode* found = list.searchByRoom(queryRoom);
                if (found) {
                    cout << "La habitación " << queryRoom << " está ocupada por " << found->name << ".\n";
                } else {
                    cout << "Esa habitación no está ocupada.\n";
                }
            } else {
                cout << "Opción de búsqueda inválida.\n";
            }
            break;
        }
        case 3:
            list.displayAlphabetical();
            break;
        case 4:
            list.displayArrival();
            break;
        case 5: {
            // Consult neighbours
            int room;
            cout << "Ingrese el número de habitación para consultar vecinos: ";
            if (!(cin >> room)) {
                cin.clear();
                clearInput();
                cout << "Número de habitación inválido.\n";
                break;
            }
            clearInput();
            list.consultNeighbors(room);
            break;
        }
        case 6:
            cout << "Saliendo del programa...\n";
            break;
        default:
            cout << "Opción inválida, intente de nuevo.\n";
            break;
        }
    } while (option != 6);

    return 0;
}