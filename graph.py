import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import _tkinter

import numpy

x_axis = [1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20]

BFS_Node = [8,19,22,39,69,200,245,374,583,753,1413,2156,4335,5837,9704,12609,29093,35195,56333,74593]
UCS_Node = [8,14,19,42,64,177,206,341,761,968,1979,1930,3652,7344,11595,18161,26895,34636,60259,70232]
DLS_Node =  [30,77,30,77,43,577,852,501,366,722,533,2380,6626,6932,3829,2018,26851,46743,54321,221292]
IDS_Node = [3,20,42,89,192,558,1061,1847,3082,5051,8552,14427,26166,42564,69959,102793,188788,300901,495708,768208]
GBFS_Node = [8,18,19,39,69,199,245,371,583,753,1410,2156,4335,5837,9704,12606,29093,35195,56333,74590]
A_Star_Node = [8,18,19,39,69,199,245,371,583,753,1410,2156,4335,5837,9704,12606,29093,35195,56333,74590]

#red,blue,green,yellow,black,pink
plt.plot(x_axis,BFS_Node,"red")
plt.plot(x_axis,UCS_Node,"blue")
plt.plot(x_axis,DLS_Node,"green")
plt.plot(x_axis,IDS_Node,"yellow")
plt.plot(x_axis,GBFS_Node,"black")
plt.plot(x_axis,A_Star_Node,"pink")


plt.title("Step - Node graph")

plt.xlabel("Step")
plt.ylabel("Generated Node")



BFS = mpatches.Patch(color="red",label="BFS")
UCS = mpatches.Patch(color="blue",label="UCS")
DLS = mpatches.Patch(color="green",label="DLS")
IDS = mpatches.Patch(color="yellow",label="IDS")
GBFS = mpatches.Patch(color="black",label="GBFS")
A_STAR = mpatches.Patch(color="pink",label="A*")

plt.legend(handles=[BFS,UCS,DLS,IDS,GBFS,A_STAR])

plt.grid()
plt.show()