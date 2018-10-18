#!/usr/bin/env python3

import sys
import argparse
import subprocess
import re


def runtest(word, inputs, shouldwin, verbose):
    intext = "\n".join(inputs) + "\n"
    p = subprocess.Popen("./hangperson {}".format(word.upper()), stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True, universal_newlines=True)
    sout, serr = p.communicate(input=intext, timeout=2)
    entries = sout.split(">>>")

    results = []
    wordstate = list(len(word) * '_')
    misses = 0

    for i, e in enumerate(entries[:-1]):
        if len(e.strip()) < 5:
            continue
        if verbose:
            print()
            print("\t\t(expected state: {}; guess: \"{}\")".format(' '.join(wordstate), inputs[i]), end="")
        # if previous guess was a letter, state should be updated
        if inputs[i-1].isalpha():
            if re.search(' '.join(wordstate), e):
                results.append(True)
                print("\u2713 ", end="")
            else:
                results.append(False)
                print("\u2717 ", end="")

            mobj = re.search("Missed: (\d+)", e, re.I)
            if mobj:
                miss = mobj.groups()[0]
                if verbose and (int(miss) != misses):
                    print("(bad number of misses: {}, expected {}".format(miss, misses))
        else:
            print("\u2713 ", end="")

        if len(inputs[i]) == 1 and inputs[i].isalpha():
            letter = inputs[i].upper()
            for j in range(len(word)):
                if letter == word[j]:
                    wordstate[j] = letter
            if letter not in word:
                misses += 1

    if (re.search('won', entries[-1], re.I) and shouldwin) or (re.search('lost', entries[-1], re.I) and not shouldwin):
        results.append(True)
    else:
        results.append(False)

    if all(results):
        print("PASS")
    elif any(results):
        print("PARTIAL PASS")
    else:
        print("FAIL")


def main():
    parser = argparse.ArgumentParser(description="Run hangperson project tests")
    parser.add_argument("--quiet", "-q", action="store_true", default=False, help="Turn on quieter output")
    args = parser.parse_args()

    tests = [
        ["A", ['A'], True],
        ["A", ['a'], True],
        ["A", ['B', 'A'], True],
        ["APPLE", ['E', 'P', 'L', 'x', ' ', 'A'], True],
        ["APPLE", ['E', 'P', 'L', 'x', 'r', 'm', 'n', 'Z', 'o', 'I', 'w'], False],
        ["AXE", ['I', 'J', '*', '?', 'r', 'm', '%', 'p', 'q', 'a', 'e', 'u'], False]
    ]
    print("Running tests")
    for i, t in enumerate(tests):
        print("\t{} (word={}): ".format(i+1, t[0]), end="")
        runtest(*t, not args.quiet)
    

if __name__ == '__main__':
    main()
