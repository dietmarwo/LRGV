/*
Copyright (C) 2008-2013 Joseph Kasprzyk, Patrick Reed, Brian Kirsch, Greg Characklis and others.

LRGV is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

LRGV is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the LRGV.  If not, see <http://www.gnu.org/licenses/>.
*/


/**
	lrgv_input.cpp
	Purpose: Write simulation output.  Read input files, and read the
	control.txt parameter file.  Also provide declarations of global variables.
	
	@author JR Kasprzyk
	@version 2013-03-10
*/

#include <global.h>

using namespace std;

const string arr1[] = {"cost", "surplus", "critrel", "drop", "rel", "cvar", "numleases", "drtranscost", "drvuln","aggcost","aggrel"};
 //obj_avail =  new vector(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0]) );
const string arr2[] = {"rel", "critrel", "cvar", "drvuln"};

Input::Input() : obj_avail(arr1, arr1 + sizeof(arr1) / sizeof(arr1[0])), constr_avail(arr2, arr2 + sizeof(arr2) / sizeof(arr2[0]) )
{
}

void Input::write_results_header(filenames_structure &filenames, string calc_param)
{
	if (params.mode == "resample")	//legacy: processing_flag == 0
	{
		results_stream << "Algorithm" << params.delim;
		results_stream << "Configuration" << params.delim;
		results_stream << "RS" << params.delim;
		results_stream << "Index" << params.delim;
	}
	results_stream << "obj-cost";
	if(calc_param == "drought_full") results_stream << params.delim << "DroughtTransfersCost";
	results_stream << params.delim << "obj-rel";
	results_stream << params.delim << "obj-crel";
	results_stream << params.delim << "obj-surplus";
	if (params.model_case > 1)
	{
		results_stream << params.delim << "obj-costvar" << params.delim << "obj-drop";
		if (params.model_case > 2)
		{
			results_stream << params.delim << "obj-numleases";
		}
	}
	results_stream << params.delim << "obj-resilience" << params.delim << "fail-vol";
	results_stream << params.delim << "obj-vulnerability" << params.delim << "max-failureperiods";
	//JRK removed these constraint violation reporting header because the values are
	//no longer actually plotted.
	//results_stream << params.delim <<  "constr-rel" << params.delim << "constr-crel";
	//if (params.model_case > 1) results_stream << params.delim << "constr-cvar";
	//results_stream << params.delim << "init_rights";
	results_stream << params.delim << "rights";
	if (params.model_case > 1)
	{
		results_stream << params.delim << "xi";
		results_stream << params.delim << "options_low";
		results_stream << params.delim << "options_high";
		results_stream << params.delim << "alpha2";
		results_stream << params.delim << "beta2";
		if (params.model_case > 2)
		{
			results_stream << params.delim << "alpha" << params.delim << "beta";
		}
	}
	results_stream << params.delim << "ifri";
	char buffer[50];
	for (int header_it = 1; header_it <= params.NumberYears; header_it++)
	{
		sprintf(buffer, "rel-%d", header_it);
		results_stream << params.delim << buffer;
	}
	results_stream << params.delim << "rel-avg";
	for (int header_it = 1; header_it <= params.NumberYears; header_it++)
	{
		sprintf(buffer, "crel-%d", header_it);
		results_stream << params.delim << buffer;
	}
	results_stream << params.delim << "crel-avg";
	if (params.model_case > 1)
	{
		for (int header_it = 1; header_it <= params.NumberYears; header_it++)
		{
			sprintf(buffer, "drop-%d", header_it);
			results_stream << params.delim << buffer;
		}
	}
	results_stream << params.delim << "drop-avg";
	for (int header_it = 1; header_it <= params.NumberYears; header_it++)
	{
		sprintf(buffer, "totcost-%d", header_it);
		results_stream << params.delim << buffer;
	}
	results_stream << params.delim << "totcost-avg";
	for (int header_it = 1; header_it <= params.NumberYears; header_it++)
	{
		sprintf(buffer, "leasecost-%d", header_it);
		results_stream << params.delim << buffer;
	}
	results_stream << params.delim << "leasecost-avg";
	results_stream << params.delim << "costpercent-rights";
	if (params.model_case > 1)
	{
		results_stream << params.delim << "costpercent-options";
		if (params.model_case > 2)
		{
			results_stream << params.delim << "costpercent-leases";
		}
	}
	if (params.model_case > 1)
	{
		for (int header_it = 1; header_it <= params.NumberYears; header_it++)
		{
			sprintf(buffer, "num-high-opt-%d", header_it);
			results_stream << params.delim << buffer;
		}
		results_stream << params.delim << "num-high-opt-avg";
		for (int header_it = 1; header_it <= params.NumberYears; header_it++)
		{
			sprintf(buffer, "num-Nx-%d", header_it);
			results_stream << params.delim << buffer;
		}
		results_stream << params.delim << "num-Nx-avg";
		for (int header_it = 1; header_it <= params.NumberYears; header_it++)
		{
			sprintf(buffer, "avg-Nx-%d", header_it);
			results_stream << params.delim << buffer;
		}
		results_stream << params.delim << "avg-Nx-avg";
	}
	if (params.model_case > 2)
	{
		for (int header_it = 1; header_it <= params.NumberYears; header_it++)
		{
			sprintf(buffer, "avg-Nl-%d", header_it);
			results_stream << params.delim << buffer;
		}
		results_stream << params.delim << "avg-Nl-avg";
	}
	for (int header_it = 1; header_it <= params.NumberYears; header_it++)
	{
		sprintf(buffer, "endwtr-%d", header_it);
		results_stream << params.delim << buffer;
	}
	results_stream << params.delim << "endwtr-avg";
	//results_stream << params.delim << "drought-initrights";
	
	//The code was not meant to be run with "synchronous sampling" AND the contiguous drought at
	//the same time.  This snippet only outputs the continguous drought if Sobol is turned on and
	//synchronous sampling is turned off.  For different combinations of these modes, this code should
	//be tested further.
	
	//On 03-16-2015, we began testing this further indeed!
	//let's just output everything because we think that it will work ok.
	//but I wonder what happens when the init_lrgv gets called in the drought itself?
	//old if: if (params.mode == "sobol" && !params.sync_flag) //changed from "or" to "and", 11-26-2010
	//cout << "calc_param is" << calc_param << endl;
	//if (calc_param == "combined") //new version on 03-16-2015
	//{
		results_stream << params.delim << "drought-transcost";
		results_stream << params.delim << "drought-vulnerability";
		results_stream << params.delim << "drought-maxfailureperiods";
		results_stream << params.delim << "drought-totalperiods";
		for (int header_it = 1; header_it <= 12; header_it++)
		{
			sprintf(buffer, "drought-avwater%d", header_it);
			results_stream << params.delim << buffer;
			sprintf(buffer, "drought-Nr%d", header_it);
			results_stream << params.delim << buffer;
			sprintf(buffer, "drought-leases%d", header_it);
			results_stream << params.delim << buffer;
			sprintf(buffer, "drought-demand%d", header_it);
			results_stream << params.delim << buffer;
			sprintf(buffer, "drought-monthlysurplus%d", header_it);
			results_stream << params.delim << buffer;
		}
		results_stream << params.delim << "drought-No";
		results_stream << params.delim << "drought-Nx" << endl;
	//}
	//else
	//{
	//	results_stream << endl;
	//}
	
	//JRK: note the commented out section should make this work for now.
	//but eventually we should update the logic of how these things work with the new modes
	return;
}
void Input::write_monthly_header(filenames_structure &filenames)
{
	char buffer[50];
	for (int header_it = 1; header_it <= (12*params.NumberYears); header_it++)
	{
		sprintf(buffer, "failurerate-%d", header_it);
		monthly_stream << buffer;
		if (header_it != (12*params.NumberYears))
		{
			monthly_stream << params.delim;
		}
	}
	for (int header_it = 1; header_it <= (12*params.NumberYears); header_it++)
	{
		sprintf(buffer, "avwater-%d", header_it);
		monthly_stream << params.delim << buffer;
	}
	for (int header_it = 1; header_it <= (12*params.NumberYears); header_it++)
	{
		sprintf(buffer, "vulnerability-%d", header_it);
		monthly_stream << params.delim << buffer;
	}
	for (int header_it = 1; header_it <= (12*params.NumberYears); header_it++)
	{
		sprintf(buffer, "leases-%d", header_it);
		monthly_stream << params.delim << buffer;
	}
	monthly_stream << endl;
	return;
}
void Input::write_monthly_output()
{
	double temp_sum = 0.0;
	double placeholder = 0.0;
	
	//report failure record
	for (int year_it = 0; year_it < params.NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			placeholder = average_array_colwise(monthly_tracker[month_it].total_failures, params.NumberSims, params.NumberYears, year_it);
			if (placeholder == 0)
			{
				monthly_stream.unsetf(ios::fixed);
			}
			else 
			{
				monthly_stream.setf(ios::fixed);
				monthly_stream << setprecision(6);
			}
			monthly_stream << placeholder << params.delim;
		}
	}

	//report available water 
	monthly_stream.setf(ios::fixed);
	monthly_stream << setprecision(1);
	for (int year_it = 0; year_it < params.NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			placeholder = average_array_colwise(monthly_tracker[month_it].av_water, params.NumberSims, params.NumberYears, year_it);
			monthly_stream << placeholder << params.delim;
		}
	}
	monthly_stream.unsetf(ios::fixed);

	//report vulnerability
	for (int year_it = 0; year_it < params.NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			temp_sum = 0.0;
			for (int sims_it = 0; sims_it < params.NumberSims; sims_it++) //fixed bug, 2009-06-15_1 version
			{
				placeholder = monthly_tracker[month_it].random_monthly_demand[sims_it][year_it]-monthly_tracker[month_it].av_water[sims_it][year_it];
				if (placeholder > 0)
					temp_sum = temp_sum + placeholder;
			}

			if (temp_sum == 0)
			{
				monthly_stream.unsetf(ios::fixed);
			}
			else 
			{
				monthly_stream.setf(ios::fixed);
				monthly_stream << setprecision(6);
			}

			monthly_stream << (1/((double)params.NumberSims))*temp_sum;
			
			if (month_it == 11)
			{ 
				if (year_it != (params.NumberYears-1))
				{
					monthly_stream << params.delim;
				}
			}
			else monthly_stream << params.delim;
		}
	}
	monthly_stream.unsetf(ios::fixed);
	//report leases
	monthly_stream.setf(ios::fixed);
	monthly_stream << setprecision(1);
	for (int year_it = 0; year_it < params.NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			placeholder = average_array_colwise(monthly_tracker[month_it].total_monthly_leases, params.NumberSims, params.NumberYears, year_it);
			monthly_stream << placeholder << params.delim;
		}
	}
	monthly_stream.unsetf(ios::fixed);

	monthly_stream << endl;
	return;
}
void Input::hydro_read(string hydro_filename)
{

	//DEBUG 10-01
	//cout << "Welcome to the hydro read function!!" << endl;
	//END DEBUG

	/* Usage: Reads hydrology data for random sampling.
	Transforms it according to Roulette Weighted Sampling */

	ifstream in;
	double junkd;

	//Read Initial Rights
	in.open("initrights.txt", ios::in);
	if(!in) {cout << "Failed to open init rights file!" << endl; exit(1);}
	general_1d_allocate(params.initrights_vec, 50000);
	for (int i = 0; i < 49999; i++)
	{
		in >> params.initrights_vec[i];
	}
	in.close();
	in.clear();

	//Read hydro file. Places data into hydro_sets

	openfile(in, hydro_filename);
	while(1)
	{
		if (in.eof()) break;
		else
		{
			char junk[255];
			in.getline(junk,255);

			//read inflow
			for (int month_it = 0; month_it<12; month_it++)
			{
				for (int hist_it = 0; hist_it<33; hist_it++)
				{
					in >> junkd;
					hydro_sets[month_it].inf.push_back(junkd);
				}
				in.getline(junk,255);
			}
			in.getline(junk,255);

			//read los
			for (int month_it = 0; month_it<12; month_it++)
			{
				for (int hist_it = 0; hist_it<33; hist_it++)
				{
					in >> junkd;
					hydro_sets[month_it].los.push_back(junkd);
				}
				in.getline(junk,255);
			}
			in.getline(junk,255);

			//read reservoir variation (this value
			//can be negative or positive)
			for (int month_it = 0; month_it<12; month_it++)
			{
				for (int hist_it = 0; hist_it<31; hist_it++)
				{
					in >> junkd;
					hydro_sets[month_it].res_var.push_back(junkd);
				}
				in.getline(junk,255);
			}
			in.getline(junk,255);

			//read new water (NW)
			for (int month_it = 0; month_it<12; month_it++)
			{
				for (int hist_it = 0; hist_it<33; hist_it++)
				{
					in >> junkd;
					hydro_sets[month_it].NW.push_back(junkd);
				}
				in.getline(junk,255);
			}
		}
		in.close();

		return;
	}
}


