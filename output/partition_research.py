#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Mar 14 11:58:45 2018

@author: complex
"""
import os
import partition_research_function as F


cwd = os.getcwd()
SO = cwd+"/Saved_Optimization"


#doing = DSsparse1
doing = cwd
F.plot_partitions(doing)
F.plot_compression(doing)

#F.get_cost(doing)
