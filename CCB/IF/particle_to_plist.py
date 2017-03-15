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

# signName is int or float
def getValueBySignName(dic, signName):
	for index1,item1 in enumerate(dic):
		if item1.text == signName:
			dict2 = dic[index1+1]
			return dict2.text
	return "0"

# signName is string
def getValueStrSignName(dic, signName):
	for index1,item1 in enumerate(dic):
		if item1.text == signName:
			dict2 = dic[index1+1]
			return dict2.text
	return ""

def loadPlistFile(plistfile, tpbfile):
	if os.path.exists(tpbfile):
		os.remove(tpbfile)
	if isCompress(plistfile):
		return

	# print 'start write ', tpbfile
	tpb = codecs.open(tpbfile, 'wb')
	tpb.write('IQOL')
	# tpb.write(struct.pack('<i', 100))
	tree = etree.parse(plistfile);
	root = tree.getroot()
	configNameLen = 0
	for dic in root:
		# -- maxParticles
		value = getValueBySignName(dic, "maxParticles")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		 # configName
		value = getValueStrSignName(dic, "configName")
		# print value, "--len(value)==", len(value)
		configNameLen = len(value)
		tpb.write(struct.pack('<f', len(value)))
		if len(value) > 0 :
			tpb.write(value)

		# -- minRadiusVariance
		# value = getValueBySignName(dic, "minRadiusVariance")
		# print value
		# tpb.write(struct.pack('<f', string.atof(value)))

		# # -- textureImageData
		# value = getValueStrSignName(dic, "textureImageData")
		# print value
		# tpb.write(struct.pack('<H', len(value)))
		# if len(value) > 0 :
		# 	tpb.write(value)

		## -- yCoordFlipped
		value = getValueBySignName(dic, "yCoordFlipped")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# # -- angle
		value = getValueBySignName(dic, "angle")
		# print "angle=",value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- angleVariance
		value = getValueBySignName(dic, "angleVariance")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- duration
		value = getValueBySignName(dic, "duration")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- blendFuncSource
		value = getValueBySignName(dic, "blendFuncSource")
		# print value
		# if configNameLen > 0 :
		tpb.write(struct.pack('<f', string.atof(value)))
		# else :
		# 	tpb.write(struct.pack('<i', string.atoi(value)))

		# -- blendFuncDestination
		value = getValueBySignName(dic, "blendFuncDestination")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))


		# color
		# -- startColorRed
		value = getValueBySignName(dic, "startColorRed")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- startColorGreen
		value = getValueBySignName(dic, "startColorGreen")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- startColorBlue
		value = getValueBySignName(dic, "startColorBlue")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- startColorAlpha
		value = getValueBySignName(dic, "startColorAlpha")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))


		# -- startColorVarianceRed
		value = getValueBySignName(dic, "startColorVarianceRed")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- startColorVarianceGreen
		value = getValueBySignName(dic, "startColorVarianceGreen")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- startColorVarianceBlue
		value = getValueBySignName(dic, "startColorVarianceBlue")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- startColorVarianceAlpha
		value = getValueBySignName(dic, "startColorVarianceAlpha")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))
		

		# -- finishColorRed
		value = getValueBySignName(dic, "finishColorRed")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- finishColorGreen
		value = getValueBySignName(dic, "finishColorGreen")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- finishColorBlue
		value = getValueBySignName(dic, "finishColorBlue")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- finishColorAlpha
		value = getValueBySignName(dic, "finishColorAlpha")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))


		# -- finishColorVarianceRed
		value = getValueBySignName(dic, "finishColorVarianceRed")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- finishColorVarianceGreen
		value = getValueBySignName(dic, "finishColorVarianceGreen")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- finishColorVarianceBlue
		value = getValueBySignName(dic, "finishColorVarianceBlue")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- finishColorVarianceAlpha
		value = getValueBySignName(dic, "finishColorVarianceAlpha")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))


		# // particle size
		# -- startParticleSize
		value = getValueBySignName(dic, "startParticleSize")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- startParticleSizeVariance
		value = getValueBySignName(dic, "startParticleSizeVariance")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- finishParticleSize
		value = getValueBySignName(dic, "finishParticleSize")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- finishParticleSizeVariance
		value = getValueBySignName(dic, "finishParticleSizeVariance")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))




		 #  position
		# -- sourcePositionx
		value = getValueBySignName(dic, "sourcePositionx")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- sourcePositiony
		value = getValueBySignName(dic, "sourcePositiony")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- sourcePositionVariancex
		value = getValueBySignName(dic, "sourcePositionVariancex")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- sourcePositionVariancey
		value = getValueBySignName(dic, "sourcePositionVariancey")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))


		# Spinning
		# -- rotationStart
		value = getValueBySignName(dic, "rotationStart")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- rotationStartVariance
		value = getValueBySignName(dic, "rotationStartVariance")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- rotationEnd
		value = getValueBySignName(dic, "rotationEnd")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- rotationEndVariance
		value = getValueBySignName(dic, "rotationEndVariance")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))



		# emitterMode == Mode::GRAVITY
		# -- emitterType
		value = getValueBySignName(dic, "emitterType")
		# print "emitterType=",value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- gravityx
		value = getValueBySignName(dic, "gravityx")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- gravityy
		value = getValueBySignName(dic, "gravityy")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- speed
		value = getValueBySignName(dic, "speed")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- speedVariance
		value = getValueBySignName(dic, "speedVariance")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- radialAcceleration
		value = getValueBySignName(dic, "radialAcceleration")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- radialAccelVariance
		value = getValueBySignName(dic, "radialAccelVariance")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- tangentialAcceleration
		value = getValueBySignName(dic, "tangentialAcceleration")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- tangentialAccelVariance
		value = getValueBySignName(dic, "tangentialAccelVariance")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- rotationIsDir
		value = getValueBySignName(dic, "rotationIsDir")
		# print "rotationIsDir=", value
		tpb.write(struct.pack('<f', string.atof(value)))


		# _emitterMode == Mode::RADIUS
		# -- maxRadius
		value = getValueBySignName(dic, "maxRadius")
		# print value
		# if configNameLen > 0 :
		tpb.write(struct.pack('<f', string.atof(value)))
		# else :
		# 	tpb.write(struct.pack('<i', string.atoi(value)))

		# -- maxRadiusVariance
		value = getValueBySignName(dic, "maxRadiusVariance")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- minRadius
		value = getValueBySignName(dic, "minRadius")
		# print value
		# if configNameLen > 0 :
		tpb.write(struct.pack('<f', string.atof(value)))
		# else :
		# 	tpb.write(struct.pack('<i', string.atoi(value)))

		# -- minRadiusVariance
		value = getValueBySignName(dic, "minRadiusVariance")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- rotatePerSecond
		value = getValueBySignName(dic, "rotatePerSecond")
		# print value
		# if configNameLen > 0 :
		tpb.write(struct.pack('<f', string.atof(value)))
		# else :
		# 	tpb.write(struct.pack('<i', string.atoi(value)))

		# -- rotatePerSecond
		value = getValueBySignName(dic, "rotatePerSecondVariance")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))



		# -- particleLifespan
		value = getValueBySignName(dic, "particleLifespan")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- particleLifespanVariance
		value = getValueBySignName(dic, "particleLifespanVariance")
		# print value
		tpb.write(struct.pack('<f', string.atof(value)))

		# -- textureFileName
		value = getValueStrSignName(dic, "textureFileName")
		# print value, "======",len(value)
		tpb.write(struct.pack('<H', len(value)))
		if len(value) > 0 :
			tpb.write(value)

		
	tpb.close()

	# print "------------------------------------"
	# read tpb file 
	tpbf = codecs.open(tpbfile, 'rb')
	content = tpbf.read()

	tpbf.close()
	os.remove(tpbfile)

	os.remove(plistfile)

	# compress tpb file
	compressFile = codecs.open(plistfile, 'wb')
	compressFile.write(struct.pack(">4sHHII", "CCZ!", 0, 1, 0, len(content)))
	compressFile.write(zlib.compress(content))
	# compressFile.write(content)
	compressFile.close()





	# print 'end write ', tpbfile

