//
//  main.cpp
//  ZIPMD5
//
//  Created by Émilie.Jiang on 15/2/25.
//  Copyright (c) 2015年 pp. All rights reserved.
//

#include <iostream>
#include "md5.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    const int fileCount = 1;
    std::string files[fileCount] ={"deeplink_gift"};
    //FILE f = fopen("/Users/AmyJiang/Documents/workspace/IF/client/zipMD5/lua.zip", w+);
    int index = 0;
    while (index<fileCount) {
        std::string filePath = "/Users/EmilieJiang/Documents/workspace/COK/trunk/client/proj.lua/md5cal/lua_" + files[index] + ".zip";
//    std::string filePath = "/Users/AmyJiang/Documents/workspace/IF/client/proj.lua/zipMD5/dr_goods1.0.1_ios.zip";
        FILE *f = fopen(filePath.c_str(), "r");
        if(f){
            std::ifstream ifs;
            ifs.open(filePath);
            MD5 md5;
            md5.update(ifs);
            std::cout << "lua_"+files[index]+".zip" +"=" + md5.toString();
//            std::cout <<  md5.toString();
            std::cout << "\n";
            ifs.close();
        }else{
             std::cout << files[index] +" is not exist ";
        }
        ++index;
    }
    
    return 0;
}
