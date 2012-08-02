//XAFS Sample Preparation Calculation Assistant
//Created by Edward Kim - ekim01@uoguelph.ca
//August 2012

#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <cstring>
#include "mucal.c"

using namespace std;

//Error codes
const int NO_ERR = 0;
const int EXIT_CMD = -1;
const int BAD_INPUT = 1;
const int NO_SAMPLES = 2;

class Sample
{
    private:

    //User Defined
    string name; //Name of sample
    vector < string > elements; //List of elements in sample (element symbols)
    vector < float > mass_percents; //Percent of each element by weight
    float density; //Bulk density of material (g/cm^3)

    //Calculated
    float energy;
    float mu;
    float absorption_length;
    float volume;
    float radius;
    float mass;
    vector < float > masses;

    public:

    Sample(string name);
    int compute(); //Compute xray properties at a given energy
    int dilute(string compound); //Dilute sample using a specified compound
    int rename(string sample_name); //Change sample name
    int write_screen(); //Write sample data to screen
    int write_file(string file_name); //Write sample data to file

    string get_name();
    int get_num_elements();

    int set_energy(float inp_energy);
    int set_density(float inp_density);
    int set_elements (vector < string > inp_elements);
    int set_num_elements(int num);
    int set_mass_percents (vector < float > inp_mass_percents);
};

Sample::Sample(string sample_name)
{
    name = sample_name;
}

string Sample::get_name()
{
    return name;
}

int Sample::get_num_elements()
{
    return elements.size();
}

int Sample::set_energy(float inp_energy)
{
    energy = inp_energy;
    return NO_ERR;
}

int Sample::set_density(float inp_density)
{
    density = inp_density;
    return NO_ERR;
}

int Sample::set_elements (vector < string > inp_elements)
{
    elements = inp_elements;
    return NO_ERR;
}

int Sample::set_num_elements(int num)
{
    elements.resize(num);
    mass_percents.resize(num);
    masses.resize(num);
    return NO_ERR;
}

int Sample::set_mass_percents (vector < float > inp_mass_percents)
{
    mass_percents = inp_mass_percents;
    return NO_ERR;
}

int Sample::write_screen()
{
    cout << endl << "------------------------------------" << endl << endl;
    cout << "Sample Name: " << name << endl << endl;
    cout << "Sample Composition:" << endl << endl;

    for (int i = 0; i < elements.size(); i++)
    {
        cout << setprecision(5) << mass_percents[i] << "  " << elements[i] << endl;
    }

    cout << endl;

    cout << "Absorption Coefficient (1/cm): " << mu << endl;
    cout << "Absorption Length (cm): " << absorption_length << endl << endl;
    cout << "Pellet Density (g/cm^3): " << density << endl;
    cout << "Pellet Radius (cm): " << radius << endl;
    cout << "Pellet Volume (cm^3): " << volume << endl;
    cout << "Pellet Mass (g): " << mass << endl;
    cout << endl << "Pellet Masses by Element (g): " << endl << endl;

    for (int i = 0; i < elements.size(); i++)
    {
        cout << setprecision(5) << masses[i] << "  " << elements[i] << endl;
    }

    cout << endl << "------------------------------------" << endl;
    cout << endl;

    return NO_ERR;
}

int Sample::compute()
{
    int err; //Used for mucal
    int print_flag = 1;

    float accumMu = 0; //accumulates sum part of mu value through multiplying

    //Return variables for mucal
    double retEnergy[9];
    double xsec[11];
    double fl_yield[4];
    char err_msg[100];
    char elemName[2];

    for (int i = 0; i < elements.size();i++)
    {
        strcpy(elemName, elements[i].c_str());
        err = mucal(elemName, 0, energy, 'c', print_flag, retEnergy, xsec, fl_yield, err_msg);

        accumMu += (mass_percents[i]*xsec[3]);
    }

    //multiply in density
    mu = accumMu * density;

    absorption_length = (1 / mu) * 10000; //Absorption length in microns

    radius = 0.65; //Radius in centimetres

    volume = 3.14 * radius * radius * (absorption_length / 10000); //Volume in cm^3

    mass = volume * density; //Total mass of pellet

    for (int i = 0; i < elements.size(); i++)
    {
        masses[i] = mass_percents[i] * (volume * density); //Compute each mass needed to form pellet
    }

    return NO_ERR;
}

