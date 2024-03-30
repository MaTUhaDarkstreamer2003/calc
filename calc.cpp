#include <iostream>
#include <string>
#include <stack>
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int priority(char c) { // ����������� ���������� ���������
    if (c == '+' || c == '-') {
        return 1;
    }
    else if (c == '*' || c == '/' || c == '%' || c == '!') {
        return 2;
    }
    else if (c == '^') {
        return 3;
    }
    else {
        return 0;
    }
}

double operate(double num1, double num2, char op) { // ���������� ����� ��������
    if (op == '+') {
        return num1 + num2;
    }
    else if (op == '-') {
        return num1 - num2;
    }
    else if (op == '*') {
        return num1 * num2;
    }
    else if (op == '/') {
        return num1 / num2;
    }
    else if (op == '^') {
        return pow(num1, num2);
    }
    else if (op == '%') {
        int numer1 = static_cast<int>(num1);
        int numer2 = static_cast<int>(num2);
        while (numer1 > numer2) {
            numer1 -= numer2;
        }
        return numer1;
    }
    else if (op == '!') { // ��� ��� ������� �� �������
        int numer1 = static_cast<int>(num1);
        int numer2 = static_cast<int>(num2);
        int i = 0;
        while (numer1 > numer2) {
            numer1 -= numer2;
            i++;
        }
        return static_cast<double>(i);
    }
    else {
        return 0;
    }
}

double calculate(string expression, stringstream& steps) { // �������� �������
    stack<double> numStack; // ���� ��� �����
    stack<char> opStack; // ���� ��� ����������
    int i = 0;

    while (i < expression.length()) {
        if (isdigit(expression[i])) { // ���� �����
            string numStr = "";
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                numStr += expression[i++];
            }
            double num = stod(numStr);
            numStack.push(num);

               steps << "Push: " << numStr << endl;
        }
        else if (expression[i] == '-') {  // ���� �����
            if (i == 0 || (!isdigit(expression[i - 1]) && expression[i - 1] != ')' && isdigit(expression[i + 1]))) { // '-' �������� ������ �����, � �� ����������
                string numStr = "-";
                i++;

                while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                    numStr += expression[i++];
                }

                double num = stod(numStr);
                numStack.push(num);

                    steps << "Push: " << numStr << endl;
            }
            else {  // '-' �������� ����������
                while (!opStack.empty() && priority(opStack.top()) >= priority('-')) {
                    char op = opStack.top();
                    opStack.pop();

                    double num2 = numStack.top();
                    numStack.pop();

                    double num1 = numStack.top();
                    numStack.pop();

                    double result = operate(num1, num2, op);
                    numStack.push(result);

                        steps << "Pop: " << num1 << " " << op << " " << num2 << " = " << operate(num1, num2, op) << endl;
                        //steps << expression.substr(0, i + 1) << " = " << result << endl;
                }

                opStack.push('-');
                i++;
            }
        } else if (expression[i] == '(') { // ���� ������� ������ - ����������� ������
            
            i++;
            int count = 1;
            string subExpression = "";
            while (count !=0) {
                subExpression += expression[i++];

                if (expression[i] == '(') {
                    count += 1;
                }
                if (expression[i] == ')') {
                    count -= 1;
                }
                //cout << 1 << endl;
            }
            i++;
            stringstream subSteps;
            double result = calculate (subExpression, subSteps);
            //cout << result << endl;
            numStack.push(result);
            //cout <<  numStack.top() << endl;
            steps << subSteps.str();
            //cout << steps.str();
        }
        else { // �� ���������
            while (!opStack.empty() && priority(opStack.top()) >= priority(expression[i])) {
                char op = opStack.top();
                opStack.pop();

                double num2 = numStack.top();
                numStack.pop();

                double num1 = numStack.top();
                numStack.pop();

                double result = operate(num1, num2, op);
                numStack.push(result);

                    steps << "Pop: " << num1 << " " << op << " " << num2 << " = " << operate(num1, num2, op) << endl;
                    //steps << expression.substr(0, i + 1) << " = " << result << endl;

            }

            opStack.push(expression[i++]);
        }
    }

    while (!opStack.empty()) {  // ���� ���� ��������� ������
        char op = opStack.top();
        opStack.pop();

        double num2 = numStack.top();
        numStack.pop();

        double num1 = numStack.top();
        numStack.pop();

        double result = operate(num1, num2, op);
        numStack.push(result);

        steps << "Pop: " << num1 << " " << op << " " << num2 << " = " << operate(num1, num2, op) << endl;
        steps << expression.substr(0, i + 1) << " = " << result << endl;
    }

    return numStack.top(); // ���������� ��������� ���������� �����
}





bool fileExists(const string& filename) { // �������� ���������� �� ����
    ifstream file(filename);
    return file.good();
}

bool writeToFile(const string& filename, const string& content, bool overwrite) { // �������� ����� �� �������� � ����, ������� ����� � ����� � ����
    //cout << content;
    ofstream file(filename, ios::out | (overwrite ? ios::trunc : ios::app));
    if (file.is_open()) {
        file << content;
        file.close();
        return true;
    }
    return false;
}

void displayWarning(const string& filename, const string& expression, bool showSteps, stringstream& steps, double result) {  // ������� ��� ����
    cout << "Warning: The file \"" << filename << "\" already exists. What would you like to do?" << endl;
    cout << "1. Continue without overwriting the file." << endl;
    cout << "2. Overwrite the file." << endl;
    cout << "3. Append the answer to the end of the file." << endl;
    cout << "Enter the number of your choice: ";

    string input;
    //getline(cin, input);
    cin >> input;

    if (input == "1") {
        cout << steps.str() << endl;
        // Do nothing, continue with the next expression
    }
    else if (input == "2") {
        if (!writeToFile(filename, steps.str(), true)) {
            cout << "Error error accessing the log file." << endl;
        }
    }
    else if (input == "3") {
        if (!writeToFile(filename, steps.str(), false)) {
            cout << "Error error accessing the log file." << endl;
        }
    }
    else {
        // ��� ��������� ������
    }
}

