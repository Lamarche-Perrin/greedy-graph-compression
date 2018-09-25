#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Feb  9 14:27:59 2018

@author: complex
"""
import os
import json
import numpy as np
import pandas as pd

import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

from itertools import product

def load_last_experiment(k):
    files = os.listdir()
    csvsop = [c for c in files if c.endswith("optim_data.csv")]   #optim data
    txts = [c for c in files if c.endswith(".txt")]     #metadata
    jsons = [c for c in files if c.endswith(".json")]   #partition
    csvsmat = [c for c in files if c.endswith("matrix_data.csv")]   #optim data
    csvsop.sort()
    txts.sort()
    jsons.sort()
    csvsmat.sort()
    
    
    lastdata = csvsop[-1-k]
    lastmeta = txts[-1-k]
    lastpart = jsons[-1-k]
    lastmat  = csvsmat[-1-k]
    
    print("last data : ", lastdata)
    print("last metadata : ", lastmeta)
    print("last json : ", lastpart)
    print("last data mat : ", lastmat)
    return lastdata, lastmeta, lastpart, lastmat

def load_experiment(name):
    files = os.listdir(name)
    data = name+[c for c in files if c.endswith("optim_data.csv")][0]   #optim data
    meta = name+[c for c in files if c.endswith(".txt")][0]
    part = name+[c for c in files if c.endswith(".json")][0]
    mat  = name+[c for c in files if c.endswith("matrix_data.csv")][0]
    
    print("data : ", data)
    print("metadata : ", meta)
    print("json : ", part)
    return data, meta, part, mat

def load_partition(part):
    return json.load(open(part))

def load_data(data):
    df = pd.read_csv(data, sep = ' ', index_col = False)
    return df.loc[:, ~df.columns.str.contains('^Unnamed')]

def read_meta(meta):
    with open(meta) as openfile:
        for line in openfile:
            for part in line.split():
                if "Dimensions" in part:
                    D = int(line.split()[-1])
                if "N" in part:
                    N = [int(i) for i in line.split()[2::]]
                if "lambda" in part:
                    lamb = float(line.split()[-1])

    return D, N, lamb


def plot_all(data, meta, part):
    df = pd.read_csv(data, sep = ' ', index_col = False)
    p = json.load(open(part))
    D, N, lamb = read_meta(meta)

    niter = range(len(df.index))

#### ANALYZE
    check_non_zero = 0
    sizes = []
    dkls = []
    non_zero = []
    color_non_zero = []
    weight = []
    
    sizes_nz = []
    dkls_nz = []
    weight_nz = []
    non_zero_nz = []
    
    for i in  p['R'].keys():
        sizes.append(p['R'][i]['size'])
        dkls.append(p['R'][i]['dkl'])
        non_zero.append(p['R'][i]['non zero'])
        weight.append(p['R'][i]['weight'])
        
        if(p['R'][i]['non zero'] != 0):
            #Rectangle is not entirely empty
            color_non_zero.append('r')
            sizes_nz.append(p['R'][i]['size'])
            dkls_nz.append(p['R'][i]['dkl'])
            weight_nz.append(p['R'][i]['weight'])
            non_zero_nz.append(p['R'][i]['non zero'])
        else:
            #Rectangle is empty
            color_non_zero.append('b')
        
        check_non_zero += p['R'][i]['non zero']
    print('check non zero : ', check_non_zero, '  E : ', p['final']['E'])

    density = [i/j for i,j in zip(non_zero, sizes)]
    density_nz = [i/j for i,j in zip(non_zero_nz, sizes_nz)]
    activity = [i/j for i,j in zip(weight, sizes)]
    activity_nz = [i/j for i,j in zip(weight_nz, sizes_nz)]
    
    
    
    #Plot parameters    
    size1 = 5   #Size of cross
    size2 = 1   #Size of dot
    linew = 2   #width of lines in hist
    nbins1 = 10  #number of bin for hist
    nbins2 = 50  #number of bin for hist
    alpha = 1.0
    
    
    plt.figure(0)
    plt.suptitle("*** - lambda = " + str(lamb) + "  final size : " + str(df['Rsize'].values[-1]))

#Objectif and penalty
    ax0 = plt.subplot(221)
    plt.title('Objectif')
    lns0 = ax0.plot(df['cost'].values, 'kx', ms = size1, label = 'cost')
    ax0.set_ylabel('cost')
    ax0.ticklabel_format(axis='y', style='sci', scilimits=(0,0))

    ax1 = ax0.twinx()
    lns1 = ax1.plot(df['ldkl'].values, 'r.', ms = size2, alpha = alpha, label = 'ldkl')
    lns2 = ax1.plot(df['penalty'].values, 'b.', ms = size2, alpha = alpha, label = 'penalty')
    ax1.set_ylabel('penalty and ldkl')
    ax1.ticklabel_format(axis='y', style='sci', scilimits=(0,0))
    
    lns = lns0+lns1+lns2
    labs = [l.get_label() for l in lns]
    ax0.legend(lns, labs, loc=0)
        
#Deltas
    ax2 = plt.subplot(222)
    plt.title('delta cost/ldkl/penalty')
    ax2.plot(df['d_cost'].values, 'kx', ms = size1, label = 'delta objectif')
    ax2.plot(df['d_ldkl'].values, 'r.', ms = size2, alpha = alpha, label = 'delta dkl')
    ax2.plot(df['d_penalty'].values, 'b.', ms = size2, alpha = alpha, label = 'delta penalty')
    ax2.legend()
    
#Size vs DKL
    plt.subplot(223)
    plt.title('Size vs dkl for rectangles')
    plt.scatter(sizes, dkls, marker = 'x', s = size1, alpha = alpha, c = color_non_zero)
    plt.xlabel('size of rectangles')
    plt.ylabel('dkl (not normalized)')
    red_patch = mpatches.Patch(color='b', label='Empty rectangle')
    blue_patch = mpatches.Patch(color='r', label='Non-empty rectangle')
    plt.legend(handles=[red_patch, blue_patch])
    
    #Actions and Temperature    
    ax5 = plt.subplot(224)
    plt.title('Actions')
    c = ['b' if i == 1 else 'r' for i in df['accepted'].values]
    red_patch = mpatches.Patch(color='b', label='Action accepted')
    blue_patch = mpatches.Patch(color='r', label='Action refused')
    ax5.scatter(niter, df['d_Rsize'].values, marker = 'x', s = size2, alpha = alpha, c = c)
    ax5.set_ylabel('Action size')

    plt.legend(handles=[red_patch, blue_patch])        
    
    

    
    plt.figure(1)
    plt.suptitle("A new title - lambda = " + str(lamb) + "  final size : " + str(df['Rsize'].values[-1]))
    

#Size Distrib
    plt.subplot(221)

        
    plt.hist(sizes, np.logspace(np.log10(1),np.log10(max(sizes)), nbins2), log = True, histtype='bar', ls = 'solid', lw = linew, label = 'All')
    plt.hist(sizes_nz, np.logspace(np.log10(1),np.log10(max(sizes)), nbins2), log = True, histtype='step', ls = 'solid', lw = linew, label = 'non empty')
    plt.gca().set_xscale("log")
    plt.yscale('log', nonposy='clip')
    plt.xscale('log', nonposx='clip')
    plt.xlabel('Rectangle size')
    plt.ylabel('Number of rectangles')
    plt.title('Size of Rectangles')
    plt.legend()
    
#DKL histogram
    plt.subplot(222)
    plt.title('dkl histogram of non empty rectangles')
    plt.hist(dkls, nbins1, histtype='bar', ls = 'solid', lw = linew, label = 'All')
    plt.hist(dkls_nz, nbins1, histtype='step', ls = 'solid', lw = linew, label = 'non empty')
    plt.xlabel('dkl')
    plt.ylabel('Number of rectangles')
    plt.legend()
    
#Density histogram
    plt.subplot(223)
    plt.title('density historgram of non empty rectangles')
    plt.hist(density, nbins1, histtype='bar', ls = 'solid', lw = linew, label = 'All')
    plt.hist(density_nz, nbins1, histtype='step', ls = 'solid', lw = linew, label = 'non empty')
    plt.xlabel('density')
    plt.ylabel('Number of rectangles')
    plt.legend()
    
#Activity histogram
    plt.subplot(224)
    plt.title('Activiy histogram of non empty rectangles')
    plt.hist(activity, nbins1, histtype='bar', ls = 'solid', lw = linew, label = 'All')
    plt.hist(activity_nz, nbins1, histtype='step', ls = 'solid', lw = linew, label = 'non empty')
    plt.xlabel('activity')
    plt.ylabel('Number of rectangles')
    plt.legend()
    
#Jacquard index distribution ?

    
    
    plt.show(block = False)
        
        
        
    
def plot_selection(df, selection):
    niter = range(len(df.index))

    for n in range(len(selection)):
        plt.figure(n)
        plt.title(selection[n])
        plt.xlabel("niter")
        plt.ylabel(selection[n])
        plt.plot(niter, df[selection[n]].values)
        
    plt.show()

def plot_objectif(data, meta):
    print("loading data")
    df = pd.read_csv(data, sep = ' ', usecols = ['cost', 'ldkl', 'penalty'], index_col = False)
    D, N, lamb = read_meta(meta)
    print("loading data done")
    
    size1 = 5   #Size of cross
    size2 = 1   #Size of dot
    alpha = 1.0
    
#Objectif and penalty
    fig, ax0 = plt.subplots()
    plt.title('Objectif')
    lns0 = ax0.plot(df['cost'].values, 'kx', ms = size1, label = 'cost')
    ax0.set_ylabel('cost')
    ax0.ticklabel_format(axis='y', style='sci', scilimits=(0,0))

    ax1 = ax0.twinx()
    lns1 = ax1.plot(df['ldkl'].values, 'r.', ms = size2, alpha = alpha, label = 'ldkl')
    lns2 = ax1.plot(df['penalty'].values, 'b.', ms = size2, alpha = alpha, label = 'penalty')
    ax1.set_ylabel('penalty and ldkl')
    ax1.ticklabel_format(axis='y', style='sci', scilimits=(0,0))
    
    lns = lns0+lns1+lns2
    labs = [l.get_label() for l in lns]
    ax0.legend(lns, labs, loc=0)
    
    plt.show(block = False)

def print_partition(lastpart, lastmeta, lastmat):
    D, N, lamb = read_meta(lastmeta)
    p = json.load(open(lastpart))
    
    mat = pd.read_csv(lastmat, sep = ' ', index_col = False, header=None,)
    mat = mat.as_matrix()
    nnz = np.nonzero(mat)
    mat[nnz] = 1.0
    
    M_nz = np.zeros((N[0], N[0]))
    k = 1
    
    for rct in range(p['final']['Rsize']):
        for i in product(*p['R'][str(rct)]["geometry"]):
            M_nz[i] = k
        k+=1
    
    
    plt.figure(10)
    plt.suptitle("Partitions")
    
    plt.subplot(121)
    plt.title("Original Matrix")
    plt.imshow(mat, cmap='YlOrRd', interpolation='nearest')
    plt.show(block = False)
    
    plt.subplot(122)
    plt.title("Partition")
    plt.imshow(M_nz, cmap='YlOrRd', interpolation='nearest')
    plt.show(block = False)


def plot_times(data):
    df = pd.read_csv(data, sep = ' ', index_col = False)
    niter = range(len(df.index))

    #Plot parameters    
    size1 = 5   #Size of cross
    alpha = 1.0
    
    plt.figure()
    plt.title("Times")
    plt.scatter(niter, df['tfind'].values, marker = 'x', s = size1, alpha = alpha, label = 'tfind')
    plt.scatter(niter, df['tdelta'].values, marker = 'x', s = size1, alpha = alpha, label = 'tdelta')
    plt.scatter(niter, df['tdone'].values, marker = 'x', s = size1, alpha = alpha, label = 'tdone')
    plt.legend()
   
    plt.show(block = False)


















