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


#include <global.h>

using namespace std;

void usage(int argc, char* argv[], Input* inp)
{
	cerr << "Usage: " << argv[0] << " [OPTIONS]" << endl;
	//UPDATE THIS LIST
	cerr << "-m <mode> \t Mode. Options are std-io and sobol. REQUIRED." << endl;
	cerr << "-b <filename_base> \t The phrase used to name parameter and output files. REQUIRED." << endl;
	cerr << "-c <calculation_type> \t Calculation type.  Options are ten-year, drought-full, combined.  REQUIRED." << endl;
	cerr << "-s <seed> \t Seed. (optional)." << endl;
	cerr << "-r <realizations> \t Number of Monte Carlo realizations.  Optional here, but if you don't give it here, it must be in the control file!" << endl;
	cerr << "-h Help (this screen)." << endl;
	cerr << endl;

	cerr << "Available objectives are: ";
	vector<string>::const_iterator cii;
	for(cii=inp->obj_avail.begin(); cii!=inp->obj_avail.end(); cii++){
	  cerr << *cii << " ";
	}
	cerr << endl;
	cerr << "Available constraints are: ";
	for(cii=inp->constr_avail.begin(); cii!=inp->constr_avail.end(); cii++){
	  cerr << *cii << " ";
	}
	cerr << endl;

	exit(-1);	
	return;
}

class Main {
public:

    //local variables
    string input_filename; //input vectors for sampler, of form "*_parameters.txt"
    string ranges_filename;
    string local_calcparam;
    int num_sol = 0; //number of sampled solutions, read from vector input file
    int opt;
    int RS = 0;
    Input* inp;
    int nvars;
    int nobjs;
    int nconstrs;