def readPlist(path):
	print path
	# tpbf = codecs.open(path, 'rb')
	# # content = tpbf.read()
	# index = 4
	# _str, value = struct.unpack("<4si", tpbf.read(8))
	# print "_str=", _str, "--value--", value

	# index += 4
	# _str = struct.pack("<i", tpbf.read(index))
	# print "i=", _str

	# tpbf.close()

def workpath(path):
	for file in os.listdir(path):
		basename, ext = os.path.splitext(file)
		if os.path.isdir(os.path.join(path, file)):
			workpath(os.path.join(path, file))
		elif ext.lower() == '.plist' and basename != "snow" and basename != "rain" and basename != "vectory_flower" and basename != "ball" and basename != "battleFire" and basename != "bossSnow" and basename != "explode1" and basename != "explode" and basename != "health1" and basename != "health2" and basename != "health3" and basename != "health4" and basename != "health5" and basename != "generalBloodParticle":
			print "basename==", file
			loadPlistFile(os.path.join(path, file), os.path.join(path, basename + '.tpb'))
			#readPlist(os.path.join(path, basename + '.tpb'))
		
if __name__=='__main__':
	reload(sys)
	sys.setdefaultencoding('utf-8')
	path = sys.argv[1]
	if len(path)==0:
		print "particle_to_plist=====> path is error!"
	else:
		workpath(path)
	# workpath(os.curdir)
	# workpath("../transfer.proj/TTransfer/res/part")
	
	