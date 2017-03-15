cd `dirname $0`
excludeList='bin/':'Resources/'
for dir in **/
do
	(
		if [[ $excludeList =~ $dir ]]; then
			echo "This dir do not need pack : $dir"
			break
		fi
		fileType=${dir%%/}
		echo "The file type is $fileType"
		cd $dir
		for tpsFile in ` find . -name "*.tps" `
		do
			fullFileName=${tpsFile##./}
			fullFileName=${fullFileName%%.tps}
			fileName=${fullFileName##_alpha_}
            if [[ "$fullFileName" == "_alpha_World_3" || "$fullFileName" == "_alpha_World_5" || "$fullFileName" == "_alpha_Imperial_30" || "$fullFileName" == "_alpha_Imperial_31"|| "$fullFileName" == "_alpha_Common_511"|| "$fullFileName" == "_alpha_Loading_1"  || "$fullFileName" == "_alpha_Loading_3"  || "$fullFileName" == "_alpha_UIComponent_1"  ]]; then
                TexturePacker $tpsFile
            elif [[ "$fullFileName" == "_alpha_Common_9"  ]]; then
                TexturePacker $tpsFile
			elif [[ "$fullFileName" == "_alpha_World_1" ]]; then
				TexturePacker --opt PVRTC4  $tpsFile  
			elif [[ "$fullFileName" == "_alpha_Common_512" || "$fullFileName" == "_alpha_Common_502" || "$fullFileName" == "_alpha_Common_500" || "$fullFileName" == "_alpha_Common_4" || "$fullFileName" == "_alpha_Common_5"  || "$fullFileName" == "_alpha_Common_6" || "$fullFileName" == "_alpha_Common_7"  || "$fullFileName" == "_alpha_Common_8" || "$fullFileName" == "_alpha_Common_11" || "$fullFileName" == "_alpha_Common_100" || "$fullFileName" == "_alpha_Common_101" || "$fullFileName" == "_alpha_Common_102" || "$fullFileName" == "_alpha_Common_104" || "$fullFileName" == "_alpha_Common_105" || "$fullFileName" == "_alpha_Common_200" || "$fullFileName" == "_alpha_Common_201" || "$fullFileName" == "_alpha_Common_202" || "$fullFileName" == "_alpha_Common_203"  || "$fullFileName" == "_alpha_Common_204" || "$fullFileName" == "_alpha_Common_205" || "$fullFileName" == "_alpha_Common_206" || "$fullFileName" == "_alpha_Common_207" || "$fullFileName" == "_alpha_Common_208" || "$fullFileName" == "_alpha_Common_209" || "$fullFileName" == "_alpha_Common_305" || "$fullFileName" == "_alpha_Common_308" || "$fullFileName" == "_alpha_Common_310" || "$fullFileName" == "_alpha_Common_501" || "$fullFileName" == "_alpha_Common_503" || "$fullFileName" == "_alpha_Common_504" || "$fullFileName" == "_alpha_Common_505" || "$fullFileName" == "_alpha_Common_506" || "$fullFileName" == "_alpha_Common_508" || "$fullFileName" == "_alpha_Common_509" || "$fullFileName" == "_alpha_Common_10" || "$fullFileName" == "_alpha_Common_307" || "$fullFileName" == "_alpha_Common_513" || "$fullFileName" == "_alpha_Common_514" || "$fullFileName" == "General_1" || "$fullFileName" == "General_2" || "$fullFileName" == "General_3" ]]; then
				TexturePacker --opt PVRTC4 --force-squared --scale 0.5 --size-constraints POT --premultiply-alpha $tpsFile 
			else
				TexturePacker --opt PVRTC4 --force-squared --size-constraints POT --premultiply-alpha $tpsFile 			
			fi
		done
	)
done

# plist to bin
echo "start plist to bin==================>"
python plistTobin.py "../../IF/Resources/Battle"
python plistTobin.py "../../IF/Resources/Common"
python plistTobin.py "../../IF/Resources/General"
python plistTobin.py "../../IF/Resources/Imperial"
python plistTobin.py "../../IF/Resources/Loading"
python plistTobin.py "../../IF/Resources/World"

echo "start particle to bin==================>"
 # python particle_to_plist.py "../../IF/Resources/particle"
echo "zip  Resources=============>"
 sh zip_ios.sh
