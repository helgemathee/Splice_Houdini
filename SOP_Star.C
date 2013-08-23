// Star SOP + Splice
#define FEC_STATIC
#define FECS_STATIC

#include <limits.h>
#include <UT/UT_DSOVersion.h>
#include <UT/UT_Math.h>
#include <UT/UT_Interrupt.h>
#include <GU/GU_Detail.h>
#include <GU/GU_PrimPoly.h>
#include <CH/CH_LocalVariable.h>
#include <PRM/PRM_Include.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>
#include "SOP_Star.h"

#include <iostream>
#include <CreationSplice.h>

using namespace MIX;
using namespace CreationSplice;
using namespace std;



/// newSopOperator is the hook that Houdini grabs from this dll
/// and invokes to register the SOP. In this case we add ourselves
// to the specified operator OP_OperatorTable

void newSopOperator(OP_OperatorTable *table)
{
	table->addOperator(
		new OP_Operator("hdk_star",			// internal name
			"MIX Star",							// UI name
			SOP_Star::myConstructor,		// how to build the SOP
			SOP_Star::myTemplateList,		// The parameters
			0, 0,							// min , max sources
			SOP_Star::myVariables,			// Local variables
			OP_FLAG_GENERATOR)				// flag it as a generator
	);
}

static PRM_Name negativeName("nradius", "Negative Radius");  

static PRM_Default fiveDefault(5); 			// default to 5 divs
static PRM_Default radiiDefaults[] = {
						PRM_Default(1),		// Outside radius
						PRM_Default(0.3)	// Inside radius
					};

PRM_Template SOP_Star::myTemplateList[] = {
	PRM_Template(PRM_INT,						// Integer parameter
				PRM_Template::PRM_EXPORT_TBX,	// Export te top viewer  when user selects this node
				1,								// One integer in this row
				&PRMdivName,						// Name of this parameter must be static
				&fiveDefault,					//Default for this parameter
				0,								// Menu this parameter
				&PRMdivision2Range				// valid range
				),
	PRM_Template(PRM_XYZ, 2, &PRMradiusName, radiiDefaults),
	PRM_Template(PRM_TOGGLE, 1, &negativeName),
	PRM_Template(PRM_XYZ, 3, &PRMcenterName),
	PRM_Template(PRM_ORD, 1, &PRMorientName, 0, &PRMplaneMenu),
	PRM_Template()
};

// here's how you define local variables for the SOP_Star
enum{
	VAR_PT,		//Point number of star
	VAR_NPT		// NUmber of points in the star
};

CH_LocalVariable SOP_Star::myVariables[] = {
	{"PT", VAR_PT, 0 },				// The table provides mapping
	{"NPT", VAR_NPT, 0},			// from text string ti integer token
	{0,0,0},
};

bool
SOP_Star::evalVariableValue(fpreal &val, int index, int thread)
{
	// myCurrPoint will be negative when we're not cooking so pnly try to
	// handle the local variables when we have a valid myCurrPoint index
	if(myCurrPoint >= 0)
	{
		// Note that "gdp" may be nul here, so we do the safe thing
		// and cache values we are interested in
		switch(index)
		{
			case VAR_PT:
				val = (fpreal) myCurrPoint;
				return true;
			case VAR_NPT:
				val = (fpreal) myTotalPoints;
				return true;
			default:
				/*do nothing*/;
		}
	}
	// Not one of our variables, must delegate to the base class
	return SOP_Node::evalVariableValue(val, index, thread);
}

OP_Node *
SOP_Star::myConstructor(OP_Network *net, const char *name, OP_Operator *op)
{
	return new SOP_Star(net, name, op);
}

SOP_Star::SOP_Star(OP_Network *net, const char *name, OP_Operator *op):SOP_Node(net, name, op)
{
	myCurrPoint = -1; 		// To prevent garbage values from being returned

}

SOP_Star::~SOP_Star() {}

OP_ERROR SOP_Star::cookMySop(OP_Context &context)
{
	fpreal			now = context.getTime();
	float 			rad, tx, ty, tz;
	int  			divisions, plane, negradius;
	int 			xcoord, ycoord, zcoord;
	float			tmp, tinc;
	GU_PrimPoly		*poly;
	UT_Interrupt	*boss;

	// Since we don't have inputs we don't need to lock them
	divisions = DIVISIONS(now)*2; 		// we need twice our divisions of points
	myTotalPoints = divisions;			// Set the NPT local variable value
	myCurrPoint = 0;					// Initialize the PT local variable

	plane 		= ORIENT();
	negradius 	= NEGRADIUS();
	tx 			= CENTERX(now);
	ty 			= CENTERY(now);
	tz 			= CENTERZ(now);

	Initialize();   // initialize Creation Splice
	// create a node
	Node node = Node("myKLEnabledNode");

	// create an operator
	string klCode = "";
	klCode += "operator helloWorldOp() {\n";
	klCode += "  report('Hello varomix from KL!');\n";
	klCode += "}\n";
	node.constructKLOperator("helloWorldOp", klCode.c_str());

	// // evaluate the node
	node.evaluate();

	Finalize();  // end Creation Splice

	switch(plane)
	{
		case 0:		// XY Plane
			xcoord = 0;
			ycoord = 1;
			zcoord = 2;
			break;
		case 1:		// YZ plane
			xcoord = 1;
			ycoord = 2;
			zcoord = 0;
			break;
		case 2:		// XZ plane
			xcoord = 0;
			ycoord = 2;
			zcoord = 1;
			break;
	}

	// Check to see that there hasn't been a critical error in cooking the SOuP  LOL
	if(error() < UT_ERROR_ABORT)
	{
		boss = UTgetInterrupt();
		if(divisions < 4)
		{
			// not possible but shows how to add error if this could happen
			addWarning(SOP_MESSAGE, "Invalid divisions");
			divisions = 4;
		}
		gdp->clearAndDestroy();

		// Start the interrupt server
		if(boss->opStart("Building Star"))
		{
			// Build a polygon
			poly = GU_PrimPoly::build(gdp, divisions, GU_POLY_CLOSED);
			tinc = M_PI*2 / (float)divisions;

			// Now set all the points of the poly
			for(int i=0; i < divisions; i++)
			{
				// check to see if the user has interrupted us...
				if(boss->opInterrupt())
					break;

				myCurrPoint = i;

				// Since we respect the local variables to be used in specifying
				// the radii, we have to evaluate the channels INSIDE the loop
				// thorough the points.....
				tmp = (float)i* tinc;
				rad = (i & 1) ? XRADIUS(now) : YRADIUS(now);
				if (!negradius && rad < 0)
					rad = 0;

				UT_Vector3 pos;
				pos(xcoord) = cos(tmp) * rad + tx;
				pos(ycoord) = sin(tmp) * rad + ty;
				pos(zcoord) = 0 + tz;

				GA_Offset ptoff = poly->getPointOffset(i);
				gdp->setPos3(ptoff, pos);
			}

			// select the star
			select(GU_SPrimitive);
		}

		// tell the interrupt that we are done, always do it
		boss->opEnd();

	}

	gdp->notifyCache(GU_CACHE_ALL);

	myCurrPoint = -1;

	return error();

}