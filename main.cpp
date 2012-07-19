//XAFS Sample Preparation Calculation Assistant
//Created by Edward Kim - ekim01@uoguelph.ca
//July 2012

#include <iostream>
#include <vector>
#include <string>
#include "mucal.h"

using namespace std;

//Error codes
const int NO_ERR = 0;
const int EXIT_CMD = -1;
const int BAD_INPUT = 1;

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
    float energy;
    float mu;
    float absorption_length;
    float volume;
    float radius;
    vector < float > masses;

    public:

    Sample(string name);
    int compute(); //Compute xray properties
    int dilute(string compound); //Dilute sample using a specified compound
    int rename(string sample_name); //Change sample name
    int write_screen(); //Write sample data to screen
    int write_file(string file_name); //Write sample data to file

    string get_name();

    int set_energy(float inp_energy);
};

Sample::Sample(string sample_name)
{
    name = sample_name;
}

string Sample::get_name()
{
    return name;
}

int setEnergy(string inp_energy)
{
    energy = inp_energy;
    return NO_ERR;
}

//Explodes a string
void string_explode(string str, string separator, vector<string>* results){
    unsigned int found;
    found = str.find_first_of(separator);
    while(found != string::npos){
        if(found > 0){
            results->push_back(str.substr(0,found));
        }
        str = str.substr(found+1);
        found = str.find_first_of(separator);
    }
    if(str.length() > 0){
        results->push_back(str);
    }
}

//Samples
vector < Sample > samples;

int parse_input(string mode = "none")
{
    int err = NO_ERR;

    string user_input;
    vector < string > filtered_input;

    cout << "Please enter a command:" << endl;
    cout << ">>";
    getline(cin, user_input);
    cout << endl;

    string_explode(user_input, " ", &filtered_input); //Explode the string by each word

    //Quit Program
    if (filtered_input[0] == "quit")
    {
        err = EXIT_CMD;
    }
    //Sample commands
    else if (filtered_input[0] == "sample")
    {
        //Create a new sample
        if (filtered_input[1] == "new")
        {
            //Check for one-word name
            if (filtered_input.size() == 3)
            {
                samples.push_back(Sample(filtered_input[2]));
            }
            else
            {
                cout << "A one-word sample name is required. Please re-input." << endl;
            }
        }
        //Compute quantities for a sample
        else if (filtered_input[1] == "compute")
        {

        }
        //Setup sample energy for xray calcs
        else if (filtered_input[1] == "energy")
        {

        }
        else
        {
            cout << "Bad subcommand under command 'sample' -- Please re-input." << endl;
        }
    }
    //List samples
    else if (filtered_input[0] == "list")
    {
        //Make sure 'samples' isn't empty
        if (samples.size() != 0)
        {
            cout << "List of samples:" << endl;

            //List samples
            for (unsigned int i = 0; i < samples.size(); i++)
            {
                cout << (i+1) << ". " << samples[i].get_name() << endl;
            }
        }
        else
        {
            cout << "No samples to list." << endl;
        }
    }
    //Bad command name
    else
    {
        cout << "Bad command name. Please re-input." << endl;
    }

    cout << endl;
    return err;
}

int main()
{
    int err = NO_ERR;

    //Welcome message
    cout << "Welcome to the XAFS Sample Prep Calculator" << endl;
    cout << "Type 'help' for help and 'quit' to quit the program" << endl;
    cout << "Created by Eddie Kim, July 2012" << endl << endl;

    //Input loop
    do
    {
        err = parse_input();

    }while(err != EXIT_CMD);

    return 0;
}
