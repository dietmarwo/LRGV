# LRGV
The Lower Rio Grande Valley (LRGV) water market simulation + Python optimization API

This fork is done to prepare a new example for https://github.com/dietmarwo/fast-cma-es

Only std-io mode is supported - but is no longer based on stdio but is indended to 
be used via Python like in the code below.

The code is refactored to avoid global variables since these don't work in connection 
with Python Multiprocessing. Using parallel execution about 150 simulations per second
can be performed on a 16 core AMD 5950x utilizing 32 parallel threads. 


from scipy.optimize import Bounds
import ctypes as ct
import numpy as np
import multiprocessing as mp

liblrgv = ct.cdll.LoadLibrary(basepath + 'path/liblrgv.so')  
  
lrgv_C = liblrgv.lrgv_C
lrgv_C.argtypes = [ct.c_int, ct.POINTER(ct.c_char_p)]
lrgv_C.restype = ct.c_long

fitness_lrgv_C = liblrgv.fitness_lrgv_C
fitness_lrgv_C.argtypes = [ct.c_long, ct.POINTER(ct.c_double),
                           ct.POINTER(ct.c_double), ct.POINTER(ct.c_double)]
 
dim = 8
nobj = 4
ncon = 3

lb = np.array([0]*3 + [0.1] + [0]*4)
ub = np.array([1]*3 + [0.4] + [3]*4)

class lrgv(object):
    
    def __init__(self):
        self.bounds = Bounds(lb, ub)      
        self.lrgv_p = 0 
        
    def __call__(self, x):
        y = np.empty(nobj) # C fitness call returns 6 objectives
        c = np.empty(ncon)
        x_p = x.ctypes.data_as(ct.POINTER(ct.c_double))
        y_p = y.ctypes.data_as(ct.POINTER(ct.c_double))  
        c_p = c.ctypes.data_as(ct.POINTER(ct.c_double))  
        try:     
            if self.lrgv_p == 0:
                args = ["-m","std-io","-b","AllDecAll","-c","ten-year"]
                arguments = [bytes(argsi, 'utf-8') for argsi in args]
                argv = (ct.c_char_p * len(arguments))()
                argv[:] = arguments
                argc = len(arguments)
                self.lrgv_p = lrgv_C(argc, argv)
            fitness_lrgv_C(self.lrgv_p, x_p, y_p, c_p)
        except Exception as ex:
            print(str(ex))     
        return np.array(list(y) + list(np.array(c)*-1.0)) # negate constraints

from fcmaes import moretry, modecpp, mode
def main():
        store = mode.store(dim, nobj, 10240)
        fun = mode.wrapper(problem, nobj, store, plot=True)
        modecpp.retry(fun, nobj, ncon, 
                      problem.bounds, num_retries=640, popsize = 512, 
                  max_evaluations = 3000000, nsga_update = False, 
                  logger = logger(), workers=32)

