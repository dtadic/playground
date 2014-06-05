#include <iostream>
#include <stack>
#include <vector>
#include <istream>
#include <fstream>
#include <string>
#include <time.h>

using namespace std;

class Interpretor{
private:
    vector<vector<char>> mem;
    stack<int> sta;
    bool bridge;
    bool strMode;
    enum direction{
        r, l, u, d
    };
    direction dir = r;
    struct position{
        int x;
        int y;
    };
    position pos;
    int pop(){
        if(sta.empty()) return 0;
        int a = sta.top();
        sta.pop();
        return a;
    }
    void push(int a){
        sta.push(a);
    }
    void add(){
        int a = pop();
        int b = pop();
        push(a+b);
    }
    void subtr(){
        int a = pop();
        int b = pop();
        push(b-a);
    }
    void multiply(){
        int a = pop();
        int b = pop();
        push(a*b);
    }
    void divide(){
        int a = pop();
        int b = pop();
        push(b/a);
    }
    void mod(){
        int a = pop();
        int b = pop();
        push(b%a);
    }
    void l_not(){
        int a = pop();
        if(!a) push(1);
        else push(0);
    }
    void gt(){
        int a = pop();
        int b = pop();
        if(b>a) push(1);
        else push(0);
    }
    void get(){
        int y = pop();
        int x = pop();
        push((int)mem[y][x]);
    }
    void put(){
        int y = pop();
        int x = pop();
        mem[y][x] = (char)pop();
    }
    void duplicate(){
        int a = pop();
        push(a);
        push(a);
    }
    void swp(){
        int a = pop();
        int b = pop();
        push(a);
        push(b);
    }
    void randDir(){
        int tmp = rand() % 4;
        switch(tmp){
            case 0: dir = r; break;
            case 1: dir = l; break;
            case 2: dir = u; break;
            case 3: dir = d; break;
        }
    }
    void horIf(){
        int tmp = pop();
        if(tmp == 0) dir = r;
        else dir = l;
    }
    void verIf(){
        int tmp = pop();
        if(tmp == 0) dir = d;
        else dir = u;
    }
    void intOut(){
        cout << pop();
    }
    void charOut(){
        cout << (char)pop();
    }

public:
    bool endProg;
    Interpretor(){};
    ~Interpretor(){};
    Interpretor(istream& file){
        strMode = false;
        bridge = false;
        endProg = false;
        pos.x = 0;
        pos.y = 0;
        string tmp;
        while(file.good()){
            getline(file, tmp);
            mem.push_back(vector<char>(tmp.begin(), tmp.end()));
        }
    }
    void jump(){
        switch(dir){
        case r:
            if(pos.x == (int)mem[pos.y].size()-1) pos.x = 0;
            else pos.x++;
            break;
        case l:
            if(pos.x == 0) pos.x = (int)mem[pos.y].size()-1;
            else pos.x--;
            break;
        case d:
            if(pos.y == (int)mem.size()-1) pos.y = 0;
            else pos.y++;
            break;
        case u:
            if(pos.y == 0) pos.y = (int)mem.size()-1;
            else pos.y--;
            break;
        }
    }
    void step(){
        char inst = mem[pos.y][pos.x];
        if(strMode){
            if(inst == '"'){
                strMode = false;
                jump();
                return;
            }
            push((int)inst);
            jump();
            return;
        }
        if(bridge){
            bridge = false;
            jump();
            return;
        }
        if(inst >= '0' && inst <= '9'){
            push(inst - '0');
            jump();
            return;
        }
        switch(inst){
        case '+':
            add();
            break;
        case '-':
            subtr();
            break;
        case '*':
            multiply();
            break;
        case '/':
            divide();
            break;
        case '%':
            mod();
            break;
        case '!':
            l_not();
            break;
        case '`':
            gt();
            break;
        case '>':
            dir = r;
            break;
        case '<':
            dir = l;
            break;
        case 'v':
            dir = d;
            break;
        case '^':
            dir = u;
            break;
        case '?':
            randDir();
            break;
        case '_':
            horIf();
            break;
        case '|':
            verIf();
            break;
        case '"':
            strMode = true;
            break;
        case ':':
            duplicate();
            break;
        case '\\':
            swp();
            break;
        case '$':
            pop();
            break;
        case '.':
            intOut();
            break;
        case ',':
            charOut();
            break;
        case'#':
            bridge = true;
            break;
        case 'g':
            get();
            break;
        case 'p':
            put();
            break;
        case '&':
            int s;
            cin >> s;
            push(s);
            break;
        case '~':
            char c;
            cin >> c;
            push((int)c);
            break;
        case '@':
            endProg = true;
            return;
        }
        jump();
    }
};

int main(int argc, char** argv)
{
    srand(time(NULL));
    if(argc != 2){
        cout << "Usage: " << argv[0] << " [filename]\n";
        return 0;
    }
    fstream file(argv[1], fstream::in);
    Interpretor i(file);
    file.close();
    while(!i.endProg){
        i.step();
    }
    cout << endl;
    return 0;
}
