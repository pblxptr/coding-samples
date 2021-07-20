#!/usr/bin/env python

import argparse
import pathlib
from collections import defaultdict

DEFAULT_CPP_VERSION = '20'

SRC_CMAKE_NAME = 'CMakeLists-default'
DEST_CMAKE_NAME = 'CMakeLists.txt'

SRC_MAIN_NAME = 'main-default'
DEST_MAIN_NAME = 'main.cpp'


def create_directories(proj):
    p = pathlib.Path(__file__).parent
    root_dir = p / proj
    build_dir = root_dir / 'build'

    if not root_dir.exists():
        root_dir.mkdir()
    
    if not build_dir.exists():
        build_dir.mkdir()


def create_cmake(proj):
    current_path = pathlib.Path(__file__).parent
    src_cmake  = current_path / SRC_CMAKE_NAME
    dest_cmake = current_path / proj / DEST_CMAKE_NAME

    src_cmake_data = ''

    with open(str(src_cmake), 'r') as file:
        src_cmake_data  = file.read()
    
    src_cmake_data = src_cmake_data.format_map(defaultdict(str, 
        project_name =proj, 
        cpp_version=DEFAULT_CPP_VERSION,
        CMAKE_PROJECT_NAME="{CMAKE_PROJECT_NAME}"
    ))

    dest_cmake.touch()
    dest_cmake.write_text(src_cmake_data)

def create_main(proj):
    current_path = pathlib.Path(__file__).parent
    src_main = current_path / SRC_MAIN_NAME
    dest_main = current_path / proj / DEST_MAIN_NAME

    src_main_data = ''

    with open(str(src_main), 'r') as file:
        src_main_data = file.read()

    dest_main.touch()
    dest_main.write_text(src_main_data)

parser = argparse.ArgumentParser(description='Create new coding sample directory.')
parser.add_argument('project', help='Directory name')
args = parser.parse_args()


create_directories(args.project)
create_cmake(args.project)
create_main(args.project)