void Input::lease_read(string lease_filename)
{

	ifstream read_strings;

	read_strings.open(lease_filename.c_str(), ios::in);

	if(!read_strings)
	{
		cout << "Failed to open input file: " << lease_filename << "!" << endl;
		exit(1);
	}

	else
	{

		//junk the headerline
		char junk[255];
		read_strings.getline(junk,255);

		string string_temp;

		//read low reservoir lease price list
		
		for (int month_it = 0; month_it<12; month_it++) //loop through the months
		{
			//get a whole line from the read_strings ifstream and put it in string_temp
			getline(read_strings, string_temp);

			//open up a string stream which will read string_temp	
			istringstream iss(string_temp);
			
			//declare a junk variable to read in from the string
			double junk_value;

			while (iss >> junk_value) //as long as there are values to read...
			{
				//we place them in our structure
				lease_sets[month_it].low_res.push_back(junk_value);
			}

			iss.clear();

			//after we run out of stuff to read, we loop through again

		} //end of for loop for the months

		read_strings.getline(junk,255);

		for (int month_it = 0; month_it<12; month_it++) //loop through the months
		{
			//get a whole line from the read_strings ifstream and put it in string_temp
			getline(read_strings, string_temp);

			//open up a string stream which will read string_temp	
			istringstream iss(string_temp);
			
			//declare a junk variable to read in from the string
			double junk_value;

			while (iss >> junk_value) //as long as there are values to read...
			{
				//we place them in our structure
				lease_sets[month_it].high_res.push_back(junk_value);
			}

			iss.clear();

			//after we run out of stuff to read, we loop through again

		} //end of for loop for the months

	} //end of the else brackets

	read_strings.close();

	return;

}

void Input::demand_read(string demand_filename)
{
	ifstream read_strings;

	read_strings.open(demand_filename.c_str(), ios::in);

	if(!read_strings)
	{
		cout << "Failed to open input file: " << demand_filename << "!" << endl;
		exit(1);
	}

	else
	{

		//junk the headerline
		char junk[255];
		read_strings.getline(junk,255);

		string string_temp;
		
		for (int month_it = 0; month_it<12; month_it++) //loop through the months
		{
			//get a whole line from the read_strings ifstream and put it in string_temp
			getline(read_strings, string_temp);

			//open up a string stream which will read string_temp	
			istringstream iss(string_temp);
			
			//declare a junk variable to read in from the string
			double junk_value;

			while (iss >> junk_value) //as long as there are values to read...
			{
				//we place them in our structure
				demand_sets[month_it].demand.push_back(junk_value);
			}

			iss.clear();

			//after we run out of stuff to read, we loop through again

		} //end of for loop for the months

		read_strings.getline(junk,255);

		for (int month_it=0; month_it<12; month_it++)
		{
			double junk_value;

			read_strings >> junk_value;

			demand_sets[month_it].mean = junk_value;

			read_strings.getline(junk,255);

		}

		read_strings.getline(junk,255);

		for (int month_it=0; month_it<12; month_it++)
		{
			double junk_value;

			read_strings >> junk_value;

			demand_sets[month_it].std_dev = junk_value;

			read_strings.getline(junk,255);

		}

		read_strings.close();

	} //end of the else brackets

	return;

}

