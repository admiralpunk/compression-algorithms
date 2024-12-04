#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstring>

using namespace std;

void parseInput(string input, vector<string> &args)
{
    stringstream ss(input);
    string word;
    while (ss >> word)
    {
        args.push_back(word);
    }
}

void executeCommand(vector<string> &args)
{
    if (args.empty())
        return;

    // Handle the 'exit' command
    if (args[0] == "exit")
    {
        exit(0);
    }

    pid_t pid = fork();
    if (pid == 0)
    { // Child process
        vector<char *> c_args;
        for (const auto &arg : args)
        {
            c_args.push_back(const_cast<char *>(arg.c_str()));
        }
        c_args.push_back(nullptr); // Null-terminate the array for execvp

        // Execute the command
        if (execvp(c_args[0], c_args.data()) == -1)
        {
            perror("Error executing command");
            exit(1);
        }
    }
    else if (pid < 0)
    {
        perror("Fork failed");
    }
    else
    {               // Parent process
        wait(NULL); // Wait for the child process to finish
    }
}

void handlePipe(string input)
{
    size_t pipePos = input.find('|');
    if (pipePos == string::npos)
    {
        return; // No pipe found
    }

    string firstCommand = input.substr(0, pipePos);
    string secondCommand = input.substr(pipePos + 1);

    vector<string> args1, args2;
    parseInput(firstCommand, args1);
    parseInput(secondCommand, args2);

    int pipefd[2];
    if (pipe(pipefd) < 0)
    {
        perror("Pipe failed");
        return;
    }

    pid_t pid1 = fork();
    if (pid1 == 0)
    {                                   // First child
        close(pipefd[0]);               // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
        close(pipefd[1]);

        // Execute the first command
        vector<char *> c_args1;
        for (const auto &arg : args1)
        {
            c_args1.push_back(const_cast<char *>(arg.c_str()));
        }
        c_args1.push_back(nullptr);
        execvp(c_args1[0], c_args1.data());
        perror("Execvp failed");
        exit(1);
    }
    else if (pid1 < 0)
    {
        perror("Fork failed");
        return;
    }

    pid_t pid2 = fork();
    if (pid2 == 0)
    {                                  // Second child
        close(pipefd[1]);              // Close unused write end
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin from pipe
        close(pipefd[0]);

        // Execute the second command
        vector<char *> c_args2;
        for (const auto &arg : args2)
        {
            c_args2.push_back(const_cast<char *>(arg.c_str()));
        }
        c_args2.push_back(nullptr);
        execvp(c_args2[0], c_args2.data());
        perror("Execvp failed");
        exit(1);
    }
    else if (pid2 < 0)
    {
        perror("Fork failed");
        return;
    }

    close(pipefd[0]);
    close(pipefd[1]);
    wait(NULL); // Wait for both children to finish
    wait(NULL);
}

void handleRedirection(string &input, vector<string> &args)
{
    size_t outPos = input.find('>');
    size_t inPos = input.find('<');

    if (outPos != string::npos)
    { // Output redirection
        string fileName = input.substr(outPos + 1);
        input = input.substr(0, outPos);
        parseInput(input, args);

        int fd = open(fileName.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0)
        {
            perror("Error opening output file");
            return;
        }

        pid_t pid = fork();
        if (pid == 0)
        {                            // Child process
            dup2(fd, STDOUT_FILENO); // Redirect STDOUT to the file
            close(fd);
            vector<char *> c_args;
            for (const auto &arg : args)
            {
                c_args.push_back(const_cast<char *>(arg.c_str()));
            }
            c_args.push_back(nullptr);
            execvp(c_args[0], c_args.data());
            perror("Execvp failed");
            exit(1);
        }
        else if (pid < 0)
        {
            perror("Fork failed");
            return;
        }

        close(fd);
        wait(NULL); // Wait for the child to finish
    }
    else if (inPos != string::npos)
    { // Input redirection
        string fileName = input.substr(inPos + 1);
        input = input.substr(0, inPos);
        parseInput(input, args);

        int fd = open(fileName.c_str(), O_RDONLY);
        if (fd < 0)
        {
            perror("Error opening input file");
            return;
        }

        pid_t pid = fork();
        if (pid == 0)
        {                           // Child process
            dup2(fd, STDIN_FILENO); // Redirect STDIN from the file
            close(fd);
            vector<char *> c_args;
            for (const auto &arg : args)
            {
                c_args.push_back(const_cast<char *>(arg.c_str()));
            }
            c_args.push_back(nullptr);
            execvp(c_args[0], c_args.data());
            perror("Execvp failed");
            exit(1);
        }
        else if (pid < 0)
        {
            perror("Fork failed");
            return;
        }

        close(fd);
        wait(NULL); // Wait for the child to finish
    }
}

int main()
{
    string input;
    vector<string> args;

    while (true)
    {
        cout << "> ";
        getline(cin, input);

        if (input.empty())
        {
            continue;
        }

        // Handle piping
        if (input.find('|') != string::npos)
        {
            handlePipe(input);
        }
        // Handle redirection (>)
        else if (input.find('>') != string::npos || input.find('<') != string::npos)
        {
            handleRedirection(input, args);
        }
        // Handle normal command execution
        else
        {
            parseInput(input, args);
            executeCommand(args);
        }
    }

    return 0;
}