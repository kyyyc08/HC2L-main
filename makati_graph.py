import osmnx as ox
import matplotlib.pyplot as plt

# Define the location
place_name = "Makati, Metro Manila, Philippines"

# Download the road network for driving
G = ox.graph_from_place(place_name, network_type="drive")

# Plot the road network
fig, ax = ox.plot_graph(G, node_size=0, edge_linewidth=0.5)