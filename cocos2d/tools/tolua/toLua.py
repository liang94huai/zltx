#!/usr/bin/env python
import os
import os.path
import sys
import hashlib

StructList = []
StructListStr = ""
EnumListStr = "MapType;FavoriteType;TileButtonState;PlayerType;MarchStateType;MarchMethodType;AAreaState;WorldCityType;AAeraDir;WorldActivityState;WorldActivityType;BattleTileType;MarchEndAction;TrebuchetState;MonsterSearchResultType;MonsterFightResultType;"
#cfile = sys.argv[1]
#className = sys.argv[2]


def generate_checktype(ktype):
	if ktype == "string" or ktype == "std::string" or ktype == "int" or ktype =="unsigned int" or ktype == "float" or ktype == "double" or ktype == "time_t" or ktype == "long" or ktype=="unsigned long" or ktype == "bool":
		return True
	if EnumListStr.find((ktype+";"))>=0:
		return True
	return False

def generate_push_string(ktype,kvalue):
	a = ""
	if ktype == "string" or ktype == "std::string":
		a = "lua_pushstring(tolua_S, "
		a = a + kvalue
		a = a + ".c_str());"
	elif ktype == "int" or EnumListStr.find((ktype+";"))>=0:
		a = "lua_pushinteger(tolua_S, "
		a = a+kvalue
		a = a + ");"
	elif ktype =="unsigned int" or ktype == "float" or ktype == "double" or ktype == "time_t" or ktype == "long" or ktype=="unsigned long":
		a = "lua_pushnumber(tolua_S, "
		a = a+kvalue
		a = a + ");"
	elif ktype == "bool":
		a = "tolua_pushboolean(tolua_S, "
		a = a+kvalue
		a = a + ");"

	return a

def generate_struct(cfile,className):
	rootpath = os.path.join(os.path.join(os.path.join(os.getcwd(),os.path.pardir),os.path.pardir),os.path.pardir)
	rootpath = os.path.join(os.path.join(rootpath,"IF"),"Classes")

	fpath = rootpath +"/"+ cfile
	fh = open(fpath)
	inStruct=False
	struncStr = ""
	for line in fh.readlines():
		findStr = "struct " + className
		posA = line.find(findStr)
		if posA >= 0:
			inStruct = True
			struncStr = "void " + className + "toLua(lua_State* tolua_S," + className + " info){\n\t"
			struncStr = struncStr + "lua_newtable(tolua_S);\n\t"
		elif inStruct == True:
			findline = line.strip()
			if len(findline)>0 and findline.find("(") < 0 and findline.find(")") < 0 and findline.find("{") < 0 and findline.find("}") < 0:
				pos3 = findline.rfind("//")
				if pos3==0:
					pass
					continue
				elif pos3>0:
					findline = findline[0:pos3]

				pos1 = findline.rfind(";")
				pos2 = findline.rfind(" ")

				if pos1 >=0 and pos2 >=0 and pos1>pos2:
					sPos = (int)(pos2+1)
					ePos = (int)(pos1)
					pKey = findline[sPos:ePos]
					sPos = (int)(pos2)
					pType = findline[0:sPos]
					if len(pKey) > 0 and len(pType)>0:
						struncStr = struncStr + "lua_pushstring(tolua_S,\"" + pKey + "\");\n\t"
						pVaule = "info."+ pKey
						if generate_checktype(pType) == True:
							struncStr = struncStr + generate_push_string(pType,pVaule) + "\n\t"
						elif pType == "WorldResourceType" or pType == "BattleTileType" or pType == "SOILDERTYPE" or pType == "TrebuchetState" or pType == "AAreaState" or pType == "ServerStateType":
								struncStr = struncStr + generate_push_string("float",pVaule) + "\n\t"
						else:
							print ("unknow type [ " + pType +" ]")
							continue
						struncStr = struncStr + "lua_settable(tolua_S, -3);\n\t"
						
			elif findline.find("}")>=0 and findline.find(";")>=0:
				struncStr = struncStr + "\n}\n"
				inStruct=False
				break
	pass
	#print ("struct_result [ " + struncStr + " ]")

	fileAuto =  rootpath + "/lua/lua_cocos2dx_if_auto.cpp"
	if_auto = open(fileAuto)
	if_arr = []
	idx=1
	insertLine=0
	isFindClass=False
	for autoLine in if_auto.readlines():
		if_arr.insert(idx,autoLine)
		findstr = "#include \"LuaBasicConversions.h\""
		if autoLine.find(findstr)>=0:
			insertLine = idx
		idx=idx+1

	if_arr.insert(insertLine,struncStr)
	if_auto.close()
	os.remove(fileAuto)
	new_if_auto = open(fileAuto,"w+")
	newoutput = ""
	newoutput = newoutput.join(if_arr)
	new_if_auto.write(newoutput)
	new_if_auto.close()


