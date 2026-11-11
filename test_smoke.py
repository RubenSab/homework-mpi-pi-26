#!/usr/bin/env python3
"""Local correctness smoke tests."""
import math
import os
from pathlib import Path
import re
import shlex
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parent
ENV = os.environ.copy()
ENV.setdefault("OMP_NUM_THREADS", "4")
FULL = os.environ.get("FULL", "0") == "1"


def run(binary, args, cwd, mpi=False):
    launcher = shlex.split(os.environ.get("MPIEXEC", "mpirun -np 4")) if mpi else []
    result = subprocess.run(launcher + [str(ROOT / binary)] + list(map(str, args)),
                            cwd=cwd, env=ENV, text=True, capture_output=True,
                            timeout=float(os.environ.get("TEST_TIMEOUT", "300")))
    if result.returncode:
        raise RuntimeError(f"{binary} exited {result.returncode}\n{result.stderr}")
    return result.stdout


def main():
    with tempfile.TemporaryDirectory(prefix="mcp-smoke-") as temporary:
        work = Path(temporary)
        for samples in (1000000, 1000003):
            output = run('pi_parallel', [samples], work, mpi=True)
            estimates = re.findall(r"Estimate of pi\s*=\s*(\S+)", output)
            if len(estimates) != 1:
                raise ValueError("expected exactly one pi estimate")
            estimate = float(estimates[0])
            if not math.isfinite(estimate) or abs(estimate - math.pi) >= 0.02:
                raise ValueError(f"incorrect pi estimate: {estimate}")
            print(f"PASS: {samples} samples")


if __name__ == "__main__":
    try:
        main()
    except (OSError, ValueError, RuntimeError, subprocess.TimeoutExpired) as error:
        print(f"FAIL: {error}", file=sys.stderr)
        sys.exit(1)
