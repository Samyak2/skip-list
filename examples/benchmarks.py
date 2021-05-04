# helper script to run benchmarks for various sizes
# and plot graphs

import subprocess
import logging
import os
import re
import math
import sys

import matplotlib.pyplot as plt

logging.basicConfig(stream=sys.stdout, level=logging.INFO)

sizes = [1, 10, 50, 100, 500, 1000, 5000, 10_000, 50_000, 100_000, 1_000_000]
iterations = 100000

curdir = os.getcwd()

re_find_time = re.compile(r"Find: (\d+\.?\d*) us")
re_insert_time = re.compile(r"Insert: (\d+\.?\d*) us")
re_erase_time = re.compile(r"Erase: (\d+\.?\d*) us")

find_times = []
insert_times = []
erase_times = []
for i in sizes:
    logging.info("Running benchmark for size %d", i)

    out = subprocess.check_output([f"{curdir}/benchmarks", f"{i}", f"{iterations}"])
    out = str(out)

    def parse_time(name, out, regex, list_):
        time_ = regex.findall(out)
        if len(time_) < 1:
            logging.error("Could not get %s time from log: %s", name, out)
        time_ = float(time_[0])
        list_.append(time_)

    parse_time("find", out, re_find_time, find_times)
    parse_time("insert", out, re_insert_time, insert_times)
    parse_time("erase", out, re_erase_time, erase_times)

size_logs = sizes
size_logs = list(map(math.log10, sizes))

for list_, name in ((find_times, "Find"), (insert_times, "Insert"), (erase_times, "Erase")):
    plt.plot(size_logs, list_)
    plt.title(name)
    plt.xlabel("log10(Size of list)")
    plt.ylabel("Runtime in microseconds")
    plt.savefig(f"{name}.png")
    plt.clf()

print("\x1b[31mToo much info? Redirect stderr to devnull : 2>/dev/null\033[0m", file=sys.stderr)
