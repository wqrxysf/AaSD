/*
 Общими требованиями к лабораторной работе являются:
   1) вводить   исходное   дерево  из  файла  в  понятной  для
пользователя форме, а не с клавиатуры;
   2) по требованию прользователя показывать дерево на экране;
   3) обрабатывать  дерево  в  оперативной памяти,  а не путем
многократного обращения к файлу;
   4) обеспечить   возможность   многократных   запросов   без
повторного запуска программы.

14. Информация  о  файлах  на  жестких  дисках   компьютера
записана с помощью дерева. Внешнюю память компьютера увеличили
путем подключения нового жесткого диска. Решено  перенести  на
него все папки, в которых содержатся только файлы *.cpp вместе
с папками до корня дерева. Сформировать и показать  на  экране
получившееся дерево папок (10).

Москвичев Александр Евгеньевич, ПС-22, Visual Studio 2022
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct TreeNode {
    string name;
    vector<TreeNode*> children;
    TreeNode* parent = nullptr;
    int level = 0;
    bool containsCppOnly = true; 
};

// Проверка строки на наличие *.cpp
bool isCppFile(const string& name) {
    return name.size() > 4 && name.substr(name.size() - 4) == ".cpp";
}

// Функция для чтения дерева из файла
TreeNode* readTreeFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл: " << filename << endl;
        return nullptr;
    }

    TreeNode* root = nullptr;
    TreeNode* lastNode = nullptr;
    int lastLevel = 0;

    string line;
    while (getline(file, line)) {
        int level = 0;
        while (level < line.size() && line[level] == '.') level++;

        string name = line.substr(level);
        auto* node = new TreeNode{ name, {}, nullptr, level };

        if (level == 0) {
            root = node; 
        }
        else if (level == lastLevel + 1) {
            lastNode->children.push_back(node);
            node->parent = lastNode;
        }
        else {
            TreeNode* parent = lastNode;
            for (int i = 0; i < lastLevel - level + 1; ++i) {
                parent = parent->parent;
            }
            parent->children.push_back(node);
            node->parent = parent;
        }

        lastNode = node;
        lastLevel = level;
    }
    file.close();
    return root;
}

// Рекурсивно отображаем дерево
void printTree(TreeNode* node, int ind = 0) {
    if (!node) return;
    for (int i = 0; i < ind; ++i) cout << ".";
    cout << node->name << endl;
    for (auto* child : node->children) {
        printTree(child, ind + 1);
    }
}

// Рекурсивная обработка дерева
void processTree(TreeNode* node) {
    if (!node) return;

    bool isFolder = node->name.find('.') == string::npos;
    if (!isFolder) { // файл
        node->containsCppOnly = isCppFile(node->name);
    }
    else { // папка
        for (auto* child : node->children) {
            processTree(child);
            if (!child->containsCppOnly) {
                node->containsCppOnly = false;
            }
        }
    }
}

// Построение нового дерева только с *.cpp
TreeNode* buildFilteredTree(TreeNode* node) {
    if (!node) return nullptr;

    if (node->name.find('.') != string::npos && !isCppFile(node->name)) {
        return nullptr; 
    }

    auto* newNode = new TreeNode{ node->name };
    for (auto* child : node->children) {
        TreeNode* filteredChild = buildFilteredTree(child);
        if (filteredChild) {
            filteredChild->parent = newNode;
            newNode->children.push_back(filteredChild);
        }
    }

    // Пустые папки
    if (newNode->children.empty() && newNode->name.find('.') == string::npos) {
        delete newNode;
        return nullptr;
    }

    return newNode;
}

// Освобождение памяти
void deleteTree(TreeNode* node) {
    if (!node) return;
    for (auto* child : node->children) {
        deleteTree(child);
    }
    delete node;
}

int main() {
    setlocale(LC_ALL, "Russian");

    TreeNode* root = nullptr;

    while (true) {
        cout << "\nКоманды:\n"
            << "1. load - Загрузить новое дерево из файла\n"
            << "2. show - Показать текущее дерево\n"
            << "3. cpp_tree - Построить и показать дерево с *.cpp\n"
            << "4. exit - Выйти из программы\n"
            << "Введите команду: ";

        string command;
        cin >> command;

        if (command == "load") {
            string filename;
            cout << "Введите имя файла: ";
            cin >> filename;

            if (root) deleteTree(root);
            root = readTreeFromFile(filename);

            if (root) {
                processTree(root);
                cout << "Дерево успешно загружено и обработано.\n";
            }
            else {
                cout << "Не удалось загрузить дерево.\n";
            }
        }
        else if (command == "show") {
            if (root) {
                cout << "Текущее дерево:\n";
                printTree(root);
            }
            else {
                cout << "Дерево не загружено. Используйте команду load.\n";
            }
        }
        else if (command == "cpp_tree") {
            if (root) {
                TreeNode* filteredTree = buildFilteredTree(root);
                if (filteredTree) {
                    cout << "Дерево с папками, содержащими только *.cpp:\n";
                    printTree(filteredTree);
                    deleteTree(filteredTree);
                }
                else {
                    cout << "Нет подходящих папок с *.cpp файлами.\n";
                }
            }
            else {
                cout << "Дерево не загружено. Используйте команду load.\n";
            }
        }
        else if (command == "exit") {
            break;
        }
        else {
            cout << "Неизвестная команда, попробуйте снова.\n";
        }
    }

    if (root) deleteTree(root); // Освобождаем память при выходе
    return 0;
}
