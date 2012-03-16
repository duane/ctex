import sys
import os
import subprocess

class Runner(object):
  """A class that represents a google test program to be run."""
  def __init__(self, name, exec_path, exec_cwd):
    super(Runner, self).__init__()
    self.name = name
    self.exec_path = exec_path
    self.exec_cwd = cwd

  def run(self):
    args = [exec_path]
    with subprocess.Popen(args, cwd=self.exec_cwd, stdout=PIPE) as runner:
      
    