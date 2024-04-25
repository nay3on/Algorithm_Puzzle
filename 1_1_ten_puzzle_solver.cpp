#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <utility>
#include <set>
using namespace std;

// 역폴란드 표기법 계산식을 계산한다
double calc_poland(const string& exp) {
    // 계산을 위한 배열
    vector<double> space;

    // 역폴란드 표기법 exp의 각 문자 c를 순서로 본다
    for (char c : exp) {
        if (c >= '0' && c <= '9') {
            // c가 수를 표시하는 문자일 경우
            // '7'처럼 문자 정수를 7과 같은 수로 변환한다
            int add = c - '0';

            // 배열 말미에 삽입한다
            space.push_back(add);
        } else {
            // c가 연산자이면 말미에서 2개의 수를 꺼낸다
            double second = space.back();
            space.pop_back();
            double first = space.back();
            space.pop_back();

            // 연산 결과를 배열 말미에 삽입한다
            if (c == '+')
                space.push_back(first + second);
            else if (c == '-')
                space.push_back(first - second);
            else if (c == '*')
                space.push_back(first * second);
            else
                space.push_back(first / second);
        }
    }
    // 배열 말미에 남아 있는 값을 반환한다
    return space.back();
}


// 역폴란드 표기법 계산식에서 기존 계산식에서 복원한다
string decode_poland(const string& exp) {
    // 기존 계산식 복원을 위한 배열
    vector<string> space;

    // 역폴란드 표기법 exp의 각 문자 c를 순서로 본다
    for (char c : exp) {
        if (c >= '0' && c <= '9') {
            // 수를 나타내는 문자 c를 문자열로 반환해 배열 말미에 삽입한다
            space.push_back({c});
        } else {
            // c가 연산자이면 말미에서 2개의 계산식을 꺼낸다
            string second = space.back();
            space.pop_back();
            string first = space.back();
            space.pop_back();

            // 곱셈, 나눗셈에서는 연산자의 우선순위가 높으므로
            // 그 전후의 계산식(단독 수 제외)에 괄호를 붙인다
            if (c == '*' || c == '/') {
                if (first.size() > 1)
                    first = "(" + first + ")";
                if (second.size() > 1)
                    second = "("+ second + ")";
            }

            // 연산자를 토대로 복원한 계산식을 배열 말미에 삽입한다
            if (c == '+')
                space.push_back(first + " + " + second);
            else if (c == '-')
                space.push_back(first + " - " + second);
            else if (c == '*')
                space.push_back(first + " * " + second);
            else
                space.push_back(first + " / " + second);
        }
    }
    return space.back();
}


// 텐퍼즐 프로그램
// val: 4개 수를 저장한 배열, target: 만들고자 하는 수
vector<string> solve(vector<int> val, int target) {
    // 답을 나타내는 계산기를 저장할 배열
    set<string> res_set;    // 중복을 방지하기 위한 set

    // 역폴란드 표기법 계산식 exp를 알아보기 위한 함수 오브젝트
    const double EPS = 1e-9;    // 충분히 작은 값
    auto check = [&](const string& exp) -> void {
        // 계산 결과와 만들고자 하는 수의 값의 차가 충분히 작을 때 일치한다고 가정한다
        if (abs(calc_poland(exp) - target) < EPS)
            res_set.insert(decode_poland(exp));
    };

    // 4개의 수 val의 정렬을 순서대로 시험한다
    sort(val.begin(), val.end());   // val을 사전순으로 최소화한다
    do {
        // 4개의 문자를 연결해 가능한 문자열 fours를 만든다
        string fours = "";
        for (int v : val) fours += to_string(v);

        // 4^3=64가지의 연산자 조합을 알아본다
        const string ops = "+-*/";  // 4개의 연산자
        for (char op1 : ops) {
            for (char op2 : ops) {
                for (char op3 : ops) {
                    // 우선 패턴 "xxxxooo"를 만든다
                    string exp = fours + op1 + op2 + op3;

                    // 패턴 "xxxxooo"를 시험해본다
                    check(exp);

                    // 패턴 "xxxoxoo"를 시험해본다
                    swap(exp[3], exp[4]), check(exp);

                    // 패턴 "xxxooxo"를 시험해본다
                    swap(exp[4], exp[5]), check(exp);

                    // 패턴 "xxoxoxo"를 시험해본다
                    swap(exp[2], exp[3]), check(exp);

                    // 패턴 "xxoxxoo"를 시험해본다
                    swap(exp[4], exp[5]), check(exp);
                }
            }
        }
    } while (next_permutation(val.begin(), val.end()));

    // set을 vector로 변환하여 반환
    vector<string> res(res_set.begin(), res_set.end());
    return res;
}

int main() {
    // 4개의 수와 만들고자 하는 수를 입력한다
    vector<int> val(4); // 4개의 수
    int target; // 만들고자 하는 수
    for (int i = 0; i < 4; ++i) {
        cout << i + 1 << " th number: ";
        cin >> val[i];
    }
    cout << "target number: ";
    cin >> target;

    // 텐퍼즐을 푼다
    vector<string> res = solve(val, target);

    // 출력
    if (res.empty()) // 가능한 계산식이 없는 경우
        cout << "No solution found.";
    else {
        cout << "Solutions:" << endl;
        for (const string& exp : res)
            cout << exp << " = " << target << endl;
    }

    return 0;
}