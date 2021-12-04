import argparse
import os
import subprocess
import shutil

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

build_dir='./build'
def compile():
    make_dir(build_dir)
    subprocess.run(['cmake', '-H.', '-B' + build_dir])
    with cd(build_dir):
        subprocess.run(['make'])

def run_tests():
    subprocess.run([build_dir + '/unit_tests/GTaceUnitTests'])

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('action', choices=['clean', 'make', 'rebuild', 'test'])
    args = parser.parse_args()

    actions = {
        'clean' : lambda: shutil.rmtree(build_dir),
        'make' : compile,
        'rebuild': lambda: [shutil.rmtree(build_dir), compile()],
        'test' : run_tests,
    }
    actions[args.action]()
