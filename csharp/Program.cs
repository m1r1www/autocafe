using System.Runtime.CompilerServices;
using Cafe;

/// <summary>Демонстрационная программа ЛР 3, часть 1 (C#).</summary>
class Program
{
    // Объект создаётся в отдельном методе: после выхода из него обычных ссылок на объект нет.
    // NoInlining не даёт компилятору встроить метод в Main и случайно удержать объект.
    [MethodImpl(MethodImplOptions.NoInlining)]
    static WeakReference<Ingredient> CreateTemporary()
    {
        var temp = new Ingredient("Списанное молоко", 5, 10);
        return new WeakReference<Ingredient>(temp);
    }

    // Проверка тоже в отдельном методе: так в Main не остаётся временной переменной с ссылкой на объект.
    [MethodImpl(MethodImplOptions.NoInlining)]
    static bool IsAlive(WeakReference<Ingredient> reference)
    {
        return reference.TryGetTarget(out _);
    }

    static void Main()
    {
        Console.OutputEncoding = System.Text.Encoding.UTF8;

        Console.WriteLine("=== 1. Ссылки и копии ===");
        var first = new Ingredient("Молоко", 50, 100);
        Ingredient second = first;   // не копия объекта, а вторая ссылка на тот же объект
        second.Consume(10);          // меняем через вторую ссылку
        Console.WriteLine($"first:  {first}");
        Console.WriteLine($"second: {second}");
        Console.WriteLine($"first == second: {first == second}");

        Console.WriteLine("-- то же самое со структурой");
        var priceA = new Money { Rubles = 150, Kopecks = 50 };
        var priceB = priceA;         // копия значения, а не ссылка
        priceB.Rubles = 300;         // меняем копию
        Console.WriteLine($"priceA: {priceA}");
        Console.WriteLine($"priceB: {priceB}");

        Console.WriteLine("\n=== 2. Композиция, агрегация и проверка правила ===");
        var machine = new VendingMachine("Автомат-1", 40);                 // создан снаружи
        var operatorOnDuty = new MachineOperator("Оператор-1", machine);   // получает готовый
        operatorOnDuty.AddCoffee(20);
        Console.WriteLine(machine);
        machine.MakeCoffee();
        machine.MakeCoffee();
        machine.MakeCoffee();
        Console.WriteLine("-- проверка правила: кофе закончился");
        machine.MakeCoffee();
        operatorOnDuty.StopMachine();
        machine.MakeCoffee();
        operatorOnDuty.StartMachine();
        operatorOnDuty.AddCoffee(40);
        Console.WriteLine(machine);

        Console.WriteLine("\n=== 3. Освобождение ресурса: using и IDisposable ===");
        using (var log = new SalesLog("Журнал смены"))
        {
            log.Record("Кофе", machine.MakeCoffee());
            log.Record("Кофе", machine.MakeCoffee());
            Console.WriteLine("-- выход из блока");
        }
        Console.WriteLine("-- блок закончился, журнал уже закрыт");
        Console.WriteLine(machine);

        Console.WriteLine("\n=== 4. Сборщик мусора ===");
        var watcher = CreateTemporary();
        Console.WriteLine("-- метод завершился, ссылок на объект больше нет");
        Console.WriteLine($"объект жив: {IsAlive(watcher)}");
        GC.Collect();                    // просьба собрать мусор
        GC.WaitForPendingFinalizers();
        Console.WriteLine("-- вызван GC.Collect()");
        Console.WriteLine($"объект жив: {IsAlive(watcher)}");

        Console.WriteLine("\n=== Конец программы ===");
    }
}
