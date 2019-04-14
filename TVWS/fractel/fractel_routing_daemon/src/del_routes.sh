routes=`route | grep 255.255.255.255 | cut -f1 -d" " | xargs echo`
for i in $routes
do
	echo $i
	route del -host $i
done
