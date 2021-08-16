import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.font_manager import FontProperties

from typing import List, Tuple

from xlwings.main import Book, Sheet
from global_file import *
import xlwings

import win32com.client    
import win32clipboard as wc
from PIL import ImageGrab   
import os
import sys
def save_chart(in_excel_path:str, out_img_path:str):
    # emptyClipboard
    os.system("echo off | clip")
    wc.OpenClipboard()
    wc.EmptyClipboard()
    wc.CloseClipboard()  
    # Open the excel application using win32com
    o = win32com.client.Dispatch("Excel.Application")
    # Disable alerts and visibility to the user
    # o.Visible = 0
    # o.DisplayAlerts = 0
    # Open workbook
    wb = o.Workbooks.Open(in_excel_path)

    # Extract first sheet
    sheet = o.Sheets(1)
    for n, shape in enumerate(sheet.Shapes):
        # Save shape to clipboard, then save what is in the clipboard to the file
        shape.Copy()
        image = ImageGrab.grabclipboard()
        try:
        # Saves the image into the existing png file (overwriting) TODO ***** Have try except?
            image.save(out_img_path, 'png')
        except Exception as e:
            print(e)
        pass
    pass

    wb.Close(True)
    o.Quit()

def is_number(str_number: str):
    return (str_number.split(".")[0]).isdigit() or str_number.isdigit() or  (str_number.split('-')[-1]).split(".")[-1].isdigit()

def parse_formatted_file(raw_path: str) -> Tuple[str, int ,List[List]]:
    with open(raw_path) as f:
        G_VAR_DATA = f.readlines()
        table_name = ""
        col_names = []
        col_values = []
        col_cnt = -1
        
        f_table_name_start = False
        f_col_names_start = False
        f_col_values_start = False
        
        for line in G_VAR_DATA:
            line_striped: str = line.strip()
            
            """ 状态机结束，和执行 """
            if f_table_name_start == True:
                if line_striped == G_SYM_SECTION_DIVIDER:
                    f_table_name_start = False
                    continue
                else:
                    table_name = line_striped

            elif f_col_names_start == True:
                if line_striped == G_SYM_SECTION_TITLE_DEIVIDER:
                    f_col_names_start = False
                    continue
                else:
                    col_names = line_striped.split(" ")
                    if col_cnt == -1:
                        col_cnt = len(col_names)
                        for i in range(col_cnt):
                            col_values.append([])
                    
            
            elif f_col_values_start == True:
                if line_striped == G_SYM_CONTENT_DIVIDER:
                    f_col_values_start = False
                    continue
                else:
                    value:List[str] = line_striped.split(G_SYM_SEP)
                    real_value = []
                    for token in value:
                        if is_number(token):
                            real_value.append(token)
                    for i in range(col_cnt):
                        col_values[i].append(real_value[i])
            """ 状态机开始 """
            if line_striped == G_SYM_SECTION_DIVIDER:
                f_table_name_start = True
                continue

            if line_striped == G_SYM_SECTION_TITLE_DEIVIDER:
                f_col_names_start = True
                continue

            if line_striped == G_SYM_CONTENT_DIVIDER:
                f_col_values_start = True
                continue
        print(table_name)
        print(col_names)
        print(col_values)
        return (table_name, col_names, col_values)

def draw(table_name: str, col_names: List[str], col_values: List[List[str]], label_names: List[str] = [], table_class: str = 'BAR'):
    
    """[summary]

    Args:
        table_name (str): [表格名字]
        col_names (List[str]): [列名字]
        col_values (List[List[str]]): [数值]
       
        col_names[0]
        col_values[0]
    """
    #创建数据
    plt.figure(figsize=(6,3))                       #设置整个图的大小,(长和宽)
    x = np.arange(len(col_values[0]))                 #数据的x轴数据
    error_attri = {"elinewidth":1,"ecolor":"black","capsize":3}   #误差棒的属性
    bar_width = 0.2
    bar_style_list = ['', '////', 'xxx', '*', 'o', 'O', '.']
    line_style_list = ['solid', 'dashed', 'dotted', 'dashdot']
    marker_style_list = ['s','X', 'D']
    #创建图形
    if table_class == 'BAR':
        for i in range(len(label_names)):
            plt.bar(
                x+i*bar_width,
                [float(x) for x in col_values[i]],
                bar_width,                  #不在外部设置width这个属性，会报错
                color="white",
                align="center",
                #yerr=std_err1,
                #error_kw=error_attri,      #error_kw是设计误差棒具体细节的属性
                label=label_names[i],
                alpha=1,
                hatch=bar_style_list[i % len(bar_style_list)],          #线条纹格式
                edgecolor='black'
            )
    elif table_class == 'LINE':
        for i in range(len(label_names)):
            plt.plot(
                x,
                [float(x) for x in col_values[i]], 
                color="black",
                label=label_names[i],
                linestyle=line_style_list[i % len(line_style_list)],    #线条纹格式
                marker=marker_style_list[i % len(marker_style_list)]    #点花纹格式
            )
    #创建辅助标签
    # plt.xlabel(col_names)
    plt.ylabel("ms")
    if table_class == 'BAR':
        plt.xticks(x + bar_width / 2, col_names)
    #xticks在py2中与3不是完全相同，tick_label用列表对名称进行了设计，此处设计其他属性
    plt.title(table_name)
    #plt.grid(axis="y",ls="/",color="purple",alpha=0.7)
    plt.legend()
    plt.show()

    # root_dir = os.path.dirname(__file__)
    # base_out_img_dir = root_dir + G_IMG_OUT_DIR
    # base_out_excels_dir = root_dir + G_EXCEL_OUT_DIR
    # out_img_path = root_dir + G_IMG_OUT_DIR + "\\img_" + table_name +".png"
    # out_xlsx_path = root_dir + G_EXCEL_OUT_DIR + "\\excel_" + table_name +".xlsx"
    
    # if not os.access(base_out_img_dir, os.F_OK):
    #     os.mkdir(base_out_img_dir)
    
    # if not os.access(base_out_excels_dir, os.F_OK):
    #     os.mkdir(base_out_excels_dir)
    
    # wb = xlwings.Book()
    # sheet : Sheet = wb.sheets[0]  
    # sheet_value = []
    # col_cnt = len(col_names)
    # row_cnt = len(col_values[0])
    # sheet_value.append(col_names)
    
    # for i in range(row_cnt):
    #     row = []
    #     for j in range(col_cnt):
    #         row.append(col_values[j][i])
    #     sheet_value.append(row)

    # print(sheet_value)

    # sheet.range("A1").value = sheet_value

    # chart = sheet.charts.add(100, 100)
    # chart.set_source_data(sheet.range("A1").expand())
    # chart.chart_type = "line"

    # chart.api[1].SetElement(2) 
    # chart.api[1].ChartTitle.Text = table_name
    

    # wb.save(out_xlsx_path)
    # wb.close()
    # print("等待中...")
    # save_chart(out_xlsx_path, out_img_path)
    # print("完成!")

def main():
    path = get_local_file()
    [table_name, col_names, col_values] = parse_formatted_file(path)
    draw(table_name, col_names, col_values, 0)

if __name__ == "__main__":
    main()