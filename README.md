simulator: https://github.com/mdblack/simulator/raw/master/simulator.jar
credit for the simulator goes to mdblack, DaveChamberlain and Amorrison6

to verify:

run createOS.sh to create diskc.img in project folder you want to run

run diskc.img in the provided simulator using the diskc.img file in the project folder from the previous step

under hard drive/cd c: select the file using the browse feature and enter 63 for cylinders, 255 for heads and 1000 for sectors

click "boot disk C:", the second button from the left side fo the window near the center of the bottom half of the simulator

wait for disk to be booted from

enter what you would like to test the string input feature

pressing enter after you are satified with what is entered on the previous instruction to have what was entered repeated back to you

what is repeated will be the same as what you entered

it will print out it is going to read from a sector on the drive

after your input is repeated it will read from a sector on the disk then print out what was read from that sector