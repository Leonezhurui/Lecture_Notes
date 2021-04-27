#########################################################################
# File Name: e3.sh
# Author: liujing
# mail: jingliu.cs@gmail.com
# Created Time: 2015年01月23日 星期五 00时09分19秒
#########################################################################
#!/bin/bash

echo '0'
echo '1'

a0=0
a1=1
i=1

while [ "$i" -lt 9 ]
do
	a2=$(($a0+$a1))
	echo "$a2"
	a1=$a2
	a0=$a1
	i=$(($i+1))
done
exit 0
