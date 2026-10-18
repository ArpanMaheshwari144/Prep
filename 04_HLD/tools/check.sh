#!/bin/bash
# check.sh <folder>: render + har scene / concept wali image ka PNG (.playwright-mcp/chk) taaki khud dekh sakun
cd /c/DSA_PRACTICE
f=04_HLD/SYSTEM_DESIGNS/$1
python 04_HLD/tools/render_diagrams.py $f/*.md 2>&1 | grep -v '^ok c[a-z0-9_]*  $'
rm -rf .playwright-mcp/chk; mkdir -p .playwright-mcp/chk
node 04_HLD/tools/svg2png.js .playwright-mcp/chk $(ls $f/img/*-*.svg | grep -v cdabba) > /dev/null
ls .playwright-mcp/chk
