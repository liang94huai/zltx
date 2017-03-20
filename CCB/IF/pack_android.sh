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


            if [[ "$fullFileName" == "_alpha_World_1"  ||  "$fullFileName" == "_alpha_World_4" || "$fullFileName" == "_alpha_World_7" ||  "$fullFileName" == "_alpha_World_8" ]]; then
                # tiledmap 
                TexturePacker --texture-format png --format cocos2d --png-opt-level 0 --opt RGBA8888 --reduce-border-artifacts --size-constraints POT --scale 1 --scale-mode Smooth --algorithm Basic --basic-sort-by Best --basic-order Ascending --pack-mode Best --enable-rotation --trim-mode None --shape-padding 0 --border-padding 0 --disable-rotation --disable-auto-alias --force-word-aligned --sheet "$fullFileName.png" "$fileName"/*.png --data "$fileName.plist"         
            elif [[ "$fullFileName" == "_alpha_World_3"  || "$fullFileName" == "_alpha_World_5" || "$fullFileName" == "_alpha_Imperial_30" || "$fullFileName" == "_alpha_Imperial_31" || "$fullFileName" == "_alpha_Loading_1" || "$fullFileName" == "_alpha_Common_511" || "$fullFileName" == "_alpha_Loading_3" || "$fullFileName" == "_alpha_UIComponent_1" ]]; then
                # spine
                TexturePacker --format libgdx --texture-format png --png-opt-level 0 --opt RGBA8888 --reduce-border-artifacts --size-constraints AnySize --scale 1 --scale-mode Smooth --algorithm MaxRects --maxrects-heuristics Best --pack-mode Best --enable-rotation --trim-mode Trim --disable-auto-alias --force-word-aligned --sheet "$fullFileName.png" "$fileName"/*.png --data "$fileName.atlas"
			elif [[ "$fullFileName" == "_alpha_Common_505" || "$fullFileName" == "_alpha_World_9" || "$fullFileName" == "_alpha_World_10" || "$fullFileName" == "_alpha_World_11" || "$fullFileName" == "_alpha_World_12" || "$fullFileName" == "_alpha_Common_2" ]]; then
                TexturePacker --texture-format png --format cocos2d --png-opt-level 0 --opt RGBA8888 --premultiply-alpha --reduce-border-artifacts --size-constraints AnySize --scale 1 --scale-mode Smooth --algorithm MaxRects --maxrects-heuristics Best --pack-mode Best --shape-padding 0 --border-padding 0 --enable-rotation --trim-mode Trim --sheet "$fullFileName.png" "$fileName"/*.png --data "$fileName.plist" 
			elif [[ "$fullFileName" == "_alpha_Particle"  ]]; then
                TexturePacker --texture-format png --format cocos2d --png-opt-level 0 --opt RGBA8888 --premultiply-alpha --reduce-border-artifacts --size-constraints NPOT --scale 1 --scale-mode Smooth --algorithm MaxRects --maxrects-heuristics LongSideFit --pack-mode Best --disable-rotation --trim-mode None --shape-padding 0 --border-padding 0 --sheet "$fullFileName.png" "$fileName"/*.png --data "$fileName.plist"
            elif [[ "$fullFileName" == "_alpha_Common_3" ]]; then
                TexturePacker --texture-format png --format cocos2d --png-opt-level 0 --opt RGBA8888 --premultiply-alpha --reduce-border-artifacts --size-constraints AnySize --scale 1 --scale-mode Smooth --algorithm MaxRects --maxrects-heuristics Best --pack-mode Best --enable-rotation --trim-mode Trim --sheet "$fullFileName.png" "$fileName"/*.png --data "$fileName.plist"
            elif [[ "$fullFileName" == "_alpha_World_6" ]] || [[ "$fullFileName" == "_alpha_Common_8" || "$fullFileName" == "_alpha_Common_11" || "$fullFileName" == "_alpha_Common_7" ]]; then
                TexturePacker --texture-format png --format cocos2d --png-opt-level 0 --opt RGBA8888 --premultiply-alpha --reduce-border-artifacts --size-constraints NPOT --scale 1 --scale-mode Smooth --algorithm MaxRects --maxrects-heuristics LongSideFit --pack-mode Best --disable-rotation --trim-mode None --shape-padding 0 --border-padding 0 --sheet "$fullFileName.png" "$fileName"/*.png --data "$fileName.plist"
           else
                # others
                TexturePacker --texture-format png --format cocos2d --png-opt-level 0 --opt RGBA8888 --reduce-border-artifacts --size-constraints AnySize --scale 1 --scale-mode Smooth --algorithm MaxRects --maxrects-heuristics Best --pack-mode Best --enable-rotation --trim-mode Trim --sheet "$fullFileName.png" "$fileName"/*.png --data "$fileName.plist"
			fi

            if [[ "$fullFileName" == "_alpha_World_3"  || "$fullFileName" == "_alpha_World_5" || "$fullFileName" == "_alpha_Imperial_30" || "$fullFileName" == "_alpha_Imperial_31" || "$fullFileName" == "_alpha_Loading_1" || "$fullFileName" == "_alpha_Common_511" || "$fullFileName" == "_alpha_Loading_3" || "$fullFileName" == "_alpha_UIComponent_1" ]]; then
                # fix atlas file from .png to .pkm
                sed -i.bak 's/'$fullFileName'.png/'$fullFileName'.pkm/g' $fileName.atlas
                rm -rf *.bak
            else
                # fix plist file from .png to .pkm
                sed -i.bak 's/'$fullFileName'.png/'$fullFileName'.pkm/g' $fileName.plist
                rm -rf *.bak
            fi

			# remember PWD for the stupid etcpack
			filePWD=$PWD
			cd "../bin"
			if [[ "$fullFileName" == "$fileName" ]]; then
				# NO alpha channel
				./etcpack "$filePWD/$fullFileName.png" "$filePWD" -c etc1
			else
				# HAVE alpha channel
				./etcpack "$filePWD/$fullFileName.png" "$filePWD" -c etc1
				TexturePacker --texture-format png --png-opt-level 0 --opt ALPHA  --size-constraints AnySize --disable-rotation --border-padding 0  --no-trim --sheet "$filePWD/tempalpha.png" --data "$filePWD/drop.plist" "$filePWD/$fullFileName.png"
				./etcpack "$filePWD/tempalpha.png" "$filePWD" -c etc1
				mv "$filePWD/tempalpha.pkm"  $filePWD'/'$fullFileName'_alpha.pkm'
				rm -f "$filePWD/drop.plist"
				rm -f "$filePWD/tempalpha.png"
				# ./etcpack "$filePWD/$fullFileName.png" "$filePWD" -c etc1 -as				
			fi

			cd $filePWD
			desDir="$filePWD/../../../Android_Resource/$fileType"
			if [[ ! -d $desDir ]]; then
				echo "Creating dir : $desDir"
				mkdir $desDir
			fi

            if [[ "$fullFileName" == "_alpha_World_3"  || "$fullFileName" == "_alpha_World_5"  || "$fullFileName" == "_alpha_Imperial_30" ||"$fullFileName" == "_alpha_Imperial_31" || "$fullFileName" == "_alpha_Loading_1" || "$fullFileName" == "_alpha_Common_511" || "$fullFileName" == "_alpha_Loading_3" || "$fullFileName" == "_alpha_UIComponent_1" ]]; then
                if [[ "$fullFileName" == "$fileName" ]]; then
                    # NO alpha channel
                    cp "$fullFileName.pkm" "$fileName.atlas" $desDir
                else
                    # HAVE alpha channel
                    cp "$fullFileName.pkm" $fullFileName'_alpha.pkm' "$fileName.atlas" $desDir
                    rm -rf $fullFileName'_alpha.pkm'
                fi
                rm -rf "$fullFileName.pkm" "$fullFileName.png" "$fileName.atlas"
            else
                if [[ "$fullFileName" == "$fileName" ]]; then
                    # NO alpha channel
                    cp "$fullFileName.pkm" "$fileName.plist" $desDir
                else
                    # HAVE alpha channel
                    cp "$fullFileName.pkm" $fullFileName'_alpha.pkm' "$fileName.plist" $desDir
                    rm -rf $fullFileName'_alpha.pkm'				
                fi
                rm -rf "$fullFileName.pkm" "$fullFileName.png" "$fileName.plist"
            fi

			echo "File $fileName finished!"
		done
	)
done

# plist to bin
echo "start plist to bin==================>"
desDir="../../Android_Resource/"
python plistTobin.py $desDir
