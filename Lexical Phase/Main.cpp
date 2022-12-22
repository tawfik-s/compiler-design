#include <bits/stdc++.h>
using namespace std;
// comand to run:  g++ Main.cpp -o main ; ./main
bool isKeyword(string buffer)
{
    vector<string> keywords =
        {"auto", "break", "main", "case", "char", "const", "continue", "default",

         "do", "double", "else", "enum", "extern", "float", "for", "goto",

         "if", "int", "long", "register", "return", "short", "signed",

         "sizeof", "static", "struct", "switch", "union",
         "unsigned", "void", "volatile", "while", "strcmp"};
    int i, flag = 0;

    for (auto word : keywords)
    {
        if ((word == buffer))
        {
            return 1;
        }
    }

    return 0;
}

void keyWorkOrIdentifier(string &buffer, int &numberOfDigits)
{
    if (buffer.size() != 0)
    {
        if (numberOfDigits == buffer.size())
            cout << "numeric const: " << buffer << endl;
        else if (isKeyword(buffer))
            cout << "keyword: " << buffer << endl;
        else
            cout << "identifier: " << buffer << endl;
    }
    numberOfDigits = 0;
    buffer = "";
}
int main()
{
    /* Declaration of operator*/
    string operators = "+-*/%=<>";
    string seperators = "(){}[],;";
    string buffer;
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    int numberOfDigits = 0;
    string line;
    while (getline(cin, line))
    {
        for (int j = 0; j < line.size(); j++)
        {
            char ch = line[j];

            bool flag = 0;
            for (auto sign : operators)
            {
                if (ch == sign)
                {
                    keyWorkOrIdentifier(buffer, numberOfDigits);
                    printf("operator: %c \n", ch);
                    flag = true;
                }
            }
            if (!flag)
                for (auto sign : seperators)
                {
                    if (ch == sign)
                    {
                        keyWorkOrIdentifier(buffer, numberOfDigits);
                        printf("special char: %c \n", ch);
                        flag = true;
                    }
                }

            if (isalnum(ch))
            {
                buffer.push_back(ch);
                if (ch >= '0' && ch <= '9')
                {
                    numberOfDigits++;
                }
            }
            else if (ch == ' ' || ch == '\n')
            {
                keyWorkOrIdentifier(buffer, numberOfDigits);
            }
        }
    }

    return 0;
}