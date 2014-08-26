STAG
===

![Stag Progress](http://i.imgur.com/s2BlCIa.png)

DESCRIPTION
---

stag generates a streaming  bar  graph  of  the data  (line  separated floats) passed through stdin.

SYNOPSIS
---
stag  [-t  TITLE]  [-m  MARGIN]  [-s  SCALE\_MODE]  [-w  BAR\_WIDTH]  [-y Y\_SPLITS]


OPTIONS
---
       -t, --title=TITLE
              Set the title displayed above the graph.

       -m, --margin=T,R,B,L
              Set the margin around the edge of the graph, defined as a comma 
              delimited list of 4 integers: top,right,bottom,left.

       -s, --scale=SCALE_MODE
              Choose how to scale the y-axis of the graph.
              dynamic: update the max using the values currently on the graph
              global: update the max to be the global maximum of all data seen
              min,max: specify the max and min using two integers

       -w, --width=BAR_WIDTH
              Set the width of the bars.

       -y, --split=Y_SPLITS
              Set the number of intermediate y-axis ticks between the min and max.

EXAMPLES
---
Trivial example:

      echo -e "1\n2\n3" | ./stag

See examples/ directory for more.

AUTHOR
---
Seena Burns (nnkd)

Stag 1.0
August 26, 2014