void Input::control_read(filenames_structure &filenames)
{
	ifstream in;
	in.open(filenames.control.c_str(), ios::in);

	//TODO add parameter defaults in case someone
	//forgets a parameter

	char junk[1000];

	if (!in)
	{
		cout << "Failed to open input file: " << filenames.control << "!" << endl;
		exit(1);
	}
	else
	{
		//general_1d_allocate(params.ifri_dist, 255); //10-09-2012 changed to std::string
		while(!in.eof())
		{
			in >> junk; //character array for the computer to read
			//for each of these lines, if it finds the tag it reads the next value
			if (!strcmp(junk, "<transform>")) in >> params.dectransform_flag;
			//else if (!strcmp(junk, "<parameter_continuity_flag>")) in >> params.paramcheck_flag;
			else if (!strcmp(junk, "<discretize_flag>")) in >> params.discretize_flag;
			else if (!strcmp(junk, "<rights>")) in >> strategy.Nrt;
			else if (!strcmp(junk, "<options_low>")) in >> strategy.No_low;
			else if (!strcmp(junk, "<options_high>")) in >> strategy.No_high;
			else if (!strcmp(junk, "<xi>")) in >> strategy.xi;
			else if (!strcmp(junk, "<alpha2>")) in >> strategy.alpha2;
			else if (!strcmp(junk, "<beta2>")) in >> strategy.beta2;
			else if (!strcmp(junk, "<alpha>")) in >> strategy.alpha;
			else if (!strcmp(junk, "<beta>")) in >> strategy.beta;
			else if (!strcmp(junk, "<initial_rights_dist>")) in >> params.ifri_dist;
			else if (!strcmp(junk, "<initial_rights_param1>")) in >> params.ifri_param1;
			else if (!strcmp(junk, "<initial_rights_param2>")) in >> params.ifri_param2;
			else if (!strcmp(junk, "<critical_reliability_threshold>")) in >> params.critical_reliability_threshold;
			else if (!strcmp(junk, "<demand_growth_factor>")) in >> params.DemGrowthFactor;
			else if (!strcmp(junk, "<initial_reservoir_volume>")) in >> params.iRo;
			else if (!strcmp(junk, "<option_exercise_month>")) in >> params.OExerciseMonth;
			else if (!strcmp(junk, "<options_price>")) in >> params.Po;
			else if (!strcmp(junk, "<rights_price>")) in >> params.Pr;
			else if (!strcmp(junk, "<strike_price>")) in >> params.Px;
			else if (!strcmp(junk, "<problem_case>")) in >> params.model_case;
			else if (!strcmp(junk, "<monte_carlo>"))
			{
				int junkSims;
				in >> junkSims;

				//Since the 2014-02-10 version of the code, the value on the command line
				//supersedes whatever is in the control file. We used to warn about this,
				//but that caused a problem with the standard-io version.  Basically we had
				//to strip anything but the most fatal errors from being reported to the
				//command line.

				if (params.NumberSims == -1) //In other words, if the NumberSims hasn't been set yet...
				{
					//set it.
					params.NumberSims = junkSims;
				}	
			}
			else if (!strcmp(junk, "<number_years>")) in >> params.NumberYears;
			else if (!strcmp(junk, "<synchronous_sampling>")) in >> params.sync_flag;
			else if (!strcmp(junk, "<calendar_run>")) in >> params.single_flag;
			else if (!strcmp(junk, "<calendar_date>")) in >> params.single_year;
			else if (!strcmp(junk, "<instream_loss>")) in >> params.in_loss;
			else if (!strcmp(junk, "<reservoir_threshold>")) in >> params.reservoir_threshold;
			else if (!strcmp(junk, "<reservoir_critical_level>")) in >> params.ReservoirCriticalLevel;
			else if (!strcmp(junk, "<total_water_rights>")) in >> params.TWR;
			else if (!strcmp(junk, "<max_rights>")) in >> params.Nrt_max;
			else if (!strcmp(junk, "<max_options>")) in >> params.No_max;
			else if (!strcmp(junk, "<output_timing>")) in >> params.timing_flag;
			else if (!strcmp(junk, "<output_yearly>")) in >> params.results_flag;
			else if (!strcmp(junk, "<output_monthly>")) in >> params.monthly_flag;
			else if (!strcmp(junk, "<output_ensemble>")) in >> params.rng_flag;
			else if (!strcmp(junk, "<output_full-sim>")) in >> params.out_flag;
			else if (!strcmp(junk, "<output_yearly_header>")) in >> params.results_header_flag;
			else if (!strcmp(junk, "<output_delim>")) in >> params.delim_flag;
			else if (!strcmp(junk, "<output_monthly_header>")) in >> params.monthly_header_flag;
			else if (!strcmp(junk, "<roulette_flag>")) in >> params.roulette_flag;
			else if (!strcmp(junk, "<inf_weight>")) in >> params.inf_weight;
			else if (!strcmp(junk, "<res_var_weight>")) in >> params.res_var_weight;			
			else if (!strcmp(junk, "<lease_weight>")) in >> params.lease_weight;
			else if (!strcmp(junk, "<demand_weight>")) in >> params.demand_weight;
			else if (!strcmp(junk, "<objectives_flag>")) in >> params.obj_flag;
			else if (!strcmp(junk, "<constraints_flag>")) in >> params.constr_flag;
			else if (!strcmp(junk, "<objectives_begin>"))
			{
				in.ignore(2000,'\n');
				
				string str_junk;
				double double_junk;

				in >> str_junk;
				do
				{
 				  if (find(obj_avail.begin(), obj_avail.end(), str_junk) == obj_avail.end()){
				    cerr << "Error! objective " << str_junk << " not recognised. Supported objectives are: ";
				    static vector<string>::const_iterator cii;
				    for(cii=obj_avail.begin(); cii!=obj_avail.end(); cii++){
				      cerr << *cii << " ";
				    }
				    cerr << endl;
				    exit(-1);
				  }
					//read name
					params.obj_names.push_back(str_junk);
					//read scaling factor
					in >> double_junk;
					params.obj_scalingfactors.push_back(double_junk);
					in >> double_junk;
					params.obj_epsilons.push_back(double_junk);
					//skip to the next line and grab the first thing in that line
					in.ignore(2000,'\n');
					in >> str_junk;
				} while(str_junk != "<objectives_end>");
			}
			else if (!strcmp(junk, "<constraints_begin>"))
			{
				in.ignore(2000,'\n');
				
				string str_junk;
				double double_junk;

				in >> str_junk;
				do
				{
 				  if (find(constr_avail.begin(), constr_avail.end(), str_junk) == constr_avail.end()){
				    cerr << "Error! constraint " << str_junk << " not recognised. Supported constraints are: ";
				    static vector<string>::const_iterator cii;
				    for(cii=constr_avail.begin(); cii!=constr_avail.end(); cii++){
				      cerr << *cii << " ";
				    }
				    cerr << endl;
				    exit(-1);
				  }
					//read name
					params.constr_names.push_back(str_junk);
					
					//read comparator
					in >> str_junk;
					params.constr_comparators.push_back(str_junk);

					//read value
					in >> double_junk;
					params.constr_values.push_back(double_junk);

					//skip to the next line and grab the first thing in that line
					in.ignore(2000,'\n');
					in >> str_junk;
				} while(str_junk != "<constraints_end>");
			}
			in.ignore(2000,'\n');
		} //end while loop
		//general_1d_allocate(params.delim, 255); //10-9-2012, removed allocation and changed delim to std::string
		if (params.delim_flag == 0)
			params.delim = ",";
		else
			params.delim = "   ";
		in.close();
		
		//9/21/2012 add check to make sure NumberSims is defined.
		if (params.NumberSims == -1)
		{
			cerr << "NumberSims not entered on command line or control file. Exiting..." << endl;
			exit(-1);
		}
			
	} //end if-statement for valid input stream

	return;
}
void Input::hist_read(filenames_structure &filenames)
{
	control_read(filenames); //7/21/2011 moved control read up so that roulette parameters are in memory before subsequent calls
	hydro_read(filenames.hydro);
	lease_read(filenames.lease);
	demand_read(filenames.demand);
	
}

void Input::general_debug_output (ofstream &out, double **data, int rows, int cols)
{
	//out << endl;

	for (int row_it = 0; row_it < rows; row_it++)
	{
		for (int col_it = 0; col_it < cols; col_it++)
		{
			out << data[row_it][col_it] << "   ";
		}
		out << endl;
	}
	out << endl;

	return;
}

void Input::sets_output_rng_text(ofstream &out, double _current_sim)
{
	int NumberYears = params.NumberYears;

	out << "Simulation " << _current_sim << endl;
	//same as debug output with some minor changes
	out << "Inflows" << endl;
	
	out.setf(ios_base::fixed, ios_base::floatfield);
	out.precision(5); //for the fixed type: precision is for numbers past decimal
	//testing the width and fill commands to even up those columns:
	//out.width(30);
	//out.fill('%');
	
	for (int year_it = 0; year_it < NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			out << samples[month_it].inf[year_it] << "   ";
		}
		out << endl;
	}

	out << "Losses" << endl;

	for (int year_it = 0; year_it < NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			out << samples[month_it].los[year_it] << "   ";
		}
		out << endl;
	}

	out << "ReservoirVariation" << endl;

	for (int year_it = 0; year_it < NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			out << samples[month_it].res_var[year_it] << "   ";
		}
		out << endl;
	}

	out << "Demand" << endl;

	for (int year_it = 0; year_it < NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			out << samples[month_it].demand[year_it] << "   ";
		}
		out << endl;
	}

	out << "LeasePriceLow" << endl;

	for (int year_it = 0; year_it < NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			out << samples[month_it].lease_low_res[year_it] << "   ";
		}
		out << endl;
	}

	out << "LeasePriceHigh" << endl;

	for (int year_it = 0; year_it < NumberYears; year_it++)
	{
		for (int month_it = 0; month_it < 12; month_it++)
		{
			out << samples[month_it].lease_high_res[year_it] << "   ";
		}
		out << endl;
	}



	return;
}

