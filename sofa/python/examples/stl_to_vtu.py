#! /usr/env/python3

import Sofa
import os
import argparse
from os.path import join, expanduser



pwd = os.getcwd()
sup_dir = join(pwd, '../../..', 'ros/srs_traj_opt/patient_description/meshes/dome/')
print(sup_dir)

mesh = 'dome'
tetramesh = mesh+".stl"

def createScene(rootNode):
    rootNode.createObject('VisualStyle', displayFlags='showVisualModels showBehaviorModels hideCollisionModels hideBoundingCollisionModels hideForceFields hideInteractionForceFields hideWireframe')
    rootNode.createObject('RequiredPlugin', pluginName='CGALPlugin')
    rootNode.createObject('RequiredPlugin', pluginName='SofaExporter')

    rootNode.createObject('BackgroundSetting', color='0 0.168627 0.211765')
    rootNode.createObject('OglSceneFrame', style="Arrows", alignment="TopRight")

    ##########################################
    # Generation                             #
    ##########################################
    node = rootNode.createChild('node')
    #1- Specify the input 3D surfacic mesh
    node.createObject('MeshSTLLoader', name='mesh', filename=sup_dir+tetramesh)
    node.createObject('MeshGenerationFromPolyhedron', name='gen', template='Vec3d', inputPoints='@mesh.position', inputTriangles='@mesh.triangles', drawTetras='1',
           # This parameter controls the size of mesh tetrahedra. It provides an upper bound on the circumradius of the mesh tetrahedra.
           # commented params for other delaunay tetras
           cellSize=".9", #"1",
           facetAngle=".1", #"10",
           # This parameter provides an upper bound for the radii of the surface Delaunay ball; a larger value may lead to larger tetrahedra.
           facetSize=".2", #".1",

           # This parameter controls the shape of mesh cells. Actually, it is an upper bound for the ratio between the circumradius of a mesh
           # tetrahedron and its shortest edge. There is a theoretical bound for this parameter: the Delaunay refinement process is guaranteed
           # to terminate for values larger than 2.
           cellRatio="4",   #Convergence problem if < 2
           # The approximation error between the boundary and the subdivision surface. It provides an upper bound for the distance
           # between the circumcenter of a surface facet and the center of a surface Delaunay ball of this facet.
           facetApproximation="1"
           )
    node.createObject('Mesh', position='@gen.outputPoints', tetrahedra='@gen.outputTetras')
    #2- Export the output 3D tetrahedras mesh
    node.createObject('VTKExporter', filename=sup_dir+mesh, edges='0', tetras='1', exportAtBegin='1')

    ##########################################
    # Visualization                          #
    ##########################################
    node.createObject('OglModel', src="@mesh", color="0.0 0.7 0.7 0.5")

    return rootNode
