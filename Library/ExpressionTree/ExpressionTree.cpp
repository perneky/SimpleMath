#include "ExpressionTree.hpp"
#include "Node.hpp"

namespace SimpleMath
{
namespace ExpressionTree
{

ExpressionTree::ExpressionTree( Node* root )
  : root( root )
{
}

size_t ExpressionTree::Evaluate( const EvaluateContext& context, EvalResult result ) const noexcept
{
  return root->Evaluate( context, result );
}

}
}