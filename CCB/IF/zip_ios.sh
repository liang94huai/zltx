#!/bin/bash

cd `dirname $0`

echo "---------ccbi-----------"
dir="../../IF/Resources/ccbi"
zip_file="ccbi.zip"
mkdir_dir="ccbi/"

cd $dir

svn update

rm -fr $zip_file
rm -fr .DS_Store
rm -fr $mkdir_dir

mkdir $mkdir_dir
cp -f *.* ${mkdir_dir}/

zip -rm $zip_file $mkdir_dir

# rm -fr $mkdir_dir

files=($zip_file "gameUI.ccbi" "gameUICoin.ccbi" "gameUILongJing.ccbi"  "FlyCBuildView.ccbi" "YesNoDialog.ccbi" "ServerUpdateView.ccbi" "FlyHintCCB.ccbi")

for file in `ls`
do
	isExits=false

	len=${#files[@]}
	for (( i = 0; i < len; i++ )); do

		if [[ "$file" == ${files[i]} ]]; then
			isExits=true
			break
		fi
	done
	
	if [[ $isExits == true ]]; then
		# echo $isExits
		continue
	fi
	
	# echo $file

	rm -f $file

done

echo "---------particle-----------"
dir="../particle"
zip_file="particle.zip"
mkdir_dir="particle/"

cd $dir

svn update

rm -fr $zip_file
rm -fr .DS_Store
rm -fr $mkdir_dir

mkdir $mkdir_dir
cp -f *.* ${mkdir_dir}/

zip -rm $zip_file $mkdir_dir

# rm -fr $mkdir_dir

files=($zip_file "Loading_1.plist" "Loading_2.plist" "Loading_3.plist" "LoadingLOGO_1.plist" "LoadingLOGO_2.plist" "LoadingMan_snow_front.plist" "LoadingMan_snow_back.plist" "LoadingFireNew_1.plist" "LoadingFireNew_2.plist" "LoadingFireNew_3.plist" "LoadingFireNew_4.plist" "LoadingFireNew_5.plist" "LoadingFireNew_6.plist" "LoadingFireNew_7.plist" "LoadingFireNew_8.plist" "LoadingFireNew_9.plist")

for file in `ls`
do
	isExits=false

	len=${#files[@]}
	for (( i = 0; i < len; i++ )); do

		if [[ "$file" == ${files[i]} ]]; then
			isExits=true
			break
		fi
	done
	
	if [[ $isExits == true ]]; then
		# echo $isExits
		continue
	fi
	
	# echo $file

	rm -f $file

done

echo "---------hdccbi-----------"
dir="../hdccbi"
zip_file="hdccbi.zip"
mkdir_dir="hdccbi/"

cd $dir

svn update

rm -fr $zip_file
rm -fr .DS_Store
rm -fr $mkdir_dir

mkdir $mkdir_dir
cp -f *.* ${mkdir_dir}/

files=($zip_file "gameUI_HD.ccbi" "gameUI_HD_title.ccbi" "FlyCBuildView.ccbi" "YesNoDialog.ccbi" "ServerUpdateView.ccbi" "FlyHintCCB.ccbi")

for file in `ls`
do
	isExits=false

	len=${#files[@]}
	for (( i = 0; i < len; i++ )); do

		if [[ "$file" == ${files[i]} ]]; then
			isExits=true
			break
		fi
	done
	
	if [[ $isExits == true ]]; then
		# echo $isExits
		continue
	fi
	
	# echo $file

	rm -f $file

done

zip -rm $zip_file $mkdir_dir

echo "---------Spine-----------"
dir="../Spine"
zip_file="Spine.zip"
mkdir_dir="Spine/"

cd $dir

svn update

rm -fr $zip_file
rm -fr .DS_Store
rm -fr $mkdir_dir

mkdir $mkdir_dir
mv -f Imperial ${mkdir_dir}/
mv -f Single ${mkdir_dir}/
mv -f UIComponent ${mkdir_dir}/
mv -f World ${mkdir_dir}/

zip -rm $zip_file $mkdir_dir


