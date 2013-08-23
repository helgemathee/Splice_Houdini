#define FEC_STATIC
#define FECS_STATIC


#include <iostream>
#include <CreationSplice.h>

using namespace CreationSplice;
using namespace std;

int main( int argc, const char* argv[] )
{
  Initialize();

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

  Finalize();

  return 0;
}
