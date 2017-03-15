#bin/sh


curDate=`date '+%Y%m%d'`
#echo $curDate
curTime=`date '+%H%M-%S'`
log_file=${curDate}-${curTime}
log_file_png_zero=${log_file}_png_zero

ccb_dir=Resources
hdccb_dir=DynamicResources
class_ldir=../../IF/Classes
xml_dir=../../IF/Resources/local/database.local.xml
spine_dir=../../IF/Resources/Spine
part_dir=../../IF/Resources/particle
part_dir2=../../IF/Resources/particleImg

pngdir=Common

echo "-----$pngdir-----"
for pig in `ls -R $pngdir |grep '.png'` 
do
	base=`echo '"'${pig%.*}`
	base2='>'$pig
	base3='"'$pig
	echo $base----$base2----$base3
	row=`grep $base2 -R $ccb_dir/* |wc -l`
	# row2=`grep $base2 -R $hdccb_dir/* |wc -l`
	row3=`grep $base -R $class_ldir/* |wc -l`
	row5=`cat ${xml_dir} |grep ${base} |wc -l`

	total=`expr $row + $row3 + $row5`
	echo $pig'-----'$total
	echo $pig"---ccb_dir:$row--class_ldir:$row3-xml_dir:$row5-spine_dir:$row6----"$total
	echo $pig"---ccb_dir:$row--class_ldir:$row3-xml_dir:$row5-spine_dir:$row6----"$total 1>>${log_file}_${pngdir}.log
	if [[ $total -eq 0 ]]; then
		echo $pig'-----'$total 1>>${log_file}_${pngdir}_zero.log
	fi
done

pngdir=Imperial

echo "-----$pngdir-----"
for pig in `ls -R $pngdir |grep '.png'` 
do
	base=`echo '"'${pig%.*}`
	# echo $base
	base2='>'$pig
	base3='"'$pig
	echo $base----$base2----$base3
	row=`grep $base2 -R $ccb_dir/* |wc -l`
	# row2=`grep $base2 -R $hdccb_dir/* |wc -l`
	row3=`grep $base -R $class_ldir/* |wc -l`
	row5=`cat ${xml_dir} |grep ${base} |wc -l`
	row6=`grep $base -R $spine_dir/* |wc -l`

	total=`expr $row + $row3 + $row5 + $row6`
	echo $pig"---ccb_dir:$row--class_ldir:$row3-xml_dir:$row5-spine_dir:$row6----"$total
	echo $pig"---ccb_dir:$row--class_ldir:$row3-xml_dir:$row5-spine_dir:$row6----"$total 1>>${log_file}_${pngdir}.log
	if [[ $total -eq 0 ]]; then
		echo $pig'-----'$total 1>>${log_file}_${pngdir}_zero.log
	fi
done


pngdir=Loading

echo "-----$pngdir-----"
for pig in `ls -R $pngdir |grep '.png'` 
do
	base=`echo '"'${pig%.*}`
	# echo $base
	base2='>'$pig
	base3='"'$pig
	echo $base----$base2----$base3
	row=`grep $base2 -R $ccb_dir/* |wc -l`
	# row2=`grep $base2 -R $hdccb_dir/* |wc -l`
	row3=`grep $base -R $class_ldir/* |wc -l`
	row5=`cat ${xml_dir} |grep ${base} |wc -l`
	row6=`grep $base -R $spine_dir/* |wc -l`

	total=`expr $row + $row3 + $row5 + $row6`
	# total=$row6
	echo $pig"---ccb_dir:$row--class_ldir:$row3-xml_dir:$row5-spine_dir:$row6----"$total
	echo $pig"---ccb_dir:$row--class_ldir:$row3-xml_dir:$row5-spine_dir:$row6----"$total 1>>${log_file}_${pngdir}.log
	if [[ $total -eq 0 ]]; then
		echo $pig'-----'$total 1>>${log_file}_${pngdir}_zero.log
	fi
done

# pngdir=Particle

