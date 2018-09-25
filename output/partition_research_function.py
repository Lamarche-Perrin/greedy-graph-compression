#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Mar 14 11:59:14 2018

@author: complex
"""
import os
import re
import json
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from itertools import product


def load_lambdas(folder):
    partition_files = load_partitions_files(folder)
    lambdas = [re.search('lambda=(.*)\_201', c).group(1) for c in partition_files]
    return lambdas

def load_partitions_files(folder):
    files = os.listdir(folder)
    return [folder+"/"+c for c in files if "lambda" in c and c.endswith(".json")]

def load_meta_files(folder):
    files = os.listdir(folder)
    return [folder+"/"+c for c in files if "meta" in c and c.endswith(".txt")]

def plot_partitions(folder):
    M = load_meta_files(folder)
    L = load_lambdas(folder)
    P = load_partitions_files(folder)
    L.sort()
    P.sort()

    
    s = len(L)+1
    nrows = int(np.sqrt(s))
    ncols = int(np.ceil(s/nrows))
    
    fig, ax = plt.subplots(nrows, ncols)
    fig.suptitle('Partitions optimisées en fonction de Lambda via exploration dichotomique')
    ax = ax.ravel()
    
    for f in range(s-1):
        print("Treating lambda = ", L[f])
        print(M[0])
        print(P[f])
        p = json.load(open(P[f]))
        mat, Rsize = make_partition_as_matrix(M[0], P[f])
        ax[f].set_title(r"$\lambda$ = " + str(L[f])[:-2] + ", size : " + str(Rsize) + "\n cost : " + str(p["final"]["cost"]) )
        ax[f].imshow(mat, cmap='YlOrRd', interpolation='nearest')
        ax[f].axes.xaxis.set_ticklabels([])
        ax[f].axes.yaxis.set_ticklabels([])
        print()
#        ax[f].axis('off')
    pb = load_problem(folder)
    f+=1
    ax[f].set_title("Problem : ")
    ax[f].imshow(pb, cmap='YlOrRd', interpolation='nearest')
    if s != nrows*ncols:
        for o in range(f, nrows*ncols):
            ax[o].axis('off')

    plt.show(block = False)
    
def plot_compression(folder):
    P = load_partitions_files(folder)
    L = load_lambdas(folder)
    
    P.sort()
    L.sort()
    
    dkl = []
    size = []
    cost = []

    for f in range(len(P)):
        p = json.load(open(P[f]))
        dkl.append(p["final"]["dkl"])
        size.append(p["final"]["Rsize"])
        cost.append(p["final"]["cost"])
    
    plt.figure(7)
    plt.title("dkl")
    plt.plot(L, dkl)
    
    plt.figure(8)
    plt.title("size")
    plt.plot(L, size)
    
    plt.figure(9)
    plt.title("cost")
    plt.plot(L, cost)
    
    plt.figure(10)
    plt.title("dkl vs size")
    plt.plot(size, dkl)
    plt.show(block = False)

    
def make_partition_as_matrix(meta, part):
    D, N, lamb = read_meta(meta)
    p = json.load(open(part))

    M = np.zeros((N[0], N[0]))
    k = 1
    print("pfinalrsize : ", p['final']['Rsize'])
    for rct in range(p['final']['Rsize']):
        for i in product(*p['R'][str(rct)]["geometry"]):
            M[i] = k
        k += 1
    return M, p['final']['Rsize']

def load_problem(folder):
    files = os.listdir(folder)
    mat_file = [folder+"/"+c for c in files if c.endswith("matrix_data.csv")]   #optim data
    print(mat_file)
    mat = pd.read_csv(mat_file[0], sep = ' ', index_col = False, header=None,)
    return mat.as_matrix()
    

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

def get_cost(folder):
    L = load_lambdas(folder)
    P = load_partitions_files(folder)
    
    P.sort()
    L.sort()
    for i in range(len(L)):
        print(i, "  ", L[i])
    l = float(L[int(input("Choose a lambda : "))])
    
    print("Choose a partition")
    for i in range(len(P)):
        print(i, "  ", P[i])    
    p = int(input("Choose a partition : "))
    
    print("you choose partition ", p, " and lambda ", l)
    
    part = json.load(open(P[p]))
    E = part["final"]["E"]
    
    #Load matrix
    M = load_problem(folder)
    S = M.size
    W = np.sum(M)
    #Do dklnorm
    dkl_norm = np.sum(M*np.ma.log(M*S/W))
    #Do dkl factor
    dkl_factor = (1-l)/dkl_norm
    print("dkl norm : ", dkl_norm)
    print("dkl factor : ", dkl_factor)
    #Do penalty factor
    penalty_factor = l/np.log2(np.sqrt(E))
    
    dkl = 0
    print(M)
    #For each rectangle
    for rct in range(part['final']['Rsize']):
        u = np.array(part['R'][str(rct)]["geometry"][0])
        v = np.array(part['R'][str(rct)]["geometry"][1])
        R = np.meshgrid(u, v)
        S = u.size*v.size
        sM = M[R]
        
        W = np.sum(sM)

        print()
        print("W", W)
        dkl += np.sum(sM*np.ma.log(sM*S/W))
        print(dkl)
        print(ldkl)
        print(part['R'][str(rct)]["geometry"])
    ldkl = dkl_factor*dkl
    #   Get DKL and ldkl
    
    #Add penalty
    #output cost
    
    
    
    
    
    