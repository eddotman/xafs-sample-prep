//XAFS Sample Preparation Calculation Assistant
//Created by Edward Kim - ekim01@uoguelph.ca
//July 2012

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Sample
{
    private:

    //User Defined
    string name; //Name of sample
    vector < string > elements; //List of elements in sample (element symbols)
    string formula; //Optional chemical formula
    vector < float > mass_percents; //Percent of each element by weight
    float density; //Bulk density of material (g/cm^3)

    //Calculated
    float mu;
    float absorption_length;
    float volume;
    float radius;
    vector < float > masses;

    public:

    Sample(string name);
    int compute(); //Compute xray properties
    int rename(string name); //Change sample name
    int write_screen(); //Write sample data to screen
    int write_file(); //Write sample data to file
};

int parse_input(string input)
{
    return 0;
}

int main()
{
    const int NO_ERR = 0;
    const int EXIT_CMD = -1;

    string user_input;
    int err = NO_ERR;

    //Input loop
    do
    {
        cout << "Please enter a command:" << endl;
        cin >> user_input;

        err = parse_input(user_input);

    }while(err != EXIT_CMD);

    return 0;
}
