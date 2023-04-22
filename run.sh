cd /home/olahb/Gyurchine/build
cmake ..
clear
make
cd ..
RESULT=$?
if [ $RESULT == 0 ]; then
	build/Gyurchine
fi
