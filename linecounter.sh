x=0
for entry in `find . -type f -iname "*.c"`; do
	y=$(cat $entry | wc -l)
	x=$((x+y))
done
for entry in `find . -type f -iname "*.h"`; do
	y=$(cat $entry | wc -l)
	x=$((x+y))
done
echo $x
