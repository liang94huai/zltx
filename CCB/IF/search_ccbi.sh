#bin/sh


curDate=`date '+%Y%m%d'`
#echo $curDate
curTime=`date '+%H%M-%S'`
log_file=${curDate}-${curTime}_ccbi
log_file_zero=${log_file}_zero

ccbidir=../../IF/Resources
classdir=../../IF/Classes

echo "-----$dir-----"
for ccbi in `ls -R $ccbidir |grep '.ccbi'` 
do
	base=`echo '"'${ccbi%.*}`
	echo $base
	row=`grep $base -R $classdir/* |wc -l`
	# row2=`grep $base -R $classdir/* |wc -l`

	total=`expr $row `
	echo $ccbi'-----'$total
	echo $ccbi'-----'$total 1>>$log_file.log
	if [[ $total -eq 0  ]]; then
		echo $ccbi'-----'$total 1>>$log_file_zero.log
	fi
	
done

