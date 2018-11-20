/*
*******************************************************************************************************************
*This program is responsible for Applying fuzzy logic algorithm to a certain problem
*Input should be well defined based on the "Fuzzy_Logic.pdf" document attatched with the project
*******************************************************************************************************************
*As all pointer variables of structs and class Toolbox are initalized in Toolbox's member function getInputFromFile
*We didnt implement any constructor 
*/

//HEADERS & NAMESPACES
#include<iostream>
#include <fstream>
#include <string>
using namespace std;

//CONSTANTS
//Both trapezoidal and triangle will be used to determine type of fuzzy member
#define TRAPZOIDAL "trapezoidal"
#define TRIANGLE   "triangle"
//Name of the input file currently being used
#define INPUT_FILE "test cases/Sample_input.txt"

//Struct to hold points whether the fuzzy set is a triangle or trapezodial shape
struct FuzzySetShape
{   //Points of the shape where we will deduce the centroid and intersection with x and y axis
	double * coordinates_of_shape;
	//Determines the size of the coordiantes_of_shape variable during runtime based on type -> triangle or trapezoidal
	int size;
	//The name of this member (Linguistcal term)
	string member_name;
	//Type of the shape
	string shape_type;
	//Storing member value after fuzzification
	double fuzzification_value;
	/*Indication for where the centroid of the shape is located
	*For triangle it's the middle coordinate , for trapezoidal its where 2 coordinates are equal
	*If trapezoidal save the range of centroid
	*/
	double * centroid_range;
};
//Holds the list of members and values for a rule
struct MemberOfRuleSet
{
	string variable_name;
	string linguistic_name;
	double fuzzy_value;
	
};
//Rule set of the fuzzy system
struct RuleSet
{   //Determins number of operators and variables
	int number_of_variables_in_rule;
	//List to hold variables + 1 for the output variable
	MemberOfRuleSet * list_of_variables_in_rule;
	//List to hold operators
	string * predicate_list;


};
//This struct will hold data and methods related to a fuzzy variable
struct FuzzySet
{   //The Name of the variable
	string variable_name;
	//Number of fuzzy members 
	int number_of_lingustical_terms;
	//Holds the data for all fuzzy members of this fuzzy set -> Trapzoidal or triangle
	FuzzySetShape * list_of_members;
	//The crisp value for the fuzzy set (We will use it for fuzzyfication process)
    double crisp_value;
	

};

