cd build
cmake ..
make
RESULT=$?
cd ..
if [ $RESULT == 0 ]; then
	build/Gyurchine
fi
