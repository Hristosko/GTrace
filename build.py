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
    subprocess.run(['cmake', '-H.', '-B' + build_dir])
    with cd(build_dir):
        subprocess.run(['make'])

def run_tests(filter, repeat):
    gtest_filter = '--gtest_filter=*' + filter + '*'
    gtest_repeat = '--gtest_repeat=' + str(repeat)
    subprocess.run([build_directory() + '/unit_tests/GTaceUnitTests', gtest_filter, gtest_repeat])

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('action', choices=['clean', 'make', 'rebuild', 'test'])
    parser.add_argument('--target', default='*')
    parser.add_argument('--repeat', default=1)
    args = parser.parse_args()
    print(args)
    actions = {
        'clean' : lambda: shutil.rmtree(build_directory()),
        'make' : compile,
        'rebuild': lambda: [shutil.rmtree(build_directory()), compile()],
        'test' : lambda: run_tests(args.target, args.repeat),
    }
    actions[args.action]()
