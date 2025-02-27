# 3004_TeamProject
Team project for Developing and testing a simulation of RaDoTech  health monitoring device.

# Team Members
Nigel Wanga, Thomas Roche, Vlad Jubea

# Member Contributions:
Nigel Wanga - GUI Interface (over half of code in mainwindow; almost all code designing the graphical side of GUI)

Thomas Roche - design + implementation for model, scanner, and about half of the design + implementation for profile and snapshot.
                Integrations between model/snapshot and mainwindow; limited mainwindow UI improvements and the addition of some data. 
                Most documentation except use cases. 
                Video Demos.
                Misc tweaks and improvements. 

Vlad Jubea - Use cases, all Database design + implementation + testing, partly integrating DBManager with the model, 
              half the work on profile and snapshot, full Profile page and subpages from it in mainwindow, and 
              much of the work integrating other pages of mainwindow with the model.

# Codebase & Executables:
/src contains all .cpp and .h files. It can be built using the VM's QTcreator or run like any other program. 
Files include:
defs.h
mainwindow.[h/cpp/ui]
model.[h/cpp]
profile.[h/cpp]
scanner.[h/cpp]
snapshot.[h/cpp]
dbmanager.[h/cpp]

# Use Cases, Textual Explanation of Design Decisions, Requirements Traceability Matrix
/documentation has all documentation. See DOCS_README.txt for more info

# Video Demo
GUI demo + success scenarios: https://youtu.be/HWb03fQWFBg
Safety scenario demo: https://youtu.be/ptwNnFw3YFQ
