#include "definitions.h"

CustomWeakForm::CustomWeakForm(double Le, double alpha, double beta, double kappa, double x1, double tau, bool JFNK, int PRECOND,
  Hermes::Hermes2D::MeshFunctionSharedPtr<double>  omega, Hermes::Hermes2D::MeshFunctionSharedPtr<double>  omega_dt, Hermes::Hermes2D::MeshFunctionSharedPtr<double>  omega_dc,
  MeshFunctionSharedPtr<double>  t_prev_time_1, MeshFunctionSharedPtr<double>  c_prev_time_1, MeshFunctionSharedPtr<double>  t_prev_time_2, MeshFunctionSharedPtr<double>  c_prev_time_2) : WeakForm<double>(2, JFNK ? true : false), Le(Le), alpha(alpha), beta(beta), kappa(kappa), x1(x1)
{
  this->set_ext({ omega_dt, omega_dc, t_prev_time_1, t_prev_time_2, c_prev_time_1, c_prev_time_2, omega });

  if (!JFNK || (JFNK && PRECOND == 1))
  {
    MatrixFormVol<double>* mfv = new JacobianFormVol_0_0(tau);
    add_matrix_form(mfv);
    MatrixFormSurf<double>* mfs = new JacobianFormSurf_0_0("Neumann", kappa);
    add_matrix_form_surf(mfs);
    mfv = new JacobianFormVol_0_1(tau);
    add_matrix_form(mfv);
    mfv = new JacobianFormVol_1_0(tau);
    add_matrix_form(mfv);
    mfv = new JacobianFormVol_1_1(tau, Le);
    add_matrix_form(mfv);
  }
  else if (PRECOND == 2)
  {
    MatrixFormVol<double>* mfv = new PreconditionerForm_0(tau, Le);
    add_matrix_form(mfv);
    mfv = new PreconditionerForm_1(tau, Le);
    add_matrix_form(mfv);
  }

  VectorFormVol<double>* vfv = new ResidualFormVol_0(tau);
  add_vector_form(vfv);
  VectorFormSurf<double>* vfs = new ResidualFormSurf_0("Neumann", kappa);
  add_vector_form_surf(vfs);
  vfv = new ResidualFormVol_1(tau, Le);
  add_vector_form(vfv);
}

double CustomWeakForm::JacobianFormVol_0_0::value(int n, double *wt, Func<double> *u_ext[],
  Func<double> *vj, Func<double> *vi, GeomVol<double> *e, Func<double> **ext) const
{
  double result = 0;
  Func<double>* domegadt = ext[0];
  for (int i = 0; i < n; i++)
    result += wt[i] * (1.5 * vj->val[i] * vi->val[i] / tau
    + vj->dx[i] * vi->dx[i] + vj->dy[i] * vi->dy[i]
    - domegadt->val[i] * vj->val[i] * vi->val[i]);
  return result;
}

Ord CustomWeakForm::JacobianFormVol_0_0::ord(int n, double *wt, Func<Ord> *u_ext[],
  Func<Ord> *vj, Func<Ord> *vi, GeomVol<Ord> *e, Func<Ord> **ext) const
{
  Ord result = Ord(0);
  Func<Ord>* domegadt = ext[0];
  for (int i = 0; i < n; i++)
    result += wt[i] * (1.5 * vj->val[i] * vi->val[i] / tau
    + vj->dx[i] * vi->dx[i] + vj->dy[i] * vi->dy[i]
    - domegadt->val[i] * vj->val[i] * vi->val[i]);
  return result;
}

double CustomWeakForm::JacobianFormSurf_0_0::value(int n, double *wt, Func<double> *u_ext[],
  Func<double> *vj, Func<double> *vi, GeomSurf<double> *e, Func<double> **ext) const
{
  double result = 0;
  for (int i = 0; i < n; i++)
    result += wt[i] * (kappa * vj->val[i] * vi->val[i]);
  return result;
}

