#include "definitions.h"

//  The purpose of this example is to use a simple linear problem with known
//  exact solution to show how to use NOX, and to compare its performance to
//  other linear solvers in Hermes (MUMPS, PETSc, UMFPACK, etc.).
//
//  PDE: Poisson equation.
//
//  Domain: Square (-1, 1)^2.
//
//  BC: Nonhomogeneous Dirichlet, see the function essential_bc_values() below.
//
//  Exact solution: x*x + y*y.
//
//  The following parameters can be changed:

// Number of initial uniform mesh refinements.
const int INIT_REF_NUM = 6;
// Initial polynomial degree of all mesh elements.
const int P_INIT = 3;

// NOX parameters.
// true = Jacobian-free method (for NOX),
// false = Newton (for NOX).
const bool TRILINOS_JFNK = true;
// Preconditioning by jacobian in case of JFNK (for NOX),
// default ML preconditioner in case of Newton.
const bool PRECOND = true;
// Name of the iterative method employed by AztecOO (ignored
// by the other solvers).
// Possibilities: gmres, cg, cgs, tfqmr, bicgstab.
const char* iterative_method = "GMRES";
// Name of the preconditioner employed by AztecOO
// Possibilities: None" - No preconditioning.
// "AztecOO" - AztecOO internal preconditioner.
// "New Ifpack" - Ifpack internal preconditioner.
// "ML" - Multi level preconditione
const char* preconditioner = "AztecOO";
// NOX error messages, see NOX_Utils.h.
unsigned message_type = NOX::Utils::Error | NOX::Utils::Warning | NOX::Utils::OuterIteration | NOX::Utils::InnerIteration | NOX::Utils::Parameters | NOX::Utils::LinearSolverDetails;

// Tolerance for linear system.
double ls_tolerance = 1e-5;
// Flag for absolute value of the residuum.
unsigned flag_absresid = 1;
// Tolerance for absolute value of the residuum.
double abs_resid = 1.0e-8;
// Flag for relative value of the residuum.
unsigned flag_relresid = 0;
// Tolerance for relative value of the residuum.
double rel_resid = 1.0e-2;
// Max number of iterations.
int max_iters = 50;