void Input::transfer_tracker(double *(&TransferTracker), double AvWater, double Demand)
{
    double NeededTransfers = 0.0;

    /* Begin Transfer Tracker section*/

    //"Transfer Tracking Section, roll the vector"
    for (int nu = 11; nu >= 0; nu--)
    {
        //start at index 12, make the index 12 member
        //equal to the index 11 member...
        TransferTracker[nu+1] = TransferTracker[nu];
    }

    //and when you get to the zero-th member, you zero it out.
    //since you've moved all the other members up
    //the last transfer in your bunch has expired.
    TransferTracker[0] = 0;

    //utilize transfers...

    if (AvWater <= Demand)
    {
        //either: you met demand EXACTLY, or you had a failure.  Either way,
        //you know that at the last month, you used all your transfers...so the
        //tracker should be set to empty.
        zeroes(TransferTracker, 13);
    }

    if (AvWater > Demand && (AvWater - sum(TransferTracker,13)) < Demand)
    {
        //if we had enough water, and also:
        //the difference between the amount of water we had and the
        //amount in our transfer account ... was less than the demand
        //(that is, you only needed a portion of your transfers) Now, the
        //transfer tracker sees where that water came from, by attempting to
        //pull the water out of the tracker in the order which you purchased
        //your transfers (oldest first)

        //NeededTransfers is how much you needed to take from your
        //leases to fulfill water last time.
        NeededTransfers = Demand - (AvWater - sum(TransferTracker,13));

        //declare a local counter here, and pop it into a while loop
        int nu = 12; //the highest index is 12 (in Matlab it is 13)

        //while you still need to pull water out and you're not at the end of the list...
        while (NeededTransfers > 0 && nu >=0)
        {
            //if the nu-th value is not zero, we can try to pull some water
            //out of that list-member
            if (TransferTracker[nu] != 0)
            {
                //if you can get all you need (and more) from one list member...
                if (TransferTracker[nu] >= NeededTransfers)
                {
                    //subtract out your needed transfers from the given member,
                    TransferTracker[nu] = TransferTracker[nu] - NeededTransfers;
                    //then zero-out your needed transfers (because you got all you need!)
                    NeededTransfers = 0;
                }
                //otherwise, you're going to pull out all you can from the
                //given member, and keep looking for some more water
                else
                {
                    NeededTransfers = NeededTransfers - TransferTracker[nu];
                    TransferTracker[nu] = 0;
                }

            }//if(TransferTracker[nu] !=0)

            nu = nu - 1; //we can try and move to the next list member.

            //and this loop is designed such that you can only get into it
            //if you know that the model needed water last month but didn't
            //need your entire amount of transfers.  So, there's a guaranteed
            //amount of water left over in the Tracker when you're done.  The
            //tracking is done after the usage calculation, so you're just
            //"cleaning up" and bookkeeping at this point.

        }//while(NeededTransfers> 0 && nu >=0)

    }//if(AvWater>Demand&& ... )

    return;
}


double Input::resilience_calc()
{
    double current_value = 0.0; //fixed 3/23/2010 -- this value is pulling members from the failures matrix,
                                //and apparently this matrix was comprised of doubles (even though it should just
                                //be zeros and ones
    double previous_value = 0;
    int number_of_events = 0;
    double raw_resilience = 0.0;
    double temp_sum = 0.0;

    double *calc_vector;
    //general_1d_allocate(calc_vector, params.NumberSims*params.NumberYears); //wrong (see next line)
    general_1d_allocate(calc_vector, 12*params.NumberYears); //JRK fix 2009-06-15_1 version

    int k = 0;

    for (int sims_it = 0; sims_it < params.NumberSims; sims_it++)
    {
        k = 0;

        for (int year_it = 0; year_it < params.NumberYears; year_it++)
        {
            for (int month_it = 0; month_it < 12; month_it++)
            {
                calc_vector[k] = monthly_tracker[month_it].total_failures[sims_it][year_it];
                k++;
            }
        }

        for (k = 0; k < 12*params.NumberYears; k++) //fixed on 2009-06-17
        {
            current_value = calc_vector[k];
            if (current_value > 0)
            {
                if (previous_value == 0) number_of_events++; //a new event
                raw_resilience++; //time of event
            }
            previous_value = current_value;
        }

        if (number_of_events == 0 || raw_resilience == 0)
        {
            temp_sum = temp_sum + 1.0;
        }
        else
        {
            temp_sum = temp_sum + ((double) number_of_events / (double) raw_resilience);
        }
    }
    zap(calc_vector);

    return ((1/(double) params.NumberSims)*temp_sum);
}

