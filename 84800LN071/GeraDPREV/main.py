#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Dec 16 10:39:48 2022

@author: vitorhpmelo
"""

#%%
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df_DBAR=pd.read_csv("../DBAR.csv",header=None,sep=";")
df_DBAR.columns = ["ID", "LIGACAO", "FASES", "TENSAO_NOM", "PNOM_A", "PNOM_B", "PNOM_C", "QNOM_A", "QNOM_B", "QNOM_C", \
            "ZIP", "VNOM_A", "VNOM_B", "VNOM_C", "ANG_VNOM_A", "ANG_VNOM_B", "ANG_VNOM_C"]

df_DMEDNC=pd.read_csv("../DMEDNC.csv",header=None,sep=",")
df_DMEDNC.columns=["ID", "Tipo", "De", "Para", "Fases", "Nprev"]         

df_DMED=pd.read_csv("../DMED.csv",header=None,sep=",")
df_DMED.columns=["LIGADO", "Tipo", "De", "Para", "Fases", "Zmed","prec"]         


LoadCruves=pd.read_csv("../CURVAS_AGREGADAS_P.dad",header=None,sep=";")

#%%
df_loadCurve=pd.read_csv("time_series_15min_singleindex.csv")


df_loadCurve['utc_timestamp']
#%%
mask1=df_loadCurve['utc_timestamp']>'2020-09-27T16:00:00'
mask2=df_loadCurve['utc_timestamp']<'2020-09-27T22:00:00'
# %%
load=df_loadCurve[mask1 & mask2].AT_load_actual_entsoe_transparency.tolist()
# %%

load=np.array(load)/np.max(load)


# %%

h=6
hinter=1/4

pontos=np.linspace(0, h-hinter,num=int(h/hinter))

#%%
plt.plot(pontos,load)
# %%

df_DPREV=pd.DataFrame()


value=11600
off=0
lst=[]
for item in pontos:
    lst.append(value+off)
    off=off+15    
    if off == 60:
        off=0
        value=value+100
    




for idx,row in df_DMEDNC.iterrows():
    mask=((df_DMED["Tipo"]==row["Tipo"]) & (df_DMED["De"]==row["De"]) & (df_DMED["Para"]==row["Para"]) & (df_DMED["Fases"]==row["Fases"]))
    Z=float(df_DMED[mask].Zmed)
    prec=float(df_DMED[mask].prec)
    d={}
    d['ID']=np.ones(len(load),dtype=int)*row["ID"]
    if row["Tipo"]!=4:
        d['zmed']=load*Z
    else:
        d['zmed']=np.ones(len(load))*Z
    d['prec']=np.ones(len(load))*prec
    d["ts"]=lst
    d["ts2"]=list(range(1,len(lst)+1))
    df_DPREV=pd.concat([df_DPREV,pd.DataFrame(d)])


df_DPREV.to_csv("../DPREV.csv",header=None,index=None)


plt.plot(range(len(df_DPREV[df_DPREV["ID"]==1].ts)),df_DPREV[(df_DPREV["ID"]==1)].zmed)


# %%
