# VirusSim

A project that executes an SIRV simulation on a population using a math based model. At the end of the program various statistics about the simulation are shown to the user.   

> In SIRV models, we say that agents have one of four health states: Susceptible, Infectious,
Recovered, Vaccinated. Susceptible agents do not have the disease but can contract it. Infectious agents have the disease and are spreading it for the length of their infectious period. Recovered agents are no longer infectious and have immunity to the disease. Vaccinated agents cannot contract the disease due to being vaccinated.   

This program was originally meant to be implemented as a object based model where each individual would have its own health states, but I found that to be fairly straightforward as well as inefficient so for more of a challenge I created a mathematical model that can represent the effects the virus will have on the population based on the parameters given in the description. This is commented on fairly well in its section of the source code (src/sim.cpp).   

The program is implemented according to the description. Some config, region, and population files are provided in this repository but new ones may also be created as long as they follow the format.

To run:  
    
    - Place (or check that you have) Region(CSV), and Population+Config(TXT) files into /virussum/  
    - make  
    - ./output
