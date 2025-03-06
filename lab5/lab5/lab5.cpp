#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cmath>
using namespace std;

struct Record {
    int key;
    string value;

    Record(int k, string v) : key(k), value(v) {}
};

const int tableSize = 10;
list<Record> table[tableSize];

int n = 0;  // Количество записей
int m = 0;  // Среднее число коллизий
int b = 0;  // Простое число, вычисленное на основе m и n
bool flag = false;

int hashFunction(int key) {
    return key % tableSize;
}

void insert(int key, const string& value) {
    int index = hashFunction(key);
    table[index].push_back(Record(key, value));
    n++; // Увеличиваем количествa записей
}

string search(int key) {
    int index = hashFunction(key);
    for (auto& record : table[index]) {
        if (record.key == key) {
            return record.value;
        }
    }
    return "Not found";
}

void remove(int key) {
    int index = hashFunction(key);
    auto& chain = table[index];

    for (auto it = chain.begin(); it != chain.end(); ++it) {
        if (it->key == key) {
            chain.erase(it);
            n--; // Уменьшаем количество записей
            cout << "Record with key " << key << " has been removed." << endl;
            return;
        }
    }
    cout << "Record with key " << key << " not found." << endl;
}

// Проверка на простоту
bool isPrime(int number) {
    if (number <= 1) return false;
    for (int i = 2; i <= sqrt(number); i++) {
        if (number % i == 0) return false;
    }
    return true;
}

// Находим среднее число
void calculateAverageCollisions() {
    int chainCount = 0;    // Количество индексов с коллизией
    int totalLength = 0;   // Сумма длин всех цепочек

    for (int i = 0; i < tableSize; i++) {
        if (table[i].size() > 1) { 
            chainCount++;
            totalLength += table[i].size();
        }
    }

    // Среднее число коллизий
    m = chainCount > 0 ? totalLength / chainCount : 0;

    // Вычисление b
    b = m > 0 ? n / m : 0;  // Целочисленное деление n на m
    while (!isPrime(b)) {   // Проверяем, является ли b простым числом
        b++;
    }
    flag = true;
}

void display() {
    for (int i = 0; i < tableSize; i++) {
        cout << "Index " << i << ": ";
        for (auto& record : table[i]) {
            cout << "(" << record.key << ", " << record.value << ") ";
        }
        cout << endl;
    }
    cout << "Total number of records: " << n << endl;
    cout << "Average number of collisions: " << m << endl;
    cout << "Value of b (nearest prime): " << b << endl;
}

// Функция для загрузки данных из файла
void loadDataFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        int key;
        string value;
        ss >> key >> value;

        if (!ss.fail()) {
            insert(key, value);
        }
        else {
            cerr << "Warning: Invalid line in file: " << line << endl;
        }
    }
    file.close();
}

void menu() {
    int choice;
    int key;

    do {
        cout << "\nMenu:\n";
        cout << "1. Display hash table\n";
        cout << "2. Search by key\n";
        cout << "3. Remove by key\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            display();
            break;
        case 2:
            cout << "\nEnter a key to search: ";
            cin >> key;
            cout << "Searching for key " << key << ": " << search(key) << endl;
            break;
        case 3:
            cout << "\nEnter a key to remove: ";
            cin >> key;
            remove(key);
            break;
        case 4:
            cout << "Exiting the program. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please enter a valid option." << endl;
            break;
        }
    } while (choice != 4);
}

int main() {
    string filename;

    cout << "Enter the name of the file containing the data: ";
    cin >> filename;
    loadDataFromFile(filename);
    if (!flag) calculateAverageCollisions(); // Пересчёт среднего числа коллизий и значения b
    menu();

    return 0;
}