Ord CustomWeakForm::JacobianFormSurf_0_0::ord(int n, double *wt, Func<Ord> *u_ext[],
  Func<Ord> *vj, Func<Ord> *vi, GeomSurf<Ord> *e, Func<Ord> **ext) const
{
  Ord result = Ord(0);
  for (int i = 0; i < n; i++)
    result += wt[i] * (kappa * vj->val[i] * vi->val[i]);
  return result;
}

double CustomWeakForm::JacobianFormVol_0_1::value(int n, double *wt, Func<double> *u_ext[],
  Func<double> *vj, Func<double> *vi, GeomVol<double> *e, Func<double> **ext) const
{
  double result = 0;
  Func<double>* domegady = ext[1];
  for (int i = 0; i < n; i++)
    result += wt[i] * (-domegady->val[i] * vj->val[i] * vi->val[i]);
  return result;
}

Ord CustomWeakForm::JacobianFormVol_0_1::ord(int n, double *wt, Func<Ord> *u_ext[],
  Func<Ord> *vj, Func<Ord> *vi, GeomVol<Ord> *e, Func<Ord> **ext) const
{
  Ord result = Ord(0);
  Func<Ord>* domegady = ext[1];
  for (int i = 0; i < n; i++)
    result += wt[i] * (-domegady->val[i] * vj->val[i] * vi->val[i]);
  return result;
}

double CustomWeakForm::JacobianFormVol_1_0::value(int n, double *wt, Func<double> *u_ext[],
  Func<double> *vj, Func<double> *vi, GeomVol<double> *e, Func<double> **ext) const
{
  double result = 0;
  Func<double>* domegadt = ext[0];
  for (int i = 0; i < n; i++)
    result += wt[i] * (domegadt->val[i] * vj->val[i] * vi->val[i]);
  return result;
}

Ord CustomWeakForm::JacobianFormVol_1_0::ord(int n, double *wt, Func<Ord> *u_ext[],
  Func<Ord> *vj, Func<Ord> *vi, GeomVol<Ord> *e, Func<Ord> **ext) const
{
  Ord result = Ord(0);
  Func<Ord>* domegadt = ext[0];
  for (int i = 0; i < n; i++)
    result += wt[i] * (domegadt->val[i] * vj->val[i] * vi->val[i]);
  return result;
}

double CustomWeakForm::JacobianFormVol_1_1::value(int n, double *wt, Func<double> *u_ext[],
  Func<double> *vj, Func<double> *vi, GeomVol<double> *e, Func<double> **ext) const
{
  double result = 0;
  Func<double>* domegady = ext[1];
  for (int i = 0; i < n; i++)
    result += wt[i] * (1.5 * vj->val[i] * vi->val[i] / tau
    + (vj->dx[i] * vi->dx[i] + vj->dy[i] * vi->dy[i]) / Le
    + domegady->val[i] * vj->val[i] * vi->val[i]);
  return result;
}

Ord CustomWeakForm::JacobianFormVol_1_1::ord(int n, double *wt, Func<Ord> *u_ext[],
  Func<Ord> *vj, Func<Ord> *vi, GeomVol<Ord> *e, Func<Ord> **ext) const
{
  Ord result = Ord(0);
  Func<Ord>* domegady = ext[1];
  for (int i = 0; i < n; i++)
    result += wt[i] * (1.5 * vj->val[i] * vi->val[i] / tau
    + (vj->dx[i] * vi->dx[i] + vj->dy[i] * vi->dy[i]) / Le
    + domegady->val[i] * vj->val[i] * vi->val[i]);
  return result;
}

double CustomWeakForm::ResidualFormVol_0::value(int n, double *wt, Func<double> *u_ext[],
  Func<double> *vi, GeomVol<double> *e, Func<double> **ext) const
{
  double result = 0;
  Func<double>* titer = u_ext[0];
  Func<double>* t_prev_time_1 = ext[2];
  Func<double>* t_prev_time_2 = ext[3];
  Func<double>* omega = ext[6];
  for (int i = 0; i < n; i++)
    result += wt[i] * ((3.0 * titer->val[i] - 4.0 * t_prev_time_1->val[i]
    + t_prev_time_2->val[i]) * vi->val[i] / (2.0 * tau) +
    (titer->dx[i] * vi->dx[i] + titer->dy[i] * vi->dy[i]) -
    omega->val[i] * vi->val[i]);
  return result;
}

