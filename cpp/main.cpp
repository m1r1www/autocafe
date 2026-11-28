// Демонстрационная программа ЛР 4. Университетский вендинг.
#include "base_product.hpp"
#include "coffee_product.hpp"
#include "instant_meal_product.hpp"
#include "machine_operator.hpp"
#include "snack_product.hpp"
#include "vending_machine.hpp"

#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    using namespace campus_vending;

    std::cout << "=== УНИВЕРСИТЕТСКИЙ ВЕНДИНГ: ЛАБОРАТОРНАЯ РАБОТА 4 ===\n";

    std::cout << "\n1. unique_ptr: один владелец и передача владения (ЛР 3)\n";
    {
        auto sugar = std::make_unique<Ingredient>("Сахар", 300, 100, 60);
        sugar->Print(10);
        auto newOwner = std::move(sugar);
        std::cout << "Исходный unique_ptr пуст: " << std::boolalpha << (!sugar) << '\n';
        newOwner->Print(10);
    }
    std::cout << "Блок закрыт: сахар уничтожен автоматически.\n";

    std::cout << "\n2. Динамический массив unique_ptr<Ingredient[]> (ЛР 3)\n";
    {
        auto ingredients = std::make_unique<Ingredient[]>(2);
        ingredients[0].Setup("Какао", 200, 100, 50);
        ingredients[1].Setup("Чай", 150, 80, 50);
        ingredients[0].Print(10);
        ingredients[1].Print(10);
    }

    std::cout << "\n2а. Массив отдельных unique_ptr<Ingredient> (ЛР 3)\n";
    {
        std::array<std::unique_ptr<Ingredient>, 2> shelf{
            std::make_unique<Ingredient>("Лимонад", 300, 200, 20),
            std::make_unique<Ingredient>("Сироп", 100, 50, 40)
        };
        for (const auto& item : shelf) item->Print(10);
    }

    std::cout << "\n3. Создание товаров и полиморфизм (ЛР 4)\n";
    auto machine = std::make_shared<VendingMachine>("Вендинг в корпусе Б", 10);
    std::cout << "shared_ptr владельцев сначала: " << machine.use_count() << '\n';
    {
        auto secondOwner = machine;
        std::cout << "После копирования shared_ptr: " << machine.use_count() << '\n';
    }
    std::cout << "После уничтожения копии shared_ptr: " << machine.use_count() << '\n';
    std::weak_ptr<VendingMachine> machineWatcher = machine;
    auto coffee = std::make_shared<CoffeeProduct>("Капучино", 130, 8, 250, true,
        std::make_shared<ProductDetails>(ProductDetails{ "Кофейный напиток для короткого перерыва" }));
    auto snack = std::make_shared<SnackProduct>("Шоколадный батончик", 75, 12, 55,
        std::make_shared<ProductDetails>(ProductDetails{ "Сладкий перекус между парами" }));
    auto meal = std::make_shared<InstantMealProduct>("Лапша быстрого приготовления", 110, 6, 5,
        std::make_shared<ProductDetails>(ProductDetails{ "Горячее блюдо, готовится за пять минут" }));

    // RegisterIn передаёт текущий объект автомату через shared_from_this() (использование this).
    coffee->RegisterIn(*machine);
    snack->RegisterIn(*machine);
    meal->RegisterIn(*machine);
    machine->PrintProducts();

    std::cout << "\n4. Конструкторы: по умолчанию, копирования, перемещения\n";
    CoffeeProduct defaultCoffee;
    CoffeeProduct copyCoffee(*coffee); // копирующий конструктор делает глубокую копию деталей
    CoffeeProduct movedCoffee(std::move(copyCoffee));
    std::cout << "Перемещённый товар: " << movedCoffee.Describe() << '\n';

    std::cout << "\n5. Поверхностная копия через shared_ptr\n";
    auto commonDetails = std::make_shared<ProductDetails>();
    commonDetails->description = "Одна общая карточка товара";
    CoffeeProduct shallowA("Американо", 100, 4, 200, false, commonDetails);
    CoffeeProduct shallowB(shallowA, ShallowCopyTag{});
    shallowB.SetDetailsDescription("Изменено через второй товар: общая карточка");
    std::cout << "Первый товар: " << shallowA.Describe() << '\n';
    std::cout << "Второй товар: " << shallowB.Describe() << '\n';

    std::cout << "\n6. Глубокая копия и Clone()\n";
    CoffeeProduct deepCopy(shallowA); // конструктор копирования создаёт собственный ProductDetails
    deepCopy.SetDetailsDescription("Отдельное описание глубокой копии");
    std::unique_ptr<BaseProduct> cloned = shallowA.Clone();
    std::cout << "Оригинал: " << shallowA.Describe() << '\n';
    std::cout << "Глубокая копия: " << deepCopy.Describe() << '\n';
    std::cout << "Clone() через BaseProduct: " << cloned->Describe() << '\n';

    std::cout << "\n7. Переопределение, строки и this\n";
    std::string label = std::string(coffee->GetName()) + " / " + std::string(snack->GetName());
    const auto found = label.find("батончик");
    std::cout << "Конкатенация: " << label << '\n';
    std::cout << "find(\"Батончик\"): " << (found != std::string::npos ? "найдено" : "не найдено") << '\n';
    std::cout << "substr: " << label.substr(0, label.find(" / ")) << '\n';
    std::cout << "Сравнение строк: " << (std::string(coffee->GetName()) == "Капучино" ? "название совпало" : "не совпало") << '\n';
    coffee->SetPrice(135).SetPrice(140); // метод возвращает *this
    *coffee = *coffee;                  // самоприсваивание безопасно: проверка this != &other
    std::cout << "Цена после цепочки вызовов: " << coffee->GetPrice() << " руб.\n";
    std::cout << "Создано объектов BaseProduct: " << BaseProduct::GetCreatedCount() << '\n';

    std::cout << "\n8. Массив unique_ptr<BaseProduct> и виртуальный деструктор\n";
    {
        std::array<std::unique_ptr<BaseProduct>, 3> shelf{
            std::make_unique<CoffeeProduct>("Латте", 145, 5, 300, true),
            std::make_unique<SnackProduct>("Ореховый батончик", 90, 7, 45),
            std::make_unique<InstantMealProduct>("Овсяная каша", 95, 4, 3)
        };
        for (const auto& item : shelf)
        {
            std::cout << item->Describe() << '\n';
        }
        std::cout << "-- выходим из блока: сначала деструктор наследника, затем BaseProduct\n";
    }

    std::cout << "\n9. Продажа товаров через автомат\n";
    MachineOperator operatorOnDuty("Дежурный оператор", machine);
    operatorOnDuty.LoadIngredient(machine, "Кофе", 100, 60, 40);
    operatorOnDuty.LoadIngredient(machine, "Вода", 2000, 1000, 40);
    operatorOnDuty.AddCups(machine, 5);
    operatorOnDuty.AddChange(machine, 500);
    operatorOnDuty.FinishMaintenance(machine);

    machine->SellProduct("Капучино", 50);  // отказ: недостаточно денег, остаток не меняется
    machine->SellProduct("Капучино", 700); // отказ: автомат не может выдать такую сдачу
    machine->SellProduct("Капучино", 150); // цена 140 руб., сдача 10 руб.
    machine->SellProduct("Шоколадный батончик", 100); // цена 75 руб., сдача 25 руб.
    machine->SellProduct("Лапша быстрого приготовления", 120); // цена 110 руб., сдача 10 руб.
    machine->SellProduct("Шоколадный батончик", 75); // без сдачи
    machine->PrintProducts();
    machine->PrintJournal();

    std::cout << "\n10. Сохранённый сценарий приготовления напитка из ЛР 3\n";
    const Recipe coffeeRecipe{ { "Кофе", 15 }, { "Вода", 100 } };
    machine->OrderDrink("Кофе", coffeeRecipe, 100, 120);
    machine->PrintState();

    std::cout << "\n11. weak_ptr: наблюдение без владения (ЛР 3)\n";
    if (auto locked = machineWatcher.lock())
        std::cout << "Пока владелец существует, lock() работает: " << locked->GetName() << '\n';
    machine.reset();
    if (machineWatcher.lock())
        std::cout << "Автомат ещё существует\n";
    else
        std::cout << "После сброса последнего shared_ptr lock() вернул пустой указатель\n";

    std::cout << "\n=== Конец программы ===\n";
    return 0;
}