//This class will hold all methods that are related to fuzzy logic
class Toolbox
{   //Rules of the Fuzzy system
	RuleSet * list_of_rules;
	//Fuzzy sets of the system
	FuzzySet * list_of_fuzzy_sets;
	//Number of fuzzy sets
	int number_of_fuzzy_set;
	//Number of rules
	int number_of_rule_set;
	
	
public:
	//Get input from file
	void getInputFromFile()
  	{   //Initialize and open the input file
		ifstream input_file;
		input_file.open(INPUT_FILE);
		//First input is the number of fuzzy sets
		input_file >> number_of_fuzzy_set;
		//We add 1 more to the number of fuzzy sets cause we have the output set also
		number_of_fuzzy_set++;
		list_of_fuzzy_sets = new FuzzySet[number_of_fuzzy_set];
		//Read the input fuzzy sets first including a condition for the output fuzzy set (we wont read crisp value)
		for (int loop1 = 0; loop1 < number_of_fuzzy_set; loop1++)
		{
			input_file >> list_of_fuzzy_sets[loop1].variable_name;
			if(loop1 !=number_of_fuzzy_set-1)
			input_file >> list_of_fuzzy_sets[loop1].crisp_value;
			input_file >> list_of_fuzzy_sets[loop1].number_of_lingustical_terms;
			list_of_fuzzy_sets[loop1].list_of_members = new FuzzySetShape[list_of_fuzzy_sets[loop1].number_of_lingustical_terms];
			for (int loop2 = 0; loop2 < list_of_fuzzy_sets[loop1].number_of_lingustical_terms; loop2++)
			{
				input_file >> list_of_fuzzy_sets[loop1].list_of_members[loop2].member_name;
				input_file >> list_of_fuzzy_sets[loop1].list_of_members[loop2].shape_type;
				if (list_of_fuzzy_sets[loop1].list_of_members[loop2].shape_type == TRIANGLE)
					list_of_fuzzy_sets[loop1].list_of_members[loop2].size = 3;
				else
					list_of_fuzzy_sets[loop1].list_of_members[loop2].size = 4;
				list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape = new double[list_of_fuzzy_sets[loop1].list_of_members[loop2].size];
				for (int loop3 = 0; loop3 < list_of_fuzzy_sets[loop1].list_of_members[loop2].size; loop3++)
				{
					input_file >> list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[loop3];
				}
				if (list_of_fuzzy_sets[loop1].list_of_members[loop2].shape_type == TRIANGLE)
				{
					list_of_fuzzy_sets[loop1].list_of_members[loop2].centroid_range = new double[1];
					list_of_fuzzy_sets[loop1].list_of_members[loop2].centroid_range[0] = list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[1];
				}
				else
				{
					list_of_fuzzy_sets[loop1].list_of_members[loop2].centroid_range = new double[2];
					if (list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[0] == list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[1])
					{
						list_of_fuzzy_sets[loop1].list_of_members[loop2].centroid_range[0] = list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[0];
						list_of_fuzzy_sets[loop1].list_of_members[loop2].centroid_range[1] = list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[2];
					}
					else if (list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[2] == list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[3])
					{
						list_of_fuzzy_sets[loop1].list_of_members[loop2].centroid_range[1] = list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[1];
						list_of_fuzzy_sets[loop1].list_of_members[loop2].centroid_range[0] = list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[3];
					}
					else
					{
						list_of_fuzzy_sets[loop1].list_of_members[loop2].centroid_range[1] = list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[1];
						list_of_fuzzy_sets[loop1].list_of_members[loop2].centroid_range[0] = list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[2];
					}
					
					
				}

			}

		}
		//Read the rule set number
		input_file >> number_of_rule_set;
		//Initialize the rule set list
		list_of_rules = new RuleSet[number_of_rule_set];
		string uneeded_extra_info_in_rule;
		//Read the rule set from the file 
		for (int loop1 = 0; loop1 < number_of_rule_set; loop1++)
		{
			input_file >> list_of_rules[loop1].number_of_variables_in_rule;
			list_of_rules[loop1].predicate_list = new string[list_of_rules[loop1].number_of_variables_in_rule - 1];
			//We increment the number of variables cause of the result variable;
			list_of_rules[loop1].number_of_variables_in_rule++;
			list_of_rules[loop1].list_of_variables_in_rule = new MemberOfRuleSet[list_of_rules[loop1].number_of_variables_in_rule];
			for (int loop2 = 0; loop2 < list_of_rules[loop1].number_of_variables_in_rule; loop2++)
			{
				input_file >> list_of_rules[loop1].list_of_variables_in_rule[loop2].variable_name;
				//For the '=' char
				input_file >> uneeded_extra_info_in_rule;
				input_file >> list_of_rules[loop1].list_of_variables_in_rule[loop2].linguistic_name;
				if (loop2 < list_of_rules[loop1].number_of_variables_in_rule - 2)
					input_file >> list_of_rules[loop1].predicate_list[loop2];

			}

		}
	    //Close the input file when done reading the data
		input_file.close();
		
	}
	//Print rules in the system
	void printRules()
	{
		cout << "Rules:" << endl;
		for (int loop1 = 0; loop1 < number_of_rule_set; loop1++)
		{
			cout << "Rule number -> " << loop1 + 1 << endl;
			for (int loop2 = 0; loop2 < list_of_rules[loop1].number_of_variables_in_rule; loop2++)
			{
				cout << " variable name = " << list_of_rules[loop1].list_of_variables_in_rule[loop2].variable_name;
				cout << "  has value = " << list_of_rules[loop1].list_of_variables_in_rule[loop2].linguistic_name;
				if (loop2 < list_of_rules[loop1].number_of_variables_in_rule - 2)
					cout << " "<<list_of_rules[loop1].predicate_list[loop2];

			}
			cout << endl;
		}

	}
	//Fuzzification function
	void fuzzificaion()
	{
		
		//Variable to store the current crisp value during each iteration over all members
		double current_crisp_value;
		//Calculate the value of each member excpet the output set
		for (int loop1 = 0; loop1 < number_of_fuzzy_set - 1; loop1++)
		{ current_crisp_value = list_of_fuzzy_sets[loop1].crisp_value;
		   /*Initialize the list of membership values for each fuzzy set 
		   *Size is determined by the type of shape -> triangle or trapezoidal
		   */
		   for (int loop2 = 0; loop2 < list_of_fuzzy_sets[loop1].number_of_lingustical_terms; loop2++)
		   {
		
			   //Next step is to calculate the member values for each fuzzy set given the crisp value
			  
			   
				   if (list_of_fuzzy_sets[loop1].list_of_members[loop2].shape_type == TRIANGLE)
				   {   //Crisp value isnt in range
					   if (current_crisp_value < list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[0] || current_crisp_value > list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[2])
						   list_of_fuzzy_sets[loop1].list_of_members[loop2].fuzzification_value = 0.0;
					   //Crisp value between triangle centroids
					   else if (current_crisp_value == list_of_fuzzy_sets[loop1].list_of_members[loop2].centroid_range[0])
						   list_of_fuzzy_sets[loop1].list_of_members[loop2].fuzzification_value = 1.0;
					   //Calculate degree of membership
					   else
					   {
						   if (current_crisp_value < list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[1])
						   {
							   list_of_fuzzy_sets[loop1].list_of_members[loop2].fuzzification_value = (current_crisp_value - list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[0]) / (list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[1] - list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[0]);

						   }
						   else
						   {
							   list_of_fuzzy_sets[loop1].list_of_members[loop2].fuzzification_value = (list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[2] - current_crisp_value) / (list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[2] - list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[1]);

						   }

					   }
				   }
				   //Trapezoidal
				   else
				   {   //Crisp value isnt in range
					   if (current_crisp_value < list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[0] || current_crisp_value > list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[3])
						   list_of_fuzzy_sets[loop1].list_of_members[loop2].fuzzification_value = 0.0;
					   //Crisp value between trapeziodal centroids
					   else if (current_crisp_value >= list_of_fuzzy_sets[loop1].list_of_members[loop2].centroid_range[0] && current_crisp_value <= list_of_fuzzy_sets[loop1].list_of_members[loop2].centroid_range[1])
					   {
						   list_of_fuzzy_sets[loop1].list_of_members[loop2].fuzzification_value = 1.0;
					   }
					   //Calculate degree of membership
					   else
					   {
						   if (list_of_fuzzy_sets[loop1].list_of_members[loop2].centroid_range[0] == list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[0])
						   {
							  list_of_fuzzy_sets[loop1].list_of_members[loop2].fuzzification_value = (current_crisp_value - list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[2]) / (list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[3] - list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[2]);
							  list_of_fuzzy_sets[loop1].list_of_members[loop2].fuzzification_value = 1.0 - list_of_fuzzy_sets[loop1].list_of_members[loop2].fuzzification_value;
						   }
						   else if(list_of_fuzzy_sets[loop1].list_of_members[loop2].centroid_range[0] == list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[1])
						   {
							   if (current_crisp_value < list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[1])
							   {
								   list_of_fuzzy_sets[loop1].list_of_members[loop2].fuzzification_value = (current_crisp_value - list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[0]) / (list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[1] - list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[0]);
								   list_of_fuzzy_sets[loop1].list_of_members[loop2].fuzzification_value = 1.0 - list_of_fuzzy_sets[loop1].list_of_members[loop2].fuzzification_value;
							   }
							   else
							   {
								   list_of_fuzzy_sets[loop1].list_of_members[loop2].fuzzification_value = (list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[3] - current_crisp_value) / (list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[3] - list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[2]);
								   list_of_fuzzy_sets[loop1].list_of_members[loop2].fuzzification_value = 1.0 - list_of_fuzzy_sets[loop1].list_of_members[loop2].fuzzification_value;
							   }
						   }
						   else
						   {
							   list_of_fuzzy_sets[loop1].list_of_members[loop2].fuzzification_value = (list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[1] - current_crisp_value) / (list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[1] - list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape[0]);
							   list_of_fuzzy_sets[loop1].list_of_members[loop2].fuzzification_value = 1.0 - list_of_fuzzy_sets[loop1].list_of_members[loop2].fuzzification_value;
						   }

					   }
				   }
			   }
		   }
	      
			

		}
	void printAfterFuzzification()
	{
		cout << "After Fuzzification : " << endl;
		for (int loop1 = 0; loop1 < number_of_fuzzy_set - 1; loop1++)
		{
			for (int loop2 = 0; loop2 < list_of_fuzzy_sets[loop1].number_of_lingustical_terms; loop2++)
			{
				cout << "variable : " << list_of_fuzzy_sets[loop1].list_of_members[loop2].member_name << "  ";
				cout << "has value = " << list_of_fuzzy_sets[loop1].list_of_members[loop2].fuzzification_value << endl;

			}
		}
	}
	//Destructor
	~Toolbox()
	{
		for (int loop1 = 0; loop1 < number_of_fuzzy_set; loop1++)
		{
			for (int loop2 = 0; loop2 < list_of_fuzzy_sets[loop1].number_of_lingustical_terms; loop2++)
			{
				delete[]  list_of_fuzzy_sets[loop1].list_of_members[loop2].coordinates_of_shape;
				delete[]  list_of_fuzzy_sets[loop1].list_of_members[loop2].centroid_range;
				
			}
			
			delete[]  list_of_fuzzy_sets[loop1].list_of_members;
			
			
			
		}
		for (int loop1 = 0; loop1 < number_of_rule_set; loop1++)
		{
		  delete[]	list_of_rules[loop1].predicate_list;
		  delete[]  list_of_rules[loop1].list_of_variables_in_rule;
		}
		delete[] list_of_fuzzy_sets;
		delete[] list_of_rules;
		
	}
	
};

//Entry Point of the program
int main()
{
	Toolbox my_tool_box;
	my_tool_box.getInputFromFile();
	my_tool_box.fuzzificaion();
	my_tool_box.printAfterFuzzification();
	system("pause");
	
}