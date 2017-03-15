#!/bin/sh
#console setting
Alert='\033[0;31m'
Info='\033[0;32m'
Normal='\033[0m'
echo "${Normal} Start Bash ${BASH_SOURCE[0]}................"

#default compile setting

defaultVersionCode=999
RepackRes=false
ReNative=true
CrashUpload=false
ReleaseOut=true
DelManifest=false
ProjectName="zltx_hoolai"
PackageName="zltx_hoolai"


Mode="$1"
echo $Mode
if [[ -z $Mode ]]; then
	Mode=default
fi

case $Mode in
	default|-d)
	RepackRes=false
	ReNative=true
	;;
	all|-a)
	RepackRes=true
	ReNative=true
	;;
	java|-j)
	RepackRes=false
	ReNative=false
	;;
	*)
	;;
esac
shift

TYPE="$1"
if [ "$TYPE" = "Release" ] || [ "$TYPE" = "release" ] ; then
	ReleaseOut=true
elif [ "$TYPE" = "Debug" ] || [ "$TYPE" = "debug" ] ; then
	ReleaseOut=false
fi

shift

USECRAS="$1"
if [ "$USECRAS" = "Upload" ] || [ "$USECRAS" = "upload" ] ; then
	CrashUpload=false
else
	CrashUpload=false
fi

shift 

DELVAR="$1"
if [ "$DELVAR" = "true" ] || [ "$DELVAR" = "True" ] ; then
	DelManifest=true
else
	DelManifest=false
fi


scriptRoot="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
scriptName=$0
echo "Script Start:$scriptRoot $scriptName"
echo "\t ${Info}Package Resource: $RepackRes"
echo "\t Compile Native: $ReNative"
echo "\t Compile Release: $ReleaseOut"
echo "\t CrashUpload Upload: $CrashUpload"
echo "\t Clear AndroidManifest After Build: $DelManifest${Normal}"


sourceroot=$scriptRoot/../
packageroot=$scriptRoot/../../package
soRootPath=$scriptRoot/../../package-so
logRoot=$packageroot

mkdir $soRootPath >/dev/null 2>/dev/null
mkdir $logRoot >/dev/null 2>/dev/null
curDate=`date '+%Y%m%d'`
#echo $curDate
curTime=`date '+%H%M-%S'`
#echo $curTime
logfileName=$logRoot/$curDate/$BUILD_NUMBER-$curDate-$curTime
#echo $logfileName
mkdir $logRoot/$curDate >/dev/null 2>/dev/null

apkSrcPath=./bin/$PackageName-release.apk
soPath=./libs/armeabi/libgame.so
apkTargetDir=$packageroot/$curDate
soTargetDir=$soRootPath/$curDate
mkdir $soTargetDir >/dev/null 2>/dev/null

apkTargetPath=$apkTargetDir/$BUILD_NUMBER-$ProjectName-$curDate-$curTime.apk
apkTargetLastPath=$apkTargetDir/$PackageName.apk

if [ ! -n "$ANDROID_HOME" ]; then
echo "Error:Need to specify ANDROID_HOME first"
exit -1
fi

echo "${Info}0.change Version Name${Normal}"
echo "****************Need SVN update*********************"
FILENAME="../../Android_Config/versionName.bin"
cat $FILENAME | while read LINE
do
	echo $LINE
	sed -i.bak 's/'$LINE'/g' AndroidManifest.xml
	rm -rf *.bak
done
echo "[Done]"
echo ""

echo "building start with log:$logfileName"
echo "*************************************************"
versionCode=$defaultVersionCode
if [ ! -n "$BUILD_NUMBER" ]; then
	read -t 5 -p "No availble version code, please input version code:" versionInput;
	if [[ -z $versionInput ]];then
		echo "\nNo Version Code Input, Use Default"
	else 
		if [[ ! $versionInput =~ '^[0-9]+$' ]];then
			echo "Wrong Version Code Input, Use Default"
		else
			# echo "Version Code Input: $versionInput"
			versionCode=$versionInput
		fi
	fi

else
	versionCode=$BUILD_NUMBER

fi
echo "VersionCode Use ${Info}$versionCode${Normal}"
sed -i.bak 's/'versionCode=\".*\"/versionCode=\"$versionCode\"'/g' AndroidManifest.xml
rm -rf *.bak

