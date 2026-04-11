#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <array>
#include <cstdint>
#include <cstring>
#include "raylib.h"

/** Struktura przechowująca model i licznik referencji. */
struct ModelResource {
    Model model{};
    int refCount = 0;
};


/** Proponuje przeczytać, bo jest grubo.
 *  Prezentuje państwu coś takiego co nazywa się ładnie hash table,
 *  Dzięki takiemu zastosowaniu alokujemy w JEDNYM miejscu (hura) w pamięci wszystkie modele/dźwięki/co dusza zapragnie.
 *  Działa prawie identycznie co unordered_map tylko z nią jest taki problem że rozwala to wszędzie po pamięci
 *  i musisz szukać co nie jest dobre dla silnika gry.
 *
 *  insert(KLUCZ, WARTOŚĆ) - liczymy hash z klucza, wyznacza on indeks startowy w tablicy.
 *      Jeśli slot jest wolny — wstawiamy. Jeśli zajęty — przesuwamy się o 1 i próbujemy znowu (linear probing).
 *
 *  get(KLUCZ) - liczymy hash, idziemy do indeksu startowego i porównujemy klucze.
 *      Jeśli klucz się zgadza — zwracamy wartość. Jeśli slot jest wolny — nie ma takiego klucza, zwracamy nullptr.
 *      Jeśli zajęty ale inny klucz — przesuwamy się o 1 i szukamy dalej (kolizja hashów).
 *
 *  remove(KLUCZ) - szukamy klucza tak samo jak get. Po usunięciu slotu trzeba przesunąć
 *      wstecz wszystkie kolejne zajęte sloty, bo mogły tam wylądować przez kolizję —
 *      bez tego get trafiłby na pusty slot i fałszywie zwróciłby nullptr.
 *  Na ten moment jednie mamy ModelResouce ale można to łatwo rozszerzyć na inne typy zasobów, wystarczy stworzyć kolejny ResourceManager.
 */
template <class T, size_t Capacity, size_t KeySize>
class ResourceManager {
    /**
     * Tak wygląda pojedynczy slot w naszej hash table. Przechowuje klucz, wskaźnik na wartość, hash klucza i flagę zajętości.
     */
    struct Slot {
        char key[KeySize]{};
        T value{};
        uint32_t hash = 0;
        bool occupied = false;
    };

    std::array<Slot, Capacity> slots{};

    // Typowa funkcja hashująca tego nikt sam nie pisze.
    uint32_t hash(const char* key) const {
        uint32_t h = 5381;
        int c;
        while ((c = *key++)) {
            h = ((h << 5) + h) + c;
        }
        return h;
    }

public:

    // Wstawia wartość przez kopię do tablicy i zwraca wskaźnik na jej miejsce w środku.
    T* insert(const char* key, const T& new_value) {
        uint32_t h = hash(key);
        size_t index = h % Capacity;

        for (size_t i = 0; i < Capacity; i++) {
            size_t probe = (index + i) % Capacity;

            if (!slots[probe].occupied) {
                std::strncpy(slots[probe].key, key, KeySize);
                slots[probe].value = new_value;
                slots[probe].hash = h;
                slots[probe].occupied = true;
                return &slots[probe].value;
            }

            // nadpisanie jeśli ten sam klucz
            if (slots[probe].hash == h &&
                std::strncmp(slots[probe].key, key, KeySize) == 0) {
                slots[probe].value = new_value;
                return &slots[probe].value;
            }
        }

        return nullptr;
    }

    T* get(const char* key) {
        uint32_t h = hash(key);
        size_t index = h % Capacity;

        for (size_t i = 0; i < Capacity; i++) {
            size_t probe = (index + i) % Capacity;

            if (!slots[probe].occupied) {
                return nullptr;
            }

            if (slots[probe].hash == h &&
                std::strncmp(slots[probe].key, key, KeySize) == 0) {
                return &slots[probe].value;
            }
        }

        return nullptr;
    }

    void remove(const char* key) {
        uint32_t h = hash(key);
        size_t index = h % Capacity;

        for (size_t i = 0; i < Capacity; i++) {
            size_t probe = (index + i) % Capacity;

            if (!slots[probe].occupied) return;

            if (slots[probe].hash == h &&
                std::strncmp(slots[probe].key, key, KeySize) == 0) {
                slots[probe].occupied = false;

                // przesuń wstecz żeby nie zostawić dziury w łańcuchu kolizji
                size_t curr = probe;
                size_t next = (curr + 1) % Capacity;
                while (slots[next].occupied) {
                    slots[curr] = slots[next];
                    slots[next].occupied = false;
                    curr = next;
                    next = (next + 1) % Capacity;
                }
                return;
            }
        }
    }
};

#endif // RESOURCEMANAGER_H
