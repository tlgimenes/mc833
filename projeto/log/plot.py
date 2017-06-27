################################################################################
##
# @file plot.py
# @date 2017-06-27
# @author Tiago Lobato Gimenes    (tlgimenes@gmail.com)
#
# @copyright Tiago Lobato Gimenes 2016. All rights reserved.
# 
# @section DESCRIPTION
#
################################################################################

import os, sys

import numpy as np
from scipy.interpolate import spline

import matplotlib.pyplot as plt

################################################################################

def extract(path, dtype):
    data = []

    for filename in os.listdir(path):
        if filename.find(dtype) > -1:
            ff = open(os.path.join(path, filename));
            line = ff.readline()
            while line:
                data.append(int(line));
                line = ff.readline()

            ff.close();

    return np.average(data), np.std(data);

################################################################################

curves = {}

for c in os.listdir('.'):
    if os.path.isdir(c):
        curves[c] = {}
        curves[c]['x'] = np.sort(np.asarray([x for x in os.listdir(c) if os.path.isdir(os.path.join(c,x))], dtype=np.int32))
        curves[c]['security'] = []
        curves[c]['security_std'] = []
        curves[c]['entertainment'] = []
        curves[c]['entertainment_std'] = []
        curves[c]['comfort'] = []
        curves[c]['comfort_std'] = []
        for x in curves[c]['x']:
            median, std = extract(os.path.join(c, str(x)), 'security')
            curves[c]['security'].append(median) 
            curves[c]['security_std'].append(std)
            median, std = extract(os.path.join(c, str(x)), 'entertainment')
            curves[c]['entertainment'].append(median) 
            curves[c]['entertainment_std'].append(std)
            median, std = extract(os.path.join(c, str(x)), 'comfort')
            curves[c]['comfort'].append(median) 
            curves[c]['comfort_std'].append(std)

################################################################################

security      = plt.figure().add_subplot(111)
entertainment = plt.figure().add_subplot(111)
comfort       = plt.figure().add_subplot(111)

security.      set_title('Security')
entertainment. set_title('Entertainment')
comfort.       set_title("Comfort")

for curvename, curve in curves.items():
    xnew = np.linspace(np.min(curve['x']), np.max(curve['x']), 100)
    security.      plot(xnew, spline(curve['x'], curve['security'], xnew), label=curvename)
    entertainment. plot(xnew, spline(curve['x'], curve['entertainment'], xnew), label=curvename)
    comfort.       plot(xnew, spline(curve['x'], curve['comfort'], xnew), label=curvename)
    #security.      errorbar(curve['x'], curve['security'], yerr=curve['security_std'], label=curvename)
    #entertainment. errorbar(curve['x'], curve['entertainment'], yerr=curve['entertainment_std'], label=curvename)
    #comfort.       plot(curve['x'], curve['comfort'], label=curvename)

security.legend()
entertainment.legend()
comfort.legend()

plt.show();

################################################################################
