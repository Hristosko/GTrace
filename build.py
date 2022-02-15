import argparse
import os
import subprocess
import shutil
from sys import platform

class cd:
    """Context manager for changing the current working directory"""
    def __init__(self, newPath):
        self.newPath = os.path.expanduser(newPath)

    def __enter__(self):
        self.savedPath = os.getcwd()
        os.chdir(self.newPath)

    def __exit__(self, etype, value, traceback):
        os.chdir(self.savedPath)

def remove_file(path):
    if os.path.isfile(path):
        os.remove(path)

def make_dir(path):
    if not os.path.isdir(path):
        os.mkdir(path)

def build_directory():
    build_dir = os.path.join(os.curdir, 'build')
    subdir = 'win' if platform == 'win32' else 'lin'
    make_dir(build_dir)
    build_dir = os.path.join(build_dir, subdir)
    make_dir(build_dir)
    return build_dir

def compile():
    build_dir = build_directory()
    compiler = []
    if platform == 'lin':
        compiler = ['-D' ,'CMAKE_CXX_COMPILER=gcc-9']
    subprocess.run(['cmake', '-H.', '-B' + build_dir] + compiler)
    with cd(build_dir):
        subprocess.run(['make', '-j', '16'])

def run_tests(filter, repeat):
    gtest_filter = '--gtest_filter=*' + filter + '*'
    gtest_repeat = '--gtest_repeat=' + str(repeat)
    bin_path = os.path.join(build_directory(), 'unit_tests', 'GTaceUnitTests')
    subprocess.run([bin_path, gtest_filter, gtest_repeat])

def render(scene):
    bin_path = os.path.join(build_directory(), 'GtraceConsole', 'GTaceConsole')
    output_file = "script.grt"
    subprocess.run([bin_path, scene, output_file])

def is_sorce_file(file):
    return file.endswith('.h') or file.endswith('.cpp')

def format_dir(dir):
    for root, _, files in os.walk(dir):
        for file in files:
            if is_sorce_file(file):
                file_path = os.path.join(root, file)
                subprocess.run(['clang-format', '-i', file_path])

def format():
    format_dir(os.path.join('.', 'GtraceLib'))
    format_dir(os.path.join('.', 'unit_tests'))

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('action', choices=['clean', 'make', 'rebuild', 'test', 'render', 'format'])
    parser.add_argument('--target', default='*')
    parser.add_argument('--repeat', default=1)
    args = parser.parse_args()
    print(args)
    actions = {
        'clean' : lambda: shutil.rmtree(build_directory()),
        'make' : compile,
        'rebuild': lambda: [shutil.rmtree(build_directory()), compile()],
        'test' : lambda: run_tests(args.target, args.repeat),
        'render': lambda: render(args.target),
        'format': format
    }
    actions[args.action]()
