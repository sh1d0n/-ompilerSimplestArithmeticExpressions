#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <fstream>
#include <unordered_map>

typedef std::unordered_map<char, int> MyMap;

struct T
{
    std::vector<std::string> variable;
    int level;
};

//Filling in a table from a file
std::unordered_map<char, MyMap> fillingFromFile()
{
    std::unordered_map<char, MyMap> table;
    std::ifstream in("input.in");
    if(in.is_open())
    {
        int i_size, j_size;
        in >> i_size >> j_size;
        for (int i = 0; i < i_size; i++)
        {
            char firstKey;
            in >> firstKey;
            for(int j = 0; j < j_size; j++)
            {
                char secondKey;
                in >> secondKey;
                in >> table[firstKey][secondKey];
            }
        }
    }
    else
    {
        std::cout << "File not found!";
    }
    in.close();

    return table;
}

bool my_isdigit(char ch)
{
    return std::isdigit(static_cast<unsigned char>(ch));
}

bool workOnNumber(std::stack<T>& operands, std::string& number, char lI)
{
    std::vector<std::string> text;
    if (!number.empty() && lI == '.')
    {
        number += lI;
        return true;
    }
    else if (number.empty() && lI == '.')
    {
        std::cout << "\nAn error has been made in the expression! Before '.' no numbers!\n";
        system("pause");
        exit(0);
    }
    else if (!number.empty())
    {
        if (number.back() == '.')
        {
            std::cout << "\nAn error has been made in the expression! After the character '.' no numbers!\n";
            system("pause");
            exit(0);
        }
        else
        {
            text.push_back('=' + number);
            operands.push({text, 0});
            number.clear();
        }
    }
    return false;
}

void solution(char topOfOS, std::stack<T>& operands, std::stack<char>& operations)
{
    T firstOperand, secondOperand, result;
    firstOperand = operands.top();
    int nullLevel = 0;
    if(firstOperand.level == 0)
    {
        std::string a = firstOperand.variable[0];
        firstOperand.variable[0] = "LOAD " + a;
        nullLevel++;
    }
    operands.pop();
    secondOperand = operands.top();
    if(secondOperand.level == 0)
    {
        std::string a = secondOperand.variable[0];
        secondOperand.variable[0] = "LOAD " + a;
        nullLevel++;
    }
    operands.pop();

    if(nullLevel == 2)
    {
        result.variable.push_back(firstOperand.variable[0]);
        result.variable.push_back("STORE $1");
        result.variable.push_back(secondOperand.variable[0]);
        if (topOfOS == '+')
        {
            result.variable.push_back("ADD $1");
        }
        else if (topOfOS == '*')
        {
            result.variable.push_back("MPY $1");
        }
        result.level = 1;
    }
    else if(nullLevel == 1)
    {
        char higherLevel;
        if(firstOperand.level > secondOperand.level)
        {
            higherLevel = firstOperand.level + 1;
            result.level = higherLevel;
            result.variable = firstOperand.variable;
            result.variable.push_back("STORE $" + std::to_string(higherLevel));
            result.variable.push_back(secondOperand.variable[0]);
        }
        else if(firstOperand.level <= secondOperand.level)
        {
            higherLevel = secondOperand.level + 1;
            result.level = higherLevel;
            result.variable = secondOperand.variable;
            result.variable.push_back("STORE $" + std::to_string(higherLevel));
            result.variable.push_back(firstOperand.variable[0]);
        }

        if(topOfOS == '+')
        {
            result.variable.push_back("ADD $" + std::to_string(higherLevel));
        }
        else if (topOfOS == '*')
        {
            result.variable.push_back("MPY $" + std::to_string(higherLevel));
        }
    }
    else
    {
        char higherLevel;
        if(firstOperand.level > secondOperand.level)
        {
            higherLevel = firstOperand.level + 1;
            result.level = higherLevel;
            result.variable = firstOperand.variable;
            result.variable.push_back("STORE $" + std::to_string(higherLevel));

            for(size_t i = 0; i < secondOperand.variable.size(); i++)
            {
                result.variable.push_back(secondOperand.variable[i]);
            }
        }
        else if(firstOperand.level <= secondOperand.level)
        {
            higherLevel = secondOperand.level + 1;
            result.level = higherLevel;
            result.variable = secondOperand.variable;
            result.variable.push_back("STORE $" + std::to_string(higherLevel));
            for(size_t i = 0; i < firstOperand.variable.size(); i++)
            {
                result.variable.push_back(firstOperand.variable[i]);
            }
        }

        if(topOfOS == '+')
        {
            result.variable.push_back("ADD $" + std::to_string(higherLevel));
        }
        else if (topOfOS == '*')
        {
            result.variable.push_back("MPY $" + std::to_string(higherLevel));
        }
    }

    operands.push(result);
    operations.pop();
}

int main()
{
    std::unordered_map<char,MyMap> table = fillingFromFile();

    std::cout << "Enter the expression >> ";
    std::string expression;
    std::cin >> expression;
    expression += '$';

    std::stack<char> operations;
    std::stack<T> operands;

    std::vector<std::string> text;
    std::string number;
    char operationSymbol;

    for(size_t i = 0; i < expression.size(); i++)
    {
        char lI = expression[i];
        if(lI >= 'A' && lI <= 'Z')
        {
            std::string s;
            s += lI;
            text.push_back(s);
            operands.push({text, 0});
            text.clear();
        }
        else if(my_isdigit(lI))
        {
            number += lI;
        }
        else
        {
            if(workOnNumber(operands, number, lI))
            {
                continue;
            }

            if(lI == '+' || lI == '*' || lI == '(' || lI == ')' || lI == '$' || lI == '=')
            {
                operationSymbol = lI;
            }

            char topOfOS; //top of operations stack
            if(operations.empty())
            {
                operations.push('$');
            }

            topOfOS = operations.top();

            if(operationSymbol != NULL)
            {
                switch(table.at(topOfOS).at(operationSymbol))
                {
                    case 2:
                        operations.push(operationSymbol);
                        operationSymbol = NULL;
                        break;
                    case 3:
                        std::cout << "\nAn error has been made in the expression! Check the correctness of the operator "
                                  << operationSymbol << "\n";
                        system("pause");
                        exit(0);
                    case 5:
                        solution(topOfOS,operands,operations);
                        i--;
                        operationSymbol = NULL;
                        break;
                    case 6:
                        operations.pop();
                        operationSymbol = NULL;
                        break;
                    case 7:
                        T operand = operands.top();
                        operands.pop();
                        operand.variable.push_back("STORE " + operands.top().variable[0]);
                        operands.pop();
                        operands.push(operand);
                        break;
                }
            }
        }
    }

    for(size_t i = 0; i < operands.top().variable.size(); i++)
    {
        std::cout << operands.top().variable[i] << std::endl;
    }


    system("pause");
}