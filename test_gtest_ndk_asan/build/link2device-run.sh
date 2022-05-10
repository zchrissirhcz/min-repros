#!/bin/bash

testbed()
{
    SID=561a22aa # Meizu 17 Pro.   Note: use `arcdb -l` to see all devices.
    DST_DIR=/data/local/tmp/ws/zz9555
    AUTH="-usr zz-new -pw 123456 -s $SID -reconn 5"
    arcdb $AUTH shell pwd
    arcdb $AUTH shell ls
    arcdb $AUTH shell mkdir -p zz9555
    # arcdb $AUTH push /home/zz/data/1920x1080.png $DST_DIR/
    arcdb $AUTH push ./android-arm64/testbed $DST_DIR/testbed
    arcdb $AUTH shell chmod +x $DST_DIR/testbed
    arcdb $AUTH shell $DST_DIR/testbed
}

testbed