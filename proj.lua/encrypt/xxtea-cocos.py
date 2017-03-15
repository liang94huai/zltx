#coding:utf-8

import sys
import os
import xxteaModule

if len(sys.argv)<2:
    print "输入参数 ... \n参数1： 加密子文件夹路径（proj.lua内文件夹路径） 参数2：是否递归文件夹（'0'-不递归  '1'-递归）example: \npython xxtea-cocos.py luaCOK2 0\n"
    sys.exit()

subpath = sys.argv[1]
recursion = sys.argv[2]

#需要的参数
curpath = os.path.join(os.getcwd(),os.path.pardir)
#print curpath
file_path =  os.path.join(curpath,subpath)
#print file_path
xxtea_key = "cok_2dxV3_lua"
xxtea_sign = "elexhcg"

# #上一级路径
# file_root = os.path.abspath(os.path.join(file_path,os.path.pardir))

def copyEncryptionFiles(source_dir, target_dir):
    #去掉 . 开头的隐藏文件
    if source_dir.find('.') == 1:
        return
    for file in os.listdir(source_dir):
        source_file = os.path.join(source_dir, file)
        target_file = os.path.join(target_dir,file)
        if os.path.isfile(source_file):
            if os.path.exists(target_file):
                os.remove(target_file)
            if source_file.find(".bin") > 0:
                continue
            #只加密lua
            fileExt = source_file[-4:]
            #print ("find " + fileExt)
            if fileExt == ".lua" or fileExt == ".cfg":
                f = open(source_file, "rb").read()
                data = xxtea_sign + xxteaModule.encrypt(f,xxtea_key)
                open(target_file, "wb").write(data)
            
        if os.path.isdir(source_file):
            if recursion == "1":
                if not os.path.exists(target_file):
                    os.mkdir(target_file)
                copyEncryptionFiles(source_file,target_file)

des_path = os.path.join(curpath,"encrypt_output")
if not os.path.exists(des_path):
    os.mkdir(des_path)

copyEncryptionFiles(file_path, des_path)