Ord CustomWeakForm::ResidualFormVol_0::ord(int n, double *wt, Func<Ord> *u_ext[],
  Func<Ord> *vi, GeomVol<Ord> *e, Func<Ord> **ext) const
{
  Ord result = Ord(0);
  Func<Ord>* titer = u_ext[0];
  Func<Ord>* t_prev_time_1 = ext[2];
  Func<Ord>* t_prev_time_2 = ext[3];
  Func<Ord>* omega = ext[6];
  for (int i = 0; i < n; i++)
    result += wt[i] * ((3.0 * titer->val[i] - 4.0 * t_prev_time_1->val[i]
    + t_prev_time_2->val[i]) * vi->val[i] / (2.0 * tau) +
    (titer->dx[i] * vi->dx[i] + titer->dy[i] * vi->dy[i]) -
    omega->val[i] * vi->val[i]);
  return result;
}

double CustomWeakForm::ResidualFormSurf_0::value(int n, double *wt, Func<double> *u_ext[],
  Func<double> *vi, GeomSurf<double> *e, Func<double> **ext) const
{
  double result = 0;
  Func<double>* t_prev_newton = u_ext[0];
  for (int i = 0; i < n; i++)
    result += wt[i] * (kappa * t_prev_newton->val[i] * vi->val[i]);
  return result;
}

Ord CustomWeakForm::ResidualFormSurf_0::ord(int n, double *wt, Func<Ord> *u_ext[],
  Func<Ord> *vi, GeomSurf<Ord> *e, Func<Ord> **ext) const
{
  Ord result = Ord(0);
  Func<Ord>* t_prev_newton = u_ext[0];
  for (int i = 0; i < n; i++)
    result += wt[i] * (kappa * t_prev_newton->val[i] * vi->val[i]);
  return result;
}

double CustomWeakForm::ResidualFormVol_1::value(int n, double *wt, Func<double> *u_ext[],
  Func<double> *vi, GeomVol<double> *e, Func<double> **ext) const
{
  double result = 0;
  Func<double>* c_prev_newton = u_ext[1];
  Func<double>* c_prev_time_1 = ext[4];
  Func<double>* c_prev_time_2 = ext[5];
  Func<double>* omega = ext[6];
  for (int i = 0; i < n; i++)
    result += wt[i] * ((3.0 * c_prev_newton->val[i] - 4.0 * c_prev_time_1->val[i] + c_prev_time_2->val[i])
    * vi->val[i] / (2.0 * tau) +
    (c_prev_newton->dx[i] * vi->dx[i] + c_prev_newton->dy[i] * vi->dy[i]) / Le +
    omega->val[i] * vi->val[i]);
  return result;
}

Ord CustomWeakForm::ResidualFormVol_1::ord(int n, double *wt, Func<Ord> *u_ext[],
  Func<Ord> *vi, GeomVol<Ord> *e, Func<Ord> **ext) const
{
  Ord result = Ord(0);
  Func<Ord>* c_prev_newton = u_ext[1];
  Func<Ord>* c_prev_time_1 = ext[4];
  Func<Ord>* c_prev_time_2 = ext[5];
  Func<Ord>* omega = ext[6];
  for (int i = 0; i < n; i++)
    result += wt[i] * ((3.0 * c_prev_newton->val[i] - 4.0 * c_prev_time_1->val[i] + c_prev_time_2->val[i])
    * vi->val[i] / (2.0 * tau) +
    (c_prev_newton->dx[i] * vi->dx[i] + c_prev_newton->dy[i] * vi->dy[i]) / Le +
    omega->val[i] * vi->val[i]);
  return result;
}

