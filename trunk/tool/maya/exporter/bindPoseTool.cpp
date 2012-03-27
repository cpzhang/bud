/*
 * Resets the skeleton into its bindpose.
 */

#include "bindPoseTool.h"

#include <maya/MObject.h>
#include <maya/MMatrix.h>
#include <maya/MFnIkJoint.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnMesh.h>
#include <maya/MDagPath.h>
#include <maya/MIkSystem.h>
#include <maya/MItDag.h>
#include <maya/MTransformationMatrix.h>
#include <iostream>

    BindPoseTool::BindPoseTool()
	: inBindPose(false)
    {}

    BindPoseTool::~BindPoseTool()
    {
	if (inBindPose)
	    UndoGoIntoBindPose();
    }

    // getBindPoseMatrix.
    // should be a relatively harmless function.
    // well you're wrong: this is one of the most stupid/difficult things in
    // Maya:
    //   1st try: just get the "bindPose"-plug, and use it as matrix:
    //             MFnIkJoint joint(jointPath.node());
    //             MPlug bindMatrixPlug = joint.findPlug("bindPose");
    //             MObject bindMatrixObject;
    //             bindMatrixPlug.getValue(bindMatrixObject);
    //             MFnMatrixData matrixData(bindMatrixObject);
    //             MMatrix bindMatrix = matrixData.matrix();
    //      Looks correct. But isn't. Not only, that we want the local
    //      transform (but that wouldn't be difficult (just look at the
    //      parent...), the bindPose plug seems
    //      to be shared between instances. If a joint is instanced, the
    //      bindMatrix would be the same as for the other instance. Bad luck :(
    //   2nd try: extract the local-transform out of the bindPose-plug. That's
    //      what is done here. (at least for now. (seen first in
    //      MS' xporttranslator).
    //      Still not the really correct way: it's possible to break the
    //      bindPose-plug. Maya still works, but our exporter doesn't.
    //   3rd (and correct way):
    //        http://www.greggman.com/pages/mayastuff.htm
    //
    // again: not yet very much error-checking...
    MMatrix
    BindPoseTool::getBindPoseMatrix(MFnIkJoint joint) const
    {
        // get bindPose-plug on joint
        MPlug tempBindPosePlug = joint.findPlug("bindPose");

        MPlugArray mapConnections;
        tempBindPosePlug.connectedTo(mapConnections, false, true);

        if (mapConnections.length() != 1)
        {
            //cout << "returning currentMatrix" << endl;
            // certainly not the most correct way of dealing with the problem...
            return joint.transformation().asMatrix();
        }

        // find the other end. actually we shouldn't call it "bindPosePlug",
        // but worldTransformPlug (as that's where it enters).

        // theoretically someone could bind the bindPose to other nodes,
        // than the bindPose-node. in this case there's a problem.
        MPlug bindPosePlug = mapConnections[0];

        // this node should be a "dagPose"-node (in case you want to look it
        // up in the help)
        MFnDependencyNode bindPoseNode(bindPosePlug.node());

        // and as such, has the "xformMatrix"-attribute.
        MObject xformMatrixAttribute = bindPoseNode.attribute("xformMatrix");

        MPlug localTransformPlug(bindPosePlug.node(), xformMatrixAttribute);
        // xformMatrix is an array. to get our localmatrix we need to select
        // the same index, as our bindPosePlug (logicalIndex()).
        localTransformPlug.selectAncestorLogicalIndex(
                            bindPosePlug.logicalIndex(), xformMatrixAttribute);

        MObject localMatrixObject;
        localTransformPlug.getValue(localMatrixObject);
        // extract the matrix out of the object.
        return MFnMatrixData(localMatrixObject).matrix();
    }

    // I'm not even sure, if this is necessary. but MS's x-exporter does it.
    // and it shouldn't hurt.
    void
    BindPoseTool::syncMeshes() const
    {
		MStatus status;
       std:: cout << "  Syncing meshes ";
        for (MItDag dagIt(MItDag::kDepthFirst, MFn::kMesh, &status);
             !dagIt.isDone();
             dagIt.next())
        {
            std::cout << ".";
            MDagPath meshPath;
            dagIt.getPath(meshPath);
            MFnMesh mesh(meshPath.node());
            mesh.syncObject();
        }
        std::cout << std::endl;
    }

    // In order to correctly extract skinweights we first
    // need to go into bindpose. (and we need to remember the current
    // pose, to be able to undo it).
    //
    // I know: nearly no error-checking.
    void
    BindPoseTool::GoIntoBindPose()
    {
		MStatus status;
        std::cout << " Going into bindpose ";
        // == turn IK off
        // save current state
        undoInfo.ikSnap = MIkSystem::isGlobalSnap(&status);
        undoInfo.ikSolve = MIkSystem::isGlobalSolve();
        // turn it off
        MIkSystem::setGlobalSnap(false);
        MIkSystem::setGlobalSolve(false);

        // == put joints into bindPose
        for (MItDag dagIt(MItDag::kDepthFirst, MFn::kJoint);
             !dagIt.isDone();
             dagIt.next())
        {
            std::cout << ".";
            MDagPath jointPath;
            dagIt.getPath(jointPath);
            if (jointPath.isInstanced())
            { // we only work on the first instance of an instanced joint.
                if (jointPath.instanceNumber() != 0)
                    continue;
            }

            BindPoseUndoInformation::JointMatrixVector::value_type joint2transform;

            MFnIkJoint joint(jointPath.node());

            MTransformationMatrix currentTransform = joint.transformation(&status);

            joint2transform.first = jointPath.node();
            joint2transform.second = currentTransform;
            undoInfo.savedTransforms.push_back(joint2transform);

            MMatrix bindPoseMatrix = getBindPoseMatrix(joint);
            joint.set(bindPoseMatrix);
        }
        std::cout << std::endl;

        //cout << "bindPose end" << endl;
        // don't know, if this is really necessary, but MS xporttranslator
        // does it...
        syncMeshes();

	// remember the change
	inBindPose = true;
    }

    // just apply the original pose, and reenable IK (if it was enabled).
    void
    BindPoseTool::UndoGoIntoBindPose()
	{
		MStatus status;
        std::cout << " Going back to initial pose ";
        for (BindPoseUndoInformation::JointMatrixVector::const_iterator
               it = undoInfo.savedTransforms.begin();
            it != undoInfo.savedTransforms.end();
            ++it)
        {
            std::cout << ".";
            status = MFnIkJoint(it->first).set(it->second);
            if (!status)
                std::cout << "prob in undoInfo" << std::endl;
        }

        MIkSystem::setGlobalSnap(undoInfo.ikSnap);
        MIkSystem::setGlobalSolve(undoInfo.ikSolve);

        std::cout << std::endl;

        syncMeshes();

	// remember the change
	inBindPose = false;
    }