# echo "-----$pngdir-----"
# for pig in `ls -R $pngdir |grep '.png'` 
# do
# 	base=`echo '"'${pig%.*}`
# 	echo $base
# 	row=`grep $pig -R $part_dir/* |wc -l`
# 	row2=`grep $pig -R $part_dir2/* |wc -l`
	
# 	total=`expr $row + $row2 `
# 	# total=$row6
# 	echo $pig"---part_dir:$row-part_dir2:$row2-----"$total
# 	echo $pig"---part_dir:$row-part_dir2:$row2-----"$total 1>>${log_file}_${pngdir}.log
# 	if [[ $total -eq 0 ]]; then
# 		echo $pig'-----'$total 1>>${log_file}_${pngdir}_zero.log
# 	fi
# done

pngdir=UIComponent

echo "-----$pngdir-----"
for pig in `ls -R $pngdir |grep '.png'` 
do
	base=`echo '"'${pig%.*}`
	# echo $base
	base2='>'$pig
	base3='"'$pig
	echo $base----$base2----$base3
	row=`grep $base2 -R $ccb_dir/* |wc -l`
	# row2=`grep $base2 -R $hdccb_dir/* |wc -l`
	row3=`grep $base -R $class_ldir/* |wc -l`
	row5=`cat ${xml_dir} |grep ${base} |wc -l`
	row6=`grep $base -R $spine_dir/* |wc -l`

	total=`expr $row + $row3 + $row5 + $row6`
	# total=$row6
	echo $pig"---ccb_dir:$row--class_ldir:$row3-xml_dir:$row5-spine_dir:$row6----"$total
	echo $pig"---ccb_dir:$row--class_ldir:$row3-xml_dir:$row5-spine_dir:$row6----"$total 1>>${log_file}_${pngdir}.log
	if [[ $total -eq 0 ]]; then
		echo $pig'-----'$total 1>>${log_file}_${pngdir}_zero.log
	fi
done

pngdir=World/World_2

echo "-----$pngdir-----"
for pig in `ls -R $pngdir |grep '.png'` 
do
	base=`echo '"'${pig%.*}`
	# echo $base
	base2='>'$pig
	base3='"'$pig
	echo $base----$base2----$base3
	row=`grep $base2 -R $ccb_dir/* |wc -l`
	# row2=`grep $base2 -R $hdccb_dir/* |wc -l`
	row3=`grep $base -R $class_ldir/* |wc -l`
	row5=`cat ${xml_dir} |grep ${base} |wc -l`
	row6=`grep $base -R $spine_dir/* |wc -l`

	total=`expr $row + $row3 + $row5 + $row6`
	# total=$row6
	echo $pig"---ccb_dir:$row--class_ldir:$row3-xml_dir:$row5-spine_dir:$row6----"$total
	echo $pig"---ccb_dir:$row--class_ldir:$row3-xml_dir:$row5-spine_dir:$row6----"$total 1>>${log_file}_World_2.log
	if [[ $total -eq 0 ]]; then
		echo $pig'-----'$total 1>>${log_file}_World_2_zero.log
	fi
done

pngdir=World/World_3

echo "-----$pngdir-----"
for pig in `ls -R $pngdir |grep '.png'` 
do
	base=`echo '"'${pig%.*}`
	# echo $base
	base2='>'$pig
	base3='"'$pig
	echo $base----$base2----$base3
	row=`grep $base2 -R $ccb_dir/* |wc -l`
	# row2=`grep $base2 -R $hdccb_dir/* |wc -l`
	row3=`grep $base -R $class_ldir/* |wc -l`
	row5=`cat ${xml_dir} |grep ${base} |wc -l`
	row6=`grep $base -R $spine_dir/* |wc -l`

	total=`expr $row + $row3 + $row5 + $row6`
	# total=$row6
	echo $pig"---ccb_dir:$row--class_ldir:$row3-${base}:$row5-spine_dir:$row6----"$total
	echo $pig"---ccb_dir:$row--class_ldir:$row3-${base}:$row5-spine_dir:$row6----"$total 1>>${log_file}_World_3.log
	if [[ $total -eq 0 ]]; then
		echo $pig'-----'$total 1>>${log_file}_World_3_zero.log
	fi
done
