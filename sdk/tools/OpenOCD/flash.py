#! /usr/bin/env python
# Copyright (C) 2016 Marvell International Ltd.
# All Rights Reserved.

# Flash programming wrapper/helper script (using OpenOCD flash commands)
# Note: sys.stdout.flush() and sys.stderr.flush() are required for proper
# console output in eclipse

import os, sys, platform, getopt, subprocess
from sys import platform as _platform

IFC_FILE = (os.getenv("DEBUG_INTERFACE", "ftdi") or "ftdi") + '.cfg'
BLOCK_SIZE = 0x10000
components = [['FC_COMP_FW','0x10000','0x60000','0','mcufw']]
comp_blob = ['FC_COMP_ALL','0x0','0x7FFF0000','0','all']

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
OPENOCD = ""

# We define which as it may not be available on Windows
def which(program):
    if _platform == "win32" or _platform == "win64":
        program = program + '.exe'

    def is_exe(fpath):
        return os.path.isfile(fpath) and os.access(fpath, os.X_OK)

    fpath, fname = os.path.split(program)
    if fpath:
        if is_exe(program):
            return program
    else:
        for path in os.environ["PATH"].split(os.pathsep):
            path = path.strip('"')
            exe_file = os.path.join(path, program)
            if is_exe(exe_file):
                return exe_file
    return ""

def get_openocd():
    global OPENOCD
    if _platform == "linux" or _platform == "linux2":
        if (platform.machine() == "i686"):
            OPENOCD = which(SCRIPT_DIR + "/Linux/openocd")
        else:
            OPENOCD = which(SCRIPT_DIR + "/Linux/openocd64")
    elif _platform == "darwin":
        OPENOCD = which("openocd")
    elif _platform == "win32" or _platform == "win64":
        OPENOCD = which(SCRIPT_DIR + "/Windows/openocd")
    if not len(OPENOCD):
        print "Error: Please install OpenOCD for your platform"
        sys.exit()

def file_path(file_name):
    if _platform == "win32" or _platform == "win64":
        return file_name.replace('\\', '/')
    else:
        return file_name

def cleanup():
    if (os.path.exists("config.cfg")):
        os.remove("config.cfg")

def exit():
    cleanup()
    sys.exit()

def print_usage():
    print ""
    print "Usage:"
    print sys.argv[0] + " [options]"
    print "Optional Usage:"
    print " [<-i | --interface> <JTAG hardware interface name>]"
    print "          Supported ones are ftdi and stlink. Default is ftdi."
    print " [--mcufw </path/to/mcufw>]"
    print "          Write MCU firmware binary <bin> to flash"
    print " [<-f | --flash> </path/to/flash_blob>]"
    print "          Program entire flash"
    print " [-r | --reset]"
    print "          Reset board"
    print " [-h | --help]"
    print "          Display usage"
    sys.stdout.flush()

def create_config():
    with open ('config.cfg','w') as cfile:
        cfile.write("# This is an auto-generated config file\n")
        cfile.write("set CONFIG_FLASH flash\n")

def component_info(comp_name):
    for comp in components:
        if (comp[4] == comp_name):
            if (int(comp[1], 0) % BLOCK_SIZE) or (int(comp[2], 0) % BLOCK_SIZE):
                print "Error: " + comp_name + " address or size is not " + format(BLOCK_SIZE, '#x') + " aligned."
                exit()
            return comp
    print "Error: component " + comp_name + " not found."
    exit()

def flash_file(comp, msg, upload_file):
    if (os.path.isfile(upload_file) == False):
        print "Error: File " + upload_file + " does not exit"
        exit()
    if (os.stat(upload_file).st_size > int(comp[2], 0)):
        print "Error: File size (" + format(os.stat(upload_file).st_size, "#x") + " bytes) is larger than " + comp[4] + " size (" + comp[2] + ")"
        exit()
    print msg + "..."
    print "Please wait while flashing is complete. DO NOT PRESS Ctrl+C..."

    print "Using OpenOCD interface file", IFC_FILE
    sys.stdout.flush()
    p = subprocess.call([OPENOCD, '-s', SCRIPT_DIR + '/interface', '-f', IFC_FILE, '-s', SCRIPT_DIR, '-f', 'config.cfg', '-f', 'openocd.cfg', '-c', ' init', '-c', 'program_image ' + comp[1] + ' ' + upload_file , '-c', 'shutdown'])
    sys.stderr.flush()
    if (p==0):
        print msg + " done..."
    else:
        print msg + " failed..."
    sys.stdout.flush()

def reset_board():
    msg = "Resetting board"
    print msg + "..."

    print "Using OpenOCD interface file", IFC_FILE
    sys.stdout.flush()
    p = subprocess.call([OPENOCD, '-s', SCRIPT_DIR + '/interface', '-f', IFC_FILE, '-s', SCRIPT_DIR, '-f', 'openocd.cfg', '-c', 'init', '-c', 'reset', '-c', 'shutdown'])
    sys.stderr.flush()
    if (p==0):
        print msg + " done..."
    else:
        print msg + " failed..."
    sys.stdout.flush()

def main():
    global IFC_FILE
    global SCRIPT_DIR
    SCRIPT_DIR = file_path(SCRIPT_DIR)
    BLOB_FILE = ""
    RESET_BOARD = 0
    get_openocd()

    if len(sys.argv) <= 1:
        print_usage()
        exit()

    try:
        opts, args = getopt.gnu_getopt(sys.argv[1:], "i:f:rh", ["interface=","flash=","mcufw=","reset","help"])
        if len(args):
            print_usage()
            exit()
    except getopt.GetoptError as e:
        print e
        print_usage()
        exit()

    cleanup()

    # This is special handling of arguments, remove options after using
    for opt, arg in reversed(opts):
        if opt in ("-i", "--interface"):
            IFC_FILE = arg + '.cfg'
            opts.remove((opt, arg))
        elif opt in ("-f", "--flash"):
            BLOB_FILE = file_path(arg)
            opts.remove((opt, arg))
        elif opt in ("-r", "--reset"):
            RESET_BOARD = 1
            opts.remove((opt, arg))
        elif opt in ("-h", "--help"):
            print_usage()
            sys.exit()

    create_config()

    if len(BLOB_FILE):
        flash_file(comp_blob, "Resetting flash to factory settings", BLOB_FILE)

    if (len(opts) == 0):
        if (RESET_BOARD == 1):
            reset_board()
        exit()

    for opt, arg in opts:
        if opt in ("--mcufw"):
            flash_file(component_info('mcufw'), 'Writing MCU firmware to flash', file_path(arg))

    cleanup()

    if (RESET_BOARD == 1):
        reset_board()

if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        cleanup()
