import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.font_manager import FontProperties

font = FontProperties(fname='simhei.ttf', size=14)
#创建数据
plt.figure(figsize=(6,3))   #设置整个图的大小,(长和宽)
x=np.arange(5)              #数据的x轴数据
y1=[100,120,70,68,82]       #bar1的y轴数据
y2=[120,75,70,78,85]        #bar2的y轴数据
std_err1=[7,3,5,2,9]        #bar1的各个方差
std_err2=[5,1,4,2,5]        #bar2的各个方差
error_attri={"elinewidth":1,"ecolor":"black","capsize":3}   #误差棒的属性
bar_width=0.3
tick_label=["园区1","园区2","y3","y4","y5"]     #x轴标签
x_label = "芒果收割区"                          #x轴名称
y_label = "收割量"                              #y轴名称
bar_label1 = "2010"
bar_label2 = "2011"

#创建图形
plt.bar(
    x,
    y1,
    bar_width,              #不在外部设置width这个属性，会报错
    color="white",
    align="center",
    yerr=std_err1,
    error_kw=error_attri,   #error_kw是设计误差棒具体细节的属性
    label=bar_label1,
    alpha=1,
    hatch='/',               #条纹
    edgecolor='black'
)
plt.bar(
    x+bar_width,            #若没有向右侧增加一个bar——width的话，第一个柱体会被遮挡住
    y2,
    bar_width,
    color="white",
    yerr=std_err2,
    error_kw=error_attri,
    label=bar_label2,
    alpha=1,
    hatch='',               #条纹
    edgecolor='black'
)
#创建辅助标签
plt.xlabel(x_label,fontproperties=font)
plt.ylabel(y_label,fontproperties=font)
plt.xticks(x+bar_width/2,tick_label,fontproperties=font)
#xticks在py2中与3不是完全相同，tick_label用列表对名称进行了设计，此处设计其他属性
plt.title("不同年份收割量",fontproperties=font)
#plt.grid(axis="y",ls="/",color="purple",alpha=0.7)
plt.legend()
plt.show()