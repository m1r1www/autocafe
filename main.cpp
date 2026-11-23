#include "ingredient.hpp"
#include "machine_operator.hpp"
#include "vending_machine.hpp"

#include <iostream>

int main()
{
    using cafe::Ingredient;
    using cafe::MachineOperator;
    using cafe::VendingMachine;

    std::cout << "=== КАФЕ-АВТОМАТ: демонстрация лабораторной ===\n\n";

    std::cout << "1. Статический объект, ссылка и указатель\n";
    static Ingredient milk("Молоко", 50, 100);
    Ingredient& milkRef = milk;
    Ingredient* milkPtr = &milk;
    milkRef.Print();
    milkPtr->Consume(10);
    milk.Print();

    std::cout << "\n2. Динамическая память\n";
    Ingredient* sugar = new Ingredient("Сахар", 30, 50);
    sugar->Print();
    delete sugar;

    std::cout << "-- динамический массив объектов\n";
    Ingredient* array = new Ingredient[2];
    array[0].Setup("Какао", 10, 50);
    array[1].Setup("Чай", 20, 50);
    array[0].Load(10);
    array[1].Load(20);
    array[0].Print();
    array[1].Print();
    delete[] array;

    std::cout << "-- массив динамических объектов\n";
    Ingredient* pointers[2] = { new Ingredient("Вода", 100, 200), new Ingredient("Сироп", 40, 50) };
    pointers[0]->Print();
    pointers[1]->Print();
    delete pointers[0];
    delete pointers[1];

    std::cout << "\n3. Композиция: автомат владеет ингредиентом\n";
    {
        VendingMachine temporary("Временный автомат", 40);
        temporary.PrintState();
        temporary.MakeCoffee();
        temporary.PrintState();
        std::cout << "-- выход из блока: автомат и его ингредиент уничтожаются\n";
    }

    std::cout << "\n4. Агрегация: оператор использует готовый автомат\n";
    VendingMachine machine("Автомат-1", 40);
    {
        MachineOperator operatorOnDuty("Оператор-1", machine);
        // machine.Stop(); и machine.RestockCoffee(20); не скомпилируются: доступны только оператору
        operatorOnDuty.AddCoffee(20);
        machine.PrintState();
        machine.MakeCoffee();
        machine.MakeCoffee();
        machine.MakeCoffee();
        std::cout << "-- проверка правила: кофе закончился\n";
        machine.MakeCoffee();
        operatorOnDuty.StopMachine();
        machine.MakeCoffee();
        operatorOnDuty.StartMachine();
    }
    std::cout << "-- оператор уничтожен, но автомат продолжает существовать\n";
    machine.PrintState();

    std::cout << "\n=== Конец программы ===\n";
    return 0;
}