def generate_member_var(cfile,className):
	rootpath = os.path.join(os.path.join(os.path.join(os.getcwd(),os.path.pardir),os.path.pardir),os.path.pardir)
	rootpath = os.path.join(os.path.join(rootpath,"IF"),"Classes")

	fpath = rootpath +"/"+ cfile
	#print ("file path [ " + fpath + "]")

	fh = open(fpath)
	isInClass=False
	classDef = "class " + className
	inMarkCnt=0
	isPublic=False
	outputTypeArr = []
	outputValArr = []
	idx=1
	for line in fh.readlines():
		#print ("line [ " + line + " ]")
		if isInClass == True:
			#print ("[ " + line + " ]")
			if line.find("{")>=0:
				inMarkCnt=inMarkCnt+1

			if isPublic == True:
				if line.find("private:")>=0 or line.find("protected:")>=0:
					break

				findline = line.strip()
				if len(line)>0 and line.find("(") < 0 and line.find(")") < 0 and line.find("{") < 0 and line.find("}") < 0:
					#print ("fl [ " + findline + " ]")
					pos3 = findline.rfind("//")
					if pos3==0:
						pass
						continue
					elif pos3>0:
						findline = findline[0:pos3]

					pos1 = findline.rfind(";")
					pos2 = findline.rfind(" ")
	
					if pos1 >=0 and pos2 >=0 and pos1>pos2:
						sPos = (int)(pos2+1)
						ePos = (int)(pos1)
						pKey = findline[sPos:ePos]
						sPos = (int)(pos2)
						pType = findline[0:sPos]
						if len(pKey) > 0 and len(pType)>0:
							outputTypeArr.insert(idx,pType)
							outputValArr.insert(idx,pKey)
							idx=idx+1
							#print ("pKey = [ " + pKey + " ] , pType = [ " + pType + " ]")

				if line.find("}")>=0:
					inMarkCnt=inMarkCnt-1
					if inMarkCnt<=0:
						isInClass=False
						break
			else:
				if line.find("public:")>=0:
					isPublic = True
					
		else:
			if line.find(classDef) >= 0:
				isInClass=True
				if line.find("{")>=0:
					inMarkCnt=inMarkCnt+1
				#print("[ " + line + " ]")



				
	fileAuto =  rootpath + "/lua/lua_cocos2dx_if_auto.cpp"
	if_auto = open(fileAuto)
	if_arr = []
	idx=1
	insertLine=0
	funcLine=0
	isFindClass=False
	for autoLine in if_auto.readlines():
		rpPos1 = autoLine.find("cc.__Dictionary")
		if rpPos1 >= 0:
			# print("find replace string")
			autoLine = autoLine.replace("cc.__Dictionary","CCDictionary")
		rpPos2 = autoLine.find("cc.__Array")
		if rpPos2 >=0:
			autoLine = autoLine.replace("cc.__Array","CCArray")
			

		if_arr.insert(idx,autoLine)
		findstr = "int lua_register_cocos2dx_if_" + className +"(lua_State* tolua_S)"
		if isFindClass==True:
			if autoLine.find("}")>=0:
				isFindClass=False
			else:
				if autoLine.find("tolua_endmodule(tolua_S);")>=0:
					funcLine=idx
		else:
			if autoLine.find(findstr)>=0:
				insertLine = idx
				isFindClass=True

		idx=idx+1

	if len(if_arr)>0 and insertLine>0 and funcLine>0:
		iStr = (str)(insertLine)
		fStr = (str)(funcLine)
		#print ("lidx " + iStr +"," + fStr +"\n")
		writeStr = "\t\ttolua_function(tolua_S,\"__newindex\",lua_cocos2dx_if_" + className + "_newindex);\n"
		if_arr.insert(funcLine-1,writeStr)
		writeStr = "\t\ttolua_function(tolua_S,\"getProperty\",lua_cocos2dx_if_" + className + "_getProperty);\n"
		if_arr.insert(funcLine-1,writeStr)

		writeStr = "static int lua_cocos2dx_if_" + className + "_getProperty(lua_State* tolua_S)\n{\n" 
		writeStr = writeStr + "\tint argc = 0;\n\t" + className +"* cobj = nullptr;\n\tbool ok  = true;\n\n"
		writeStr = writeStr + "#if COCOS2D_DEBUG >= 1\n\ttolua_Error tolua_err;\n\tif (!tolua_isusertype(tolua_S,1,\"" + className +"\",0,&tolua_err)){\n\t"
		writeStr = writeStr + "\ttolua_error(tolua_S,\"invalid 'cobj' in function 'lua_cocos2dx_if_" + className +"_getProperty'\", nullptr);\n\t\treturn 0;\n\t}\n#endif\n\n"
		writeStr = writeStr + "\tcobj = ("+ className +"*)tolua_tousertype(tolua_S,1,0);\n"
		writeStr = writeStr + "\tint n = lua_gettop(tolua_S);\n\tstring property = lua_tostring(tolua_S, n);\n"

		writeStr1 = "static int lua_cocos2dx_if_" + className +"_newindex(lua_State* tolua_S)\n{\n\t" + className +"* cobj = *("+className+"**)lua_topointer(tolua_S, 1);\n"
		tmpStr="\t"
		tmpStr1 = "\t"
		for oIdx in xrange(0,len(outputTypeArr)):
			paramT = outputTypeArr[oIdx].strip()

			if paramT == "string" or paramT=="std::string":
				tmpStr = tmpStr + "if (strcmp(property.c_str(), \"" + outputValArr[oIdx] + "\") == 0){\n\t\t" + "lua_pushstring(tolua_S, cobj->"+outputValArr[oIdx] +".c_str());\n\t}else "
				tmpStr1 = tmpStr1 + "if (strcmp(lua_tostring(tolua_S, 2), \"" + outputValArr[oIdx] +"\") == 0){\n\t\t" + "cobj->" + outputValArr[oIdx] +"= (string)lua_tostring(tolua_S, 3);\n\t}else "
			elif paramT == "int" or EnumListStr.find((paramT+";"))>=0:
				tmpStr = tmpStr + "if (strcmp(property.c_str(), \"" + outputValArr[oIdx] + "\") == 0){\n\t\t" + "lua_pushinteger(tolua_S, cobj->"+outputValArr[oIdx] +");\n\t}else "
				tmpStr1 = tmpStr1 + "if (strcmp(lua_tostring(tolua_S, 2), \"" + outputValArr[oIdx] +"\") == 0){\n\t\t" + "cobj->" + outputValArr[oIdx] +"= ("+ paramT +")lua_tointeger(tolua_S, 3);\n\t}else "
			elif paramT =="WorldCityType" or paramT =="unsigned int" or paramT == "float" or paramT == "double" or paramT == "time_t" or paramT == "long" or paramT=="unsigned long":
				tmpStr = tmpStr + "if (strcmp(property.c_str(), \"" + outputValArr[oIdx] + "\") == 0){\n\t\t" + "lua_pushnumber(tolua_S, cobj->"+outputValArr[oIdx] +");\n\t}else "
				tmpStr1 = tmpStr1 + "if (strcmp(lua_tostring(tolua_S, 2), \"" + outputValArr[oIdx] +"\") == 0){\n\t\t" + "cobj->" + outputValArr[oIdx] +"= ("+ paramT +")lua_tonumber(tolua_S, 3);\n\t}else "
			elif paramT == "bool":
				tmpStr = tmpStr + "if (strcmp(property.c_str(), \"" + outputValArr[oIdx] + "\") == 0){\n\t\t" + "tolua_pushboolean(tolua_S,(bool)cobj->"+outputValArr[oIdx] +");\n\t}else "
				tmpStr1 = tmpStr1 + "if (strcmp(lua_tostring(tolua_S, 2), \"" + outputValArr[oIdx] +"\") == 0){\n\t\t" + "cobj->" + outputValArr[oIdx] +"= (bool)lua_toboolean(tolua_S, 3);\n\t}else "
			elif paramT.find("vector<")>=0 and paramT.find("map<")<0:
				posM = paramT.find("<")
				posN = paramT.find(">")
				typeStr = paramT[posM+1:posN]
				typeStr = typeStr.strip()
				#print ("vector type [ " +typeStr +" ]")
				if generate_checktype(typeStr) == True:
					tmpStrV = "if(strcmp(property.c_str(), \""+outputValArr[oIdx]+"\") == 0){\n\t\tlua_newtable(tolua_S);\n\t\tauto iter = cobj->"+outputValArr[oIdx]+".begin();\n\t\tint idx = 1;\n\t\twhile(iter!=cobj->"+ outputValArr[oIdx] +".end()){\n\t\t\tlua_pushinteger(tolua_S, idx);\n\t\t\t"
					tmpStrV = tmpStrV + generate_push_string(typeStr,"(*iter)") + "\n\t\t\tlua_settable(tolua_S, -3);\n\t\t\t++idx;\n\t\t\t++iter;\n\t\t}\n\t}else "
					tmpStr = tmpStr + tmpStrV
				else:
					print ("undefine [ " + paramT  + " ]")
			elif paramT.find("map<")>=0:
				posM = paramT.find("<")
				posN = paramT.find(",")
				posP= paramT.rfind(">")
				typeStr = paramT[posM+1:posN]
				typeStr = typeStr.strip()
				typeStr1 = paramT[posN+1:posP]
				typeStr1 = typeStr1.strip()
				#print ("vector type [ " +typeStr + "," + typeStr1 +" ]")
				if generate_checktype(typeStr) == True and generate_checktype(typeStr1) == True:
					tmpStrV = "if(strcmp(property.c_str(), \"" + outputValArr[oIdx] +"\") == 0){\n\t\tlua_newtable(tolua_S);\n\t\tauto iter = cobj->"+outputValArr[oIdx]+".begin();\n\t\tint idx = 1;\n\t\twhile(iter!=cobj->"+outputValArr[oIdx]+".end()){\n\t\t\tlua_pushinteger(tolua_S, idx);\n\t\t\tlua_newtable(tolua_S);\n\t\t\t"
					tmpStrV = tmpStrV + generate_push_string(typeStr,"(*iter).first") +"\n\t\t\t" + generate_push_string(typeStr1,"(*iter).second")+"\n\t\t\tlua_settable(tolua_S, -3);\n\t\t\tlua_settable(tolua_S, -3);\n\t\t\t++idx;\n\t\t\t++iter;\n\t\t}\n\t}else "
					tmpStr = tmpStr + tmpStrV
				else:
					print ("undefine [ " + paramT  + " ]")
			elif StructListStr.find((paramT+";"))>=0:
				tmpStr = tmpStr + "if (strcmp(property.c_str(), \"" + outputValArr[oIdx] + "\") == 0){\n\t\t" + paramT + "toLua(tolua_S, cobj->"+outputValArr[oIdx] +");\n\t}else "
			else:
				if  paramT.find("return")<0:
					print ("undefine [ " + paramT  + " ]")

		tmpStr = tmpStr + "{\n\t\treturn 0;\n\t}\n\treturn 1;\n}\n"
		writeStr = writeStr + tmpStr
		tmpStr1 = tmpStr1 + "{\n\n\t}\n\treturn 0;\n}\n"
		writeStr1 = writeStr1 + tmpStr1
		if_arr.insert(insertLine-1,writeStr1)
		if_arr.insert(insertLine-1,writeStr)
		if_auto.close()
		os.remove(fileAuto)
		new_if_auto = open(fileAuto,"w+")
		newoutput = ""
		newoutput = newoutput.join(if_arr)
		new_if_auto.write(newoutput)
		new_if_auto.close()



