#include <cstdio>
#include <assert.h>
#include <map>
#include <string>
#include <iostream>
#include <utility>
#include <sstream>
#include <vector>

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

vector<string> ParseLine(const string& line)
{
    vector<string> tokens;
    istringstream iss(line);
    string token;
    while (iss >> token)
    {
        tokens.push_back(token);
    }
    return tokens;
}

int main()
{
    map<string, string> g_cache;

    printf("Welcome to LocalCache!\n");
    do
    {
        printf("> ");
        string input;
        getline(cin, input);
        if (input.empty())
        {
            continue;
        }
        auto tokens = ParseLine(input);
        string command = tokens[0];

        if (command == "exit")
        {
            break;
        }
        if (command == "list")
        {
            ListValues(g_cache);
            continue;
        }
        if (command == "set")
        {
            assert(tokens.size() > 2);
            SetValue(tokens[1], tokens[2], g_cache);
            continue;
        }
        if (command == "get")
        {
            assert(tokens.size() > 1);
            GetValue(tokens[1], g_cache);
            continue;
        }
        if (command == "remove")
        {
            assert(tokens.size() > 1);
            RemoveValue(tokens[1], g_cache);
            continue;
        }
        if (command == "clear")
        {
            ClearValues(g_cache);
            continue;
        }
        if (command == "stats")
        {
            // Count entry count, cache hits, cache misses, evictions, etc.
        }
        if (command == "help")
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