void displayWarning2(const string& filename, const string& expression, bool showSteps, stringstream& steps, double result) { // ������� ��� ����
    cout << "Warning: The file \"" << filename << "\" already exists. What would you like to do?" << endl;
    cout << "1. Continue without overwriting the file." << endl;
    cout << "2. Overwrite the file." << endl;
    cout << "3. Append the answer to the end of the file." << endl;
    cout << "Enter the number of your choice: ";

    string input;
    //getline(cin, input);
    cin >> input;

    if (input == "1") {
        cout << expression << " answer: " << to_string(result) << endl;
    }
    else if (input == "2") {
        if (!writeToFile(filename, /*expression + " answer: " + */to_string(result) + "\n", true)) {
            cout << "Error error accessing the out file." << endl;
        }
    }
    else if (input == "3") {
        if (!writeToFile(filename, /*expression + " answer: " + */ to_string(result) + "\n", false)) {
            cout << "Error error accessing the out file." << endl;
        }
    }
    else {
        // ��� ��������� ������
    }
}

void readFromFile(vector<string>& arr, const string& filename) { // ������� ��� ������ ����� �� �����
    ifstream file(filename);
  
    if (!file.is_open()) {
        cout << "I can't open " << filename << endl;
        return;
    }
  
    string line;
    int lineNum = 1;
  
    while (getline(file, line)) {
        bool valid = true;
        // ��������� ������ ������ � ������
        for (char c : line) {
            if (!isdigit(c) && c != '-' && c != '+' && c != '!' && c != '*' 
                && c != '^' && c != '%' && c != '/' && c != '(' && c != ')' && c != ' ' && c != '.') {
                cout << "Line " << lineNum << " of file: " << filename << " contain invalid symbol " << c << " this line will be skipped" << endl;
                valid = false;
                break;
            }
        }
      
        if (valid) {
            arr.push_back(line);
        }
       
        lineNum++;
    }
  
    file.close();
}

int main(int argc, char** argv) {  // ��� ����
    bool showSteps = false; // ���� ��� ����������� ����� ����������
    bool outputToFile = false; // ���� ��� ������ ���������� � ����
    bool logToFile = false; // ���� ��� ������ ��������� � ����� ���������� � ���� ����
    string outputFile = ""; // ��� ����� ������
    string logFile = ""; // ��� ����� ����
    vector<string> expressions; // ������ ��� �������� ���������

    double result = 0.0;

    if (argc > 1) { // �������� �������
	//�����
        for (int i = 1; i < argc; i++) {
            string arg = argv[i];

            if (arg == "-v") {
                showSteps = true;
            }
            else if (arg == "-o" && i < argc - 1) {
                outputToFile = true;
                outputFile = argv[i + 1];
                i++;
            }
            else if (arg == "-l" && i < argc - 1) {
                logToFile = true;
                logFile = argv[i + 1];
                i++;
            }
            else if (arg == "-h" || arg == "-help") {
                cout << "Usage: ./calc [options] [expressions]" << endl << endl;
                cout << "Use % to calculate remainder of the division" << endl;
                cout << "Use ! to integer division" << endl << endl;
                cout << "Expressions must be written without spaces" << endl << endl;
                cout << "Options:" << endl;
                cout << "  -v             Show calculation steps" << endl;
                cout << "  -o <file>      Output results to file" << endl;
                cout << "  -l <file>      Log expressions and calculation steps to file" << endl;
                cout << "  -h, -help      Show this help message" << endl;
                return 0;
            } 
			 // �� �����
			else {
                if (arg[0] == '-' || isdigit (arg[0]) || arg[0] == '(') {
                    expressions.push_back(arg);
                } else {
                    readFromFile (expressions, arg);
                }
            }
        }
    }
    else { // ���� ������� �� ����
        string input;
        cout << "Enter expression, only one please, or i wont calculate anything: ";
        getline(cin, input);
        if (input[0] == '-' || isdigit (input[0])) {
                    expressions.push_back(input);
                } else {
                    readFromFile (expressions, input);
                }
    }
	
    for (int i = 0; i < expressions.size(); i++) { // ������� � ���������� �� �����
        stringstream steps;
        string expression = expressions[i];
        result = calculate(expression, steps); // ����������� �������� result
        // ��������� ������ ���������� � ���� ������
        if (outputToFile) {
            if (fileExists(outputFile)) {
                displayWarning2(outputFile, expression, showSteps, steps, result); // �������� result � displayWarning2
            }
            else {
                if (!writeToFile(outputFile, expression + " answer: " + to_string(result) + "\n", true)) {
                    cout << "Error opening/creating output file." << endl;
                    continue;
                }
            }
        }

        // ��������� ������ ����� ���������� � ���� ����
        if (logToFile) {
            if (fileExists(logFile)) {
                displayWarning(logFile, expression, showSteps, steps, result); // �������� result � displayWarning
            }
            else {
                if (!writeToFile(logFile, steps.str() + "\n", true)) {
                    cout << "Error opening/creating log file." << endl;
                    continue;
                }
            }
        }

        // ����� ���������� �� �����
        if (showSteps || (!outputToFile && !logToFile)) {
            cout << expression << " answer: " << result << endl;
            if (showSteps) {    
                cout << steps.str();
            }
        }
    }
    cout << "delo sdelano" << endl;
    return 0;
}