#Packaing Processing
if $RepackRes ; then

	cd $scriptRoot/../../Android_Resource >/dev/null 2>/dev/null
	rm -rf *

	cd $scriptRoot/../../CCB/IF >/dev/null 2>/dev/null
	echo "0.pack_all.command..."
	 sh pack_android.sh
	echo "[Done]"
	echo ""

cd $scriptRoot/../../IF/Resources >/dev/null 2>/dev/null
echo "remove ios resources"
rm -rf Icon*.png 
rm -rf Default*.png
cd $scriptRoot
else
	echo "${Info}0.Skip Pack Resource${Normal}"
fi

#Native Processing
if $ReNative ; then
#cd - >/dev/null 2>/dev/null
cd $scriptRoot/../../proj.android/jni >/dev/null 2>/dev/null
echo "1.Making inclues and source..."
sh mksrc.sh
echo "[Done]"
echo ""

cd $scriptRoot/../../proj.android >/dev/null 2>/dev/null
echo "2.Building for libgame.so..."
#echo $logfileName
sh build_native.sh  #>$logfileName.log 2>$logfileName.err
if $ReleaseOut ; then
python build_native.py -b release
else
python build_native.py
fi

echo "copy shareSDK.xml $PackageName "
cp $scriptRoot/ShareSDK.xml assets

if [ -f "$soPath" ]; then
	echo "[Done]"
	echo ""
	rm $logfileName.err
else
	echo "[Failed]:$errorMsg"
	exit -1
fi

cd $scriptRoot/assets


rm -rf Battle/_alpha_Battle_a034_alpha.pkm
rm -rf Battle/_alpha_Battle_a034.pkm
rm -rf Battle/Battle_11.pkm

cd $scriptRoot
cp -rf libs/local/ assets >/dev/null


mkdir $soTargetDir/IF_$BUILD_NUMBER >/dev/null 2>/dev/null
copyPath=$soTargetDir/IF_$BUILD_NUMBER/
echo $copyPath
cp -rf ../../proj.android/obj/local/armeabi $copyPath >/dev/null

else
	echo "${Info}2. Skip Including And Sourcing"
	echo "3. Skip LibGame.So Compile${Normal}"
fi


#Ant Clean
echo "${Info}3.Cleaning for packing APK...${Normal}"
cd $scriptRoot
ant clean #>$logfileName.log 2>$logfileName.err
# errorMsg=`cat $logfileName.err`

echo "${Info}4.Packing APK...${Normal}"
cd $scriptRoot
if $ReleaseOut; then
ant release #>$logfileName.log 2>$logfileName.err
else
ant debug #>$logfileName.log 2>$logfileName.err
fi



mkdir $apkTargetDir >/dev/null 2>/dev/null
echo "5.Copying from [$apkSrcPath] to [$apkTargetPath]..."
cp $apkSrcPath $apkTargetPath >/dev/null
cp -f $apkSrcPath $apkTargetLastPath >/dev/null
echo "[Done]"
echo ""


if $CrashUpload ; then
   ant crashlytics-symbols
else
	echo "${Info}Skip CrashUpload${Normal}"
fi

if $DelManifest ; then
rm -rf AndroidManifest.xml
fi

installScriptPath=$apkTargetDir/install.sh
echo "6.Making install script..."

	echo 'echo "1.Waiting for device..."' > $installScriptPath
	echo 'adb wait-for-device' >> $installScriptPath
	echo 'echo "Done"' >> $installScriptPath
	echo 'echo ""' >> $installScriptPath

	echo 'echo "2.Uninstalling old package..."' >> $installScriptPath
	echo 'adb uninstall org.elex.IF' >> $installScriptPath
	echo 'echo "[Done]"' >> $installScriptPath
	echo 'echo ""' >> $installScriptPath

	echo 'echo "3.Installing new package..."' >> $installScriptPath
	echo "adb install IF_debug.apk" >> $installScriptPath
	echo 'echo "[Done]"' >> $installScriptPath
	echo 'echo ""' >> $installScriptPath

	echo 'echo "****************************"' >> $installScriptPath
	echo 'echo "Complete"' >> $installScriptPath

chmod 777 $installScriptPath
echo "[Done]"
echo ""

uploadScriptPath=$apkTargetDir/upload.sh
echo "7.apk upload script..."
uploadSrcPath=./upload.sh
cp $uploadSrcPath $apkTargetDir >/dev/null

echo "[Done]"
echo ""


echo "*************************************************"

