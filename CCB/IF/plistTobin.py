#!/usr/bin/env python

import sys
import os
import codecs
import struct
import string
import xml.etree.ElementTree as etree
import zlib

def isCompress(plistfile):
	
	plistF = codecs.open(plistfile, 'rb')
	content = plistF.read(4)
	plistF.close()
	strcontet = struct.unpack("4s", content)
	if strcontet[0] == 'CCZ!':
		print(plistfile, " is had compress")
		return True
	
	return False

def loadPlistFile(plistfile, tpbfile):
	if os.path.exists(tpbfile):
		os.remove(tpbfile)
	if isCompress(plistfile):
		return

	# print 'start write ', tpbfile
	tpb = codecs.open(tpbfile, 'wb')
	tpb.write('IQOL')
	tree = etree.parse(plistfile);
	root = tree.getroot()
	for dic in root:
		for index1,item1 in enumerate(dic):
			#print item1.tag, index1
			if index1 == 0:
				#===========
				dict3 = dic[index1+3]
				for index3,item3 in enumerate(dict3):
					if item3.text == 'textureFileName':
						dict3_1 = dict3[index3+1]
						tpb.write(struct.pack('<B', len(dict3_1.text)));
						tpb.write(dict3_1.text);
				#=============
				dict2 = dic[index1+1]
				i=0
				while i < len(dict2):
					# print dict2[i].text
					tpb.write(struct.pack('<B', len(dict2[i].text)))
					tpb.write(dict2[i].text)
					dict2_1 = dict2[i+1]
					for index2_1,item2_1 in enumerate(dict2_1):
						#print item2_1.text
						if item2_1.text == 'frame':
							#=======
							dict2_1_2 = dict2_1[index2_1+1]
							# print dict2_1_2.text
							sizelist = dict2_1_2.text.replace('{', '').replace('}','').split(',')
							# print sizelist[0], sizelist[1], sizelist[2], sizelist[3]
							# print string.atoi(sizelist[0]), string.atoi(sizelist[1]), string.atoi(sizelist[2]), string.atoi(sizelist[3])
							#a1, a2 = struct.unpack("", str)
							tpb.write(struct.pack('<HHHH', string.atoi(sizelist[0]), string.atoi(sizelist[1]), string.atoi(sizelist[2]), string.atoi(sizelist[3])))
							#tpb.write(struct.pack('<h', string.atoi(sizelist[0])))
							#tpb.write(struct.pack('<h', string.atoi(sizelist[1])))
							#tpb.write(struct.pack('<h', string.atoi(sizelist[2])))
							#tpb.write(struct.pack('<h', string.atoi(sizelist[3])))

						elif item2_1.text == 'offset':
							dict2_1_3 = dict2_1[index2_1+1]
							offsetlist = dict2_1_3.text.replace('{', '').replace('}', '').split(',')
							#print offsetlist[0], offsetlist[1]
							tpb.write(struct.pack('<hh', string.atof(offsetlist[0]), string.atof(offsetlist[1])))
						elif item2_1.text == 'rotated':
							dict2_1_4 = dict2_1[index2_1+1]
							if dict2_1_4.tag.lower() == 'false':
								tpb.write(struct.pack('<B', 0))
							elif dict2_1_4.tag.lower() == 'true':
								tpb.write(struct.pack('<B', 1))
							else:
								tpb.write(struct.pack('<B', 0))
						elif item2_1.text == 'sourceSize':
							dict2_1_3 = dict2_1[index2_1+1]
							sourceSizelist = dict2_1_3.text.replace('{', '').replace('}', '').split(',')
							#print sourceSizelist[0], sourceSizelist[1]
							tpb.write(struct.pack('<HH', string.atoi(sourceSizelist[0]), string.atoi(sourceSizelist[1])))							
					i = i+2
	tpb.close()
	# read tpb file 
	tpbf = codecs.open(tpbfile, 'rb')
	content = tpbf.read()
	tpbf.close()
	os.remove(tpbfile)
	# os.remove(plistfile)
	# compress tpb file
	compressFile = codecs.open(plistfile, 'wb')
	compressFile.write(struct.pack(">4sHHII", "CCZ!", 0, 1, 0, len(content)))
	compressFile.write(zlib.compress(content))
	compressFile.close()

	# os.remove(tpbfile)
	# print 'end write ', tpbfile

def workpath(path):
	for file in os.listdir(path):
		basename, ext = os.path.splitext(file)

		if os.path.isdir(os.path.join(path, file)) and os.path.join(path, file) != 'res_pak/res/part':
			workpath(os.path.join(path, file))
		elif ext.lower() == '.plist':
			loadPlistFile(os.path.join(path, file), os.path.join(path, basename + '.tpb'))
		
if __name__=='__main__':
	reload(sys)
	sys.setdefaultencoding('utf-8')
	# workpath("res_pak/res/animate")
	# workpath(os.curdir)
	path=sys.argv[1]
	if len(path) == 0:
		print "plisttobin argvs is error!"
	else:
		workpath(path)
	
	