    Main(int argc, char **argv): nvars(-1), nobjs(-1), nconstrs(-1) {

        inp = new Input();
        //UPDATES:
        //-Uses getopt with wider array of command line options.
        //-Integrated MOEAframework-compatible code with old Sobol version and MORDM roulette features.
        //-Now the number of random samples can be input on the command line (or in the control file)
        //-No more LRGV_SAMPLER preprocessor definitions, with an intelligent inp->params.mode value that controls how
        //data is handled.
        //-Two new objectives called aggregated cost and reliability.  Basic combination of several ten-year cost and
        //reliability measures with measures from the concurrent drought.


        //TO DO:
        //
        //
        //Add parsing of getopt directly through some sort of function, for "embedded" CBorg-MS.
        //
        //Allow some decision variables to be hard coded and others to change.  This is already available in
        //"sobol mode" but not in std-io, as far as I can see.

        //modes:
        //std-io reads inputs from standard in, outputs to standard out

        //cases:
        //should it be based on the papers? let them make their own case?
        //if so how should the decision variable string be read in?

        inp->params.mode = "unset";
        local_calcparam = "unset";
        inp->params.NumberSims = -1;
        //cout << "local_calcparam is originally " << local_calcparam << "." << endl;

        //while ((opt = getopt(argc, argv, "m:b:c:s::h::")) != -1)
        while ((opt = getopt(argc, argv, "m:b:c:s:r:h")) != -1)
        {
            switch (opt)
            {
                case 'm':   //modeling mode
                    //cout << "We hit the m case and optarg is: " << optarg << "." << endl;
                    inp->params.mode = optarg;
                    //cout << "set mode to: " << inp->params.mode << "." << endl;
                    break;
                case 'b':   //filename base
                    //cout << "We hit the b case and optarg is: " << optarg << "." << endl;
                    inp->params.filename_base = optarg;
                    //cout << "Set inp->params.filename_base to: " << inp->params.filename_base << "." << endl;
                    break;
                case 'c':   //calculation type (ten year, drought, etc)
                    //cout << "We hit the c case and optarg is: " << optarg << "." << endl;
                    local_calcparam = optarg;
                    //cout << "Set local_calcparam to: " << local_calcparam << "." << endl;
                    break;
                case 's':   //random seed
                    //cout << "We hit the s case and optarg is: " << optarg << "." << endl;
                    RS = atoi(optarg);
                    //cout << "set RS to: " << RS << "." << endl;
                    break;
                case 'r':   //realizations
                    inp->params.NumberSims = atoi(optarg);
                    break;
                case 'h':   //help
                    usage(argc, argv, inp);
                    break;
                default:
                    usage(argc, argv, inp);
                    break;
            }
        }

        //check required parameters
        if (inp->params.mode != "std-io" && inp->params.mode != "sobol")
        {
            if (inp->params.mode == "unset")     cerr << "Error! m (mode) is a required parameter. Exiting..." << endl;
            else                        cerr << "Mode is wrong.  You entered: " << inp->params.mode << "." << endl;
            exit(-1);
        }

        if (local_calcparam != "ten-year" && local_calcparam != "drought-full" && local_calcparam != "combined")    //CHECK THIS
        {
            if (local_calcparam == "unset")     cerr << "Error! m (mode) is a required parameter. Exiting..." << endl;
            else                        cerr << "Mode is wrong (either ten-year, drought-full, or combined). You entered: " << inp->params.mode << "." << endl;
            exit(-1);
        }

        //check mode compatibility
        if (local_calcparam == "combined" && inp->params.mode == "sobol")
        {
            cerr << "Error! Sobol is not yet compatible with the combined (10-year and drought) mode.  Sorry!" << endl;
            exit(-1);
        }

        //
        //INITIALIZATION
        //

        //no matter what, initialize random number generator
        //Note: The cout commands are for debugging only.  They may interrupt the flow of the Borg standard/io communication if used!
        //cout << "Init seeds...";
        inp->init_seeds();
        //cout << "OK!" << endl;

        //cout << "Randomize...";
        inp->randomize(inp->seed[RS]);
        //cout << "OK!" << endl;

        //initialize the model
        //cout << "Init_LRGV()...";
        if (local_calcparam == "drought-full" || local_calcparam == "ten-year")
        {
            //only initialize once and pass the parameter in directly
            inp->init_LRGV(argv, local_calcparam);
        }
        else if (local_calcparam == "combined")
        {
            inp->init_LRGV(argv, "ten-year");
            inp->init_LRGV(argv, "drought_noinit");
        }

        //cout << "OK!" << endl;

        //initialize the individual
        //Individual actual_ind;
        //Individual *ind = &actual_ind;
        //double *xreal   = ind->xreal;
        //cout << "Created individual and xreal pointer." << endl;
        //cout << "Obj size is: " << inp->params.obj_names.size() << "!" << endl;

        //Parse the cases.  Cases 1-6 are used for DECISION VARIABLES ONLY.  Objectives and
        //constraints are handled in the control file.

        if (inp->params.obj_names.size() <= 0)   {cerr << "Error! Objectives count from control file is: " << inp->params.obj_names.size() << ". Exiting."; exit(-1);}

        nobjs = int(inp->params.obj_names.size());
        //Fixed a bug, where the program would think there are still constraints even if the constraints
        //flag is turned off in the control file.
        if (inp->params.constr_flag == 1)
        {
            nconstrs = int(inp->params.constr_names.size());
        }
        else
        {
            nconstrs = 0;
            if (inp->params.constr_names.size() != 0)
            {
                //This is an error because there are constraints in the control file, but the constraint flag has been turned off.
                cerr << "Error! Constraints count from control file is: " << inp->params.constr_names.size() << ", but the constraint_flag indicates there are no constraints. Exiting."; exit(-1);
            }
        }

        if (inp->params.model_case == 1)     {nvars = 1;}    //WRR: Case A
        else if (inp->params.model_case == 2)    {nvars = 6;}    //WRR: Case B
        else if (inp->params.model_case == 3)    {nvars = 8;}    //WRR: Case C/D, de Novo Case IV.
        else if (inp->params.model_case == 4)    {nvars = 3;}    //de Novo Case I
        else if (inp->params.model_case == 5)    {nvars = 4;}    //de Novo Case II
        else if (inp->params.model_case == 6)    {nvars = 6;}    //de Novo Case III

        //Need to enable the ability to have some variables 'hard-coded'!

        //allocate memory
        //cout << "Allocating memory...with objs " << nobjs << ", constrs " << nconstrs << ", and vars " << nvars << "." << endl;
        //ind->obj =    new double[nobjs];
        //ind->constr = new double[nconstrs];
        //ind->xreal =  new double[nvars];

    }

