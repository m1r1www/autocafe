# Кафе-автомат: ЛР 3, часть 2 (C++, умные указатели)

Проект из ЛР 2, в котором обычные указатели заменены на умные:

- `std::unique_ptr` — объекты и массивы, у которых один владелец (`main.cpp`);
- `std::shared_ptr` — автомат (`std::make_shared<VendingMachine>`), им владеет `main`;
- `std::weak_ptr` — оператор следит за автоматом, но не владеет им (`machine_operator.hpp`).

В проекте нет ни одного `new` и ни одного оператора `delete`. Запись `= delete` в заголовках —
это запрет копирования класса, а не оператор `delete`.
Композиция `VendingMachine` — `Ingredient` осталась полем-объектом по значению (так и надо).

## Состав

- **Язык:** C++ (стандарт C++17)
- **Сборка:** g++ 
- **Данные:** текстовые файлы (menu.txt, orders.txt)

## Сборка и запуск

```bash
g++  *.cpp -o app
./app
```


## Состав
- `ingredient.hpp` / `ingredient.cpp` — класс `Ingredient`.
- `vending_machine.hpp` / `vending_machine.cpp` — класс `VendingMachine`.
- `machine_operator.hpp` / `machine_operator.cpp` — класс `MachineOperator`.
- `main.cpp` — демонстрация.

Дополнительные файлы сборки не нужны. `tests.cpp` в проект не входит.

## Демонстрируется
- private-поля;
- конструкторы и деструкторы;
- предметные методы;
- проверка недостатка ингредиента;
- статический объект, ссылка и указатель;
- `new/delete`, `new[]/delete[]`;
- массив указателей на динамические объекты;
- композиция `VendingMachine` → `Ingredient`;
- агрегация `MachineOperator` → `VendingMachine`;
- порядок уничтожения объектов.
