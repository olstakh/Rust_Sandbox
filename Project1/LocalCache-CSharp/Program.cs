
var cache = new LocalCache();

void ProcessInput(string input)
{
    switch (input.ToLower())
    {
        case "help":
            Console.WriteLine("Available commands: help, exit, list");
            break;
        case "list":
            Console.Out.WriteLine("Listing all items...");
            foreach (var kvp in cache.GetAll())
            {
                Console.WriteLine($"{kvp.Key}: {kvp.Value}");
            }
            break;
        default:
            Console.Error.WriteLine($"Unknown command: {input}");
            break;
    }
}

Console.WriteLine("Welcome to LocalCache-CSharp! Type your commands below.");
while (true)
{
    Console.Write("> ");
    var input = Console.In.ReadLine();
    if (input == null || input.ToLower() == "exit")
    {
        break;
    }

    ProcessInput(input);
}

Console.WriteLine("Goodbye!");