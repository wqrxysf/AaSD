/*
Москвичев Александр Евгеньевич, ПС-22, Visual Studio 2022

 8. Выборы  старосты  в  группе  студентов  из   M   человек
организованы  по следующим правилам.  Задаются целые числа N и
K.  Студенты становятся по  кругу  в  соответствии  со  своими
номерами в журнале. Начиная от N-го студента отсчитывается K-й
студент.  Счет ведется циклически по возрастанию номеров. Этот
студент   выбывает  из  претендентов.  Начиная  со  следующего
студента,  процедура повторяется. Последний оставшийся студент
становится старостой. Ввести значения M,  N,  K и найти номер
старосты (7).
*/

#include <iostream>
#include <list>

int main() {
    std::setlocale(LC_ALL, "RU");

    int M, N, K;
    std::cout << "Введите количество студентов (M): ";
    std::cin >> M;
    std::cout << "Введите номер студента, с которого начинается отсчет (N): ";
    std::cin >> N;
    std::cout << "Введите шаг отсчета (K): ";
    std::cin >> K;
    if (M <= 0 || N <= 0 || N > M || K <= 0) {
        std::cout << "Некорректные значения M, N или K." << std::endl;
        return 1;
    }

    std::list<int> students;
    for (int i = 1; i <= M; ++i) {
        students.push_back(i);
    }

    auto it = students.begin();
    std::advance(it, N - 1);

    while (students.size() > 1) {
        for (int i = 1; i < K; ++i) {
            ++it;
            if (it == students.end()) {
                it = students.begin();
            }
        }
        it = students.erase(it);
        if (it == students.end()) {
            it = students.begin();
        }
    }
    std::cout << "Номер старосты: " << *students.begin() << std::endl;

    return 0;
}
 
