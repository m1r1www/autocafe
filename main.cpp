// Демонстрационная программа лабораторной работы №2.
// Вся логика находится в классах библиотеки cafe_core; здесь только создание объектов и вызовы методов.

#include "ingredient.hpp"
#include "machine_operator.hpp"
#include "vending_machine.hpp"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    using cafe::Ingredient;
    using cafe::MachineOperator;
    using cafe::Recipe;
    using cafe::VendingMachine;

    const Recipe coffee{ { "Кофе", 15 }, { "Вода", 100 } };
    constexpr int TODAY = 10;

    // ---------------------------------------------------------------- 1
    std::cout << "=== 1. Статическая инициализация, ссылка и указатель ===\n";
    Ingredient milk("Молоко", 500, 200, 12);

    Ingredient& milkRef = milk; // работа по ссылке
    milkRef.Print(TODAY);
    milkRef.Consume(50, TODAY); // корректное действие
    milkRef.Print(TODAY);

    Ingredient* milkPtr = &milk;     // работа по указателю
    milkPtr->Consume(50, TODAY + 5); // нарушение правила: молоко просрочено
    milkPtr->Load(400);              // нарушение правила: превышение вместимости
    milkPtr->Print(TODAY + 5);

    // ---------------------------------------------------------------- 2
    std::cout << "\n=== 2. Динамическая память ===\n";
    std::cout << "-- один объект: new / delete\n";
    Ingredient* sugar = new Ingredient("Сахар", 1000, 300, 60);
    sugar->Print(TODAY);
    delete sugar;

    std::cout << "-- динамический массив объектов: new[] / delete[]\n";
    Ingredient* array = new Ingredient[2];
    array[0].Setup("Какао", 200, 100, 50);
    array[1].Setup("Чай", 200, 150, 50);
    array[0].Print(TODAY);
    array[1].Print(TODAY);
    delete[] array;

    std::cout << "-- массив динамических объектов (указатели)\n";
    Ingredient* pointers[2] = { new Ingredient("Лимонад", 300, 300, 20), new Ingredient("Сироп", 100, 80, 40) };
    for (const Ingredient* item : pointers)
    {
        item->Print(TODAY);
    }
    for (Ingredient* item : pointers)
    {
        delete item;
    }

    // ---------------------------------------------------------------- 3
    std::cout << "\n=== 3. Композиция: целое создаётся во вложенном блоке ===\n";
    {
        VendingMachine temporary("Временный автомат", TODAY);
        MachineOperator temporaryOperator("Оператор-2", temporary);
        temporaryOperator.LoadIngredient(temporary, "Кофе", 100, 50, 40);
        temporaryOperator.LoadIngredient(temporary, "Вода", 2000, 1000, 40);
        temporaryOperator.AddCups(temporary, 1);
        temporaryOperator.AddChange(temporary, 50);
        temporaryOperator.FinishMaintenance(temporary);
        std::cout << "-- часть используется через методы целого: заказ списывает ингредиенты\n";
        temporary.OrderDrink("Кофе", coffee, 100, 100);
        temporary.PrintState();
        std::cout << "-- выход из блока\n";
    }
    std::cout << "-- блок закрыт\n";

    // ---------------------------------------------------------------- 4
    std::cout << "\n=== 4. Агрегация: автомат создан раньше, оператор живёт в блоке ===\n";
    VendingMachine machine("Автомат-1", TODAY);
    VendingMachine spareMachine("Автомат-2", TODAY);
    {
        MachineOperator operatorOnDuty("Оператор-1", machine);
        operatorOnDuty.LoadIngredient(machine, "Кофе", 100, 50, 40);
        operatorOnDuty.LoadIngredient(machine, "Вода", 2000, 1000, 40);
        operatorOnDuty.AddCups(machine, 2);
        operatorOnDuty.AddChange(machine, 50);
        operatorOnDuty.FinishMaintenance(machine);
        machine.PrintState();

        std::cout << "-- корректный заказ\n";
        machine.OrderDrink("Кофе", coffee, 100, 120);

        std::cout << "-- нарушение: внесено меньше цены\n";
        machine.OrderDrink("Кофе", coffee, 100, 50);

        std::cout << "-- нарушение: не хватает размена\n";
        machine.OrderDrink("Кофе", coffee, 100, 200);

        std::cout << "-- нарушение правила оператора: автомат не закреплён\n";
        operatorOnDuty.AddCups(spareMachine, 1);

        std::cout << "-- сбой автомата, заказ и прямой возврат в «работает»\n";
        machine.ReportFault();
        machine.OrderDrink("Кофе", coffee, 100, 100);
        operatorOnDuty.FinishMaintenance(machine);

        std::cout << "-- обслуживание: пополнение и возврат в работу\n";
        operatorOnDuty.StartMaintenance(machine);
        operatorOnDuty.Restock(machine, "Кофе", 80); // нарушение: превысит вместимость
        operatorOnDuty.Restock(machine, "Кофе", 20);
        operatorOnDuty.AddCups(machine, 1);
        operatorOnDuty.FinishMaintenance(machine);
        machine.PrintState();
        std::cout << "-- выход из блока (оператор уничтожается)\n";
    }

    std::cout << "-- оператора больше нет, автомат жив:\n";
    machine.PrintState();
    machine.OrderDrink("Кофе", coffee, 100, 100);
    machine.PrintJournal();

    std::cout << "\n=== Конец main: уничтожаются автоматы и статический ингредиент ===\n";
    return 0;
}
