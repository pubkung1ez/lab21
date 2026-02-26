
#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) {

    if (argc == 1) {
        cout << "Please input numbers to find average.\n";
        return 0;
    }

    int count = argc - 1;
    double sum = 0.0;

    for (int i = 1; i < argc; i++) {
        sum += atof(argv[i]); 
    }

    double average = sum / count;

    cout << "---------------------------------\n";
    cout << "Average of " << count << " numbers = " << average << "\n";
    cout << "---------------------------------\n";

    return 0;
}