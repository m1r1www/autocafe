#include "ingredient.hpp"
#include "machine_operator.hpp"
#include "vending_machine.hpp"

#include <iostream>
#include <memory>

int main()
{
    using cafe::Ingredient;
    using cafe::MachineOperator;
    using cafe::VendingMachine;

    std::cout << "=== ЛР 3, часть 2: умные указатели ===\n\n";

    std::cout << "0. Статический объект (как в ЛР 2)\n";
    static Ingredient milk("Молоко", 50, 100);
    milk.Print();

    // ------------------------------------------------------------------ unique_ptr
    std::cout << "\n1. unique_ptr: один владелец\n";
    {
        std::unique_ptr<Ingredient> sugar = std::make_unique<Ingredient>("Сахар", 30, 50);
        sugar->Print();
        std::cout << "-- выход из блока: освобождать память вручную не нужно\n";
    }
    std::cout << "-- блок закрыт, Сахар уже уничтожен\n";

    std::cout << "-- передача владения через std::move\n";
    {
        std::unique_ptr<Ingredient> honey    = std::make_unique<Ingredient>("Мёд", 20, 40);
        std::unique_ptr<Ingredient> newOwner = std::move(honey);
        if (!honey)
        {
            std::cout << "honey пуст: владение передано\n";
        }
        newOwner->Print();
        std::cout << "-- выход из блока\n";
    }

    std::cout << "-- динамический массив: unique_ptr<Ingredient[]>\n";
    {
        std::unique_ptr<Ingredient[]> array = std::make_unique<Ingredient[]>(2);
        array[0].Setup("Какао", 10, 50);
        array[1].Setup("Чай", 20, 50);
        array[0].Print();
        array[1].Print();
        std::cout << "-- выход из блока\n";
    }

    std::cout << "-- массив из unique_ptr на отдельные объекты\n";
    {
        std::unique_ptr<Ingredient> pointers[2];
        pointers[0] = std::make_unique<Ingredient>("Вода", 100, 200);
        pointers[1] = std::make_unique<Ingredient>("Сироп", 40, 50);
        pointers[0]->Print();
        pointers[1]->Print();
        std::cout << "-- выход из блока\n";
    }

    // ------------------------------------------------------------------ shared_ptr
    std::cout << "\n2. shared_ptr: несколько владельцев\n";
    std::shared_ptr<VendingMachine> machine = std::make_shared<VendingMachine>("Автомат-1", 40);
    std::cout << "владельцев автомата: " << machine.use_count() << '\n';
    {
        std::shared_ptr<VendingMachine> second = machine;
        std::cout << "после копирования указателя: " << machine.use_count() << '\n';
        std::cout << "-- выход из блока: копия исчезла, автомат жив\n";
    }
    std::cout << "после выхода из блока: " << machine.use_count() << '\n';

    // ------------------------------------------------------------------ weak_ptr
    std::cout << "\n3. weak_ptr: оператор наблюдает за автоматом\n";
    MachineOperator operatorOnDuty("Оператор-1", machine);
    std::cout << "владельцев автомата (оператор не владеет): " << machine.use_count() << '\n';
    operatorOnDuty.PrintStatus();

    operatorOnDuty.AddCoffee(20);
    machine->PrintState();
    machine->MakeCoffee();
    machine->MakeCoffee();
    machine->MakeCoffee();
    std::cout << "-- проверка правила: кофе закончился\n";
    machine->MakeCoffee();
    operatorOnDuty.StopMachine();
    machine->MakeCoffee();
    operatorOnDuty.StartMachine();

    std::cout << "-- последний владелец отказывается от автомата\n";
    machine.reset();
    std::cout << "-- автомат уничтожен, оператор ещё жив\n";
    operatorOnDuty.PrintStatus();
    operatorOnDuty.AddCoffee(10);

    std::cout << "\n=== Конец программы ===\n";
    return 0;
}
