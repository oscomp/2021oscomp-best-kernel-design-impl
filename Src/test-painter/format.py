from os import access, mkdir, path, sep
import os
from typing import List
from global_file import *



def get_sections_from_raw(path: str) -> List:
    """从原始路径文件获取每个以空行分隔的section内容

    Args:
        path (str): 最原始的测试文件，文件格式为:

        120
        130
        140
        150
        160

        20
        30
        40
        50
        60

        12
        14
        15
        16
        17

    Returns:
        [List]: 以section为单位的列表
    """
    sections:List = []
    with open(path) as f:
        G_VAR_DATA = f.readlines()
        section_size = -1
        section: List = []
        for line in G_VAR_DATA:
            if line.endswith("\n"): 
                line_striped : str = line.strip("")[:-1]
            if line_striped == "":
                if len(section) == 0:
                    continue
                new_section = section.copy()
                sections.append(new_section)
                if section_size == -1:
                    section_size = len(section)
                else :
                    if len(section) != section_size:
                        raise Exception(get_sections_from_raw.__name__ + ": section大小不同")
                section.clear()
            elif line_striped.startswith("["):
                continue 
            else :
                section.append(line_striped)
        """ 添加剩下的一点点 """
        if len(section) != 0:
            new_section = section.copy()
            sections.append(new_section)
            if section_size == -1:
                section_size = len(section)
            else :
                if len(section) != section_size:
                    raise Exception(get_sections_from_raw.__name__ + ": section大小不同")
            section.clear()
    return sections

def print_section(section: List, section_title: str) -> None:
    section_size = len(section)
    counter = len(section)
    print(G_SYM_SECTION_DIVIDER + "[SECTION " + str(section_title) + "]" + G_SYM_SECTION_DIVIDER)
    while counter:
        if (section_size - counter) % 5 == 0 and (section_size - counter) != 0:
            print("")
        print(section[section_size - counter], sep="\t", end=" ")
        counter -= 1
    print("")
    print(G_SYM_SECTION_DIVIDER + "[THE END " + str(section_title) + "]" + G_SYM_SECTION_DIVIDER)
    return

def name_sections(sections: List[List]) -> None:
    sections_cnt = len(sections)
    section_names = []
    need_rename = False
    if sections_cnt == 0:
        raise Exception(name_sections.__name__ + ": section为空")
    while sections_cnt:
        section = sections[len(sections) - sections_cnt]
        print_section(section, len(sections) - sections_cnt)
        section_name = input("输入该Section的名字: ")
        section_names.append(section_name)
        sections_cnt -= 1
    for section_name in section_names:
        if section_names.count(section_name) > 1:
            print(name_sections.__name__ + ": %s 重复! 请重新输入" % section_name)
            print("\n\n")
            need_rename = True
            break
    if need_rename:
        name_sections(sections)
    else:
        for section in sections:
            section.insert(G_VAR_SECTION_TITLE_POS, section_names[sections.index(section)])
    print(sections)

def give_table_name() -> str:
    return input("输入表名: ")

def format(raw_path : str, sections : List[List], table_name: str) -> str :
    root_file_name = __file__
    root_dir = os.path.dirname(root_file_name)
    raw_name = os.path.basename(raw_path) 
    
    base_dir = root_dir + G_VAR_OUT_DIR
    path_out = base_dir + "\\test_" + raw_name
    print("输出至文件: " + path_out)
    out = []
    """ SECTOR描述 """
    out.append(G_SYM_SECTION_DIVIDER + G_SYM_NEXT_LINE)
    out.append(table_name + G_SYM_NEXT_LINE)
    out.append(G_SYM_SECTION_DIVIDER + G_SYM_NEXT_LINE)
    """ SECTOR 名 """
    out.append(G_SYM_SECTION_TITLE_DEIVIDER + G_SYM_NEXT_LINE)
    for section in sections: 
        out.append(section[G_VAR_SECTION_TITLE_POS] + G_SYM_SPACE)
        if sections[-1] == section:
            out.append(G_SYM_NEXT_LINE)
    out.append(G_SYM_SECTION_TITLE_DEIVIDER + G_SYM_NEXT_LINE)
    
    section_cnt = len(sections)
    section_size = len(sections[0])
    out.append(G_SYM_CONTENT_DIVIDER + G_SYM_NEXT_LINE)
    for i in range(section_size):           # Row
        if i == 0:
            continue
        for j in range(section_cnt):        # Col
            out.append(G_SYM_SEP)
            out.append(sections[j][i])
        out.append(G_SYM_SEP + G_SYM_NEXT_LINE)
    out.append(G_SYM_CONTENT_DIVIDER + G_SYM_NEXT_LINE)
    if not access(base_dir, os.F_OK):
        mkdir(base_dir)

    with open(path_out, "w+") as f:
        for line in out:
            f.write(line)
    return path_out

def main():
    raw_path = get_local_file()
    sections = get_sections_from_raw(raw_path)
    name_sections(sections)
    table_name = give_table_name()
    format(raw_path, sections, table_name)

if __name__ == "__main__":
    main()