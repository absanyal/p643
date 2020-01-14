#include<iostream>
#include<fstream>

using namespace std;

int main(){
    ofstream f;
    f.open("home1.txt");
    f << "Hello world!" <<endl;
    f.close();
}