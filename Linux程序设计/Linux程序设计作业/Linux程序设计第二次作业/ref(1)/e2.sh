#########################################################################
# File Name: e2.sh
# Author: liujing
# mail: jingliu.cs@gmail.com
# Created Time: 2015年01月22日 星期四 23时15分47秒
#########################################################################
#!/bin/bash

echo '1) Continue'
echo '2) Finish'

#read -p "#?" var
var=1

while [ "$var" -ne 2 ]
do
	read -p "#?" var
	if [ "$var" -ne 1 -a "$var" -ne 2 ];then
		echo 'Wrong choice! Please select again'
	fi
done

exit 0
