import os
import sys
backup_path = "./README_BAKUP.md"
readme_path = "./README.md"
readme_generate_path = "./README_GEN.md" 
docs_dir = "./Records/Docs/"
ccb_dir = "./Records/CCB/"
enter_symbol = "\n"

if __name__ == "__main__":
    print("start...")
    print("正在备份文件...")
    with open(readme_path, 'r', encoding="utf-8") as f1:
        with open(backup_path, 'w+', encoding="utf-8") as f2:
            data = f1.readlines()
            for line in data:
                f2.write(line)
    print("备份完成...")
    
    
    with open(readme_path, 'r', encoding="utf-8") as f:
        """  
            修改 Log Part
        """
        
        print("\n修改LOG...")
        data = f.readlines()
        line_count = -1
        week_count = -1
        for line in data:
            line_count += 1
            if line.startswith("### Week"):
                week_count += 1
            if line.startswith("## ConfigurationControlBoard(CCB)"):
                break
        
        while data[line_count - 1] == "\n":
            line_count -= 1
        
        current_line = line_count
        new_week = week_count + 1
        buffer = []
        is_find = False
        files = os.listdir(docs_dir)
        for file in files:
            number_str = "0" + str(new_week)
            if file[0:2] == number_str[-2:]:
                is_find = True
                file_name = file.split(".")[0]
                sub_title = ("["+ file_name +"](./Records/Docs/" + file +")" + enter_symbol + enter_symbol +
                                "在这里填写相关内容" + enter_symbol)
                buffer.append(sub_title)

        if is_find:
            seg_title = "### Week" + str(new_week) + enter_symbol
            notation_content = "> 本周总结：[第n阶段-内容(yyyy-mm-dd)](./Records/Files/PPT/xxxx.pptx)" + enter_symbol + enter_symbol
            buffer.insert(0, notation_content)
            buffer.insert(0, seg_title)
            
        print("正在将LOG的修改写入缓存...")
        print(buffer)
        for content in buffer:
            data.insert(current_line, content)
            current_line += 1
        print("LOG缓存成功!\n")
        """  
            修改 CCB Part
        """
        print("\n修改CCB...")
        line_count = -1
        ccb_collector = []
        for line in data:
            line_count += 1
            if line.find(r"./Records/CCB") != -1:
                ccb_collector.append(line[-15:-2])
            if line.startswith("## Basic Develop Method"):
                break
        while data[line_count - 1] == "\n":
            line_count -= 1

        ccb_count = len(ccb_collector)
        current_line = line_count
        buffer = []
        files = os.listdir(ccb_dir)
        for file in files:
            if file not in ccb_collector and file != "README.md":
                ccb_count += 1
                file_name = file.split(".")[0]
                ccb_content = str(ccb_count)+". ["+ file_name +"](./Records/CCB/" + file +")" + enter_symbol
                buffer.append(ccb_content)
        
        print("正在将CCB的修改写入缓存...")
        print(buffer)
        for content in buffer:
            data.insert(current_line, content)
            current_line += 1
        print("CCB缓存成功!\n")
    print("等待将缓存数据写入README...")
    with open(readme_path, 'w', encoding="utf-8") as f:
        for line in data:
            f.write(line)    
    print("写入完成！！")
    print("end...")