#include <cstdio>
#include <map>
#include <string>
#include <iostream>
#include <utility>

using namespace std;

void GetValue(const string& key, const map<string, string>& cache)
{
    auto it = cache.find(key);
    if (it != cache.end())
    {
        printf("%s\n", it->second.c_str());
    }
    else
    {
        printf("Key not found\n");
    }
}

void ListValues(const map<string, string>& cache)
{
    for (const auto& pair : cache)
    {
        printf("%s: %s\n", pair.first.c_str(), pair.second.c_str());
    }
}

void SetValue(const string& key, const string& value, map<string, string>& cache)
{
    cache.insert_or_assign(key, value);
}

void RemoveValue(const string& key, map<string, string>& cache)
{
    cache.erase(key);
}

void ClearValues(map<string, string>& cache)
{
    cache.clear();
}

int main()
{
    map<string, string> g_cache;

    printf("Welcome to LocalCache!\n");
    do
    {
        printf("> ");
        string input;
        if (!(cin >> input))
        {
            break;
        };

        if (input == "exit")
        {
            break;
        }
        if (input == "list")
        {
            ListValues(g_cache);
            continue;
        }
        if (input == "set")
        {
            string key, value;
            cin >> key >> value;
            SetValue(key, value, g_cache);
            continue;
        }
        if (input == "get")
        {
            string key;
            cin >> key;
            GetValue(key, g_cache);
            continue;
        }
        if (input == "remove")
        {
            string key;
            cin >> key;
            RemoveValue(key, g_cache);
            continue;
        }
        if (input == "clear")
        {
            ClearValues(g_cache);
            continue;
        }
        if (input == "help")
        {
            printf("Commands:\n");
            printf("  set <key> <value> - Set a key-value pair\n");
            printf("  get <key> - Get the value for a key\n");
            printf("  remove <key> - Remove a key-value pair\n");
            printf("  list - List all key-value pairs\n");
            printf("  clear - Clear all key-value pairs\n");
            printf("  exit - Exit the program\n");
            continue;
        }
        printf("Unknown command. Type 'help' for a list of commands.\n");
    } while (true);
}
