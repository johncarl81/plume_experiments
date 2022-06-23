#!/usr/bin/python

from matplotlib.patches import Ellipse
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

def coordinates(input):
    split = input.split(',')
    return [float(split[0][1::]),  float(split[1][:-1:])]

def main():
    plt.figure()
    ax = plt.gca()

    color = 'k'

    with open("sketch_plot.txt", "r") as fp:
        Lines = fp.readlines()
        for line in Lines:
            split = line.split()
            if split[0] == 'Ellipse':
                # print("Ellipse " + split[1])
                [centerx, centery] = coordinates(split[1])
                width = 2 * float(split[2])
                height = 2 * float(split[3])

                ellipse = Ellipse(xy=(centerx, centery), width=width, height=height, edgecolor='forestgreen', fc='greenyellow', fill=True)
                ax.add_patch(ellipse)
            elif split[0] == 'Line':
                # print("Line")
                [startx, starty] = coordinates(split[1])
                [endx, endy] = coordinates(split[2])

                if not (startx - endx == 0 and starty - endy == 0):
                    plt.plot([startx, endx], [starty, endy], color, lw=0.5)
                    #plt.arrow((startx + endx) / 2, (starty + endy) / 2, (endx - startx) / 10, (endy - starty) / 20, color=color, shape='full', length_includes_head=True, head_width=.001)
            elif split[0] == 'Point':
                [x, y] = coordinates(split[1])
                plt.plot([x], [y], marker="o")
            elif split[0] == 'Pen':
                color = split[1]

    # ax.set_ylim([-0.1, 0.1])
    # ax.set_xlim([-0.51, -0.45])

    pp = PdfPages('sketch.pdf')
    pp.savefig()
    pp.close()

if __name__ == '__main__':
    main()