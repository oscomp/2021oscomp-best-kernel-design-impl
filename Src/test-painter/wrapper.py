from paint import *
from format import *
from global_file import *
import numpy as np
DRAW_TYPE_BASIC_IO          = 0
DRAW_TYPE_BASIC_IO_DETAIL   = 1
DRAW_TYPE_MOUNT             = 2
DRAW_TYPE_GC                = 3
DRAW_TYPE_MERGEABLE_TREE    = 4

def normalization(array, i):
    normalized_array = []
    base_elem        = array[i]
    for elem in array:
        normalized_array.append(float(elem / base_elem))
    return normalized_array

def main():
    print("=================================")
    print("=       Automatic Painter       =")
    print("=================================")
    print("draw type: ")
    print("\t0: basic io")
    print("\t1: basic io detail")
    print("\t2: mount")
    print("\t3: gc")
    print("\t4: mergeable tree")
    draw_type = input("请输入绘制类型选项: (0, 1, 2, 3, 4)")
    if not draw_type.isdigit():
        print("参数错误，重新输入")
        main()
    draw_type = int(draw_type)
    table_name:str               = give_table_name()
    col_names:List[str]          = []
    label_names:List[str]        = []
    col_values:List[List[float]] = []
    table_class:str              = "BAR"

    # raw_path = get_local_file()
    # out_path = format(raw_path, sections, table_name)
    # [table_name, col_names, col_values] = parse_formatted_file(out_path)
    if draw_type == DRAW_TYPE_BASIC_IO:
        label_names = ["hoitfs-merge", "hoitfs-no-merge", "spiffs"]
        col_names   = ["RR", "SR", "RW", "SW"]
        raw_paths   = ["./raw/random_read_1.txt",
                       "./raw/seq_read_1.txt", 
                       "./raw/random_write_1.txt",
                       "./raw/seq_write_1.txt"]
        for i in range(0, len(label_names)):
            col_values.append([])
        for raw_path in raw_paths:
            sections = get_sections_from_raw(raw_path)
            normalized_array = []
            for index, section in enumerate(sections):
                sum = 0
                num = 0
                for number in section:
                    sum += float(number)
                    num += 1
                normalized_array.append((sum / num))

            normalized_array = normalization(normalized_array, 1)
            for index, section in enumerate(sections):
                col_values[index].append(normalized_array[index])
    elif draw_type == DRAW_TYPE_BASIC_IO_DETAIL:
        table_class = "LINE"
        label_names = ["hoitfs-merge", "hoitfs-no-merge", "spiffs"]
        # col_names   = ["RR", "SR", "RW", "SW"]
        # raw_paths   = ["./raw/random_read_1.txt",
        #                "./raw/seq_read_1.txt", 
        #                "./raw/random_write_1.txt",
        #                "./raw/seq_write_1.txt"]
        col_names = ["RR"]
        raw_paths = ["./raw/random_read_1.txt"]
        for i in range(0, len(label_names)):
            col_values.append([])
        for raw_path in raw_paths:
            sections = get_sections_from_raw(raw_path)
            for index, section in enumerate(sections):
                col_values[index].extend(section)
            
    elif draw_type == DRAW_TYPE_MERGEABLE_TREE:
        table_class = "LINE"

    # temp_matrix = np.matrix(col_values)
    # temp_matrix:np.matrix = normalization(temp_matrix)
    # col_values  = temp_matrix.tolist() 

    draw(
        table_name = table_name, 
        col_names = col_names, 
        col_values = col_values, 
        label_names = label_names,
        table_class = table_class
    )
    
if __name__ == '__main__':
    main()