int main(int argc, char **argv)
{
  // Time measurement.
  Hermes::Mixins::TimeMeasurable cpu_time;
  cpu_time.tick();

  // Load the mesh.
  MeshSharedPtr mesh(new Mesh);
  MeshReaderH2D mloader;
  mloader.load("square.mesh", mesh);

  // Perform initial mesh refinemets.
  for (int i = 0; i < INIT_REF_NUM; i++) mesh->refine_all_elements();

  // Set exact solution.
  MeshFunctionSharedPtr<double> exact(new CustomExactSolution(mesh));

  // Initialize boundary conditions
  DefaultEssentialBCNonConst<double> bc_essential("Bdy", exact);
  EssentialBCs<double> bcs(&bc_essential);

  // Initialize the weak formulation.
  WeakFormSharedPtr<double> wf1(new CustomWeakFormPoisson);

  // Create an H1 space with default shapeset.
  SpaceSharedPtr<double> space(new H1Space<double>(mesh, &bcs, P_INIT));
  int ndof = Space<double>::get_num_dofs(space);
  Hermes::Mixins::Loggable::Static::info("ndof: %d", ndof);

  Hermes::Mixins::Loggable::Static::info("---- Assembling by NewtonSolver, solving by regular solver:");

  // Begin time measurement of assembly.
  cpu_time.tick();

  // Initialize the Newton solver.
  Hermes::Hermes2D::NewtonSolver<double> newton(wf1, space);

  // Perform Newton's iteration and translate the resulting coefficient vector into a Solution.
  MeshFunctionSharedPtr<double> sln1(new Solution<double>);
  MeshFunctionSharedPtr<double> sln2(new Solution<double>);
  try
  {
    newton.solve();
  }
  catch (std::exception& e)
  {
    std::cout << e.what();
  }

  // Translate solution vector into a Solution.
  Solution<double>::vector_to_solution(newton.get_sln_vector(), space, sln1);

  // CPU time measurement.
  double time = cpu_time.tick().last();

  // Calculate errors.
  DefaultErrorCalculator<double, HERMES_H1_NORM> errorCalculator1(RelativeErrorToGlobalNorm, 1);
  errorCalculator1.calculate_errors(sln1, exact);
  double rel_err_1 = errorCalculator1.get_total_error_squared() * 100;

  Hermes::Mixins::Loggable::Static::info("CPU time: %g s.", time);
  Hermes::Mixins::Loggable::Static::info("Exact H1 error: %g%%.", rel_err_1);

  // View the solution and mesh.
  ScalarView sview("Solution", new WinGeom(0, 0, 440, 350));
  sview.show(sln1);
  //OrderView  oview("Polynomial orders", new WinGeom(450, 0, 400, 350));
  //oview.show(space);

  // TRILINOS PART:

  Hermes::Mixins::Loggable::Static::info("---- Assembling by NewtonSolverNOX, solving by NOX:");

  // Initialize the weak formulation for Trilinos.
  WeakFormSharedPtr<double> wf2(new CustomWeakFormPoisson(TRILINOS_JFNK));

  // Time measurement.
  cpu_time.tick();

  // Calculate initial vector for NOX.
  Hermes::Mixins::Loggable::Static::info("Projecting to obtain initial vector for the Newton's method.");
  double* coeff_vec = new double[ndof];
  MeshFunctionSharedPtr<double> sln2_init(new ConstantSolution<double>(mesh, 123.));
  OGProjection<double>::project_global(space, sln1, coeff_vec);

  // Initialize the NOX solver.
  Hermes::Mixins::Loggable::Static::info("Initializing NOX.");
  NewtonSolverNOX<double> nox_solver(wf2, space);
  nox_solver.set_output_flags(message_type);

  // Set various NOX parameters.
  nox_solver.set_ls_type(iterative_method);
  nox_solver.set_ls_tolerance(ls_tolerance);
  nox_solver.set_conv_iters(max_iters);
  if (flag_absresid)
    nox_solver.set_conv_abs_resid(abs_resid);
  if (flag_relresid)
    nox_solver.set_conv_rel_resid(rel_resid);

  // Choose preconditioning.
  MlPrecond<double> pc("sa");
  if (PRECOND)
  {
    if (TRILINOS_JFNK) nox_solver.set_precond(pc);
    else nox_solver.set_precond(preconditioner);
  }

  // Assemble and solve using NOX.
  try
  {
    nox_solver.solve(coeff_vec);
  }
  catch (std::exception& e)
  {
    std::cout << e.what();
  }

  // Convert resulting coefficient vector into a Solution.
  Solution<double>::vector_to_solution(nox_solver.get_sln_vector(), space, sln2);

  Hermes::Mixins::Loggable::Static::info("Number of nonlin iterations: %d (norm of residual: %g)",
    nox_solver.get_num_iters(), nox_solver.get_residual());
  Hermes::Mixins::Loggable::Static::info("Total number of iterations in linsolver: %d (achieved tolerance in the last step: %g)",
    nox_solver.get_num_lin_iters(), nox_solver.get_achieved_tol());

  // CPU time needed by NOX.
  time = cpu_time.tick().last();

  // Show the NOX solution.
  ScalarView view2("Solution 2 (Trilinos)", new WinGeom(450, 0, 460, 350));
  view2.show(sln2);

  // Calculate errors.
  DefaultErrorCalculator<double, HERMES_H1_NORM> errorCalculator2(RelativeErrorToGlobalNorm, 1);
  errorCalculator2.calculate_errors(sln2, exact);
  double rel_err_2 = errorCalculator2.get_total_error_squared() * 100;

  Hermes::Mixins::Loggable::Static::info("CPU time: %g s.", time);
  Hermes::Mixins::Loggable::Static::info("Exact H1 error: %g%%.", rel_err_2);

  // Wait for all views to be closed.
  View::wait();

  return 0;
}