double Input::failvol_calc()
{
    double temp_sum = 0.0;
    double placeholder = 0.0;

    for (int sims_it = 0; sims_it < params.NumberSims; sims_it++)
    {
        for (int year_it = 0; year_it < params.NumberYears; year_it++)
        {
            for (int month_it = 0; month_it < 12; month_it++)
            {
                if (monthly_tracker[month_it].av_water[sims_it][year_it] < monthly_tracker[month_it].random_monthly_demand[sims_it][year_it])
                    placeholder = monthly_tracker[month_it].random_monthly_demand[sims_it][year_it]-monthly_tracker[month_it].av_water[sims_it][year_it];
                else placeholder = 0.0;
                temp_sum = temp_sum + placeholder;
            }
        }
    }
    return ((1/(double) params.NumberSims)*temp_sum);
}
void Input::vulnerability_calc(int local_sims, int local_years)
{
    double *calc_vector;
    int k, period_counter, temp_currentperiodmax, temp_outermax_periods, total_periods;
    double temp_sum, temp_currentmax, temp_outersum;
    general_1d_allocate(calc_vector, 12*local_years);

    temp_outersum = 0.0; //for doing an expectation across monte carlo simulations
    temp_outermax_periods = 0;
    total_periods = 0;
    for (int sims_it = 0; sims_it < local_sims; sims_it++)
    {
        //we are within the (sims_it)th time series.  We pull out all 12*(NumberYears) values for
        //failure volume regardless of month or year.
        k = 0;
        for (int year_it = 0; year_it < local_years; year_it++)
        {
            for (int month_it = 0; month_it < 12; month_it++)
            {
                if (monthly_tracker[month_it].av_water[sims_it][year_it] < monthly_tracker[month_it].random_monthly_demand[sims_it][year_it])
                    calc_vector[k] = monthly_tracker[month_it].random_monthly_demand[sims_it][year_it]-monthly_tracker[month_it].av_water[sims_it][year_it];
                else calc_vector[k] = 0.0;
                k++;
            }
        }
        //now, we take this time series and perform some calculations.  We are looking for the
        //sojourn into failure that has the highest volume.
        k = 0;
        temp_sum = 0.0;
        temp_currentmax = 0.0;
        temp_currentperiodmax = 0;

        //cout << "Inside the vulnerability calc, local_years is " << local_years << endl;

        while (k < 12*local_years)
        {
            temp_sum = 0.0;
            //count the total failure volume in this sojourn.  The do-while control structure
            //will count a failure volume even if there is only one period in this sojourn.
            period_counter = 0;
            do
            {
                if (calc_vector[k] > 0.0)
                {
                    temp_sum += calc_vector[k];
                    period_counter++;
                    total_periods++;
                    k++;
                }
            }while(calc_vector[k] > 0.0 && k < 12*local_years); //added k < 12*local_years, 3/29/2010
            //since the summation is over for this sojourn, we check to see if ours was the highest
            if (temp_sum > temp_currentmax) temp_currentmax = temp_sum;
            if (period_counter > temp_currentperiodmax) temp_currentperiodmax = period_counter;
            //once we are past this code, we know we have to look for the next sojourn, so k is iterated
            //so that we can continue stepping through the timeseries
            k++;
        }
        temp_outersum += temp_currentmax;
        if (temp_currentperiodmax > temp_outermax_periods) temp_outermax_periods = temp_currentperiodmax;
    }

    zap(calc_vector);
    //For vulnerability, the final returned value is the expected value of the maximum total failure volumes.
    g.vulnerability = temp_outersum / local_sims;
    //For failure periods, this value is the expected value of maximum failure period length.
    g.failure_periods = temp_outermax_periods;
    g.total_periods = total_periods; //only used in drought for now, strict sum
    //cout << "vulnerability calc over" << endl;
}
void Input::single_sync_sampling()
{
    int NumberYears = params.NumberYears;
    int NumberSims = params.NumberSims;
    super.y = new year_structure[NumberSims];
    for (int sims_it = 0; sims_it < NumberSims; sims_it++)
    {
        for (int month_it = 0; month_it < 12; month_it++)
        {
            //have to allocate memory first for each sample and month...
            super.y[sims_it].samples[month_it].demand = new double [NumberYears];
            super.y[sims_it].samples[month_it].inf = new double [NumberYears];
            super.y[sims_it].samples[month_it].lease_high_res = new double [NumberYears];
            super.y[sims_it].samples[month_it].lease_low_res = new double [NumberYears];
            super.y[sims_it].samples[month_it].los = new double [NumberYears];
            //super.y[sims_it].samples[month_it].NW = new double [NumberYears];
            super.y[sims_it].samples[month_it].res_var = new double [NumberYears];
        }
    }

    if (params.sync_flag)
    {
        for (int sims_it = 0; sims_it < NumberSims; sims_it++)
        {
            for (int month_it = 0; month_it < 12; month_it++)
            {
                randsample(hydro_sets[month_it], super.y[sims_it].samples[month_it], NumberYears);
                randsample(lease_sets[month_it], super.y[sims_it].samples[month_it], NumberYears);
                randsample(demand_sets[month_it], super.y[sims_it].samples[month_it], NumberYears, params.DemGrowthFactor);
                //We have: 12 samples structures, one for each month
                //with enough randomly-sampled values to run
                //one n-iteration (monte carlo iteration).
            }
        }
    }
    else
    {
        for (int month_it = 0; month_it < 12; month_it++)
        {
            super.y[0].samples[month_it].inf[0] = hydro_sets[month_it].inf[params.single_year];
            super.y[0].samples[month_it].los[0] = hydro_sets[month_it].los[params.single_year];
            super.y[0].samples[month_it].res_var[0] = hydro_sets[month_it].res_var[params.single_year];

            super.y[0].samples[month_it].lease_high_res[0] = max_array(lease_sets[month_it].high_res, (int)lease_sets[month_it].high_res.size());
            super.y[0].samples[month_it].lease_low_res[0] = max_array(lease_sets[month_it].low_res, (int)lease_sets[month_it].low_res.size());

            super.y[0].samples[month_it].demand[0] = max_array(demand_sets[month_it].demand,(int)demand_sets[month_it].demand.size())*pow((1+params.DemGrowthFactor), 9);
        }
    }
}
void Input::init_LRGV(char** argv, string init_param)
{
    //You always call the ten-year option first, then you use the
    //information from that call within the drought calculations.
    if (init_param == "ten-year")
    {
        filenames_structure filenames;

        filenames.hydro = "hydro.txt";
        filenames.lease = "lease.txt";
        filenames.demand = "demand.txt";

        //these filenames don't depend on things in the control file:
        //REMOVED 9/21/2012
//#ifndef LRGV_SAMPLER
//      params.filename_base = new char[255];
//      strcpy(params.filename_base, argv[2]);
//#endif
        //END REMOVED
        filenames.control = params.filename_base + "_control.txt";
        filenames.timing = params.filename_base + "_timing.txt";
        filenames.rng = params.filename_base + "_random_ensemble.txt";
        filenames.out = params.filename_base + "_results-full.txt";

        hist_read(filenames); //read control file (*_control.txt) and historical data

        //these filenames do depend on things in the control file:

        //results filename is based on delimiter.
        if (params.delim_flag == 0)
        {
            filenames.results = params.filename_base + "_results-yearly.csv";
            filenames.monthly = params.filename_base + "_results-monthly.csv";
        }
        else
        {
            filenames.results = params.filename_base + "_results-yearly.txt";
            filenames.monthly = params.filename_base + "_results-monthly.txt";
        }

        //open output files
        if (params.timing_flag) time_stream.open(filenames.timing.c_str());
        if (params.results_flag)
        {
            results_stream.open(filenames.results.c_str());
            if (params.results_header_flag) write_results_header(filenames, init_param);
        }
        if (params.monthly_flag)
        {
            monthly_stream.open(filenames.monthly.c_str());
            if (params.monthly_header_flag) write_monthly_header(filenames);
        }
        if (params.rng_flag) rng_stream.open(filenames.rng.c_str());
        if (params.out_flag) out_stream.open(filenames.out.c_str());

        if (params.timing_flag)
        {
            endtime = clock()-start;
            time_stream << CLOCKS_PER_SEC << "   " << endtime << "   " << endl;
        }

        sampler_count = 0;
        if (params.single_flag || params.sync_flag) single_sync_sampling();
    }
    if (init_param == "drought-full")
    {
        filenames_structure filenames;

        //Added these back in, 2012-10-01. Could've corrupted memory

        filenames.hydro = "hydro.txt";
        filenames.lease = "lease.txt";
        filenames.demand = "demand.txt";

        //these filenames don't depend on things in the control file:
        //REMOVED 9/21/2012.  The filename base should already be taken care of in main.
//#ifndef LRGV_SAMPLER
//      params.filename_base = new char[255];
//      strcpy(params.filename_base, argv[2]);
//#endif

        filenames.control = params.filename_base + "_control.txt";
        filenames.timing = params.filename_base + "_timing.txt";
        filenames.rng = params.filename_base + "_random_ensemble.txt";
        filenames.out = params.filename_base + "_results-full.txt";

        hist_read(filenames); //read control file (*_control.txt) and historical data

        //TEST: We'll try rewriting some of the params variables, in order to
        //run the drought with the same control file as the ten-year.

        drought.NumberSims = 1;
        drought.NumberYears = 1;
        drought.single_year = 19;
        drought.single_flag = 1;

        params.NumberSims = drought.NumberSims;
        params.NumberYears = drought.NumberYears;
        params.single_year = drought.single_year;
        params.single_flag = drought.single_flag;

        //these filenames do depend on things in the control file:

        //results filename is based on delimiter.
        if (params.delim_flag == 0)
        {
            filenames.results = params.filename_base + "_results-yearly.csv";
            filenames.monthly = params.filename_base + "_results-monthly.csv";
        }
        else
        {
            filenames.results = params.filename_base + "_results-yearly.txt";
            filenames.monthly = params.filename_base + "_results-monthly.txt";
        }

        //open output files
        if (params.timing_flag) time_stream.open(filenames.timing.c_str());
        if (params.results_flag)
        {
            results_stream.open(filenames.results.c_str());
            if (params.results_header_flag) write_results_header(filenames, init_param);
        }
        if (params.monthly_flag)
        {
            monthly_stream.open(filenames.monthly.c_str());
            if (params.monthly_header_flag) write_monthly_header(filenames);
        }
        if (params.rng_flag) rng_stream.open(filenames.rng.c_str());
        if (params.out_flag) out_stream.open(filenames.out.c_str());

        if (params.timing_flag)
        {
            endtime = clock()-start;
            time_stream << CLOCKS_PER_SEC << "   " << endtime << "   " << endl;
        }

        sampler_count = 0;
        //if (params.single_flag || params.sync_flag) single_sync_sampling();

        int NumberSims = drought.NumberSims;
        int NumberYears = drought.NumberYears;
        super.y = new year_structure[NumberSims];
        for (int sims_it = 0; sims_it < NumberSims; sims_it++)
        {
            for (int month_it = 0; month_it < 12; month_it++)
            {
                //have to allocate memory first for each sample and month...
                super.y[sims_it].samples[month_it].demand = new double [NumberYears];
                super.y[sims_it].samples[month_it].inf = new double [NumberYears];
                super.y[sims_it].samples[month_it].lease_high_res = new double [NumberYears];
                super.y[sims_it].samples[month_it].lease_low_res = new double [NumberYears];
                super.y[sims_it].samples[month_it].los = new double [NumberYears];
                //super.y[sims_it].samples[month_it].NW = new double [NumberYears];
                super.y[sims_it].samples[month_it].res_var = new double [NumberYears];
            }
        }
        //this assumes the multi-year scenario is not using synchronous sampling
        //(it is overwriting the super structure)
        for (int month_it = 0; month_it < 12; month_it++)
        {
            super.y[0].samples[month_it].inf[0] = hydro_sets[month_it].inf[drought.single_year];
            super.y[0].samples[month_it].los[0] = hydro_sets[month_it].los[drought.single_year];
            super.y[0].samples[month_it].res_var[0] = hydro_sets[month_it].res_var[drought.single_year];

            super.y[0].samples[month_it].lease_high_res[0] = max_array(lease_sets[month_it].high_res, (int)lease_sets[month_it].high_res.size());
            super.y[0].samples[month_it].lease_low_res[0] = max_array(lease_sets[month_it].low_res, (int)lease_sets[month_it].low_res.size());

            super.y[0].samples[month_it].demand[0] = max_array(demand_sets[month_it].demand,(int)demand_sets[month_it].demand.size())*pow((1+params.DemGrowthFactor), 9);
        }
    }
    else if(init_param == "drought_noinit")
    {
        //we are going to store things that are different than the ten-year
        //in the drought structure.
        drought.NumberSims = 1;
        drought.NumberYears = 1;
        drought.single_year = 19;
        drought.single_flag = 1;
        int NumberSims = drought.NumberSims;
        int NumberYears = drought.NumberYears;
        super.y = new year_structure[NumberSims];
        for (int sims_it = 0; sims_it < NumberSims; sims_it++)
        {
            for (int month_it = 0; month_it < 12; month_it++)
            {
                //have to allocate memory first for each sample and month...
                super.y[sims_it].samples[month_it].demand = new double [NumberYears];
                super.y[sims_it].samples[month_it].inf = new double [NumberYears];
                super.y[sims_it].samples[month_it].lease_high_res = new double [NumberYears];
                super.y[sims_it].samples[month_it].lease_low_res = new double [NumberYears];
                super.y[sims_it].samples[month_it].los = new double [NumberYears];
                //super.y[sims_it].samples[month_it].NW = new double [NumberYears];
                super.y[sims_it].samples[month_it].res_var = new double [NumberYears];
            }
        }
        //this assumes the multi-year scenario is not using synchronous sampling
        //(it is overwriting the super structure)
        for (int month_it = 0; month_it < 12; month_it++)
        {
            super.y[0].samples[month_it].inf[0] = hydro_sets[month_it].inf[drought.single_year];
            super.y[0].samples[month_it].los[0] = hydro_sets[month_it].los[drought.single_year];
            super.y[0].samples[month_it].res_var[0] = hydro_sets[month_it].res_var[drought.single_year];

            super.y[0].samples[month_it].lease_high_res[0] = max_array(lease_sets[month_it].high_res, (int)lease_sets[month_it].high_res.size());
            super.y[0].samples[month_it].lease_low_res[0] = max_array(lease_sets[month_it].low_res, (int)lease_sets[month_it].low_res.size());

            super.y[0].samples[month_it].demand[0] = max_array(demand_sets[month_it].demand,(int)demand_sets[month_it].demand.size())*pow((1+params.DemGrowthFactor), 9);
        }

    }

    //simulator.setInitialCall(1);
}


