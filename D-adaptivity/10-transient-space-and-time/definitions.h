#include "hermes2d.h"

using namespace Hermes;
using namespace Hermes::Hermes2D;

/* Nonlinearity lambda(u) = pow(u, alpha) */

class CustomNonlinearity : public Hermes1DFunction<double>
{
public:
  CustomNonlinearity(double alpha);

  virtual double value(double u) const;

  virtual Ord value(Ord u) const;

  virtual double derivative(double u) const;

  virtual Ord derivative(Ord u) const;

protected:
  double alpha;
};

/* Essential boundary condition */

class EssentialBCNonConst : public EssentialBoundaryCondition<double>
{
public:
  EssentialBCNonConst(std::string marker);

  virtual EssentialBCValueType get_value_type() const;

  virtual double value(double x, double y) const;
};

/* Initial condition */

class CustomInitialCondition : public ExactSolutionScalar<double>
{
public:
  CustomInitialCondition(MeshSharedPtr mesh) : ExactSolutionScalar<double>(mesh) {};

  virtual void derivatives(double x, double y, double& dx, double& dy) const;

  virtual double value(double x, double y) const;

  virtual Ord ord(double x, double y) const;

  MeshFunction<double>* clone() const;
};
