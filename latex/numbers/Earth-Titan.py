import numpy as numpy
import spiceypy as spice
import math

spice.furnsh("/Users/alex/Development/OpenSpace/data/spice/naif0011.tls")
spice.furnsh("/Users/alex/Development/OpenSpace/data/spice/de430_1850-2150.bsp")
spice.furnsh("/Users/alex/Development/OpenSpace/data/spice/sat375.bsp")

et = spice.str2et("2016 DEC 11")
print(et)

positions, lightTimes = spice.spkpos('Titan', et, "J2000", "NONE", "EARTH")
length = math.sqrt(positions[0] * positions[0] + positions[1] * positions[1] + positions[2] * positions[2])
print(positions)
print(length)