void Input::transform_LRGV(double* vars)
{
    //DEBUG 10-01
    //cout << "Calling transform function..." << endl;
    //END DEBUG

    //if (params.timing_flag) start = clock();
    //check if this should be called in the sampler, I don't think it should be.
    double temp;

    if (params.model_case == 2 || params.model_case == 3)
    {
        //delta_o_high discretization
        if (vars[2] < 0.05) vars[2] = 0;
        else if (vars[2] < 0.15) vars[2] = 0.1;
        else if (vars[2] < 0.25) vars[2] = 0.2;
        else if (vars[2] < 0.35) vars[2] = 0.3;
        else if (vars[2] < 0.45) vars[2] = 0.4;
        else if (vars[2] < 0.55) vars[2] = 0.5;
        else if (vars[2] < 0.65) vars[2] = 0.6;
        else if (vars[2] < 0.75) vars[2] = 0.7;
        else if (vars[2] < 0.85) vars[2] = 0.8;
        else if (vars[2] < 0.95) vars[2] = 0.9;
        else vars[2] = 1.0;

        //xi discretization
        if (vars[3] < 0.125) vars[3] = 0.10;
        else if (vars[3] < 0.175) vars[3] = 0.15;
        else if (vars[3] < 0.225) vars[3] = 0.20;
        else if (vars[3] < 0.275) vars[3] = 0.25;
        else if (vars[3] < 0.325) vars[3] = 0.30;
        else if (vars[3] < 0.375) vars[3] = 0.35;
        else vars[3] = 0.40;

        //alpha/beta discretization
        temp = vars[4];
        vars[4] = rounding(temp, 2.0);
        temp = vars[5];
        vars[5] = rounding(temp, 2.0);

        //alpha2 / beta2
        if (vars[4] + vars[5] > 3.0) vars[5] = 3.0 - vars[4]; //transform

        //alpha / beta
        if (params.model_case == 3)
        {
            temp = vars[6];
            vars[6] = rounding(temp, 2.0);
            temp = vars[7];
            vars[7] = rounding(temp, 2.0);
            if (vars[6] + vars[7] > 3.0) vars[7] = 3.0 - vars[6];
        }
    }
    else if (params.model_case == 4 || params.model_case == 5)
    {
        temp = vars[2];
        vars[2] = rounding(temp, 2.0);
        if (params.model_case == 5)
        {
            temp = vars[3];
            vars[3] = rounding(temp,2.0);
        }
    }
    else if (params.model_case == 6)
    {
        temp = vars[2];
        vars[2] = rounding(temp, 2.0);
        temp = vars[3];
        vars[3] = rounding(temp, 2.0);
        temp = vars[4];
        vars[4] = rounding(temp, 2.0);
        temp = vars[5];
        vars[5] = rounding(temp, 2.0);

        //alpha2 / beta2
        if (vars[2] + vars[3] > 3.0) vars[3] = 3.0 - vars[2];

        if (vars[4] + vars[5] > 3.0) vars[5] = 3.0 - vars[4];
    }
    //if (params.timing_flag)
    //{
    //  end = clock() - start;
    //  time_stream << end << endl;
    //}

    return;
}

void Input::calc_LRGV(double* vars, double* objs, double* consts, string calc_param)
{
    //new wrapper for object-oriented call.
    //can later be integrated into problemdef.cpp...
    simulator.calc_LRGV(vars, objs, consts, calc_param, this);
}

//void calc_LRGV(Simulation &sim, Individual *ind, char *calc_param)
//{
//  //new wrapper for object-oriented call.
//  //can later be integrated into problemdef.cpp...
//  sim.calc_LRGV(ind, calc_param);
//  //simulator.calc_LRGV(ind, calc_param);
//}

void Input::finalize_LRGV()
{
    /* Deallocate memory */
    //note futures, monthly_tracker, and samples were statically
    //allocated to 12 members each.
    zap(annual_tracker.annual_demand);
    zap(annual_tracker.annual_nlpl);
    zap(annual_tracker.annual_NW);
    zap(annual_tracker.early_NW);
    zap(annual_tracker.end_of_year_water);
    zap(annual_tracker.late_demand);
    zap(annual_tracker.late_NW);
    zap(annual_tracker.option_tracker);
    zap(annual_tracker.Pl5);
    zap(annual_tracker.portfolio_cost);
    zap(annual_tracker.ToLeasePrice);

    for (int month_it = 0; month_it < 12; month_it++)
    {

        zap(futures[month_it].demand_mean);
        zap(futures[month_it].demand_std_dev);
        zap(futures[month_it].ENr);
        zap(samples[month_it].demand);
        zap(samples[month_it].inf);
        zap(samples[month_it].lease_high_res);
        zap(samples[month_it].lease_low_res);
        zap(samples[month_it].los);
        zap(samples[month_it].res_var);
    }

    int NumberSims = params.NumberSims;
    zap(sims_years_tracker.early_demand, NumberSims);
    zap(sims_years_tracker.early_NW, NumberSims);
    zap(sims_years_tracker.end_of_yr_water_tracker, NumberSims);
    zap(sims_years_tracker.ex_option_cost_list, NumberSims);
    zap(sims_years_tracker.late_demand, NumberSims);
    zap(sims_years_tracker.late_NW, NumberSims);
    zap(sims_years_tracker.Pl5, NumberSims);
    zap(sims_years_tracker.ToLeasePrice, NumberSims);
    zap(sims_years_tracker.total_Nx_tracker, NumberSims);
    zap(sims_years_tracker.total_Po_list, NumberSims);
    zap(sims_years_tracker.yearly_exercised_options, NumberSims);
    zap(sims_years_tracker.yearly_purchased_leases, NumberSims);
    zap(sims_years_tracker.No, NumberSims);

    for (int month_it=0; month_it<12; month_it++)
    {
        zap(monthly_tracker[month_it].inflows, NumberSims);
        zap(monthly_tracker[month_it].lease_high_res, NumberSims);
        zap(monthly_tracker[month_it].lease_low_res, NumberSims);
        zap(monthly_tracker[month_it].losses, NumberSims);
        zap(monthly_tracker[month_it].random_monthly_demand, NumberSims);
        zap(monthly_tracker[month_it].res_level_tracker, NumberSims);
        zap(monthly_tracker[month_it].res_threshold_tracker, NumberSims);
        zap(monthly_tracker[month_it].res_var, NumberSims);
        zap(monthly_tracker[month_it].total_cfailures, NumberSims);
        zap(monthly_tracker[month_it].total_failures, NumberSims);
        zap(monthly_tracker[month_it].total_monthly_leases, NumberSims);
        zap(monthly_tracker[month_it].yearly_lease_cost, NumberSims);
        zap(monthly_tracker[month_it].dropped_transfer_tracker, NumberSims);
        zap(monthly_tracker[month_it].av_water, NumberSims);
        zap(monthly_tracker[month_it].Nr, NumberSims);
    }

    zap(params.initrights_vec);

    if(params.timing_flag) time_stream.close();
    if(params.results_flag) results_stream.close();
    if(params.rng_flag) rng_stream.close();
    if(params.out_flag) out_stream.close();
    if(params.monthly_flag) monthly_stream.close();

    return;
}

