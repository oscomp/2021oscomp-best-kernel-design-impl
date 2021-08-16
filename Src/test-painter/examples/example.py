import xlwings as xw


app = xw.App()

wb = app.books.active

sht = wb.sheets.active

# 生成图表的数据
sht.range('A1').value = [['时间', '数量'], ['1日', 2], ['2日', 1], ['3日', 3]
                         , ['4日', 4], ['5日', 5], ['6日', 6]]

"""图表类型参数，被注释的那几个，无法生成对应的图表"""
dic = {
    '3d_area': -4098,
    '3d_area_stacked': 78,
    '3d_area_stacked_100': 79,
    '3d_bar_clustered': 60,
    '3d_bar_stacked': 61,
    '3d_bar_stacked_100': 62,
    '3d_column': -4100,
    '3d_column_clustered': 54,
    '3d_column_stacked': 55,
    '3d_column_stacked_100': 56,
    '3d_line': -4101,
    '3d_pie': -4102,
    '3d_pie_exploded': 70,
    'area': 1,
    'area_stacked': 76,
    'area_stacked_100': 77,
    'bar_clustered': 57,
    'bar_of_pie': 71,
    'bar_stacked': 58,
    'bar_stacked_100': 59,
    'bubble': 15,
    'bubble_3d_effect': 87,
    'column_clustered': 51,
    'column_stacked': 52,
    'column_stacked_100': 53,
    'cone_bar_clustered': 102,
    'cone_bar_stacked': 103,
    'cone_bar_stacked_100': 104,
    'cone_col': 105,
    'cone_col_clustered': 99,
    'cone_col_stacked': 100,
    'cone_col_stacked_100': 101,
    'cylinder_bar_clustered': 95,
    'cylinder_bar_stacked': 96,
    'cylinder_bar_stacked_100': 97,
    'cylinder_col': 98,
    'cylinder_col_clustered': 92,
    'cylinder_col_stacked': 93,
    'cylinder_col_stacked_100': 94,
    'doughnut': -4120,
    'doughnut_exploded': 80,
    'line': 4,
    'line_markers': 65,
    'line_markers_stacked': 66,
    'line_markers_stacked_100': 67,
    'line_stacked': 63,
    'line_stacked_100': 64,
    'pie': 5,
    'pie_exploded': 69,
    'pie_of_pie': 68,
    'pyramid_bar_clustered': 109,
    'pyramid_bar_stacked': 110,
    'pyramid_bar_stacked_100': 111,
    'pyramid_col': 112,
    'pyramid_col_clustered': 106,
    'pyramid_col_stacked': 107,
    'pyramid_col_stacked_100': 108,
    'radar': -4151,
    'radar_filled': 82,
    'radar_markers': 81,
    # 'stock_hlc': 88,
    # 'stock_ohlc': 89,
    # 'stock_vhlc': 90,
    # 'stock_vohlc': 91,
    # 'surface': 83,
    # 'surface_top_view': 85,
    # 'surface_top_view_wireframe': 86,
    # 'surface_wireframe': 84,
    'xy_scatter': -4169,
    'xy_scatter_lines': 74,
    'xy_scatter_lines_no_markers': 75,
    'xy_scatter_smooth': 72,
    'xy_scatter_smooth_no_markers': 73
}

w = 385
h = 241
n = 0
x = 100
y = 10

for i in dic.keys():
    xx = x + n % 3*w    # 用来生成图表放置的x坐标。
    yy = y + n//3*h     # 用来生成图表放置的y坐标。
    chart = sht.charts.add(xx, yy)
    chart.set_source_data(sht.range('A1').expand())
    chart.chart_type = i
    chart.api[1].ChartTitle.Text = i
    chart.name = i
    n += 1

wb.save()
wb.close()
app.quit()