    void fitness(double *vars, double *objs, double *consts) {
        //Call a different execution for std-io under the "combined" scenario.  Do this comparison outside the loop to save time.
        //MOEA_Init(nobjs, nconstrs);
        // the flag would have indicated that it was supposed to be used!
        if (inp->params.discretize_flag) inp->transform_LRGV(vars);

        if (local_calcparam == "combined") {
            inp->calc_LRGV(vars, objs, consts, "ten-year");
            inp->calc_LRGV(vars, objs, consts, "drought_noinit");
        }
        else {
            inp->calc_LRGV(vars, objs, consts, local_calcparam);
        }
        //MOEA_Write(objs, consts);
    }
};

extern "C" {

    intptr_t lrgv_C(int argc, char** args) {
        char* argv[argc+1];
        for (int i = 0; i < argc; i++)
            argv[i+1] = args[i];
        argc++;
//        for (int i = 0; i < argc; i++)
//            cout << argv[i] << endl;
        Main* lrgv = new Main(argc, argv);
        return (intptr_t) lrgv;
    }

    void fitness_lrgv_C(intptr_t ilrgv, double* x, double* objs, double* cons) {
        try {
            Main* lrgv = (Main*) ilrgv;
            lrgv->fitness(x, objs, cons);
        } catch (std::exception &e) {
              cout << e.what() << endl;
        }
    }

    void initfit_lrgv_C(int argc, char **args, double *x, double *objs,
        double *cons) {
        try {
            char *argv[argc + 1];
            for (int i = 0; i < argc; i++)
                argv[i + 1] = args[i];
            argc++;
            Main *lrgv = new Main(argc, argv);

            lrgv->fitness(x, objs, cons);
        } catch (std::exception &e) {
            cout << e.what() << endl;
        }
    }
}


int main(int argc, char **argv)
{
    for (int i = 0; i < argc; i++)
        cout << argv[i] << endl;

    Main* m = new Main(argc, argv);
    Input* inp = m->inp;
    double *vars, *objs, *consts;
    cout << "nvars = " << m->nvars << endl;
    cout << "nobjs = " << m->nobjs << endl;
    cout << "nconstrs = " << m->nconstrs << endl;
    vars = new double [m->nvars];
    objs = new double [m->nobjs];
    consts = new double [m->nconstrs];
    while (true) { //(MOEA_Next_solution() == MOEA_SUCCESS) {
        //MOEA_Read_doubles(nvars,vars); //first read the solution
        try {

            for (int i = 0; i < m->nvars; i++)
                vars[i] = ((double) (rand() % 10000)) / 10000.0;
            MOEA_Init(m->nobjs, m->nconstrs);
            m->fitness(vars, objs, consts);
            MOEA_Write(objs, consts);
//            if (inp->params.discretize_flag)
//                inp->transform_LRGV(vars);
//            MOEA_Init(m->nobjs, m->nconstrs);
//            inp->calc_LRGV(vars, objs, consts, m->local_calcparam);
//            MOEA_Write(objs, consts);
        } catch (std::exception &e) {
              cout << e.what() << endl;
        }
        //cerr << "Just calculated" << endl;
    }

    //m->fitness(argc, argv);
    return 0;
}