void Input::randsample (const hydro_structure& set, sampled_data& sample, int NumberYears)
{
    /* Usage Notes:

    - hydro structure contains, NW, los, inf, res_var

    - this function takes one structure (not an array of structures)
    and samples for every data type within that structure. See function call
    from outside to see how months/years/sims are handled.

    -The randsample function is overloaded for each different type of
    historical data.

    -The random sampling is done as follows: rnd returns an integer value
    from low to high, where the usage is rnd(low,high). That integer is then used
    to index in to a set of data. sample is the actual sampled value used (the output)
    set is the set of historical values from which we sample (the input)

    -Roulette Weighted Sampling artifically "lengthens" the input dataset so that
    some samples become more likely than others. The rationale is the same, though.*/

    if (params.roulette_flag)
    {
        for (int year_it = 0; year_it < NumberYears; year_it ++)
        {
            sample.inf[year_it] =       roulette_draw (set.inf, set.inf_cdf);
            sample.los[year_it] =       roulette_draw (set.los, set.los_cdf);
            sample.res_var[year_it] =   roulette_draw (set.res_var, set.los_cdf);
        }
    }
    else
    {
        for (int year_it = 0; year_it < NumberYears; year_it++)
        {
            int random_draw;
            random_draw = rnd(0,32);
            sample.inf[year_it] = set.inf[random_draw];

            random_draw = rnd(0,32);
            sample.los[year_it] = set.los[random_draw];

            //NW sampling is not needed, since NW is only used
            //to calculate means...
            //random_draw = rnd(0,32);
            //sample.NW[year_it] = set.NW[random_draw];

            random_draw = rnd(0,30);
            sample.res_var[year_it] = set.res_var[random_draw];
        }
    }
    return;
}

void Input::randsample (const lease_structure& set, sampled_data& sample, int NumberYears)
{
    // lease structure contains lease prices for high and low reservoir levels
    if (params.roulette_flag)
    {
        for (int year_it = 0; year_it<NumberYears; year_it++)
        {
            sample.lease_high_res[year_it] =    roulette_draw (set.high_res, set.high_res_cdf);
            sample.lease_low_res[year_it] =     roulette_draw (set.low_res, set.low_res_cdf);
        }
    }
    else
    {
        for (int year_it = 0; year_it<NumberYears; year_it++)
        {
            int random_draw;
            random_draw = rnd(0, (int) set.high_res.size()-1);
            sample.lease_high_res[year_it] = set.high_res[random_draw];

            random_draw = rnd(0, (int) set.low_res.size()-1);
            sample.lease_low_res[year_it] = set.low_res[random_draw];
        }
    }

    return;

}

void Input::randsample (const demand_structure& set, sampled_data& sample, int NumberYears, double DemGrowthFactor)
{
    //NOTE: Demand is automatically scaled to demand growth here in the random sample.
    if (params.roulette_flag)
    {
        for (int year_it = 0; year_it<NumberYears; year_it++)
        {
            sample.demand[year_it] =
                roulette_draw (set.demand, set.demand_cdf)*pow((1+DemGrowthFactor), (double) year_it);
        }
    }
    else
    {
        for (int year_it = 0; year_it<NumberYears; year_it++)
        {
            int random_draw;
            random_draw = rnd(0, (int) set.demand.size()-1);
            sample.demand[year_it] = set.demand[random_draw]*pow((1+DemGrowthFactor), (double) year_it);
        }
    }
    return;

}

double Input::roulette_draw(vector <double> dataset, int *cdf)
{
    int success_flag = 0;
    int random_draw = rnd(1, cdf[dataset.size()-1]);

    for (int j = 0; j < dataset.size()-1; j++)
    {
        if ( (random_draw >= cdf[j]) && (random_draw < cdf[j+1]) )
        {
            success_flag = 1;
            return dataset[j];
            break;
        }
    }
    if (random_draw == cdf[dataset.size()-1])
    {
        success_flag = 1;
        return dataset[dataset.size()-1];
    }

    if (success_flag == 0)
    {
        winexit("Interpolation failed to find a value in the CDF.");
    }
}

int Input::init_roulette_calcs(string mode, vector <double> &input_data, int *(&output_cdf), int highclassix, int weight)
{
    int inputlength = input_data.size();
    //first sort data
    if (mode == "ascending")
    {
        sort(input_data.begin(), input_data.end());
    }
    else if (mode == "descending")
    {
        sort(input_data.begin(), input_data.end(), std::greater<double>());
    }
    else
    {
        winexit("Invalid mode passed in to init_roulette_calcs!");
    }


    //get ready to store cdf
    output_cdf = new int [inputlength];

    int k = 1;
    for (int i = 0; i < inputlength; i++)
    {
        output_cdf[i] =                     k;
        if (i+1 < highclassix)
            k++;
        else
            k += weight;
    }
    return output_cdf[inputlength-1];
}

void Input::init_roulette()
{
    int inputlength, highclassix;
    double gamma = 0.25; //hard-coded for now
    int weight;

    //in the following choose:
    //ascending: if you want to focus on the HIGH part of the distribution
    //descending: if you want to focus on the LOW part of the distribution

    //inf
    inputlength = hydro_sets[0].inf.size();
    weight = params.inf_weight;
    highclassix = inputlength - (int) floor( gamma*(inputlength) );

    for (int month_it = 0; month_it < 12; month_it++)
    {
        params.inf_cdf_max =
            init_roulette_calcs("descending", hydro_sets[month_it].inf, hydro_sets[month_it].inf_cdf, highclassix, weight);
    }

    //los
    inputlength = hydro_sets[0].los.size();
    weight = params.los_weight;
    highclassix = inputlength - (int) floor( gamma*(inputlength) );

    for (int month_it = 0; month_it < 12; month_it++)
    {
        params.los_cdf_max =
            init_roulette_calcs("ascending", hydro_sets[month_it].los, hydro_sets[month_it].los_cdf, highclassix, weight);
    }

    //res_var
    inputlength = hydro_sets[0].res_var.size();
    weight = params.res_var_weight;
    highclassix = inputlength - (int) floor( gamma*(inputlength) );

    for (int month_it = 0; month_it < 12; month_it++)
    {
        params.res_var_cdf_max =
            init_roulette_calcs("descending", hydro_sets[month_it].res_var, hydro_sets[month_it].res_var_cdf, highclassix, weight);
    }

    //lease low/high
    weight = params.lease_weight;
    //inputlength and weight are month-specific since we have variable-length lists

    for (int month_it = 0; month_it < 12; month_it++)
    {
        //high
        inputlength = lease_sets[month_it].high_res.size();
        highclassix = inputlength - (int) floor( gamma*(inputlength) );
        params.lease_high_cdf_max =
            init_roulette_calcs("ascending", lease_sets[month_it].high_res, lease_sets[month_it].high_res_cdf, highclassix, weight);

        //low
        inputlength = lease_sets[month_it].low_res.size();
        highclassix = inputlength - (int) floor( gamma*(inputlength) );
        params.lease_low_cdf_max =
            init_roulette_calcs("ascending", lease_sets[month_it].low_res, lease_sets[month_it].low_res_cdf, highclassix, weight);
    }

    //demand
    //note: demand growth happens later
    //also note that inputlength and weight are insite the loop since we have variable-length lists
    weight = params.demand_weight; //fixed 8.5.2011
    for (int month_it = 0; month_it < 12; month_it++)
    {
        inputlength = demand_sets[month_it].demand.size();
        highclassix = inputlength - (int) floor( gamma*(inputlength) );
        params.demand_cdf_max =
            init_roulette_calcs("ascending", demand_sets[month_it].demand, demand_sets[month_it].demand_cdf, highclassix, weight);
    }

    return;
}

void Input::finalize_roulette()
{
    for (int month_it = 0; month_it < 12; month_it++)
    {
        zap(hydro_sets[month_it].inf_cdf);
        zap(hydro_sets[month_it].los_cdf);
        zap(hydro_sets[month_it].res_var_cdf);
        zap(lease_sets[month_it].high_res_cdf);
        zap(lease_sets[month_it].low_res_cdf);
        zap(demand_sets[month_it].demand_cdf);
    }
}

