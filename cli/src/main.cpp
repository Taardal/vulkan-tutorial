#include "App.h"
#include "Command.h"
#include "FileSystem.h"
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

using namespace VulkandemoCLI;

struct Env
{
    std::string Key;
    std::string Value;

    std::string ToString()
    {
        std::stringstream ss;
        ss << Key << "=" << Value;
        return ss.str();
    }

    static Env FromString(const std::string& envString)
    {
        int equalSignPosition = envString.find("=");
        Env env;
        env.Key = envString.substr(0, equalSignPosition);
        env.Value = envString.substr(equalSignPosition + 1, envString.length());
        return env;
    }
};

void ReadEnvs(const char* path)
{
    FileSystem fs;

    const std::string& foo = fs.ReadFile(path);
    printf("%s\n", foo.c_str());
    printf("\n");

    std::vector<Env> envs;
    std::istringstream iss(foo);
    for (std::string line; std::getline(iss, line);)
    {
        printf("%s\n", line.c_str());
        /*
        int delimiterPosition = line.find("=");
        printf("%d\n", delimiterPosition);
        Env env;
        env.Key = line.substr(0, delimiterPosition);
        env.Value = line.substr(delimiterPosition + 1, line.length());
        printf("%s, %s\n", env.Key.c_str(), env.Value.c_str());
        */
        envs.push_back(Env::FromString(line));
    }
    printf("\n");

    printf("%lu\n", envs.size());
    for (Env env : envs)
    {
        printf("%s\n", env.Key.c_str());
    }
    printf("\n");

    printf("%lu\n", envs.size());
    for (Env env : envs)
    {
        printf("%s\n", env.Value.c_str());
    }
    printf("\n");

    printf("%lu\n", envs.size());
    for (Env env : envs)
    {
        printf("%s, %s\n", env.Key.c_str(), env.Value.c_str());
    }
    printf("\n");

    for (Env env : envs)
    {
        printf("GETENV BEFORE: %s\n", getenv(env.Key.c_str()));

        //putenv(&env.ToString()[0]);

        std::string e = env.ToString();
        char char_array[e.length() + 1];
        strcpy(char_array, e.c_str());
        //putenv(char_array);
        int overwrite = 0;
        setenv(env.Key.c_str(), env.Value.c_str(), overwrite);

        printf("GETENV AFTER: %s\n", getenv(env.Key.c_str()));
    }
    printf("\n");

    printf("%s\n", "WORK?");
    printf("%s\n", getenv("GLFW"));
    printf("%s\n", getenv("GLM"));
    printf("%s\n", getenv("SPDLOG"));
    printf("\n");
}

int main(int argc, char* argv[])
{
    ReadEnvs(".env");
    ReadEnvs(".dependencies");

    auto* app = new App();
    Command* command = app->GetCommand(argc, argv);
    if (command != nullptr)
    {
        command->Execute();
    }
    delete app;
    return 0;
}