//Explodes a string
void string_explode(string str, string separator, vector< string > * results){
    size_t found;

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

    if (mode == "none")
    {
        cout << "Please enter a command:" << endl;
        cout << ">>";
        getline(cin, user_input);
        cout << endl;

        string_explode(user_input, " ", &filtered_input); //Explode the string by each word
    }
    else if (mode == "list")
    {
        filtered_input.resize(1);
        filtered_input[0] = "list";
    }

    //Quit Program
    if (filtered_input[0] == "quit")
    {
        err = EXIT_CMD;
    }
    //Help menu
    else if (filtered_input[0] == "help")
    {
        //Display all commands
        cout << "List of available commands:" << endl << endl;
        cout << "list                  ---List all samples" << endl;
        cout << "sample new [name]     ---Creates a new sample" << endl;
        cout << "sample setup          ---Setup sample properties" << endl;
        cout << "sample compute        ---Compute xray data for sample" << endl;
        cout << "sample write          ---Write sample data to screen" << endl;
        cout << "quit                  ---Quit program" << endl;

    }
    //Sample commands
    else if (filtered_input[0] == "sample")
    {
        if (filtered_input.size() == 1) filtered_input.push_back(" ");

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
            //Show samples
            err = parse_input("list");

            if (err != NO_SAMPLES)
            {
                int sample_ID;

                //Get the sample ID
                do
                {
                    cout << "Enter the ID of the sample you wish to compute: ";
                    getline(cin, user_input);

                }while(!isdigit(*user_input.c_str()));


                sample_ID = atoi(user_input.c_str());

                //Get the energy
                do
                {
                    cout << "Enter the desired photon energy (in keV): ";
                    getline(cin, user_input);

                }while(!isdigit(*user_input.c_str()));

                err = samples[sample_ID].set_energy(atof(user_input.c_str()));

                err = samples[sample_ID].compute();
            }

        }
        //Setup the sample
        else if (filtered_input[1] == "setup")
        {
            //Show samples
            err = parse_input("list");

            if (err != NO_SAMPLES)
            {
                int sample_ID;
                vector < string > inp_elements;
                vector < float > inp_mass_percents;

                //Get the sample ID
                do
                {
                    cout << "Enter the ID of the sample you wish to setup: ";
                    getline(cin, user_input);

                }while(!isdigit(*user_input.c_str()));


                sample_ID = atoi(user_input.c_str());

                //Get the density
                do
                {
                    cout << "Enter the bulk density (in g/cm^3): ";
                    getline(cin, user_input);

                }while(!isdigit(*user_input.c_str()));

                samples[sample_ID].set_density(atof(user_input.c_str()));

                //Get the elements
                do
                {
                    cout << "Enter the number of elements in the compound: ";
                    getline(cin, user_input);

                }while(!isdigit(*user_input.c_str()));

                samples[sample_ID].set_num_elements(atoi(user_input.c_str()));

                for (int i = 0; i < samples[sample_ID].get_num_elements(); i++)
                {
                    cout << "Please enter the symbol for element #" << i+1 << ": ";
                    getline(cin, user_input);

                    inp_elements.push_back(user_input);

                    cout << "Please enter the mass percentage for element #" << i+1 << " (fraction between 0-1): ";
                    getline(cin, user_input);

                    inp_mass_percents.push_back(atof(user_input.c_str()));
                }

                err = samples[sample_ID].set_elements(inp_elements);
                err = samples[sample_ID].set_mass_percents(inp_mass_percents);
            }


        }
        //Write sample info to screen
        else if (filtered_input[1] == "write")
        {
            //Show samples
            err = parse_input("list");

            if (err != NO_SAMPLES)
            {
                int sample_ID;

                //Get the sample ID
                do
                {
                    cout << "Enter the ID of the sample you wish to write to screen: ";
                    getline(cin, user_input);

                }while(!isdigit(*user_input.c_str()));


                sample_ID = atoi(user_input.c_str());

                err = samples[sample_ID].write_screen();
            }

        }
        else
        {
            cout << "Bad subcommand under command 'sample' -- Please re-input." << endl;
            err = BAD_INPUT;
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
                cout << i << ". " << samples[i].get_name() << endl;
            }
        }
        else
        {
            cout << "No samples have been created yet." << endl;
            err = NO_SAMPLES;
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
        err = parse_input("none");

    }while(err != EXIT_CMD);

    return 0;
}