void Input::global_trackers_allocation(int &initial_call)
{
    int NumberYears = params.NumberYears;
    int NumberSims = params.NumberSims;

    if (initial_call)
    {
        /* Begin memory allocation to pointers in global structures */

        //annual_tracker yearly lists...
        //these are GLOBAL, YEARLY lists, one entry per year
        //(not stored across different iterations... a new set
        //of values is recorded in annual_tracker at every iteration)

        //annual_tracker.future_Nrt = new double[NumberYears];//create an array of future permanent rights values
        //annual_tracker.No = new double[NumberYears];
        annual_tracker.annual_demand = new double[NumberYears];
        annual_tracker.annual_nlpl = new double[NumberYears];
        annual_tracker.annual_NW = new double[NumberYears];
        annual_tracker.early_NW = new double[NumberYears];
        annual_tracker.end_of_year_water = new double[NumberYears];
        annual_tracker.late_demand = new double[NumberYears];
        annual_tracker.late_NW = new double[NumberYears];
        annual_tracker.option_tracker = new double[NumberYears];
        annual_tracker.Pl5 = new double[NumberYears];
        annual_tracker.portfolio_cost = new double[NumberYears];
        annual_tracker.ToLeasePrice = new double[NumberYears];
        //annual_tracker.Nx_tracker = new double[NumberYears];
        //annual_tracker.Po_list = new double[NumberYears];

        //Allocate futures array: demand, ENr
        //Needed since demand_mean, demand_std_dev, ENr are declared as pointers in the struct
        //added samples stuff in here
        for (int month_it = 0; month_it < 12; month_it++)
        {
            //futures structure
            futures[month_it].demand_mean = new double [NumberYears];
            futures[month_it].demand_std_dev = new double [NumberYears];
            futures[month_it].ENr = new double [NumberYears];
            //samples structure
            //if (!params.sync_flag)
            //{
                //these allocations are done earlier if you are doing
                //"synchronous" sampling -- see init_LRGV()

                samples[month_it].demand = new double [NumberYears];
                samples[month_it].inf = new double [NumberYears];
                samples[month_it].lease_high_res = new double [NumberYears];
                samples[month_it].lease_low_res = new double [NumberYears];
                samples[month_it].los = new double [NumberYears];
                //samples[month_it].NW = new double [NumberYears];
                samples[month_it].res_var = new double [NumberYears];
            //}
        }

        //allocate sims_years lists, that is, there's a value stored at
        //every year, also for every simulation.  These are GLOBAL lists
        //which cycle over every YEAR and every SIMULATION (but not every month).
        //each allocation here is for a multi-dimensional array of sims x years

        //sims_years_allocate sets each array member to zero.

        general_2d_allocate(sims_years_tracker.early_demand, NumberSims, NumberYears);
        general_2d_allocate(sims_years_tracker.early_NW, NumberSims, NumberYears);
        general_2d_allocate(sims_years_tracker.end_of_yr_water_tracker, NumberSims, NumberYears);
        general_2d_allocate(sims_years_tracker.ex_option_cost_list, NumberSims, NumberYears);
        general_2d_allocate(sims_years_tracker.late_demand, NumberSims, NumberYears);
        general_2d_allocate(sims_years_tracker.late_NW, NumberSims, NumberYears);
        general_2d_allocate(sims_years_tracker.Pl5, NumberSims, NumberYears);
        general_2d_allocate(sims_years_tracker.ToLeasePrice, NumberSims, NumberYears);
        general_2d_allocate(sims_years_tracker.total_Nx_tracker, NumberSims, NumberYears);
        general_2d_allocate(sims_years_tracker.total_Po_list, NumberSims, NumberYears);
        general_2d_allocate(sims_years_tracker.yearly_exercised_options, NumberSims, NumberYears);
        general_2d_allocate(sims_years_tracker.yearly_purchased_leases, NumberSims, NumberYears);
        general_2d_allocate(sims_years_tracker.No, NumberSims, NumberYears);

        //Now allocate sims_years_monthly lists, where there's a value stored
        //for every MONTH, YEAR, and ITERATION...
        for (int month_it=0; month_it<12; month_it++)
        {
            general_2d_allocate(monthly_tracker[month_it].inflows, NumberSims, NumberYears);
            general_2d_allocate(monthly_tracker[month_it].lease_high_res, NumberSims, NumberYears);
            general_2d_allocate(monthly_tracker[month_it].lease_low_res, NumberSims, NumberYears);
            general_2d_allocate(monthly_tracker[month_it].losses, NumberSims, NumberYears);
            general_2d_allocate(monthly_tracker[month_it].random_monthly_demand, NumberSims, NumberYears);
            general_2d_allocate(monthly_tracker[month_it].res_level_tracker, NumberSims, NumberYears);
            general_2d_allocate(monthly_tracker[month_it].res_threshold_tracker, NumberSims, NumberYears);
            general_2d_allocate(monthly_tracker[month_it].res_var, NumberSims, NumberYears);
            general_2d_allocate(monthly_tracker[month_it].total_cfailures, NumberSims, NumberYears);
            general_2d_allocate(monthly_tracker[month_it].total_failures, NumberSims, NumberYears);
            general_2d_allocate(monthly_tracker[month_it].total_monthly_leases, NumberSims, NumberYears);
            general_2d_allocate(monthly_tracker[month_it].yearly_lease_cost, NumberSims, NumberYears);
            general_2d_allocate(monthly_tracker[month_it].dropped_transfer_tracker, NumberSims, NumberYears);
            //NEW tracker for the C++ translation ...
            general_2d_allocate(monthly_tracker[month_it].av_water, NumberSims, NumberYears);
            general_2d_allocate(monthly_tracker[month_it].Nr, NumberSims, NumberYears);
        }

        initial_call = 0;
    } //end if initial_call == 1

    /* End memory allocation for pointers */
    return;
}

void Input::randomize(double seed)
{
      int j1;
      for(j1=0; j1<=54; j1++)
      {
            oldrand[j1] = 0.0;
      }
      jrand=0;
      warmup_random (seed);
      return;
}

/* Get randomize off and running */
void Input::warmup_random (double seed)
{
      int j1, ii;
      double new_random, prev_random;
      oldrand[54] = seed;
      new_random = 0.000000001;
      prev_random = seed;
      for(j1=1; j1<=54; j1++)
      {
            ii = (21*j1)%54;
            oldrand[ii] = new_random;
            new_random = prev_random-new_random;
            if(new_random<0.0)
            {
                  new_random += 1.0;
            }
            prev_random = oldrand[ii];
      }
      advance_random ();
      advance_random ();
      advance_random ();
      jrand = 0;
      return;
}

/* Create next batch of 55 random numbers */
void Input::advance_random ()
{
      int j1;
      double new_random;
      for(j1=0; j1<24; j1++)
      {
            new_random = oldrand[j1]-oldrand[j1+31];
            if(new_random<0.0)
            {
                  new_random = new_random+1.0;
            }
            oldrand[j1] = new_random;
      }
      for(j1=24; j1<55; j1++)
      {
            new_random = oldrand[j1]-oldrand[j1-24];
            if(new_random<0.0)
            {
                  new_random = new_random+1.0;
            }
            oldrand[j1] = new_random;
      }
}

/* Fetch a single random number between 0.0 and 1.0 */
double Input::randomperc()
{
      jrand++;
      if(jrand>=55)
      {
            jrand = 1;
            advance_random();
      }
      return((double)oldrand[jrand]);
}

/* Fetch a single random integer between low and high including the bounds */
int Input::rnd (int low, int high)
{
    int res;
    if (low >= high)
    {
        res = low;
    }
    else
    {
        res = low + (randomperc()*(high-low+1));
        if (res > high)
        {
            res = high;
        }
    }
    return (res);
}

/* Fetch a single random real number between low and high including the bounds */
double Input::rndreal (double low, double high)
{
    return (low + (high-low)*randomperc());
}

void Input::init_seeds()
{
    //50 Random seeds to select from
    seed[1] = 0.842;
    seed[2] = 0.194;
    seed[3] = 0.016;
    seed[4] = 0.936;
    seed[5] = 0.904;
    seed[6] = 0.830;
    seed[7] = 0.439;
    seed[8] = 0.319;
    seed[9] = 0.635;
    seed[10] = 0.959;
    seed[11]= 0.652;
    seed[12]= 0.291;
    seed[13]= 0.668;
    seed[14]= 0.716;
    seed[15]= 0.601;
    seed[16]= 0.424;
    seed[17]= 0.808;
    seed[18]= 0.232;
    seed[19]= 0.275;
    seed[20]= 0.057;
    seed[21]= 0.612;
    seed[22]= 0.305;
    seed[23]= 0.095;
    seed[24]= 0.101;
    seed[25]= 0.251;
    seed[26]= 0.780;
    seed[27]= 0.198;
    seed[28]= 0.522;
    seed[29]= 0.282;
    seed[30]= 0.752;
    seed[31]= 0.376;
    seed[32]= 0.963;
    seed[33]= 0.416;
    seed[34]= 0.048;
    seed[35]= 0.318;
    seed[36]= 0.580;
    seed[37]= 0.963;
    seed[38]= 0.035;
    seed[39]= 0.646;
    seed[40]= 0.543;
    seed[41]= 0.901;
    seed[42]= 0.504;
    seed[43]= 0.306;
    seed[44]= 0.960;
    seed[45]= 0.607;
    seed[46]= 0.657;
    seed[47]= 0.144;
    seed[48]= 0.021;
    seed[49]= 0.319;
    seed[50]= 0.263;
    seed[51]= 0.213;
}


