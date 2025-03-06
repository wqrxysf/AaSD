#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <sstream>

using namespace std;

struct Apartment {
    int rooms;
    bool balcony;
    bool phone;

    bool operator==(const Apartment& other) const {
        return rooms == other.rooms && balcony == other.balcony && phone == other.phone;
    }

    bool operator<(const Apartment& other) const {
        return tie(rooms, balcony, phone) < tie(other.rooms, other.balcony, other.phone);
    }

    string toString() const {
        return to_string(rooms) + (balcony ? "+" : "-") + (phone ? "+" : "-");
    }
};

struct Exchange {
    Apartment from;
    Apartment to;
    string clientLastName;
};

// Хэш-функция для Apartment
struct ApartmentHash {
    size_t operator()(const Apartment& apt) const {
        return hash<int>()(apt.rooms) ^ hash<bool>()(apt.balcony) ^ hash<bool>()(apt.phone);
    }
};

// Класс для работы с графом обменов
class CityExchange {
private:
    unordered_map<Apartment, vector<pair<Apartment, string>>, ApartmentHash> exchanges;

    void addExchange(const Apartment& from, const Apartment& to, const string& clientLastName) {
        exchanges[from].emplace_back(to, clientLastName);
        exchanges[to].emplace_back(from, clientLastName); // Учитываем реверсивные обмены
    }

    void findCycles(const Apartment& start, vector<Apartment>& path, set<Apartment>& visited, vector<vector<string>>& cycles) {
        path.push_back(start);
        visited.insert(start);

        for (const auto& neighbor : exchanges[start]) { // Исправлено: используем правильное определение переменной
            if (find(path.begin(), path.end(), neighbor.first) == path.end()) {
                findCycles(neighbor.first, path, visited, cycles);
            }
            else if (neighbor.first == path[0] && path.size() > 2) {
                vector<string> cycle;
                for (const auto& apt : path) {
                    for (const auto& exchange : exchanges[apt]) {
                        cycle.push_back(exchange.second);
                        break;
                    }
                }
                sort(cycle.begin(), cycle.end());
                cycles.push_back(cycle);
            }
        }

        path.pop_back();
    }

public:
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        string line;

        while (getline(file, line)) {
            istringstream ss(line);
            string lastName, fromStr, toStr;
            getline(ss, lastName, ',');
            getline(ss, fromStr, ',');
            getline(ss, toStr, ',');

            Apartment from = parseApartment(fromStr);
            Apartment to = parseApartment(toStr);

            addExchange(from, to, lastName);
        }
    }

    void findExchanges(const Apartment& clientApartment, const string& clientLastName) const {
        cout << "Possible exchanges for You:" << endl;
        for (const auto& exchange : exchanges) { // Исправлено: правильная структура цикла
            const Apartment& apartment = exchange.first;
            const vector<pair<Apartment, string>>& list = exchange.second;

            if (apartment == clientApartment) {
                for (const auto& neighbor : list) {
                    cout << neighbor.second << endl;
                }
            }
        }
    }

    void findAllCycles() {
        set<Apartment> visited;
        vector<Apartment> path;
        vector<vector<string>> cycles;

        for (const auto& exchange : exchanges) { // Исправлено: правильная структура цикла
            const Apartment& apartment = exchange.first;

            if (visited.find(apartment) == visited.end()) {
                findCycles(apartment, path, visited, cycles);
            }
        }

        // Убираем дубликаты и выводим циклы
        sort(cycles.begin(), cycles.end());
        cycles.erase(unique(cycles.begin(), cycles.end()), cycles.end());

        cout << "\nAll possible exchange cycles:" << endl;
        for (const auto& cycle : cycles) {
            for (size_t i = 0; i < cycle.size(); ++i) {
                cout << cycle[i];
                if (i != cycle.size() - 1) cout << " -> ";
            }
            cout << endl;
        }
    }

private:
    Apartment parseApartment(const string& str) const {
        Apartment apt;
        apt.rooms = str[0] - '0';
        apt.balcony = (str[1] == '+');
        apt.phone = (str[2] == '+');
        return apt;
    }
};

int main() {
    CityExchange city;
    string filename = "exchanges.txt";

    city.loadFromFile(filename);

    Apartment clientApartment;
    string input;

    cout << "Enter your apartment and moving conditions (e.g., 2++,1--): ";
    getline(cin, input);

    istringstream ss(input);
    string aptStr;
    getline(ss, aptStr, ',');
    clientApartment = city.parseApartment(aptStr);

    cout << "\nYour options:" << endl;
    city.findExchanges(clientApartment, "You");

    city.findAllCycles();

    return 0;
}
