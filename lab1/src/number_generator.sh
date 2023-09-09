for i in $(seq 1 $1)
do
	random=$(od -A n -t d -N 1 < /dev/random)
	echo $random
done
