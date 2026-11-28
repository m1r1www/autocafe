using VendingCatalog = CampusVending.VendingMachine;
using static System.Console;
using CampusVending;

internal static class Program
{
    private static void Main()
    {
        OutputEncoding = System.Text.Encoding.UTF8;
        WriteLine("=== УНИВЕРСИТЕТСКИЙ ВЕНДИНГ: ЛАБОРАТОРНАЯ РАБОТА 4 (C#) ===");

        WriteLine("\n1. Абстрактный класс, наследники и полиморфизм");
        var machine = new VendingCatalog("Вендинг в корпусе Б");
        var coffee = new CoffeeProduct("Капучино", 130, 8, 250, true,
            new ProductDetails("Кофе с молоком для короткого перерыва"));
        var snack = new SnackProduct("Шоколадный батончик", 75, 12, 55);
        var meal = new InstantMealProduct("Лапша быстрого приготовления", 110, 6, 5,
            new ProductDetails("Горячее блюдо за 5 минут"));

        coffee.RegisterIn(machine);
        snack.RegisterIn(machine);
        meal.RegisterIn(machine);
        machine.PrintProducts();

        WriteLine("\n2. Поверхностное копирование через MemberwiseClone()");
        var shallow = coffee.ShallowCopy();
        shallow.SetDetailsDescription("Описание изменено через поверхностную копию");
        WriteLine("Оригинал: " + coffee.Describe());
        WriteLine("Поверхностная копия: " + shallow.Describe());

        WriteLine("\n3. Глубокое копирование и Clone()");
        var deep = new CoffeeProduct(coffee);
        deep.SetDetailsDescription("Собственное описание глубокой копии");
        BaseProduct clone = coffee.Clone();
        WriteLine("Оригинал: " + coffee.Describe());
        WriteLine("Глубокая копия: " + deep.Describe());
        WriteLine("Clone(): " + clone.Describe());

        WriteLine("\n4. Строки, this, статические члены и сравнение объектов");
        string label = coffee.Name + " / " + snack.Name;
        WriteLine("Конкатенация: " + label);
        WriteLine("IndexOf: " + label.IndexOf("батончик", StringComparison.OrdinalIgnoreCase));
        WriteLine("Substring: " + label.Substring(0, label.IndexOf(" / ", StringComparison.Ordinal)));
        WriteLine("Сравнение строк: " + (string.Compare(coffee.Name, "Капучино", StringComparison.Ordinal) == 0));
        coffee.SetPrice(135).SetPrice(140); // метод возвращает this
        var sameCoffee = new CoffeeProduct("Капучино", 140, 8, 250, true);
        var differentCoffee = new CoffeeProduct("Капучино", 140, 8, 300, true);
        WriteLine("Одинаковые характеристики: " + coffee.Equals(sameCoffee));
        WriteLine("Другой объём: " + coffee.Equals(differentCoffee));
        WriteLine("HashCode: " + coffee.GetHashCode());
        WriteLine("Создано товаров: " + BaseProduct.CreatedCount);

        WriteLine("\n5. Продажа товаров через автомат");
        var operatorOnDuty = new MachineOperator("Дежурный оператор", machine);
        operatorOnDuty.AddCoffee(20);
        operatorOnDuty.AddChange(500);
        operatorOnDuty.StartMachine();
        operatorOnDuty.AddCoffee(5);  // отказ: пополнение разрешено только при выключенном автомате
        operatorOnDuty.AddChange(100); // отказ: размен пополняется только при выключенном автомате
        operatorOnDuty.StartMachine(); // отказ: автомат уже работает
        machine.MakeCoffee();
        machine.SellProduct("Капучино", 50);  // отказ: недостаточно денег
        machine.SellProduct("Капучино", 700); // отказ: недостаточно размена
        machine.SellProduct("Капучино", 150); // цена 140 руб., сдача 10 руб.
        machine.SellProduct("Шоколадный батончик", 100); // цена 75 руб., сдача 25 руб.
        machine.SellProduct("Лапша быстрого приготовления", 120); // цена 110 руб., сдача 10 руб.
        machine.PrintProducts();
        machine.PrintJournal();
        WriteLine(machine);
        operatorOnDuty.StopMachine();
        operatorOnDuty.StopMachine(); // отказ: автомат уже выключен

        WriteLine("\n=== Конец программы ===");
    }
}