// Preconditioner weak forms.
double CustomWeakForm::PreconditionerForm_0::value(int n, double *wt, Func<double>* u_ext[], Func<double> *vj,
  Func<double> *vi, GeomVol<double> *e, Func<double> **ext) const
{
  double result = 0;
  for (int i = 0; i < n; i++)
    result += wt[i] * (1.5 * vj->val[i] * vi->val[i] / tau
    + vj->dx[i] * vi->dx[i] + vj->dy[i] * vi->dy[i]);
  return result;
}

Ord CustomWeakForm::PreconditionerForm_0::ord(int n, double *wt, Func<Ord>* u_ext[], Func<Ord> *vj,
  Func<Ord> *vi, GeomVol<Ord> *e, Func<Ord> **ext) const
{
  return vj->val[0] * vi->val[0] + vj->dx[0] * vi->dx[0] + vj->dy[0] * vi->dy[0];
}

double CustomWeakForm::PreconditionerForm_1::value(int n, double *wt, Func<double>* u_ext[], Func<double> *vj,
  Func<double> *vi, GeomVol<double> *e, Func<double> **ext) const
{
  double result = 0;
  for (int i = 0; i < n; i++)
    result += wt[i] * (1.5 * vj->val[i] * vi->val[i] / tau
    + (vj->dx[i] * vi->dx[i] + vj->dy[i] * vi->dy[i]) / Le);
  return result;
}

Ord CustomWeakForm::PreconditionerForm_1::ord(int n, double *wt, Func<Ord>* u_ext[], Func<Ord> *vj,
  Func<Ord> *vi, GeomVol<Ord> *e, Func<Ord> **ext) const
{
  return vj->val[0] * vi->val[0] + vj->dx[0] * vi->dx[0] + vj->dy[0] * vi->dy[0];
}

void CustomFilter::filter_fn(int n, double* x, double* y, const std::vector<const double *>& values, const std::vector<const double *>& dx, const std::vector<const double *>& dy,
  double* out, double* outdx, double* outdy)
{
  for (int i = 0; i < n; i++)
  {
    double t1 = std::max(values.at(0)[i], 0.0) - 1.0;
    double t2 = t1 * beta;
    double t3 = 1.0 + t1 * alpha;
    double t4 = sqr(beta) / (2.0*Le) * exp(t2 / t3);
    double t5 = (beta / (t3 * t3)) * values.at(1)[i];
    out[i] = t4 * values.at(1)[i];
    outdx[i] = t4 * (dx.at(1)[i] + dx.at(0)[i] * t5);
    outdy[i] = t4 * (dy.at(1)[i] + dy.at(0)[i] * t5);
  }
}

void CustomFilterDt::filter_fn(int n, double* x, double* y, const std::vector<const double *>& values, const std::vector<const double *>& dx, const std::vector<const double *>& dy,
  double* out, double* outdx, double* outdy)
{
  for (int i = 0; i < n; i++)
  {
    double t1 = std::max(values.at(0)[i], 0.0) - 1.0;
    double t2 = t1 * beta;
    double t3 = 1.0 + t1 * alpha;
    double t4 = sqr(beta) / (2.0*Le) * exp(t2 / t3);
    double t5 = (beta / (t3 * t3));
    out[i] = t4 * t5 * values.at(1)[i];
    outdx[i] = 0.0;
    outdy[i] = 0.0; // not important
  }
}

void CustomFilterDc::filter_fn(int n, double* x, double* y, const std::vector<const double *>& values, const std::vector<const double *>& dx, const std::vector<const double *>& dy,
  double* out, double* outdx, double* outdy)
{
  for (int i = 0; i < n; i++)
  {
    double t1 = std::max(values.at(0)[i], 0.0) - 1.0;
    double t2 = t1 * beta;
    double t3 = 1.0 + t1 * alpha;
    double t4 = sqr(beta) / (2.0*Le) * exp(t2 / t3);
    out[i] = t4;
    outdx[i] = 0.0;
    outdy[i] = 0.0; // not important
  }
}