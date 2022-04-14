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


#ifndef _global_h
#define _global_h

#define LRGV

//This is a stripped-down version of the header file which is used for our
//testing program.

#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <float.h>
#include <ctype.h>
#include <time.h>
#include <iostream>
#include <string>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <assert.h>

//new to this version:
#include <unistd.h>
#include "moeaframework.h"

using namespace std;

struct Individual {
  double  *xreal;
  double  *obj;
  double  *constr;
};

#define START_TIME clock()
#define GET_TIME(start) ((clock()-start)/(double)CLOCKS_PER_SEC)

//void param_read(char *input_filename, char *output_filename, double *xreal, int model_case);
//void lrgv_sampler(string input_filename, string ranges_filename, int num_sol, char *function_calcparam);
//void usage(int argc, char **argv);

#include "lrgv.h"
#include "Simulation.h"
//void calc_LRGV(Simulation &sim, Individual *ind, char *calc_param);
#endif

class Input {

public:

Input();

const vector<string> obj_avail;
const vector<string> constr_avail;

//const vector<string> obj_avail;
//const vector<string> constr_avail;

year_tracker_structure annual_tracker;
sims_years_structure sims_years_tracker;
monthly_tracker_structure monthly_tracker[12];

hydro_structure hydro_sets[12];
lease_structure lease_sets[12];
demand_structure demand_sets[12];
param_structure params;

sampled_data samples[12];
future_structure futures[12];
strategy_structure strategy;

super_structure super;
timer_structure timers;
drought_structure drought;

global_reporting_structure g;

ofstream time_stream;
ofstream results_stream;
ofstream rng_stream;
ofstream out_stream;
ofstream monthly_stream;

Simulation simulator;

double start, endtime;

int sampler_count;

double seed[52];
double oldrand[55];
int jrand;

void write_results_header(filenames_structure &filenames, string calc_param);
void write_monthly_header(filenames_structure &filenames);
void write_monthly_output();
void hydro_read(string hydro_filename);
void lease_read(string lease_filename);
void demand_read(string demand_filename);
void control_read(filenames_structure &filenames);
void hist_read(filenames_structure &filenames);
void general_debug_output (ofstream &out, double **data, int rows, int cols);
void sets_output_rng_text(ofstream &out, double _current_sim);

void transfer_tracker(double *(&TransferTracker), double AvWater, double Demand);

double resilience_calc();
double failvol_calc();
void vulnerability_calc(int local_sims, int local_years);
void single_sync_sampling();
void init_LRGV(char** argv, string init_param);
void transform_LRGV(double* vars);
void calc_LRGV(double* vars, double* objs, double* consts, string calc_param);
void finalize_LRGV();

void randsample (const hydro_structure& set, sampled_data& sample, int NumberYears);
void randsample (const lease_structure& set, sampled_data& sample, int NumberYears);
void randsample (const demand_structure& set, sampled_data& sample, int NumberYears, double DemGrowthFactor);

double roulette_draw(vector <double> dataset, int *cdf);
int init_roulette_calcs(string mode, vector <double> &input_data, int *(&output_cdf), int highclassix, int weight);
void init_roulette();
void finalize_roulette();

void global_trackers_allocation(int &initial_call);


/* Function declarations for the random number generator */
void randomize(double seed);
void warmup_random (double seed);
void advance_random ();
double randomperc();
int rnd (int low, int high);
double rndreal (double low, double high);
void init_seeds();

};