cpath = os.getcwd()
cfgpath = cpath +"/cocos2dx_if.ini"
cfg = open(cfgpath)
for cfgl in cfg.readlines():
	if cfgl.find("#member_variable_output_class")>=0:
		pos = cfgl.find("=")
		cfgl = cfgl[pos+1:]
		cfgl = cfgl.strip()
		cfglist = cfgl.split(',')
		for idx in xrange(0,len(cfglist)):
			cfgele = (cfglist[idx].strip()).split(':')
			#print ("element [ " + cfglist[idx] + " ]")
			if len(cfgele)>1:
				fileP = cfgele[0]
				#print ("element file [ " + fileP + " ]")
				for idxxx in xrange(1,len(cfgele)):
					print ("generate_member_var [ " + fileP + " :: "+ cfgele[idxxx] + " ]")
					generate_member_var(fileP,cfgele[idxxx])
					pass
				pass
		continue
	elif cfgl.find("#sturct_output_class")>=0:
		pos = cfgl.find("=")
		cfgl = cfgl[pos+1:]
		cfgl = cfgl.strip()
		cfglist = cfgl.split(',')
		sidx = 0
		for idx in xrange(0,len(cfglist)):
			cfgele = (cfglist[idx].strip()).split(':')
			if len(cfgele)>1:
				fileP = cfgele[0]
				for idxxx in xrange(1,len(cfgele)):
					print ("generate_struct [ " + fileP + " :: "+ cfgele[idxxx] + " ]")
					StructList.insert(sidx,(cfgele[idxxx] + ";"))
					generate_struct(fileP,cfgele[idxxx])
					sidx = sidx + 1
					pass
				pass
		StructListStr = StructListStr.join(StructList)
    	